/*
 * PackageLoader.h
 *
 *  Created on: 2014-11-18
 *      Author: Administrator
 */

#ifndef PACKAGELOADER_H_
#define PACKAGELOADER_H_

#include "zpack.h"
#include "zpPackage.h"

namespace hex
{

#define BINARY_LOAD (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

class PackageLoader
{

public:

	static PackageLoader & instance()
	{
		static PackageLoader packageLoader;

		return packageLoader;
	}

	void genUIPackage(const std::string& packageName);
};

}


#endif /* PACKAGELOADER_H_ */
