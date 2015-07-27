#pragma once
#include "baseentity.h"
#include "Sprite.h"

class CScannable
{
public:

	COutputEvent m_OnScanStarted;
	//COutputEvent m_OnScanAborted;		//this won't work as of now, i need to find a workaround.
	COutputEvent m_OnScanCompleted;
	float m_flRequiredScanTime;
	string_t m_szScanInfo;
	string_t m_szSpritePath;
	string_t teststr;

	bool IsKnown();
	void UpdateScanTime(float scantime, CBasePlayer *pPlayer, CBaseEntity *pBase);
	
	/* accessors to call on scan state activation/deactivation */
	void SetSpriteVisibility(bool visible);
	

protected:
	bool m_bScanComplete; //this state should be persistent. once scanned, always in your database.
	bool m_bMessageSent;
	CHandle<CSprite> m_hSprite;
	void SendShowScanInfo(CBasePlayer *pPlayer, CBaseEntity *pBase);
	void InitScannableSprite(CBaseEntity *pParent, Vector origin, bool animate);

private:
	float m_flLastScanTime;
};

//additional DATADESC fields for scannables
#define DEFINE_SCANNABLE_DATADESC() \
	\
DEFINE_FIELD(m_bScanComplete, FIELD_BOOLEAN), \
DEFINE_KEYFIELD(m_flRequiredScanTime, FIELD_FLOAT, "scantime"), \
DEFINE_KEYFIELD(m_szScanInfo, FIELD_STRING, "scaninfo"), \
DEFINE_KEYFIELD(m_szSpritePath, FIELD_STRING, "spritepath"), \
	\
DEFINE_FIELD(m_hSprite, FIELD_EHANDLE), \
	\
DEFINE_OUTPUT(m_OnScanStarted, "OnScanStarted"), \
DEFINE_OUTPUT(m_OnScanCompleted, "OnScanCompleted")

