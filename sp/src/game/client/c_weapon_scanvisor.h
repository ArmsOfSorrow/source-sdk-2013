#include "cbase.h"
#include "c_basehlcombatweapon.h"
#include "c_weapon__stubs.h"
#pragma once

class C_WeaponScanvisor : public C_BaseHLCombatWeapon
{
public:
	DECLARE_CLASS(C_WeaponScanvisor, C_BaseHLCombatWeapon);
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();

	/*C_WeaponScanvisor();
	virtual void ClientThink();*/
	bool m_bIsCurrentlyScanning;
	//bool m_bLockedOnTarget;
	
private:
	C_BasePlayer *m_pPlayer;
	
	//idea for view interpolation:
	//- network target entity pointer
	//- if we have a target, interpolate angles based on the target position
};

STUB_WEAPON_CLASS_IMPLEMENT(weapon_scanvisor, C_WeaponScanvisor);

IMPLEMENT_CLIENTCLASS_DT(C_WeaponScanvisor, DT_WeaponScanvisor, CWeaponScanvisor)
RecvPropBool(RECVINFO(m_bIsCurrentlyScanning))//,
//RecvPropBool(RECVINFO(m_bLockedOnTarget))
END_RECV_TABLE()