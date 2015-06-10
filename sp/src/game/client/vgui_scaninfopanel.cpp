#include "cbase.h"
#include "iscaninfopanel.h"
#include <vgui_controls\EditablePanel.h>
#include <vgui\IVGui.h>
#include "VGuiMatSurface/IMatSystemSurface.h"
#include <vgui_controls/Controls.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/IPanel.h>
#include "hud_macros.h"

class CScanInfoPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CScanInfoPanel, vgui::EditablePanel);

public:
	CScanInfoPanel(vgui::VPANEL parent);
	~CScanInfoPanel();

	virtual void Init(int x, int y, int wide, int tall) override;
	virtual void OnTick();
	//virtual void OnCommand(const char *pszCommand);
	//virtual bool ShouldDraw();

private:
	//MESSAGE_FUNC(OnShowScanInfo, "ShowScanInfo");
	
	void __MsgFunc_ShowScanInfo();
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

	vgui::ivgui()->AddTickSignal(GetVPanel(), 250);
	DevMsg("scaninfopanel has been constructed.\n");
}

void CScanInfoPanel::Init(int x, int y, int wide, int tall)
{
	BaseClass::Init(x, y, wide, tall);
}

CScanInfoPanel::~CScanInfoPanel()
{

}

void CScanInfoPanel::OnTick()
{

}

void CScanInfoPanel::__MsgFunc_ShowScanInfo()
{

}

class CScanInfo : public IScanInfoPanel
{
private:
	CScanInfoPanel *pScanInfoPanel;

public:
	CScanInfo()
	{
		pScanInfoPanel = nullptr;
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