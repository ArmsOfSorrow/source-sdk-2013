#include "cbase.h"
#include "hudelement.h"
#include <vgui_controls\Panel.h>
#include "VGuiMatSurface\IMatSystemSurface.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include <vgui\ILocalize.h>
#include "in_buttons.h"

class CHudScanInfo : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CHudScanInfo, vgui::Panel);

public:
	CHudScanInfo(const char *pElementName);
	~CHudScanInfo();
	void Init();
	bool ShouldDraw();
	void Paint();
	void MsgFunc_ShowScanInfo(bf_read &msg);
	void OnThink();

private:
	wchar_t *m_pScanInfo;
	int m_nOldButtonState;
	float timer;
};

DECLARE_HUDELEMENT(CHudScanInfo);
DECLARE_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);
//maybe i should add a destroy scan info msg too. or just use a think function.

CHudScanInfo::CHudScanInfo(const char *pElementName) : BaseClass(nullptr, "HudScanInfo"), CHudElement(pElementName)
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);
	m_pScanInfo = new wchar_t[2048];
	m_nOldButtonState = 0;
	timer = 0.0f;
}

CHudScanInfo::~CHudScanInfo()
{
	delete[] m_pScanInfo;
}

void CHudScanInfo::Init()
{
	HOOK_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);
}

bool CHudScanInfo::ShouldDraw()
{
	return timer > 0.0f;
}

void CHudScanInfo::Paint()
{
	g_pMatSystemSurface->DrawUnicodeString(m_pScanInfo);
}

void CHudScanInfo::MsgFunc_ShowScanInfo(bf_read &msg)
{
	timer = 0.5f;
	char szString[2048];
	msg.ReadString(szString, sizeof(szString));

	//this might lead to fuckups in sizes. do some damage control. or testing.
	wchar_t *pTmpStr = g_pVGuiLocalize->Find(szString); 

	wcscpy(m_pScanInfo, pTmpStr);
	//use wcsncpy to copy new token into text member.
	//further ideas: limit size of string that can be transmitted to ~64.
	//longer tokens should not be needed.

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer)
	{
		m_nOldButtonState = pPlayer->m_nButtons;
	}
	
}

void CHudScanInfo::OnThink()
{
	//i'm curious if this will produce unexpected results on underflow.
	timer -= gpGlobals->frametime;
	if (!(m_nOldButtonState && IN_ATTACK))
	{
		m_nOldButtonState = 0;
	}
}