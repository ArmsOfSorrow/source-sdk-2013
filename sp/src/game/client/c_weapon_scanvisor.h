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
	/*C_WeaponScanvisor();*/
	//void ClientThink();

	bool m_bIsCurrentlyScanning;
	//client think for updating convar
};

STUB_WEAPON_CLASS_IMPLEMENT(weapon_scanvisor, C_WeaponScanvisor);

IMPLEMENT_CLIENTCLASS_DT(C_WeaponScanvisor, DT_WeaponScanvisor, CWeaponScanvisor)
RecvPropBool(RECVINFO(m_bIsCurrentlyScanning))
END_RECV_TABLE()