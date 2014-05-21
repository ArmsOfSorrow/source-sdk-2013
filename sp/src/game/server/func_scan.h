#pragma once
#include "baseentity.h"

class CScannable
{
public:

	CScannable(CBaseEntity *pBase);
	~CScannable();
	DECLARE_CLASS_NOBASE(CScannable);
	//DECLARE_DATADESC();
	
	COutputEvent m_OnScanStarted;
	//COutputEvent m_OnScanAborted;		//this won't work as of now, i need to find a workaround.
	COutputEvent m_OnScanCompleted;
	float m_flRequiredScanTime;
	string_t m_szScanInfo;

	bool IsKnown();
	void UpdateScanTime(float scantime, CBasePlayer *pPlayer, CBaseEntity *pBase);
	void InputShowScanInfo(inputdata_t &inputdata); //handle future vgui stuff in there


private:
	bool m_bScanned; //this state should be persistent. once scanned, always in your database.
	float m_flLastScanTime;
	void ShowScanInfo();
	CBaseEntity *m_pBase;
};

#define IMPLEMENT_SCANNABLE() \
DEFINE_KEYFIELD(m_flRequiredScanTime, FIELD_FLOAT, "scantime"), \
DEFINE_KEYFIELD(m_szScanInfo, FIELD_STRING, "scaninfo"), \
	\
DEFINE_OUTPUT(m_OnScanStarted, "OnScanStarted"), \
DEFINE_OUTPUT(m_OnScanCompleted, "OnScanCompleted") 