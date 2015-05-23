#include "cbase.h"
#include "scannableprops.h"
#include "func_scan.h"


CScannableDynamicProp::CScannableDynamicProp() 
	: CScannable()
{
}

CScannableDynamicProp::~CScannableDynamicProp()
{
}

//void CScannableDynamicProp::Spawn()
//{
//	Msg(m_szSpritePath.ToCStr());
//}

void CScannableDynamicProp::Activate()
{
	BaseClass::Activate();

	InitScannableSprite(GetBaseEntity(), GetAbsOrigin(), false);
}


BEGIN_DATADESC(CScannableDynamicProp)

DEFINE_SCANNABLE_DATADESC()

END_DATADESC()

LINK_ENTITY_TO_CLASS(scannable_prop_dynamic, CScannableDynamicProp);