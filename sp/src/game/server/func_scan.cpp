#include "cbase.h"
#include "func_scan.h"

CScannable::CScannable()
{
}

CScannable::~CScannable()
{
}

bool CScannable::IsKnown()
{
	return m_bScanned;
}

void CScannable::UpdateScanTime(float scantime, CBasePlayer *pPlayer, CBaseEntity *pBase)
{
	//problem: i don't know how to call this when a scan is aborted.
	//possible workaround: a periodically called think function
	//maybe schedule think on first scan and stop on abort? that seems like a not too expensive option to me.
	pPlayer->SetScannedEntity(pBase);

	if (m_bScanned)
	{
		SendShowScanInfo(pPlayer, pBase);
	}
	else
	{
		if (scantime <= m_flLastScanTime)//can't be, that means a new scan is started
		{
			m_OnScanStarted.FireOutput(pPlayer, pBase);
			m_flLastScanTime = scantime;
			Msg("Scanning %f\n", scantime);
		}
		else
		{
			if (scantime >= m_flRequiredScanTime)
			{
				
				m_bScanned = true;
			}
		}
		m_flLastScanTime = scantime;
	}
}

/*
Sends the ShowScanInfo UserMessage with the appropriate string token to the HUD.
*/
void CScannable::SendShowScanInfo(CBasePlayer *pPlayer, CBaseEntity *pBase)
{
	CSingleUserRecipientFilter user(pPlayer);
	user.MakeReliable();

	UserMessageBegin(user, "ShowScanInfo");
	WRITE_STRING(m_szScanInfo.ToCStr());
	MessageEnd();

	m_OnScanCompleted.FireOutput(pPlayer, pBase);
}