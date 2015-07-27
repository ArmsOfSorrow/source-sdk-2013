//==================================================//
// Author: ArmsOfSorrow
// Purpose: A weapon to mimic Metroid Prime's scanning 
//			mechanic
//==================================================//

#pragma once
#include "cbase.h"
#include "basehlcombatweapon.h"
#include "func_scan.h"


#define SCAN_TIME_NORMAL 3.0f
#define SCAN_RANGE_NORMAL 512 //test range, might change during balancing (if I ever come that far)

class CWeaponScanvisor : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponScanvisor, CBaseHLCombatWeapon);

public:
	CWeaponScanvisor();
	virtual void PrimaryAttack() override;
	virtual void ItemPreFrame() override;
	virtual void Activate() override;
	//void DryFire(); //if object is out of range. not the best name, though.
	virtual bool Holster(CBaseCombatWeapon *pSwitchingTo) override;
	virtual bool Deploy() override;

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

private:
	bool m_bIsCurrentlyScanning;
	bool m_bShowingScannables;
	float m_flScanTime;

	CBaseEntity *m_pTarget;

	void AcquireTarget(CBasePlayer *pPlayer);
	void ShowScannablesInLevel(bool show);
};

