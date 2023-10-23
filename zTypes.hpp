#pragma once

typedef int32_t ZN_STATUS;

enum ZN_STATUS_REASON
{
	ZN_SUCCESS,
	ZN_UNKNOWN_ERROR,
	FILE_READ_DIRECTORY_CHANGES_ERROR,
	FILE_OBJECT_TIMER_ERROR,
	FILE_GET_OVERLAPPED_RESULT_ERROR,
	FILE_CREATE_FILE_ERROR,
	FILE_CREATE_EVENT_ERROR,
	FILE_FIND_FIRST_FILE_ERROR,
	ZN_LOADLIBRARY_FAILED,
	ZN_START_FUNCTION_NOT_FOUND,
	ZN_SECRET_FUNCTION_NOT_FOUND,
	ZN_FREE_LIBRARY_ERROR,
	ZN_DELETE_FILE_ERROR,
	ZN_FILE_NOT_EXIST,
};

enum FILE_ACTION
{
	FILE_ADD,
	FILE_DELETE,
	FILE_MODIFY,
	FILE_RENAME_OLD_NAME,
	FILE_RENAME_NEW_NAME,
	UNKNOWN
};

std::string GetZNStatusReason(ZN_STATUS zReason)
{
	switch (zReason)
	{
	case ZN_SUCCESS:return "Success";
	case FILE_READ_DIRECTORY_CHANGES_ERROR: return "ReadDirectoryChanges Failed";
	case FILE_OBJECT_TIMER_ERROR: return "WaitForSingleObject didn't return WAIT_OBJECT_0";
	case FILE_GET_OVERLAPPED_RESULT_ERROR: return "GetOverlappedResult failed";
	case FILE_CREATE_FILE_ERROR: return "CreateFile failed";
	case FILE_CREATE_EVENT_ERROR: return "CreateEvent failed";
	case FILE_FIND_FIRST_FILE_ERROR: return "FindFirstFile failed";
	case ZN_LOADLIBRARY_FAILED: return "LoadLibrary failed";
	case ZN_START_FUNCTION_NOT_FOUND: return "Exported function [start] not found on the dll";
	case ZN_SECRET_FUNCTION_NOT_FOUND: return "Exported function [secret] not found on the dll";
	case ZN_FILE_NOT_EXIST: return "File Doesn't exist!";
	case ZN_FREE_LIBRARY_ERROR: return "FreeLibrary failed";
	case ZN_DELETE_FILE_ERROR: return "DeleteFile failed";
	default: return "Unknown Error";
	}
	return "Unknown Error Unscoped";
}