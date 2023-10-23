#pragma once
#include "Includes.hpp"


class Int3_hook
{
private:
	uintptr_t Address = 0;
	void* callback = nullptr;
	uint8_t bytes[1]; //for 0xCC opcode
public:

	Int3_hook(uintptr_t Target, void* cb, uintptr_t* ret) { 
		this->Address = Target; 
		this->callback = cb; 
		*ret = (uintptr_t)(this->Address + 2); 
	}
	~Int3_hook() { 
		this->Address = 0; 
		this->callback = nullptr;
		ZeroMemory(&bytes, sizeof(bytes)); 
	}

	auto GetAddress() { return this->Address; }
	auto GetCallback() { return this->callback; }

	/// <summary>
	/// Changes the page protection of the specified address and sets int3(0xCC) instruction
	/// </summary>
	/// <returns></returns>
	bool Hook() {
		return MemoryAPI::SaveBytes(this->Address, this->bytes) && MemoryAPI::SetInt3(this->Address, 1);
	}

	/// <summary>
	/// Restores the original opcode to the specified address
	/// </summary>
	/// <returns></returns>
	bool Unhook() {
		return MemoryAPI::RestoreBytes(this->Address, this->bytes);
	}
};