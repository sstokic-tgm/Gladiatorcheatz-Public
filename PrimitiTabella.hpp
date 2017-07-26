#pragma once

#include "PrimitiImovina.hpp"

class PrimitiTabella
{
public:

	PrimitiImovina *m_pProps;
	int m_nProps;
	void *m_pDecoder;
	char *m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};