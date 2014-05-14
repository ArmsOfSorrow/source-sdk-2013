#pragma once
#include "baseentity.h"

class CScannable : public CBaseEntity
{
public:
	DECLARE_CLASS(CScannable, CBaseEntity);

	COutputEvent m_OnScanStarted;
	COutputEvent m_OnScanAborted;
	COutputEvent m_OnScanCompleted;
	//this is going to be incoherent as hell.
	inline bool IsKnown();

private:
	bool m_scanned; //this state should be persistent. once scanned, always in your database.
};