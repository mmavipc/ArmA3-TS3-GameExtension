// Ext_Tester.cpp : Defines the entry point for the console application.
//
#include <string>
#include <stdarg.h>
#include <sstream>
#include <iostream>

#pragma comment(lib, "arma3_ts3_gameextension.lib")

extern "C"
{
	void __stdcall RVExtension(char *output, int outputSize, const char *function);
};

std::string uitos(unsigned int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

std::string CallFunc(const std::string &strFunc, unsigned int count, ...)
{
	std::string strCall;
	strCall = strFunc + " ";

	char output[4096];
	va_list vaParams;
	va_start(vaParams, count);
	for(unsigned int i = 0; i < count; i++)
	{
		std::string strParam = (const char*)va_arg(vaParams, const char*);

		strCall += uitos(strParam.size()) + " " + strParam;
	}

	RVExtension(output, 4096, strCall.c_str());

	return output;
}

int main(int argc, char* argv[])
{
	CallFunc("A3TS3 playerid", 1, "2250");
	CallFunc("A3TS3 spupdate", 6, "0", "0", "0", "1", "0", "0");
	char output[4096];
	RVExtension(output, 4096, "TestLib Func1 10 hello 10le3 two4 four4 five");
	return 0;
}