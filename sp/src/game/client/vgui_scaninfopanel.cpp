//#include "cbase.h"
//#include "iscaninfopanel.h"
//#include <vgui_controls\Panel.h>
//#include <vgui\IVGui.h>
//#include "VGuiMatSurface/IMatSystemSurface.h"
//#include <vgui_controls/Controls.h>
//#include <vgui/ISurface.h>
//#include <vgui/IScheme.h>
//#include <vgui/IPanel.h>

#include "cbase.h"
#include "ifpspanel.h"
#include <vgui_controls/Panel.h>
#include "view.h"
#include <vgui/IVGui.h>
#include "VGuiMatSurface/IMatSystemSurface.h"
#include <vgui_controls/Controls.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/IPanel.h>
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "filesystem.h"
#include "../common/xbox/xboxstubs.h"
#include "steam/steam_api.h"
#include "iscaninfopanel.h"

class CScanInfoPanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CScanInfoPanel, vgui::Panel);

public:
	CScanInfoPanel(vgui::VPANEL parent);
	~CScanInfoPanel();

	virtual void OnTick();
	virtual void OnCommand(const char *pszCommand);
};

CScanInfoPanel::CScanInfoPanel(vgui::VPANEL parent) : BaseClass(NULL, "ScanInfoPanel")
{
	SetParent(parent);
	SetKeyBoardInputEnabled(true);
	SetMouseInputEnabled(true);
	SetProportional(false);
	SetScheme(vgui::scheme()->LoadSchemeFromFile("resource/SourceScheme.res", "SourceScheme"));

	vgui::ivgui()->AddTickSignal(GetVPanel(), 250);
	DevMsg("scaninfopanel has been constructed.");
}

CScanInfoPanel::~CScanInfoPanel()
{

}

void CScanInfoPanel::OnTick()
{

}

void CScanInfoPanel::OnCommand(const char *pszCommand)
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

	void Create(vgui::VPANEL parent)
	{
		pScanInfoPanel = new CScanInfoPanel(parent);
	}

	void Destroy()
	{
		if (pScanInfoPanel)
		{
			pScanInfoPanel->SetParent(nullptr); // i wonder if this actually works
			delete pScanInfoPanel;
			pScanInfoPanel = nullptr;
		}
	}
};

static CScanInfo g_ScanInfoPanel;
IScanInfoPanel *sp = (IScanInfoPanel *)&g_ScanInfoPanel;