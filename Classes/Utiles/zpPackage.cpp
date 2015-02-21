#include "zpPackage.h"
#include "zpFile.h"
#include "zpCompressedFile.h"
#include "zlib/zlib.h"
#include <cassert>
#include <sstream>

//#include "PerfUtil.h"
//#include "windows.h"

//temp
//double g_addFileTime = 0;
//double g_compressTime = 0;

namespace zp
{

const u32 HASH_TABLE_SCALE = 4;
const u32 MIN_HASH_BITS = 8;
const u32 MIN_HASH_TABLE_SIZE = (1<<MIN_HASH_BITS);
const u32 MAX_HASH_TABLE_SIZE = 0x100000;
const u32 MIN_CHUNK_SIZE = 0x1000;

const u32 HASH_SEED = 131;

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
Package::Package(const Char* filename, bool readonly, bool readFilename)
	: m_stream(NULL)
	, m_fileData(cocos2d::Data::Null)
	, m_hashBits(MIN_HASH_BITS)
	, m_packageEnd(0)
	, m_hashMask(0)
	, m_readonly(readonly)
	, m_lastSeekFile(NULL)
	, m_dirty(false)
{
#ifdef _ZP_WIN32_THREAD_SAFE
	::InitializeCriticalSection(&m_cs);
#endif

	//require filename to modify package
	if (!readFilename && !readonly)
	{
		return;
	}
	if (!readonly)
	{
		return;
	}

	//m_stream = Fopen(filename, _T("rb"));

	m_fileData = cocos2d::FileUtils::getInstance()->getDataFromFile(filename);
	if (m_fileData.isNull())
	{
		return;
	}

	if (!readHeader() || !readFileEntries())
	{
		goto Error;
	}
	if (readFilename && !readFilenames())
	{
		goto Error;
	}
	if (!buildHashTable())
	{
		goto Error;
	}
	m_packageFilename = filename;
	if (!readonly)
	{
		//for compress output
		m_compressBuffer.resize(m_header.chunkSize);
		m_chunkData.resize(m_header.chunkSize);
	}
	return;
Error:
	if (m_stream != NULL)
	{
		fclose(m_stream);
		m_stream = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
Package::~Package()
{
	if (m_stream != NULL)
	{
		removeDeletedEntries();
		fclose(m_stream);
	}
#ifdef _ZP_WIN32_THREAD_SAFE
	::DeleteCriticalSection(&m_cs);
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::valid() const
{
	return !m_fileData.isNull();
	//return (m_stream != NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::readonly() const
{
	return m_readonly;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
const Char* Package::packageFilename() const
{
	return m_packageFilename.c_str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::hasFile(const Char* filename) const
{
	SCOPE_LOCK;

	return (getFileIndex(filename) >= 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
IReadFile* Package::openFile(const Char* filename)
{
	SCOPE_LOCK;

	int fileIndex = getFileIndex(filename);
	if (fileIndex < 0)
	{
		return NULL;
	}
	FileEntry& entry = getFileEntry(fileIndex);
	if ((entry.flag & FILE_COMPRESS) == 0)
	{
		return new File(this, entry.byteOffset, entry.packSize, entry.flag, entry.nameHash);
	}
	u32 chunkSize = entry.chunkSize == 0 ? m_header.chunkSize : entry.chunkSize;
	CompressedFile* file = new CompressedFile(this, entry.byteOffset, entry.packSize, entry.originSize,
												chunkSize, entry.flag, entry.nameHash);
	if ((file->flag() & FILE_DELETE) != 0)
	{
		delete file;
		file = NULL;
	}
	return file;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Package::closeFile(IReadFile* file)
{
	SCOPE_LOCK;

	if ((file->flag() & FILE_COMPRESS) == 0)
	{
		delete static_cast<File*>(file);
	}
	else
	{
		delete static_cast<CompressedFile*>(file);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::getFileInfo(u32 index, Char* filenameBuffer, u32 filenameBufferSize, u32* fileSize,
							u32* packSize, u32* flag, u32* availableSize, u64* contentHash) const
{
	SCOPE_LOCK;

	if (index >= m_filenames.size())
	{
		return false;
	}
	if (filenameBuffer != NULL)
	{
		//Strcpy(filenameBuffer, filenameBufferSize, m_filenames[index].c_str());
		//strncpy(filenameBuffer, m_filenames[index].c_str(), filenameBufferSize);
		//filenameBuffer[filenameBufferSize - 1] = 0;
	}
	const FileEntry& entry = getFileEntry(index);
	if (fileSize != NULL)
	{
		*fileSize = entry.originSize;
	}
	if (packSize != NULL)
	{
		*packSize = entry.packSize;
	}
	if (flag != NULL)
	{
		*flag = entry.flag;
	}
	if (availableSize != NULL)
	{
		*availableSize = entry.availableSize;
	}
	if (contentHash != NULL)
	{
		*contentHash = entry.contentHash;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::getFileInfo(const Char* filename, u32* fileSize, u32* packSize, u32* flag,
						u32* availableSize, u64* contentHash) const
{
	SCOPE_LOCK;

	int fileIndex = getFileIndex(filename);
	if (fileIndex < 0)
	{
		return false;
	}
	const FileEntry& entry = getFileEntry(fileIndex);
	if (fileSize != NULL)
	{
		*fileSize = entry.originSize;
	}
	if (packSize != NULL)
	{
		*packSize = entry.packSize;
	}
	if (flag != NULL)
	{
		*flag = entry.flag;
	}
	if (availableSize != NULL)
	{
		*availableSize = entry.availableSize;
	}
	if (contentHash != NULL)
	{
		*contentHash = entry.contentHash;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
u32 Package::getFileUserDataSize() const
{
	assert(m_header.fileEntrySize >= sizeof(FileEntry));
	return m_header.fileEntrySize - sizeof(FileEntry);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::writeFileUserData(const Char* filename, const u8* data, u32 dataLen)
{
	SCOPE_LOCK;

	if (dataLen > getFileUserDataSize())
	{
		return false;
	}
	int fileIndex = getFileIndex(filename);
	if (fileIndex < 0)
	{
		return false;
	}
	FileEntry& entry = getFileEntry(fileIndex);
	memcpy(&entry + 1, data, dataLen);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::readFileUserData(const Char* filename, u8* data, u32 dataLen)
{
	SCOPE_LOCK;

	if (dataLen > getFileUserDataSize())
	{
		return false;
	}
	int fileIndex = getFileIndex(filename);
	if (fileIndex < 0)
	{
		return false;
	}
	FileEntry& entry = getFileEntry(fileIndex);
	memcpy(data, &entry + 1, dataLen);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::readHeader()
{
	u64 packageSize = m_fileData.getSize();

	u8 fileHeader[sizeof(zp::PackageHeader)] = { 0 };
	memcpy(fileHeader, m_fileData.getBytes(), sizeof(zp::PackageHeader));
	m_header = *((zp::PackageHeader*)fileHeader);

	if (m_header.sign != PACKAGE_FILE_SIGN
		|| m_header.headerSize != sizeof(PackageHeader)
		|| m_header.fileEntryOffset < m_header.headerSize
		|| m_header.fileEntryOffset + m_header.allFileEntrySize > packageSize
		|| m_header.filenameOffset < m_header.fileEntryOffset + m_header.allFileEntrySize
		|| m_header.filenameOffset + m_header.allFilenameSize > packageSize
		|| m_header.chunkSize < MIN_CHUNK_SIZE)
	{
		return false;
	}
	if (m_header.version != CURRENT_VERSION && !m_readonly)
	{
		return false;
	}
	if (m_header.fileEntrySize == 0)
	{
		m_header.fileEntrySize = sizeof(FileEntry);
	}
	if (m_header.fileEntrySize < sizeof(FileEntry))
	{
		return false;
	}
	m_packageEnd = m_header.filenameOffset + m_header.allFilenameSize;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::readFileEntries()
{
	m_fileEntries.resize(m_header.fileCount * m_header.fileEntrySize);
	if (m_header.fileCount == 0)
	{
		return true;
	}

	u8* f_b = m_fileData.getBytes();
	f_b = f_b + m_header.fileEntryOffset;

	if (m_header.allFileEntrySize == m_header.fileCount * m_header.fileEntrySize)
	{
		//not compressed
		memcpy(&m_fileEntries[0], f_b, m_header.allFileEntrySize);
	}
	else
	{
		vector<u8> srcBuffer(m_header.allFileEntrySize);
		memcpy(&srcBuffer[0], f_b, m_header.allFileEntrySize);
		u32 dstBufferSize = m_header.fileCount * m_header.fileEntrySize;
		int ret = uncompress(&m_fileEntries[0], &dstBufferSize, &srcBuffer[0], m_header.allFileEntrySize);
		if (ret != Z_OK || dstBufferSize != m_header.fileCount * m_header.fileEntrySize)
		{
			return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::readFilenames()
{
	if (m_fileEntries.empty())
	{
		return true;
	}
	if (m_header.allFilenameSize == 0)
	{
		return false;
	}

	u8* f_b = m_fileData.getBytes();
	f_b = f_b + m_header.filenameOffset;

	vector<u8> dstBuffer(m_header.originFilenamesSize);
	if (m_header.allFilenameSize == m_header.originFilenamesSize)
	{
		//not compressed
		memcpy(&dstBuffer[0], f_b, m_header.allFilenameSize);
	}
	else
	{
		vector<u8> tempBuffer(m_header.allFilenameSize);
		memcpy(&tempBuffer[0], f_b, m_header.allFilenameSize);
		u32 originSize = m_header.originFilenamesSize;
		int ret = uncompress(&dstBuffer[0], &originSize, &tempBuffer[0], m_header.allFilenameSize);
		if (ret != Z_OK || originSize != m_header.originFilenamesSize)
		{
			return false;
		}
	}
	
	String names;
	names.assign((Char*)&dstBuffer[0], m_header.originFilenamesSize / sizeof(Char));
	
	u32 fileCount = getFileCount();
	m_filenames.resize(fileCount);
	
	IStringStream iss(names, IStringStream::in);
	for (u32 i = 0; i < fileCount; ++i)
	{
		Char out[MAX_FILENAME_LEN];
		iss.getline(out, MAX_FILENAME_LEN);
		m_filenames[i] = out;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void Package::removeDeletedEntries()
{
	if (m_readonly)
	{
		return;
	}
	assert(getFileCount() == m_filenames.size());

	//m_header.fileCount and m_header.allFilenameSize will not change
	vector<String>::iterator nameIter = m_filenames.begin();
	u32 fileCount = getFileCount();
	for (u32 i = 0; i < fileCount;)
	{
		FileEntry& entry = getFileEntry(i);
		if ((entry.flag & FILE_DELETE) != 0)
		{
			std::vector<u8>::iterator eraseBegin = m_fileEntries.begin() + i * m_header.fileEntrySize;
			m_fileEntries.erase(eraseBegin, eraseBegin + m_header.fileEntrySize);
			nameIter = m_filenames.erase(nameIter);
			m_dirty = true;
			--fileCount;
			continue;
		}
		++i;
		++nameIter;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::buildHashTable()
{
	u32 requireSize = getFileCount() * HASH_TABLE_SCALE;
	u32 tableSize = MIN_HASH_TABLE_SIZE;
	m_hashBits = MIN_HASH_BITS;
	while (tableSize < requireSize)
	{
		if (tableSize >= MAX_HASH_TABLE_SIZE)
		{
			return false;
		}
		tableSize *= 2;
		++m_hashBits;
	}
	m_hashMask = (1 << m_hashBits) - 1;

	bool wrong = false;
	m_hashTable.clear();
	m_hashTable.resize(tableSize, -1);
	u32 fileCount = getFileCount();
	for (u32 i = 0; i < fileCount; ++i)
	{
		const FileEntry& currentEntry = getFileEntry(i);
		u32 index = (currentEntry.nameHash & m_hashMask);
		while (m_hashTable[index] != -1)
		{
			const FileEntry& conflictEntry = getFileEntry(m_hashTable[index]);
			if (!wrong && (conflictEntry.flag & FILE_DELETE) == 0
				&& conflictEntry.nameHash == currentEntry.nameHash)
			{
				wrong = true;
			}
			if (++index >= tableSize)
			{
				index = 0;
			}
		}
		m_hashTable[index] = i;
	}
	return !wrong;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Package::getFileIndex(const Char* filename) const
{
	u64 nameHash = stringHash(filename, HASH_SEED);

	return getFileIndex(nameHash);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
int Package::getFileIndex(u64 nameHash) const
{
	u32 hashIndex = (nameHash & m_hashMask);
	int fileIndex = m_hashTable[hashIndex];
	while (fileIndex >= 0)
	{
		const FileEntry& entry = getFileEntry(fileIndex);
		if (entry.nameHash == nameHash)
		{
			if ((entry.flag & FILE_DELETE) != 0)
			{
				return -1;
			}
			return fileIndex;
		}
		if (++hashIndex >= m_hashTable.size())
		{
			hashIndex = 0;
		}
		fileIndex = m_hashTable[hashIndex];
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
u64 Package::stringHash(const Char* str, u32 seed) const
{
	u64 out = 0;
	while (*str)
	{
		Char ch = *(str++);
		if (ch == _T('\\'))
		{
			ch = _T('/');
		}
	#if (ZP_CASE_SENSITIVE)
		out = out * seed + ch;
	#else
		#if defined ZP_USE_WCHAR
			out = out * seed + towlower(ch);
		#else
			out = out * seed + tolower(ch);
		#endif
	#endif
	}
	return out;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
u32 Package::getFileAvailableSize(u64 nameHash) const
{
	int fileIndex = getFileIndex(nameHash);
	if (fileIndex < 0)
	{
		return 0;
	}
	const FileEntry& entry = getFileEntry(fileIndex);
	return entry.availableSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Package::setFileAvailableSize(u64 nameHash, u32 size)
{
	int fileIndex = getFileIndex(nameHash);
	if (fileIndex < 0)
	{
		return false;
	}
	FileEntry& entry = getFileEntry(fileIndex);
	entry.availableSize = size;
	m_dirty = true;
	return true;
}

}
