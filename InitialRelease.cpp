//void BusyWaitingApproach() {
//	WIN32_FIND_DATAW file_data;
//	// CR: 'firstFile' is a resource leak - you opened the handle, but did you close it? (in every loop iteration!)
//	HANDLE firstFile = FindFirstFileW(FILE_PATH_ALL, &file_data);
//	if (firstFile == INVALID_HANDLE_VALUE)
//	{
//		std::cout << "Invalid handle value Error code = " << GetLastError() << std::endl;
//		CloseHandle(firstFile);
//		return;
//	}
//	// CR: If 'FindNextFile' fails, don't you want to know about it, and know why?
//	while (BOOL bFileResult = FindNextFileW(firstFile, &file_data))
//	{
//		//	std::wstring filter_data(file_data.cFileName);
//			// CR: What if there is a file called "list.of.my.dlls.txt", for example?
//		if (!bFileResult)
//		{
//			std::cout << "FindNextFileW Failed with error code = " << GetLastError() << std::endl;
//			continue;
//		}
//		std::wstring filepath(FILE_PATH), filename(file_data.cFileName);//filepath will be used for PathFindExtensionW function, the filename will be used for file whitelisting
//		filepath.append(file_data.cFileName);
//
//		auto file_extension = PathFindExtensionW(filepath.c_str());
//		if (file_extension)
//		{
//			if (!wcscmp(file_extension, L".dll"))//Only accept dll files base on their file extension
//			{
//				bool isWhitelisted = false;
//				for (int i = 0; i < sizeof(whitelisted_dlls) / sizeof(whitelisted_dlls[0]); i++)
//				{
//					if (filename.find(whitelisted_dlls[i]) != filename.npos)
//					{
//						isWhitelisted = true;
//						break;
//					}
//				}
//				if (!isWhitelisted) {
//					std::wcout << filepath << ": " << file_extension << std::endl;
//					// CR: Prefer using (W) API to support UNICODE. Here and everywhere else.
//					HMODULE CurrentLoadedDll = LoadLibraryW(filepath.c_str());
//					if (!CurrentLoadedDll) {
//						// CR: Why no using 'GetLastError'? Here and everywhere follows
//						std::cout << "Failed to load library error code = " << GetLastError() << std::endl;
//						CloseHandle(firstFile);
//					}
//					else // CR: Can you see the resource leak if an exception is thrown somewhere from this point?
//					{
//						std::cout << "Library Loaded successfully!" << std::endl;
//						start  ostart = (start)GetProcAddress(CurrentLoadedDll, "start@24");//start@24 might not be static for other dll's 
//						if (!ostart) {
//							std::cout << "Exported function start not found! error code = " << GetLastError() << std::endl;
//							// CR: Resource leak! Can you determine which?
//							CloseHandle(firstFile);
//							return;
//						}
//						uintptr_t SECRET = (uintptr_t)GetProcAddress(CurrentLoadedDll, "SECRET");//SECRET as well
//						if (!SECRET) {
//							std::cout << "Exported function SECRET not found! error code =" << GetLastError() << std::endl;
//							// CR: Resource leak! Can you determine which?
//							CloseHandle(firstFile);
//							return;
//						}
//
//						std::cout << "Calling start function with the correct passphrase ..." << std::endl;
//						const char* passphrase = *reinterpret_cast<const char**>(SECRET);
//						std::cout << "passphrase is: " << passphrase << std::endl;
//						ostart(SECRET);//Pass the value of the address or the pointer to passphrase
//
//						Sleep(100);//Short delay is needed as the program is too fast
//
//						if (FreeLibrary(CurrentLoadedDll))//Unload dll
//							std::wcout << "File [" << filename << "] is Unloaded successfully!" << std::endl;
//						else
//							std::wcout << "Failed to Unload file [" << filename << "] error code = " << GetLastError() << std::endl;
//
//						if (DeleteFileW(filepath.c_str()))//Delete the dll
//							std::wcout << "File [" << filename << "] is deleted successfully!" << std::endl;
//						else
//							std::wcout << "Failed to delete file [" << filename << "] error code = " << GetLastError() << std::endl;
//					}
//					Sleep(100);
//				}
//			}
//		}
//
//		//if (filter_data.find(".dll") != filter_data.npos)//only store dll files
//		//{
//		//	bool isWhitelisted = false;
//		//	for (int i = 0; i < sizeof(whitelisted_dlls) / sizeof(whitelisted_dlls[0]); i++)
//		//	{
//		//		if (filter_data.find(whitelisted_dlls[i]) != filter_data.npos)
//		//		{
//		//			isWhitelisted = true;
//		//			break;
//		//		}
//		//	}
//		//	if (!isWhitelisted) {
//		//		std::string filepath(FILE_PATH);
//		//		filepath.append(filter_data.c_str());
//		//		// CR: Prefer using (W) API to support UNICODE. Here and everywhere else.
//		//		HMODULE CurrentLoadedDll = LoadLibraryA(filepath.c_str());
//		//		if (!CurrentLoadedDll) {
//		//			// CR: Why no using 'GetLastError'? Here and everywhere follows
//		//			std::cout << "Failed to load library error code = " << GetLastError() << std::endl;
//		//			CloseHandle(firstFile);
//		//		}
//		//		else // CR: Can you see the resource leak if an exception is thrown somewhere from this point?
//		//		{
//		//			std::cout << "Library Loaded successfully!" << std::endl;
//		//			start  ostart = (start)GetProcAddress(CurrentLoadedDll, "start@24");//start@24 might not be static for other dll's 
//		//			if (!ostart) {
//		//				std::cout << "Exported function start not found! error code = " << GetLastError() << std::endl;
//		//				// CR: Resource leak! Can you determine which?
//		//				CloseHandle(firstFile);
//		//				return;
//		//			}
//		//			uintptr_t SECRET = (uintptr_t)GetProcAddress(CurrentLoadedDll, "SECRET");//SECRET as well
//		//			if (!SECRET) {
//		//				std::cout << "Exported function SECRET not found! error code =" << GetLastError() << std::endl;
//		//				// CR: Resource leak! Can you determine which?
//		//				CloseHandle(firstFile);
//		//				return;
//		//			}
//
//		//			std::cout << "Calling start function with the correct passphrase ..." << std::endl;
//		//			const char* passphrase = *reinterpret_cast<const char**>(SECRET);
//		//			std::cout << "passphrase is: " << passphrase << std::endl;
//		//			ostart(SECRET);//Pass the value of the address or the pointer to passphrase
//
//		//			Sleep(100);//Short delay is needed as the program is too fast
//
//		//			if (FreeLibrary(CurrentLoadedDll))//Unload dll
//		//				std::cout << "File [" << filter_data << "] is Unloaded successfully!" << std::endl;
//		//			else
//		//				std::cout << "Failed to Unload file [" << filter_data << "] error code = " << GetLastError() << std::endl;
//
//		//			if (DeleteFileA(filepath.c_str()))//Delete the dll
//		//				std::cout << "File [" << filter_data << "] is deleted successfully!" << std::endl;
//		//			else
//		//				std::cout << "Failed to delete file [" << filter_data << "] error code = " << GetLastError() << std::endl;
//		//		}
//		//		Sleep(100);
//		//	}
//		//}
//	}
//
//	CloseHandle(firstFile);
//
//	std::cin.get();
//}
