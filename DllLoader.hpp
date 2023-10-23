#pragma once

class DllLoader
{
	HMODULE hModule = nullptr;
	std::wstring wFile;
	ZN_STATUS zStatus = ZN_SUCCESS;

	template <typename T, typename ...Args>
	T call_function(uintptr_t Address, Args... args) //Simple template wrapper in-case we need it variadically
	{
		typedef T(__stdcall* tFn)(Args...);
		tFn oFn = (tFn)(Address);
		return oFn(args...);
	}


public:

	DllLoader(std::wstring dll) : wFile(dll.c_str())
	{
		
	}

	~DllLoader() {}

	ZN_STATUS ExecuteDllFunction() {

		this->hModule = LoadLibraryW(wFile.c_str());
		if (!this->hModule)
		{
			std::cout << "LoadLibrary failed with error code: " << GetLastError() << std::endl;
			this->zStatus = ZN_LOADLIBRARY_FAILED;
			return this->zStatus;
		}
		else {
			std::wcout << "[+]DllFile: " << wFile << " Loaded! : " << std::hex << this->hModule << std::endl;
		}

		if (this->zStatus == ZN_SUCCESS) {
			uintptr_t fnAddress = (uintptr_t)GetProcAddress(this->hModule, "start@24");
			if (!fnAddress) {
				std::cout << "Exported function start not found! error code = " << GetLastError() << std::endl;
				return ZN_START_FUNCTION_NOT_FOUND;
			}
			uintptr_t SECRET = (uintptr_t)GetProcAddress(this->hModule, "SECRET");
			if (!SECRET) {
				std::cout << "Exported function SECRET not found! error code =" << GetLastError() << std::endl;
				return ZN_SECRET_FUNCTION_NOT_FOUND;
			}

			const char* passphrase = *reinterpret_cast<const char**>(SECRET);
			std::cout << "passphrase is: " << passphrase << std::endl;


			int iCall = call_function<int,uintptr_t>(fnAddress, SECRET);//not a string, but an int* since it points to the libgcc runtime sdk libraries
			if (iCall) {
			//Sleep(1000);//Short delay is needed as the program is too fast, only add this if you want to see the console output moving


			if (!FreeLibrary(this->hModule))
			{
				std::cout << "FreeLibrary failed with error code: " << GetLastError() << std::endl;
				return ZN_FREE_LIBRARY_ERROR;
			}

			if (!DeleteFile(wFile.c_str()))
			{
				std::cout << "DeleteFile failed with error code: " << GetLastError() << std::endl;
				return ZN_DELETE_FILE_ERROR;
			}

				std::cout << "[~]DllFile unloaded & deleted" << std::endl;
				return ZN_SUCCESS;
			}
		
		}
		return ZN_UNKNOWN_ERROR;
	}


};