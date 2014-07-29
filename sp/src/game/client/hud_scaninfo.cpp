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
#include "iscaninfopanel.h"


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
	void MsgFunc_ShowScanInfo(bf_read &msg);
	

private:

	int m_nOldButtonState;
	float m_flLastInputTime;
	const float m_flFadeThreshold = 0.5f;
	bool m_bScanCompleted;
	bool m_bShouldDraw;
	bool m_bOldDrawState;
	char m_szToken[128];
	vgui::Label *m_pScanTextLabel;
	
	CPanelAnimationVar(vgui::HFont, m_hTextFont, "TextFont", "HudSelectionText");
	CPanelAnimationVar(float, m_flAlphaOverride, "Alpha", "0");
	CPanelAnimationVar(float, m_flTextAlphaOverride, "TextAlpha", "0");
	
};

DECLARE_HUDELEMENT(CHudScanInfo);
DECLARE_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);

CHudScanInfo::CHudScanInfo(const char *pElementName) : CHudElement(pElementName), BaseClass(nullptr, "HudScanInfo")
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent(pParent);

	m_nOldButtonState = 0;
	m_flLastInputTime = 0.0f;
	m_bScanCompleted = false;
	m_bShouldDraw = false;
	m_bOldDrawState = false;

	LoadControlSettings("resource/scaninfo.res");
	m_pScanTextLabel = FindControl<vgui::Label>("ScanTextLabel", true);
}

CHudScanInfo::~CHudScanInfo()
{
	if (m_pScanTextLabel && !m_pScanTextLabel->IsAutoDeleteSet())
	{
		delete m_pScanTextLabel;
		m_pScanTextLabel = nullptr;
	}
}

void CHudScanInfo::Init()
{
	HOOK_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);
}

void CHudScanInfo::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	int x, y, wide, tall; //tall (from hud size) is not used in resizing
	GetPos(x, y);
	GetHudSize(wide, tall);

	SetBounds(x, y, wide-2*x, GetTall());
}

bool CHudScanInfo::ShouldDraw()
{
	m_bOldDrawState = m_bShouldDraw;

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (pPlayer)
	{
		if (pPlayer->GetScannedEntity() && m_bScanCompleted)
		{
			m_bShouldDraw = true;
		}
		else
		{
			//we don't have a scanned entity anymore
			m_bScanCompleted = false;
			m_bShouldDraw = false;
		}
	}
	
	return m_bShouldDraw || (gpGlobals->curtime - m_flLastInputTime) < m_flFadeThreshold;
}

void CHudScanInfo::Paint()
{
	//maybe check for player state/entity flags to not draw the HUD under special circumstances
	
	SetAlpha(m_flAlphaOverride);
	g_pMatSystemSurface->DrawSetTextFont(m_hTextFont);
	g_pMatSystemSurface->DrawSetTextColor(255,255,255, m_flTextAlphaOverride);

	m_pScanTextLabel->SetText(m_szToken);
	m_pScanTextLabel->SetAlpha(m_flTextAlphaOverride);
}

void CHudScanInfo::MsgFunc_ShowScanInfo(bf_read &msg)
{
	//use tokens only, as string size is limited to 128 bytes.
	msg.ReadString(m_szToken, sizeof(m_szToken));
	m_bScanCompleted = true;
}

void CHudScanInfo::OnThink()
{
	//if we landed here, ShouldDraw() returned true. now there are 3 cases:
	//- it's the first frame after having finished scanning. in that case, fade in the panel.
	//- it's not the first frame, but we're still scanning. we should update the last input time.
	//- we're not scanning anymore. start fading out.

	if (gHUD.m_iKeyBits & IN_ATTACK)
	{
		if (m_bOldDrawState == false && m_bScanCompleted)
		{
			//fade in, as we were not drawing in the last frame
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("OpenScanInfo");
		}
		m_flLastInputTime = gpGlobals->curtime;
	}
	else
	{
		if (m_nOldButtonState & IN_ATTACK)
		{
			//start fading out here.
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence("FadeOutScanInfo");
		}
	}
	m_nOldButtonState = gHUD.m_iKeyBits;
}