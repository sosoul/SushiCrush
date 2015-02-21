#include "zpack.h"
#include "zpPackage.h"
#include "zpFile.h"
#include <fstream>

using namespace std;

namespace zp
{

///////////////////////////////////////////////////////////////////////////////////////////////////
IPackage* open(const Char* filename, u32 flag)
{
	Package* package = new Package(filename, 
									(flag & OPEN_READONLY) != 0,
									(flag & OPEN_NO_FILENAME) == 0);
	if (!package->valid())
	{
		delete package;
		package = NULL;
	}
	return package;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void close(IPackage* package)
{
	delete static_cast<Package*>(package);
}

}
