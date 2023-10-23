#pragma once

class EventNotification
{
private:
	HANDLE hFile = INVALID_HANDLE_VALUE;
	uint8_t buffer[1024];
	OVERLAPPED overlapped;
	int FileIndex = -1;//For future usage like monitoring of removal of files
	std::map<uint32_t, std::wstring> MonitoredFiles; //monitored files
	ZN_STATUS zStatus = ZN_SUCCESS;
	FILE_ACTION last_file_action = FILE_ACTION::UNKNOWN; //for debugging purpose
	std::wstring wPath;

	ZN_STATUS NextQueue() {
		auto result = ReadDirectoryChangesW(this->hFile, this->buffer, sizeof(this->buffer), TRUE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &this->overlapped, NULL);//Monitor everything
		if (!result)
		{
			std::cout << "ReadDirectoryChangesW failed with error code: " << GetLastError() << std::endl;
			ZeroMemory(&this->overlapped, sizeof(OVERLAPPED));
			return FILE_READ_DIRECTORY_CHANGES_ERROR;
		}
		return ZN_SUCCESS;
	}

	ZN_STATUS bWait() {
		DWORD result = WaitForSingleObject(this->overlapped.hEvent, 0);
		if (result == WAIT_OBJECT_0)return ZN_SUCCESS;
		return FILE_OBJECT_TIMER_ERROR;
	}

	ZN_STATUS GetOverlappedRes() {
		DWORD dwBytes = 0;
		auto result = GetOverlappedResult(this->hFile, &this->overlapped, &dwBytes, FALSE);
		if (!result)
		{
			std::cout << "GetOverlappedResult failed with error code: " << GetLastError() << std::endl;
			return FILE_GET_OVERLAPPED_RESULT_ERROR;
		}
		return ZN_SUCCESS;
	}
public:

	EventNotification(std::wstring path) : wPath(path)
	{
		std::wcout << "Monitoring file path: " << path << "..." << std::endl;
		ZeroMemory(&this->buffer, sizeof(this->buffer));
		this->hFile = CreateFileW(path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);//Monitor everything
		if (this->hFile == INVALID_HANDLE_VALUE)
		{
			std::cout << "CreateFileW failed with error code: " << GetLastError();
			this->zStatus = FILE_CREATE_FILE_ERROR;
		}
		overlapped.hEvent = CreateEventW(NULL, FALSE, 0, NULL);
		if (overlapped.hEvent == INVALID_HANDLE_VALUE)
		{
			std::cout << "CreateEventW failed with error code: " << GetLastError();
			CloseHandle(this->hFile);
			this->zStatus = FILE_CREATE_EVENT_ERROR;
		}
		else
		{
			this->zStatus = this->NextQueue();
			MonitoredFiles.clear();
		}
	}

	void MonitorFilesAction(ZN_STATUS file_added_callback(std::wstring file)) {
		if (this->zStatus == ZN_SUCCESS) {
			auto zTimer = this->bWait();
			if (zTimer == ZN_SUCCESS) {
				auto zOverlap = this->GetOverlappedRes();
				if (zOverlap != ZN_SUCCESS) {
					std::cout << "zOverlap was: " << GetZNStatusReason(zOverlap) << std::endl;
					return;
				}

				FILE_NOTIFY_INFORMATION* m_FileNotifInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(this->buffer);

				DWORD name_len = m_FileNotifInfo->FileNameLength / sizeof(wchar_t);


				do {
					switch (m_FileNotifInfo->Action)
					{
					case FILE_ACTION_MODIFIED:
						wprintf(L"Modif          to: %.*s\n", name_len, m_FileNotifInfo->FileName);
						//std::wcout << "[Modified] File Monitored: " << m_FileNotifInfo->FileName << std::endl;
						last_file_action = FILE_ACTION::FILE_MODIFY;
						break;
					case FILE_ACTION_ADDED://Handle file add event & call our custom callback
						if (IsValidDLLFile(m_FileNotifInfo->FileName)) {
							if (!IsWhitelistedDLL(m_FileNotifInfo->FileName)) {
								FileIndex++;
								MonitoredFiles[FileIndex] = wPath;
								MonitoredFiles[FileIndex].append(m_FileNotifInfo->FileName);
								std::wcout << "[Added] File Monitored: " << MonitoredFiles[FileIndex].c_str() << std::endl;
								Sleep(500);//Let the file move-in completely
								if (file_added_callback(MonitoredFiles[FileIndex]) == ZN_STATUS_REASON::ZN_SUCCESS)
								{
									std::wcout << "[Removed] File Monitored: " << MonitoredFiles[FileIndex] << std::endl;
									MonitoredFiles[FileIndex].erase();
									FileIndex--;
								}
								last_file_action = FILE_ACTION::FILE_ADD;
							}
						}
						break;
					case FILE_ACTION_REMOVED://Handle Deletetion event
						for (auto i : this->MonitoredFiles)
						{
							if (i.second.find(m_FileNotifInfo->FileName) != i.second.npos)
							{
								auto key = this->MonitoredFiles.find(i.first);//Find the monitored file using key
								std::wcout << "[Removed] File Monitored: " << i.second << std::endl;
								MonitoredFiles.erase(key);
							}
						}
						last_file_action = FILE_ACTION::FILE_DELETE;
						break;

					

					case FILE_ACTION_RENAMED_OLD_NAME:
						wprintf(L"Old          to: %.*s\n", name_len, m_FileNotifInfo->FileName);
						//std::wcout << "[RenamedOld] File Monitored: " << m_FileNotifInfo->FileName << std::endl;
						last_file_action = FILE_ACTION::FILE_RENAME_OLD_NAME;
						break;

					case FILE_ACTION_RENAMED_NEW_NAME:
						wprintf(L"New          to: %.*s\n", name_len, m_FileNotifInfo->FileName);
						//std::wcout << "[RenamedNew] File Monitored: " << m_FileNotifInfo->FileName << std::endl;
						last_file_action = FILE_ACTION::FILE_RENAME_NEW_NAME;
						break;
					default:
						wprintf(L"Unk          to: %.*s\n", name_len, m_FileNotifInfo->FileName);
					//	std::wcout << "[Unknown] File Monitored: " << m_FileNotifInfo->FileName << std::endl;
						last_file_action = FILE_ACTION::UNKNOWN;
						break;
					}

					*reinterpret_cast<uintptr_t**>(&m_FileNotifInfo) += m_FileNotifInfo->NextEntryOffset;//Dereference and handle the next event on the queue
				} while (m_FileNotifInfo->NextEntryOffset);

				this->zStatus = this->NextQueue();
			}
		}
		else
		{
			std::cout << "zStatus Status: " <<GetZNStatusReason(this->zStatus) << std::endl;
			std::cout << "GetLastErrorCode Status: " << GetLastError() << std::endl;
		}
	}
};
