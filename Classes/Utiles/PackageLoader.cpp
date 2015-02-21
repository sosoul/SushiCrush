/*
 * PackageLoader.cpp
 *
 *  Created on: 2014-11-18
 *      Author: Administrator
 */

#include "PackageLoader.h"
#include "spine/spine-cocos2dx.h"

USING_NS_CC;

namespace hex
{

	void PackageLoader::genUIPackage(const std::string& packageName)
	{
#if(BINARY_LOAD)
		std::string packagePath = packageName + ".hex";
		std::string fullpath = FileUtils::getInstance()->fullPathForFilename(packagePath);

		zp::IPackage * t_pack = zp::open(fullpath.c_str(), zp::OPEN_READONLY);

		zp::IReadFile * plist_read = nullptr;
		zp::IReadFile * png_read = nullptr;
		int cnt = 0;
		do
		{
			std::string fileName = packageName + StringUtils::toString(cnt);
			std::string fileNamePlist = fileName + ".plist";
			std::string fileNamePng = fileName + ".png";

			plist_read = t_pack->openFile(fileNamePlist.c_str());
			if (plist_read == nullptr)
			{
				break;
			}

			png_read = t_pack->openFile(fileNamePng.c_str());
			if (png_read == nullptr)
			{
				break;
			}

			unsigned char* plistBuffer = new unsigned char[plist_read->size() + 1];
			plist_read->read((zp::u8*)plistBuffer, plist_read->size());
			plistBuffer[plist_read->size()] = '\0';

			unsigned char* textureBuffer = new unsigned char[png_read->size()];
			png_read->read((zp::u8*)textureBuffer, png_read->size());
			Image* image = new Image();
			bool bRet = image->initWithImageData(textureBuffer, png_read->size());
			Texture2D * texture = nullptr;
			if (bRet)
			{
				texture = Director::getInstance()->getTextureCache()->addImage(image, fileNamePng);
				std::string pliststr = (const char *)plistBuffer;
				SpriteFrameCache::getInstance()->addSpriteFramesWithFileContent(pliststr, texture);
			}

			++cnt;

			t_pack->closeFile(plist_read);
			t_pack->closeFile(png_read);

		} while (1);

		zp::close(t_pack);
#else
		std::string fileNamePlist = packageName + ".plist";

		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(fileNamePlist);
#endif
	}
}
