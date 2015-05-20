#pragma once
#include "baseentity.h"
#include "Sprite.h"

class CScannable
{
public:

	CScannable();
	~CScannable();
	DECLARE_CLASS_NOBASE(CScannable);

	COutputEvent m_OnScanStarted;
	//COutputEvent m_OnScanAborted;		//this won't work as of now, i need to find a workaround.
	COutputEvent m_OnScanCompleted;
	float m_flRequiredScanTime;
	string_t m_szScanInfo;


	bool IsKnown();
	void UpdateScanTime(float scantime, CBasePlayer *pPlayer, CBaseEntity *pBase);


protected:
	bool m_bScanned; //this state should be persistent. once scanned, always in your database.
	void SendShowScanInfo(CBasePlayer *pPlayer, CBaseEntity *pBase);

private:
	float m_flLastScanTime;
	string_t m_szSpritePath;
};

//additional DATADESC fields for scannables
#define DEFINE_SCANNABLE_DATADESC() \
	\
DEFINE_FIELD(m_bScanned, FIELD_BOOLEAN), \
DEFINE_KEYFIELD(m_flRequiredScanTime, FIELD_FLOAT, "scantime"), \
DEFINE_KEYFIELD(m_szScanInfo, FIELD_STRING, "scaninfo"), \
	\
DEFINE_OUTPUT(m_OnScanStarted, "OnScanStarted"), \
DEFINE_OUTPUT(m_OnScanCompleted, "OnScanCompleted")

