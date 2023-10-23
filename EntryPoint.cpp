//#include "MemoryAPI.hpp"
//#include "Int3Hook.hpp"
//#include "FunctionHelper.hpp"
//
////Passphrase: qwerty2023
//
//typedef int(__cdecl* start)(int passphraseaddress);//return value doesn't seem to hold anything? it's just a ptr not a string or whatever
////std::vector<Int3_hook*> Int3Hooks; //My first approach
//////veh hook is what we're going to use
////LONG WINAPI MyExceptionHandler(_EXCEPTION_POINTERS* ExceptionInfo)
////{
////	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
////	{
////		for (auto target : Int3Hooks)
////		{
////#ifdef _WIN64
////			if (ExceptionInfo->ContextRecord->Rip == target->GetAddress())
////			{
////				ExceptionInfo->ContextRecord->Rip = reinterpret_cast<DWORD64>(target->GetCallback());
////				return EXCEPTION_CONTINUE_EXECUTION;
////			}
////#else
////			if (ExceptionInfo->ContextRecord->Eip == target->GetAddress())
////			{
////				ExceptionInfo->ContextRecord->Eip = reinterpret_cast<DWORD>(target->GetCallback());
////				return EXCEPTION_CONTINUE_EXECUTION;
////			}
////#endif
////		}
////	}
////	return EXCEPTION_CONTINUE_SEARCH;
////}
//
//start ostart = 0;
//uintptr_t SECRET = 0;
//#include <random>
//std::string RandomANString(int length /*= 0*/)
//{
//	auto alphabet = "1234567890";
//	static std::random_device rd;
//	static std::uniform_int_distribution<> dist(0, strlen(alphabet) - 2);
//	static std::uniform_int_distribution<> dist_len(10, 30);
//	std::string result;
//
//	// Get random string length
//	if (length == 0)
//		length = dist_len(rd);
//
//	for (int i = 0; i < length; i++)
//		result.push_back(alphabet[dist(rd)]);
//
//	return result;
//}
//
//#define encConst(x) x
//#define encStr(x) x
//void SpoofPC()
//{
//	const std::string registryKeyPath = encStr("SYSTEM\\ControlSet001\\Enum\\PCI");
//	HKEY hKey;
//	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, registryKeyPath.c_str(), 0, KEY_READ, &hKey);
//	if (result != ERROR_SUCCESS) {
//		std::cerr << encConst("ErrorCode: 0x6 \nFailed to Open Serial key: ") << result << std::endl;
//		system(encConst("pause"));
//		return;
//	}
//
//	DWORD index = 0;
//	TCHAR subkeyName[MAX_PATH];
//	DWORD subkeyNameLength = MAX_PATH;
//	while (RegEnumKeyEx(hKey, index, subkeyName, &subkeyNameLength, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
//		HKEY subkey;
//		result = RegOpenKeyEx(hKey, subkeyName, 0, KEY_READ | KEY_WRITE, &subkey);
//		if (result == ERROR_SUCCESS)
//		{
//			DWORD subindex = 0;
//			TCHAR subsubkeyName[MAX_PATH];
//			DWORD subsubkeyNameLength = MAX_PATH;
//			while (RegEnumKeyEx(subkey, subindex, subsubkeyName, &subsubkeyNameLength, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
//				HKEY subsubkey;
//				result = RegOpenKeyEx(subkey, subsubkeyName, 0, KEY_READ | KEY_WRITE, &subsubkey);
//				if (result == ERROR_SUCCESS)
//				{
//					TCHAR friendlyName[MAX_PATH];
//					DWORD friendlyNameLength = MAX_PATH * sizeof(TCHAR);
//					result = RegQueryValueEx(subsubkey, encConst("FriendlyName"), NULL, NULL, (LPBYTE)friendlyName, &friendlyNameLength);
//					if (result == ERROR_SUCCESS && std::string(friendlyName).find(encConst("@")) == std::string::npos)
//					{
//						std::string currentFriendlyName(friendlyName);
//						std::size_t found = currentFriendlyName.find(encConst("#"));
//						std::string updatedFriendlyName;
//
//						if (found != std::string::npos)
//						{
//							updatedFriendlyName = currentFriendlyName.substr(0, found + 1) + RandomANString(6);
//						}
//						else
//						{
//							updatedFriendlyName = currentFriendlyName + "#" + RandomANString(6);
//						}
//						result = RegSetValueEx(subsubkey, encConst("FriendlyName"), 0, REG_SZ, (BYTE*)updatedFriendlyName.c_str(), updatedFriendlyName.length() + 1);
//						if (result != ERROR_SUCCESS)
//						{
//							std::cerr << encConst("ErrorCode: 0x7 \nFailed to Change Serial: ") << result << std::endl;
//							system(encConst("pause"));
//							RegCloseKey(hKey);
//						}
//						else
//						{
//							std::cout << "Spoofed shit" << std::endl;
//						}
//					}
//					RegCloseKey(subsubkey);
//				}
//				subindex++;
//				subsubkeyNameLength = MAX_PATH;
//			}
//			RegCloseKey(subkey);
//		}
//		index++;
//		subkeyNameLength = MAX_PATH;
//	}
//	std::cout << "Done" << std::endl;
//}
//
//void MainFunction() {
//	WIN32_FIND_DATAA file_data;
//	HANDLE firstFile = FindFirstFileA(FILE_PATH_ALL, &file_data);
//	if (firstFile == INVALID_HANDLE_VALUE)
//	{
//		std::cout << "Invalid handle value Error code = " << GetLastError() << std::endl;
//		return;
//	}
//	while (FindNextFileA(firstFile, &file_data))
//	{
//		std::string filter_data(file_data.cFileName);
//		if (filter_data.find(".dll") != filter_data.npos)//only store dll files
//		{
//			bool isWhitelisted = false;
//			for (int i = 0; i < sizeof(whitelisted_dlls) / sizeof(whitelisted_dlls[0]); i++)
//			{
//				if (filter_data.find(whitelisted_dlls[i]) != filter_data.npos)
//				{
//					isWhitelisted = true;
//					break;
//				}
//			}
//			if (!isWhitelisted) {
//				std::string filepath(FILE_PATH);
//				filepath.append(filter_data.c_str());
//				HMODULE CurrentLoadedDll = LoadLibraryA(filepath.c_str());
//				if (!CurrentLoadedDll) {
//					std::cout << "Failed to load library" << std::endl;
//				}
//				else
//				{
//					std::cout << "Library Loaded successfully!" << std::endl;
//					ostart = (start)GetProcAddress(CurrentLoadedDll, "start@24");//start@24 might not be static for other dll's 
//					if (!ostart) {
//						std::cout << "Exported function start not found!" << std::endl;
//						return;
//					}
//					SECRET = (uintptr_t)GetProcAddress(CurrentLoadedDll, "SECRET");//SECRET as well
//					if (!SECRET) {
//						std::cout << "Exported function SECRET not found!" << std::endl;
//						return;
//					}
//
//					std::cout << "Calling start function with the correct passphrase ..." << std::endl;
//					const char* passphrase = *reinterpret_cast<const char**>(SECRET);
//					std::cout << "passphrase is: " << passphrase << std::endl;
//					ostart(SECRET);//Pass the value of the address or the pointer to passphrase
//
//					Sleep(100);//Short delay is needed as the program is too fast
//
//					if (FreeLibrary(CurrentLoadedDll))//Unload dll
//						std::cout << "File [" << filter_data << "] is Unloaded successfully!" << std::endl;
//					else
//						std::cout << "Failed to Unload file [" << filter_data << "] error code = " << GetLastError() << std::endl;
//
//					if (DeleteFileA(filepath.c_str()))//Delete the dll
//						std::cout << "File [" << filter_data << "] is deleted successfully!" << std::endl;
//					else
//						std::cout << "Failed to delete file [" << filter_data << "] error code = " << GetLastError() << std::endl;
//				}
//				Sleep(100);
//			}
//		}
//	}
//}
//
//int main(void)
//{
//	SpoofPC();
//	while (true)
//	{
//		__try {//Not really necessary but it's good to figure out where the program might crash
//			MainFunction();
//		}
//		__except (EXCEPTION_EXECUTE_HANDLER)
//		{
//			std::cout << "EXCEPTION" << std::endl;
//		}
//	}
//	return 0;
//}