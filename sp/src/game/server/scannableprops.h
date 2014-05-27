#pragma once
#include "baseentity.h"
#include "props.h"
#include "func_scan.h"

class CScannableDynamicProp : public CDynamicProp, public CScannable
{
public:

	DECLARE_CLASS(CScannableDynamicProp, CDynamicProp);

	DECLARE_DATADESC();
	CScannableDynamicProp();
	~CScannableDynamicProp();
	void Spawn();
	void InputShowScanInfo(inputdata_t &inputdata);

private:
};
