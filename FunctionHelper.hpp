#pragma once
#include "Includes.hpp"



const wchar_t* whitelisted_dlls[7] = { L"libatomic-1.dll",L"libgcc_s_dw2-1.dll",L"libgomp-1.dll",L"libquadmath-0.dll",L"libssp-0.dll",L"libstdc++-6.dll",L"libwinpthread-1.dll" };//Hard-coded whitelisted dll's because secretdll is not statically linked

//Our WINAPI file filter
bool IsValidDLLFile(std::wstring file)
{
	auto file_extension = PathFindExtensionW(file.c_str());
	if (file_extension)
	{
		return !wcscmp(file_extension, L".dll");
	}
	return false;
}

//Our function helper to filter whitelisted dll's
bool IsWhitelistedDLL(std::wstring file)
{
	bool isWhitelisted = false;
	for (int i = 0; i < sizeof(whitelisted_dlls) / sizeof(whitelisted_dlls[0]); i++)
	{
		if (file.find(whitelisted_dlls[i]) != file.npos)
		{
			isWhitelisted = true;
			break;
		}
	}
	return isWhitelisted;
}

//-1 meaning file doesn't exist according to msdn
BOOL FileExists(std::wstring szPath)
{
	return GetFileAttributes(szPath.c_str()) != INVALID_FILE_ATTRIBUTES;
}