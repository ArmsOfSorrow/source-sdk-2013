#pragma once
#include "baseentity.h"

class CScannable : public /*virtual*/ CBaseEntity //todo: should this be virtual?
{
public:
	DECLARE_CLASS(CScannable, CBaseEntity);
	DECLARE_DATADESC();
	COutputEvent m_OnScanStarted;
	//COutputEvent m_OnScanAborted;		//this won't work as of now, i need to find a workaround.
	COutputEvent m_OnScanCompleted;
	float m_flRequiredScanTime;
	string_t m_szScanInfo;

	bool IsKnown();
	void UpdateScanTime(float scantime, CBasePlayer *pPlayer);
	void InputShowScanInfo(inputdata_t &inputdata); //handle future vgui stuff in there


private:
	bool m_bScanned; //this state should be persistent. once scanned, always in your database.
	float m_flLastScanTime;
	void ShowScanInfo();
	
};