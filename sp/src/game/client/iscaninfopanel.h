//==================================================//
// Author: ArmsOfSorrow
// Purpose: Separate panel to display entity info
//			when scanned.
//==================================================//

#pragma once

#include <vgui\VGUI.h>

namespace vgui
{
	class EditablePanel;
	class AnimationController;
}

abstract_class IScanInfoPanel
{
public:
	virtual void Create(vgui::VPANEL parent) = 0;
	virtual void Destroy() = 0;
	virtual vgui::EditablePanel *Get() = 0;
	virtual vgui::AnimationController *GetAnimationController() = 0;
};

extern IScanInfoPanel *sp;