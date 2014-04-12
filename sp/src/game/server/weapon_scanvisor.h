//==================================================//
// Author: ArmsOfSorrow
// Purpose: A weapon to mimic Metroid Prime's scanning 
//			mechanic
//==================================================//

#pragma once
#include "cbase.h"
#include "basehlcombatweapon.h"
#include "basecombatweapon_shared.h"
#include "basecombatcharacter.h"
#include "in_buttons.h"
#include "playerlocaldata.h"
#include "util.h"
#include "usercmd.h"
#include "const.h"


#define SCAN_TIME_NORMAL 3.0f
#define SCAN_RANGE_NORMAL 512 //test range, might change during balancing (if I ever come that far)

class CWeaponScanvisor : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponScanvisor, CBaseHLCombatWeapon);

public:
	CWeaponScanvisor();
	void PrimaryAttack();
	void ItemPreFrame();
	void DryFire();
	int UpdateTransmitState();

	CNetworkVar(bool, m_bIsCurrentlyScanning);
	CNetworkVar(EHANDLE, m_hTarget);

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

private:
	float m_flScanTime;
	int m_nOldButtonState;

	CBasePlayer *m_pPlayer;
	CBaseEntity *m_pTarget; //this should be an own type, to vary scanning times

	void AcquireTarget();
	void LockOnTarget(CBaseEntity *pEnt);
};

