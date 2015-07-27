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
#include <vgui_controls\RichText.h>
#include "iinput.h"
#include "iscaninfopanel.h"


class CHudScanInfo : public CHudElement, public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CHudScanInfo, vgui::EditablePanel);
	
public:

	CHudScanInfo(const char *pElementName);
	~CHudScanInfo();
	virtual void Init() override;
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme) override;
	virtual void ApplySettings(KeyValues* inResourceData) override;
	virtual bool ShouldDraw() override;
	virtual void Paint() override;
	virtual void ProcessInput() override;
	virtual void OnThink() override;
	void MsgFunc_ShowScanInfo(bf_read &msg);
	

private:

	int m_nOldButtonState;
	float m_flLastInputTime;
	const float m_flFadeThreshold = 0.5f;
	bool m_bFadingOut;
	bool m_bScanCompleted;
	bool m_bShouldDraw;
	bool m_bOldDrawState;
	char m_szToken[128];
	vgui::RichText *m_pScanTextLabel;
	
	CPanelAnimationVar(float, m_flAlphaOverride, "Alpha", "0");
	CPanelAnimationVar(float, m_flTextAlphaOverride, "TextAlpha", "0");
	
};

DECLARE_HUDELEMENT(CHudScanInfo);
DECLARE_HUD_MESSAGE(CHudScanInfo, ShowScanInfo);

CHudScanInfo::CHudScanInfo(const char *pElementName) : CHudElement(pElementName), BaseClass(nullptr, "HudScanInfo")
{
	//parent it to scaninfopanel instead of viewport
	vgui::Panel *pParent = sp->Get();

	int wide, tall;
	pParent->GetSize(wide, tall);
	DevMsg("scaninfopanel width: %d, height: %d\n", wide, tall);

	SetParent(pParent);
	SetFgColor(Color(0, 0, 0, 255));

	m_nOldButtonState = 0;
	m_flLastInputTime = 0.0f;
	m_bFadingOut = false;
	m_bScanCompleted = false;
	m_bShouldDraw = false;
	m_bOldDrawState = false;
	
	//this does not load font and other settings because richtext does not support it.
	//either change ApplySettings in richtext.cpp or write stuff into HudScanInfo.res, as those
	//keyvalues are available in ApplySettings
	LoadControlSettings("resource/scaninfo.res"); 
	m_pScanTextLabel = FindControl<vgui::RichText>("ScanTextLabel");
	if (m_pScanTextLabel)
	{
		m_pScanTextLabel->SetProportional(true);
		m_pScanTextLabel->SetVisible(true);
	}
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

	DevMsg("CHudScanInfo width: %d, height: %d\n", GetWide(), GetTall());
}

void CHudScanInfo::ApplySettings(KeyValues* inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	CKeyValuesDumpContextAsDevMsg ctx(1);
	inResourceData->Dump(&ctx);

	//LoadControlSettings("resource/scaninfo.res");
	
	//KeyValues *pFontKey = inResourceData->FindKey("font");
	//if (pFontKey)
	//{
	//	//m_pScanTextLabel->SetFont(pFontKey->GetString());
	//	DevMsg("found font key\n");
	//}
	

	//m_pScanTextLabel->SetBgColor();
	
	
	/* man, this is a real pain in the ass. plan for next time:
	   1. find a way to override scrollbar colors within richtext control
	   
	   this could come in handy: https://developer.valvesoftware.com/wiki/Understanding_VGUI2_Resource_Files
	   */
}

void CHudScanInfo::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
	
	/* ugly solution, but fucking around with non-funtional resource files 
	   isn't much fun either */
	vgui::HFont font = pScheme->GetFont("DebugFixed");
	if (font)
	{
		m_pScanTextLabel->SetFont(font);
	}
	
	//Color bgColor = pScheme->GetColor("BgColor", Color(0,0,0,76));
	//m_pScanTextLabel->SetBgColor(bgColor);
}

