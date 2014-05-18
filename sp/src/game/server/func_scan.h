#pragma once
#include "baseentity.h"

class CScannable : public CBaseEntity
{
public:
	DECLARE_CLASS(CScannable, CBaseEntity);

	COutputEvent m_OnScanStarted;
	//COutputEvent m_OnScanAborted;		//this won't work as of now, i need to find a workaround.
	COutputEvent m_OnScanCompleted;
	
	bool IsKnown();
	void UpdateScanTime(float scantime, CBasePlayer *pPlayer);
	void InputShowScanInfo(inputdata_t &inputdata); //handle future vgui stuff in there


private:
	bool m_bScanned; //this state should be persistent. once scanned, always in your database.
	float m_flRequiredScanTime;
	float m_flLastScanTime;
	void ShowScanInfo();
};