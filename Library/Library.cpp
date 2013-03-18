#include "Library.h"
#include <hash_map>
#include <string>

std::hash_map<std::string, Library*> mLibNames;

ExposeLibrary::ExposeLibrary(const std::string &strLibName, Library* lib)
{
	mLibNames[strLibName] = lib;
}

Library* Library::GetLibrary(const std::string &strLibName)
{
	return mLibNames[strLibName];
}