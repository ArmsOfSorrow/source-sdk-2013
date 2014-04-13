#pragma once

#include "cbase.h"
#include "c_basehlcombatweapon.h"
#include "c_weapon__stubs.h"



class C_WeaponScanvisor : public C_BaseHLCombatWeapon
{
public:
	DECLARE_CLASS(C_WeaponScanvisor, C_BaseHLCombatWeapon);
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	/*C_WeaponScanvisor();*/
	//void ClientThink();

	bool m_bIsCurrentlyScanning;
	//EHANDLE m_hTarget;
	//client think for updating convar
};

STUB_WEAPON_CLASS_IMPLEMENT(weapon_scanvisor, C_WeaponScanvisor);

IMPLEMENT_CLIENTCLASS_DT(C_WeaponScanvisor, DT_WeaponScanvisor, CWeaponScanvisor)
RecvPropBool(RECVINFO(m_bIsCurrentlyScanning)),
//RecvPropEHandle(RECVINFO(m_hTarget))
END_RECV_TABLE()