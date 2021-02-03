#pragma once
#include <stdint.h>

#include "cwMutex.h"

class cwOrderReference
{
public:
	cwOrderReference();
	~cwOrderReference();

	void			UpdateOrderRef(const char * pOrderRef);
	void			UpdateOrderRef(char * pOrderRef);

	uint64_t		GetOrderRef();
private:
	cwMUTEX			m_OderRefMutex;
	uint64_t		m_OrderReferenceIndex;

#ifdef _MSC_VER
	DWORD			m_dwCurrentProcessID;
#else
	int				m_iCurrentProcessID;
#endif
};

