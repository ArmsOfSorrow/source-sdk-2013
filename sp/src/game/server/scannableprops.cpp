#include "cbase.h"
#include "scannableprops.h"
#include "func_scan.h"


CScannableDynamicProp::CScannableDynamicProp()
{
}

CScannableDynamicProp::~CScannableDynamicProp()
{
}

void CScannableDynamicProp::Spawn()
{
	BaseClass::Spawn();
}

void CScannableDynamicProp::InputShowScanInfo(inputdata_t &inputdata)
{
	//ShowScanInfo();
}

BEGIN_DATADESC(CScannableDynamicProp)

DEFINE_INPUTFUNC(FIELD_VOID, "ShowScanInfo", InputShowScanInfo),
IMPLEMENT_SCANNABLE()

END_DATADESC()

LINK_ENTITY_TO_CLASS(scannable_prop_dynamic, CScannableDynamicProp);