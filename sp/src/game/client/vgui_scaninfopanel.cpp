#include "cbase.h"
#include "iscaninfopanel.h"
#include <vgui_controls\EditablePanel.h>
#include <vgui\IVGui.h>
#include "VGuiMatSurface/IMatSystemSurface.h"
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/Controls.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/IPanel.h>

class CScanInfoPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CScanInfoPanel, vgui::EditablePanel);

public:
	CScanInfoPanel(vgui::VPANEL parent);
	~CScanInfoPanel();

	virtual void Init(int x, int y, int wide, int tall) override;
	//virtual void OnTick();
	virtual void OnThink() override;
	vgui::AnimationController *GetAnimationController();


private:

	//our scaninfo HUD element is not parented to the viewport, which
	//owns "the" AnimationController. that's why we need another one.
	vgui::AnimationController *m_pAnimController;
};

CScanInfoPanel::CScanInfoPanel(vgui::VPANEL parent) : BaseClass(NULL, "ScanInfoPanel")
{
	int w, h;
	vgui::surface()->GetScreenSize(w, h);
	SetSize(w, h);
	SetParent(parent);
	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));
	SetEnabled(true);

	m_pAnimController = new vgui::AnimationController(this);
	m_pAnimController->SetProportional(true);
	
	
	if (!m_pAnimController->SetScriptFile(GetVPanel(), "scripts/hudanimations.txt", true))
		Assert(0);

	//vgui::ivgui()->AddTickSignal(GetVPanel(), 250);
	DevMsg("scaninfopanel has been constructed.\n");
}

void CScanInfoPanel::Init(int x, int y, int wide, int tall)
{
	BaseClass::Init(x, y, wide, tall);
}

CScanInfoPanel::~CScanInfoPanel()
{

}

//void CScanInfoPanel::OnTick()
//{
//
//}

void CScanInfoPanel::OnThink()
{
	m_pAnimController->UpdateAnimations(engine->Time());
}


vgui::AnimationController *CScanInfoPanel::GetAnimationController()
{
	return this->m_pAnimController;
}

/* yeah, I know it's a clusterfuck, but the whole codebase is (more or less). */
class CScanInfo : public IScanInfoPanel
{
private:
	CScanInfoPanel *pScanInfoPanel;

public:
	CScanInfo()
	{
		pScanInfoPanel = nullptr;
	}

	vgui::AnimationController *GetAnimationController()
	{
		return pScanInfoPanel->GetAnimationController();
	}

	vgui::EditablePanel *Get()
	{
		if (pScanInfoPanel)
		{
			return pScanInfoPanel;
		}

		DevMsg("scaninfopanel does not exist.");
		return nullptr;
	}

	void Create(vgui::VPANEL parent)
	{
		pScanInfoPanel = new CScanInfoPanel(parent);
	}

	void Destroy()
	{
		if (pScanInfoPanel)
		{
			pScanInfoPanel->SetParent(nullptr);
			delete pScanInfoPanel;
			pScanInfoPanel = nullptr;
		}
	}
};

static CScanInfo g_ScanInfoPanel;
IScanInfoPanel *sp = (IScanInfoPanel *)&g_ScanInfoPanel;