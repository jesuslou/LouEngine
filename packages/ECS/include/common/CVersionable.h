#pragma once

class CVersionable
{
public:
	CVersionable()
		: m_version(0)
	{}

	void SetVersion(int version) { m_version = version; }
	int GetVersion() const { return m_version; }

protected:
	int m_version;
};
