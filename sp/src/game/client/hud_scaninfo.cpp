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
//#include <vgui_controls\Label.h>
#include <vgui_controls\RichText.h>
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
	virtual void ProcessInput();
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
	vgui::RichText *m_pScanTextLabel;
	
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
	m_pScanTextLabel = FindControl<vgui::RichText>("ScanTextLabel", true);
	m_pScanTextLabel->SetKeyBoardInputEnabled(true);
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

void CHudScanInfo::ProcessInput()
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if (pPlayer)
	{
		//so, the next time you come back, the plan looks like this:
		//1. leave it parented to viewport, since changing it to scaninfopanel would involve
		//   changes in regards to input.  (HUD has the ProcessInput method)
		//2. fix richtext's localization functions to handle multiple tokens at once
		//3. on button press (ATTACK2), "scroll", or rather load the text from the
		//   next token (if there is any left) and display that. Deactivate that scrollbar 
		//   while you're at it, too.
		//4. ShouldDraw and OnThink take care of fading out and deactivating.
		
		//anyway, I've just found out that I can set key bindings and a lot of useful stuff inside config.cfg
		//kb_act.lst is most likely for the UI
		//F4 is default to set the paused state, but commands seem to go through; the problem is, that most
		//of them are only executed on unpause (though unknown commands trigger a message in dev mode)

		//TODO: find the right scroll amount and input bits for mouse wheel (how does weapon selection do that?)
		if (gHUD.m_iKeyBits & IN_ATTACK2)
		{
			KeyValues* kv = new KeyValues("MoveScrollBar", "delta", -20);
			this->PostMessage(m_pScanTextLabel, kv);
		}
	}

}

void CHudScanInfo::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	int x, y, wide, tall; //tall (from hud size) is not used in resizing
	GetPos(x, y);
	GetHudSize(wide, tall);

	SetBounds(x, y, wide-2*x, GetTall());
	m_pScanTextLabel->SetFont(pScheme->GetFont("DebugFixed"));
	
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

	m_pScanTextLabel->SetAlpha(m_flTextAlphaOverride);
	//Msg("Focus: %d ", m_pScanTextLabel->HasFocus());
}

void CHudScanInfo::MsgFunc_ShowScanInfo(bf_read &msg)
{
	//use tokens only, as string size is limited to 128 bytes.
	if (!IsVisible())
	{
		msg.ReadString(m_szToken, sizeof(m_szToken));
		m_bScanCompleted = true; //this is set when receiving the HUD message and unset in ShouldDraw...actually does the same as m_bShouldDraw, doesn't it?
		m_pScanTextLabel->SetText(m_szToken);
		//m_pScanTextLabel->InsertString(m_szToken);
	}
}

//heads up: there is a bug probably related to the animations:
//if quickly tapping attack while the menu is fading out, it doesn't fade in anymore
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