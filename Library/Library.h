#pragma once //This isn't going to be x-platform so who gives a fuck?

#include <string>
#include <vector>

class Library
{
	public:
		static Library* GetLibrary(const std::string &strLibName);
		virtual std::string ProcessFunction(const std::string &strFunc, const std::vector<std::string> &vParams) = 0;
};

class ExposeLibrary
{
	public:
		ExposeLibrary(const std::string &strLibName, Library* Lib);
};