// : Defines the exported functions for the DLL application.
//
#include <string>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Library/Library.h"

DWORD temp;
void WriteInt(HANDLE handle, int out)
{
	WriteFile(handle, (char*)&out, sizeof(int), &temp, NULL);
}

void WriteString(HANDLE handle, std::string out)
{
	WriteInt(handle, out.size());
	WriteFile(handle, out.c_str(), out.size(), &temp, NULL);
}

void WriteFloat(HANDLE handle, float out)
{
	WriteFile(handle, (char*)&out, sizeof(float), &temp, NULL);
}

class A3TS3 : public Library
{
	virtual std::string ProcessFunction(const std::string &strFunc, const std::vector<std::string> &vParams)
	{
		if(strFunc == "ppupdate")//player position update
		{
			WaitNamedPipeA("\\\\.\\pipe\\A3TS3_pipe", 100);
			DWORD error = ERROR_FILE_NOT_FOUND;
			HANDLE pipe = INVALID_HANDLE_VALUE;
			unsigned char attempts = 0;
			while(error == ERROR_FILE_NOT_FOUND && attempts < 10)
			{
				pipe = CreateFileA("\\\\.\\pipe\\A3TS3_pipe", GENERIC_WRITE, 2, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				error = GetLastError();
				Sleep(10);
			}
			if(pipe == INVALID_HANDLE_VALUE)
			{
				return "no pipe";
			}
			char opcode = 0;
			WriteFile(pipe, &opcode, 1, &temp, NULL);
			WriteString(pipe, vParams[0]);
			WriteFloat(pipe, atof(vParams[1].c_str()));
			WriteFloat(pipe, atof(vParams[2].c_str()));
			WriteFloat(pipe, atof(vParams[3].c_str()));
			CloseHandle(pipe);
		}
		else if(strFunc == "spupdate") //self positon and orientation update
		{
			WaitNamedPipeA("\\\\.\\pipe\\A3TS3_pipe", 100);
			DWORD error = ERROR_FILE_NOT_FOUND;
			HANDLE pipe = INVALID_HANDLE_VALUE;
			unsigned char attempts = 0;
			while(error == ERROR_FILE_NOT_FOUND && attempts < 10)
			{
				pipe = CreateFileA("\\\\.\\pipe\\A3TS3_pipe", GENERIC_WRITE, 2, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				error = GetLastError();
				Sleep(10);
			}
			if(pipe == INVALID_HANDLE_VALUE)
			{
				return "no pipe";
			}
			char opcode = 2;
			WriteFile(pipe, &opcode, 1, &temp, NULL);
			WriteFloat(pipe, atof(vParams[0].c_str()));
			WriteFloat(pipe, atof(vParams[1].c_str()));
			WriteFloat(pipe, atof(vParams[2].c_str()));
			WriteFloat(pipe, atof(vParams[3].c_str()));
			WriteFloat(pipe, atof(vParams[4].c_str()));
			WriteFloat(pipe, atof(vParams[5].c_str()));
			WriteFloat(pipe, atof(vParams[6].c_str()));
			WriteFloat(pipe, atof(vParams[7].c_str()));
			WriteFloat(pipe, atof(vParams[8].c_str()));
			CloseHandle(pipe);
		}
		else if(strFunc == "playerid") //self player id update
		{
			WaitNamedPipeA("\\\\.\\pipe\\A3TS3_pipe", 100);
			DWORD error = ERROR_FILE_NOT_FOUND;
			HANDLE pipe = INVALID_HANDLE_VALUE;
			unsigned char attempts = 0;
			while(error == ERROR_FILE_NOT_FOUND && attempts < 10)
			{
				pipe = CreateFileA("\\\\.\\pipe\\A3TS3_pipe", GENERIC_WRITE, 2, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				error = GetLastError();
				Sleep(10);
			}
			if(pipe == INVALID_HANDLE_VALUE)
			{
				return "no pipe";
			}
			char opcode = 1;
			WriteFile(pipe, &opcode, 1, &temp, NULL);
			WriteString(pipe, vParams[0]);
			CloseHandle(pipe);
		}
		return "";
	}
};

ExposeLibrary expose_a3ts3("A3TS3", new A3TS3());

extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function)
	{
		std::string strIn(function);
		std::string strOut;
		if(strIn == "exists")
		{
			strOut = "yes";
		};

		size_t cursor = 0;
		size_t space = strIn.find(' ', cursor);
		std::string strLib = strIn.substr(cursor, space-cursor);
		OutputDebugStringA((strLib + "end").c_str());
		cursor = space+1;

		Library *lib = Library::GetLibrary(strLib);
		if(lib)
		{
			space = strIn.find(' ', cursor);
			std::string strFunc = strIn.substr(cursor, space-cursor);
			OutputDebugStringA((strFunc + "end").c_str());
			cursor = space+1;

			std::vector<std::string> vParams;
			bool bMore = true;
			while(bMore)
			{
				space = strIn.find(' ', cursor);
				if(space == std::string::npos)
				{
					bMore = false;
				}
				else
				{
					std::string strSize = strIn.substr(cursor, space-cursor);
					cursor = space+1;
					int size = atoi(strSize.c_str());

					std::string strParam = strIn.substr(cursor, size);
					cursor += size;
					vParams.resize(vParams.size()+1);
					vParams[vParams.size()-1] = strParam;
				}
			}

			strOut = lib->ProcessFunction(strFunc, vParams);
		}

		memcpy(output, strOut.c_str(), strOut.size()+1 > outputSize ? outputSize : strOut.size()+1);
	}
};