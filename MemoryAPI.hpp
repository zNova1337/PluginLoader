#pragma once
#include "Includes.hpp"

/// <summary>
/// Using uintptr_t and size_t for x86 and x64 compatibility
/// </summary>
namespace MemoryAPI
{

	/// <summary>
	/// Changes the memory protection of the Address parameter and overwrites the value
	/// </summary>
	/// <param name="Address">Target Address</param>
	/// <param name="src">Source Value</param>
	/// <param name="len">Length</param>
	/// <returns>false if VirtualProtectAPI failed</returns>
	bool WriteMemory(uintptr_t Address, const void* src, size_t len)
	{
		DWORD Old;
		if (!VirtualProtect(reinterpret_cast<LPVOID>(Address), len, PAGE_EXECUTE_READWRITE, &Old))
		{
			std::cout << "[WriteMemory] VirtualProtect failed with = " << GetLastError() << std::endl;
			return false;
		}
		memcpy(reinterpret_cast<void*>(Address), src, len);
		if (!VirtualProtect(reinterpret_cast<LPVOID>(Address), len, Old, &Old))
		{
			std::cout << "[WriteMemory] VirtualProtect Restoration failed with = " << GetLastError() << std::endl;
			return false;
		}
		return true;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Address">Target address</param>
	/// <param name="nopsize"></param>
	/// <returns></returns>
	bool SetInt3(uintptr_t Address, size_t nopsize = 0)
	{
		bool bRet = false;
		bRet = WriteMemory(Address, "\xCC", 1);
		if (nopsize) {
			bRet = WriteMemory(Address + 1, "\x90", nopsize);//nop the instruction after so it doesn't mess with the instruction later
		}
		return bRet;
	}

	/// <summary>
	/// Stores the bytes of the target address into the buffer
	/// </summary>
	/// <typeparam name="size"></typeparam>
	/// <param name="TargetFunction">Target Address</param>
	/// <param name="backup">buffer that will hold the original value</param>
	/// <returns></returns>
	template <size_t size>
	bool SaveBytes(uintptr_t TargetFunction, uint8_t(&backup)[size])
	{
		if (!TargetFunction || size == 0)return false;
		memcpy((void*)backup, (void*)TargetFunction, size);
		return true;
	}

	/// <summary>
	/// Overwrites the bytes of the target address with the original bytes
	/// </summary>
	/// <typeparam name="size"></typeparam>
	/// <param name="TargetFunction"></param>
	/// <param name="original_bytes"></param>
	/// <returns></returns>
	template <size_t size>
	bool RestoreBytes(uintptr_t TargetFunction, uint8_t(&original_bytes)[size])
	{
		if (!TargetFunction || size == 0)return false;
		DWORD dwOldProtection;
		if (!VirtualProtect(reinterpret_cast<LPVOID>(TargetFunction), size, PAGE_EXECUTE_READWRITE, &dwOldProtection))
		{
			std::cout << "[RestoreBytes] VirtualProtect failed with = " << GetLastError() << std::endl;
			return false;
		}
		memcpy((void*)TargetFunction, (void*)original_bytes, size);
		if (!VirtualProtect(reinterpret_cast<LPVOID>(TargetFunction), size, dwOldProtection, &dwOldProtection))
		{
			std::cout << "[RestoreBytes] VirtualProtect Restoration failed with = " << GetLastError() << std::endl;
			return false;
		}
		return true;
	}
}