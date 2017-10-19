#pragma once

#include <hash/CStrID.h>

#include <functional>
#include <type_traits>

namespace std
{
	template<>
	struct hash<CStrID>
	{
		std::size_t operator()(CStrID e) const
		{
			return static_cast<std::size_t>(e);
		}
	};
}
