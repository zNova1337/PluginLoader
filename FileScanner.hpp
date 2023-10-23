#pragma once


class FileScanner
{
private:
	HANDLE hFile = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA file_data;
	ZN_STATUS zStatus = ZN_SUCCESS;
	std::wstring wPath;


public:

	FileScanner(std::wstring path) : wPath(path)
	{

		this->wPath.append(L"*");//Append asterisk meaning everything inside the folder
		std::wcout << wPath << std::endl;
		this->hFile = FindFirstFileW(this->wPath.c_str(), &file_data);
		if (this->hFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "FindFirstFileW failed with error code: " << GetLastError() << std::endl;
			this->zStatus = FILE_FIND_FIRST_FILE_ERROR;
		}
		this->wPath.erase(this->wPath.end() - 1, this->wPath.end());//Erase the appended * for later use
	}
	~FileScanner() {}
	void ScanFiles() {
		if (this->zStatus == ZN_SUCCESS) {
			BOOL bFileResult = FindNextFile(this->hFile, &this->file_data);
			do
			{
				if (!bFileResult)
				{
					std::cout << "FindNextFile failed with error code: " << GetLastError() << std::endl;
					break;
				}

				std::wstring filepath(this->wPath.c_str());//filepath will be used for PathFindExtensionW function, the filename will be used for file whitelisting

				

				filepath.append(file_data.cFileName);

				std::wcout << filepath << std::endl;

				if (IsValidDLLFile(filepath))//Check if valid dll file
				{
					if (!IsWhitelistedDLL(filepath))//Check if whitelisted dll
					{
						std::unique_ptr< DllLoader > m_pDllLdr = std::make_unique < DllLoader >(filepath);

						auto DllExecuteResult = m_pDllLdr->ExecuteDllFunction();
						if (DllExecuteResult != ZN_SUCCESS)
						{
							std::cout << "[!]DllExecution failed with error reason: " << GetZNStatusReason(DllExecuteResult) << std::endl;
							//this->CleanUp();
						}
					}
				}
			} while (bFileResult = FindNextFile(this->hFile, &this->file_data));

			if (!FindClose(this->hFile))
			{
				std::cout << "FindClose failed with error code: " << GetLastError() << std::endl;
			}
		}
		else
		{
			std::cout << "ScanFiles Failed with error reason: " << GetZNStatusReason(this->zStatus) << std::endl;
		}
	}
};