void CHudScanInfo::ProcessInput()
{
	CHudElement::ProcessInput();
	
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

		//TODO: find the right scroll amount
		//there are no input bits for the mouse wheel. weapon selection does its work with a usercmd, but we will
		//need weapon selection (most likely), so we need to find another way to make this
		if ((gHUD.m_iKeyBits & IN_ATTACK2) && !(m_nOldButtonState & IN_ATTACK2))
		{
			KeyValues* kv = new KeyValues("MoveScrollBarDirect", "delta", -2);
			this->PostMessage(m_pScanTextLabel, kv);
		}
	}

}

//void CHudScanInfo::ApplySchemeSettings(vgui::IScheme *pScheme)
//{
//	
//	BaseClass::ApplySchemeSettings(pScheme);
//	LoadControlSettings("resource/scaninfo.res");
//	//int x, y, wide, tall; //tall (from hud size) is not used in resizing
//	//GetPos(x, y);
//	//GetHudSize(wide, tall);
//
//	//SetBounds(x, y, wide-2*x, GetTall());
//	//m_pScanTextLabel->SetFont(pScheme->GetFont("DebugFixed"));
//	
//}

bool CHudScanInfo::ShouldDraw()
{
	m_bOldDrawState = m_bShouldDraw;

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();

	if (pPlayer)
	{
		if (pPlayer->GetScannedEntity() && m_bScanCompleted)
		{
			m_bShouldDraw = true;
			m_bFadingOut = false;
		}
		else
		{
			//we don't have a scanned entity anymore
			m_bScanCompleted = false;
			m_bShouldDraw = false;
			m_bFadingOut = true;
		}
	}
	
	
	//if we don't have a scanned entity, draw it until it fades out
	m_bShouldDraw = m_bShouldDraw || (gpGlobals->curtime - m_flLastInputTime) < m_flFadeThreshold;
	return m_bShouldDraw;
}

void CHudScanInfo::Paint()
{
	//maybe check for player state/entity flags to not draw the HUD under special circumstances
	BaseClass::Paint();

	SetAlpha(m_flAlphaOverride);
	g_pMatSystemSurface->DrawSetTextColor(255,255,255, m_flTextAlphaOverride);

	m_pScanTextLabel->SetAlpha(m_flTextAlphaOverride);
}

void CHudScanInfo::MsgFunc_ShowScanInfo(bf_read &msg)
{
	//use tokens only, as string size is limited to 128 bytes.
	//if (!IsVisible())
	//{
		msg.ReadString(m_szToken, sizeof(m_szToken));
		m_bScanCompleted = true;
		m_pScanTextLabel->SetText(m_szToken);
		DevMsg("got text message\n");
		//m_pScanTextLabel->GotoTextStart();
	//}
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
		
		if ((m_bOldDrawState == false && m_bScanCompleted) || (m_bFadingOut && m_bScanCompleted)) //or if we're fading out
		{
			DevMsg("fading in %d %d %d\n", m_bOldDrawState, m_bScanCompleted, m_bFadingOut);
			m_bOldDrawState = true;

			//fade in, as we were not drawing in the last frame
			sp->GetAnimationController()->StartAnimationSequence("OpenScanInfo");
			
			m_bFadingOut = false;
			engine->ClientCmd("pause");
		}

		m_flLastInputTime = gpGlobals->curtime;
	}
	else
	{
		if ((m_nOldButtonState & IN_ATTACK) /*&& !(gHUD.m_iKeyBits & IN_ATTACK)*/ && !(m_bScanCompleted))
		{
			//start fading out here if we released the attack button
			sp->GetAnimationController()->StartAnimationSequence("FadeOutScanInfo");
			DevMsg("fading out %d %d\n", m_nOldButtonState & IN_ATTACK, gHUD.m_iKeyBits & IN_ATTACK);
			engine->ClientCmd("pause");
		}
	}
	m_nOldButtonState = gHUD.m_iKeyBits;
}