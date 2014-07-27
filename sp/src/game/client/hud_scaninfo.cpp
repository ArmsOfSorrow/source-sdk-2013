#include "cbase.h"
#include "hudelement.h"
#include <vgui_controls\EditablePanel.h>
#include "VGuiMatSurface\IMatSystemSurface.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include <vgui\ILocalize.h>
#include <vgui_controls\AnimationController.h>
#include "in_buttons.h"
#include <vgui\IVGui.h>
#include <vgui\IScheme.h>
#include <vgui_controls\Label.h>
#include "iinput.h"


class CHudScanInfo : public CHudElement, public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CHudScanInfo, vgui::EditablePanel);
	
public:
	CHudScanInfo(const char *pElementName);
	~CHudScanInfo();
	virtual void Init();
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual bool ShouldDraw();
	virtual void Paint();
	virtual void OnThink();
	virtual void OnTick();
	void MsgFunc_ShowScanInfo(bf_read &msg);
	

private:
	wchar_t *m_pScanInfo;
	char m_szToken[128];
	int m_nOldButtonState;
	bool m_bScanCompleted;
	bool m_bShouldDraw;
	bool m_bOldDrawState;
	float m_flLastInputTime;
	vgui::Label *m_scanTextLabel;
	
	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "HudSelectionText");
	CPanelAnimationVar(float, m_flAlphaOverride, "Alpha", "0");
	CPanelAnimationVar(float, m_flTextAlphaOverride, "TextAlpha", "0");
	
};

DECLARE_HUDELEMENT(CHudScanInfo);
DECLARE_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);

CHudScanInfo::CHudScanInfo(const char *pElementName) : CHudElement(pElementName), BaseClass(nullptr, "HudScanInfo")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport(); //change this to scaninfopanel
	SetParent(pParent);

	LoadControlSettings("resource/scaninfo.res");
	m_scanTextLabel = FindControl<vgui::Label>("ScanTextLabel", true);
	m_pScanInfo = new wchar_t[2048];
	m_nOldButtonState = 0;
	m_bScanCompleted = false;
	m_bOldDrawState = false;
	m_flLastInputTime = -1.5f;
}

CHudScanInfo::~CHudScanInfo()
{
	delete[] m_pScanInfo;

	if (m_scanTextLabel && !m_scanTextLabel->IsAutoDeleteSet())
	{
		delete m_scanTextLabel;
		m_scanTextLabel = nullptr;
	}
}

void CHudScanInfo::Init()
{
	HOOK_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);
	g_pVGui->AddTickSignal(this->GetVPanel(), 50);
}

void CHudScanInfo::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}

bool CHudScanInfo::ShouldDraw()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (pPlayer)
	{
		if (pPlayer->GetScannedEntity() && m_bScanCompleted)
		{
			m_bShouldDraw = true;
			return m_bShouldDraw;
		}
		else
		{
			m_bScanCompleted = false;
		}
	}
	m_bShouldDraw = false; //we don't have a scanned entity anymore

	return m_bShouldDraw || (gpGlobals->curtime - m_flLastInputTime) < 1.5f;
}

void CHudScanInfo::Paint()
{
	//check if it needs to be drawn at all, i.e. if the player has a scanned entity 
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (!pPlayer)
		return;
	
	int x, y, wide, tall;
	Color c(0, 0, 0);
	GetPos(x, y);
	wide = GetWide();
	tall = GetTall();
	SetAlpha(m_flAlphaOverride);
	
	//Msg("Alpha: %i\n", GetAlpha());
	g_pMatSystemSurface->DrawSetTextFont(m_hTextFont);
	g_pMatSystemSurface->DrawSetTextColor(255,255,255, m_flTextAlphaOverride);
	m_scanTextLabel->SetText(m_szToken);
	m_scanTextLabel->SetAlpha(m_flTextAlphaOverride);
	//g_pMatSystemSurface->DrawUnicodeString(m_pScanInfo);
}

void CHudScanInfo::MsgFunc_ShowScanInfo(bf_read &msg)
{
	//use tokens only, as string size is limited to 128 bytes.
	msg.ReadString(m_szToken, sizeof(m_szToken));
	m_bScanCompleted = true;
}

//this is only called when the panel is drawn, that means the old draw state will always be true.
void CHudScanInfo::OnThink()
{
	//check for anim state and update accordingly

	//i need to know the last input time...or just check if the player is still holding down the attack key. if not, start fading out.

	if (gHUD.m_iKeyBits & IN_ATTACK)
	{
		if (m_bOldDrawState == false)
		{
			//fade in, as we were not drawing in the last frame
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("OpenScanInfo");
		}
	}
	else
	{
		//he was holding down last frame
		if (m_nOldButtonState & IN_ATTACK)
		{
			m_flLastInputTime = gpGlobals->curtime;
			m_nOldButtonState &= ~IN_ATTACK; //zero out the attack bit, since we're not holding down anymore
			//start fading out here.
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("FadeOutScanInfo");
		}
	}
	m_nOldButtonState = gHUD.m_iKeyBits;
}

void CHudScanInfo::OnTick()
{
	//maybe i should find a better workaround, but for now this'll stay here.
	m_bOldDrawState = m_bShouldDraw;
}