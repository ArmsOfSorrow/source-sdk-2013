//==================================================//
// Author: ArmsOfSorrow
// Purpose: Separate panel to display entity info
//			when scanned.
//==================================================//

#pragma once

#include <vgui\VGUI.h>

namespace vgui
{
	class Panel;
}

abstract_class IScanInfoPanel
{
public:
	virtual void Create(vgui::VPANEL parent) = 0;
	virtual void Destroy() = 0;
	virtual vgui::Panel *Get() = 0;
};

extern IScanInfoPanel *sp;