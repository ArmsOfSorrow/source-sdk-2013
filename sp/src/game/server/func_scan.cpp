#include "cbase.h"
#include "func_scan.h"

CScannable::CScannable(CBaseEntity *pBase)
{
	m_pBase = pBase;
}

CScannable::~CScannable()
{
}

bool CScannable::IsKnown()
{
	return m_bScanned;
}

void CScannable::UpdateScanTime(float scantime, CBasePlayer *pPlayer, CBaseEntity *pBase) //i could pass a pointer to base here, couldn't i?
{
	Msg("updating scan time...");
	//problem: i don't know how to call this when a scan is aborted.
	//possible workaround: a periodically called think function
	//maybe schedule think on first scan and stop on abort? that seems like a not too expensive option to me.

	if (scantime <= m_flLastScanTime)//can't be, that means a new scan is started
	{
		m_OnScanStarted.FireOutput(pPlayer, pBase);
		//pPlayer->SetScannedEntity(this);
		m_flLastScanTime = scantime;
		Msg("Scanning %f\n", scantime);
	}
	else
	{
		//scantime == m_flLastScanTime is not likely to happen, is it?
		//anyway, we're still scanning. that means update time and check if it's above or equal to required
		if (scantime >= m_flRequiredScanTime)
		{
			m_OnScanCompleted.FireOutput(pPlayer, pBase);
			ShowScanInfo(); //this should display vgui overlay and pause everything but the gui.
		}
	}
}

void CScannable::ShowScanInfo()
{
	//do vgui stuff inside here
	
}

//input handler for showing scan info immediately
void CScannable::InputShowScanInfo(inputdata_t &inputdata)
{
	//ShowScanInfo();
	Msg("InputShowScanInfo fired.\n");
}

//LINK_ENTITY_TO_CLASS(func_scannable, CScannable);

BEGIN_DATADESC_NO_BASE(CScannable)
//BEGIN_DATADESC(CScannable)

DEFINE_KEYFIELD(m_flRequiredScanTime, FIELD_FLOAT, "scantime"),
DEFINE_KEYFIELD(m_szScanInfo, FIELD_STRING, "scaninfo"),

DEFINE_OUTPUT(m_OnScanStarted, "OnScanStarted"),
DEFINE_OUTPUT(m_OnScanCompleted, "OnScanCompleted"),

//DEFINE_INPUTFUNC(FIELD_VOID, "ShowScanInfo", InputShowScanInfo)

END_DATADESC()