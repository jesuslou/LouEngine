#pragma once

class CMsgHandlerBase
{
public:
	template<typename C, typename T>
	void Execute(C* caller, const T& param)
	{
		CMsgHandler<C, T>* myself = static_cast<CMsgHandler<C, T>*>(this);
		myself->ExecuteFunction(caller, param);
	}
};

template<typename C, typename T>
class CMsgHandler : public CMsgHandlerBase
{
public:
	CMsgHandler(void(C::*function)(const T&))
		: m_function(function)
	{}

	void ExecuteFunction(C* caller, const T& param)
	{
		(caller->*m_function)(param);
	}

private:
	void (C::*m_function)(const T&);
};
