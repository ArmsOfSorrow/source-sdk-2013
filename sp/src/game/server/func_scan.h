#pragma once
#include "baseentity.h"

class CScannable
{
public:

	CScannable(/*CBaseEntity *pBase*/);
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
	void ShowScanInfo(CBasePlayer *pPlayer, CBaseEntity *pBase);

private:
	float m_flLastScanTime;
};

//this should be always at the end.
#define IMPLEMENT_SCANNABLE() \
	\
DEFINE_FIELD(m_bScanned, FIELD_BOOLEAN), \
DEFINE_KEYFIELD(m_flRequiredScanTime, FIELD_FLOAT, "scantime"), \
DEFINE_KEYFIELD(m_szScanInfo, FIELD_STRING, "scaninfo"), \
	\
DEFINE_OUTPUT(m_OnScanStarted, "OnScanStarted"), \
DEFINE_OUTPUT(m_OnScanCompleted, "OnScanCompleted")

//alright, so there won't be a showscaninfo input. unless i extend CBaseAnimating
//itself, that is. which i'll probably do, since this shit is driving me mad.
//but again, what do i do with the fgd?