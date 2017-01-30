#pragma once

class CTypeHasher
{
public:
	CTypeHasher() = delete;

	template<typename T>
	static long long Hash()
	{
		typedef long long(*FunctionPtr)();
		static FunctionPtr functionAddress = &Hash<T>;
		return reinterpret_cast<long long>(functionAddress);
	}
};
