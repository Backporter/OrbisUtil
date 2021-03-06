#pragma once

#include <stdint.h>
#include <iostream>
#include <stdint.h>
#include <type_traits>
#include <cstring>

#include "RelocationManager.h"

namespace OrbisMemoryHandler
{
	void WriteBuffer(uint64_t dst, void* src, size_t len);

	template <typename T>
	void WriteType(uintptr_t dst, T data, size_t size = sizeof(T))
	{
		if constexpr (std::is_pointer<T>::value)
		{
			WriteBuffer(dst, data, size);
		}
		else
		{
			WriteBuffer(dst, &data, size);
		}
	}

	template <typename T>
	void WriteType(uintptr_t dst, T data, bool AddBase, size_t size = sizeof(T))
	{
		if constexpr (std::is_pointer<T>::value)
		{
			WriteBuffer(dst + RelocationManager::RelocationManager::RelocationManager::ApplicationBaseAddress, data, size);
		}
		else
		{
			WriteBuffer(dst + RelocationManager::RelocationManager::RelocationManager::ApplicationBaseAddress, &data, size);
		}
	}
}