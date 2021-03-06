#pragma once

#include "RelocationManager.h"
#include "OrbisOffsertManger.h"
#include "MemoryHandler.h"

#include <stdio.h>
#include <stdint.h>

// NAX ID LENGTH =  0x28/40
template <typename T>
class Relocation
{
public:
	Relocation() { }

	Relocation(uintptr_t offset)
	{
		this->Offset = reinterpret_cast <void*>(offset + RelocationManager::RelocationManager::ApplicationBaseAddress);
	}

	// 0x28/40
	Relocation(const char* ID, uintptr_t defaultvalue)
	{
		auto dboffset = OrbisOffsetManger::OffsetManger::GetSingleton()->_GetOffset(ID);
		this->Offset = reinterpret_cast <void*>((dboffset == 0 ? defaultvalue : dboffset) + RelocationManager::RelocationManager::ApplicationBaseAddress);
	}

	Relocation(const char* a_id, const char* a_id2, uintptr_t defaultvalue)
	{
		auto dboffset = OrbisOffsetManger::OffsetManger::GetSingleton()->_GetOffset(a_id);
		
		if (dboffset == NULL)
			dboffset = OrbisOffsetManger::OffsetManger::GetSingleton()->_GetOffset(a_id2);

		this->Offset = reinterpret_cast <void*>((dboffset == 0 ? defaultvalue : dboffset) + RelocationManager::RelocationManager::ApplicationBaseAddress);
	}


	operator T()
	{
		return reinterpret_cast <T>(Offset);
	}

	uintptr_t operator=(uintptr_t address)
	{
		this->Offset = reinterpret_cast<void*>(address);
	}

	uintptr_t operator += (uint64_t count)
	{
		Offset = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(Offset) + count);
		return reinterpret_cast<uintptr_t>(Offset);
	}

	uintptr_t GetUIntPtr() const
	{
		return reinterpret_cast <uintptr_t>(Offset);
	}

	// operator T() doesn't work with T* so we need to do this to stop the compiller from returning this instead of the pointer...
	T GetPtr()
	{
		return reinterpret_cast<T>(Offset);
	}

	uintptr_t address()
	{
		return (uintptr_t)Offset;
	}

	template <class X>
	uintptr_t write_vfunc(int idx, X a_newFunc)
	{
		uintptr_t addr = (uintptr_t)Offset + (sizeof(void*) * idx);
		const auto result = *reinterpret_cast<uintptr_t*>(addr);
		OrbisMemoryHandler::WriteType(addr, (uintptr_t)a_newFunc);
		return result;
	}

private:
	void*		Offset;
};