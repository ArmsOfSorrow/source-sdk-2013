//==================================================//
// Author: ArmsOfSorrow
// Purpose: A weapon to mimic Metroid Prime's scanning 
//			mechanic
//==================================================//

#include "cbase.h"
#include "basehlcombatweapon.h"
#include "basecombatweapon_shared.h"
#include "basecombatcharacter.h"
#include "in_buttons.h"
#include "playerlocaldata.h"
#include "util.h"


//#include "VGuiMatSurface/IMatSystemSurface.h"
//#include <vgui_controls/Controls.h>



#include "memdbgon.h"

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

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

private:
	float m_flScanTime;
	int m_nOldButtonState;
	bool m_bIsCurrentlyScanning;
	CBasePlayer *m_pPlayer;
	CBaseEntity *m_pTarget; //this should be an own type, to vary scanning times
	
	void AcquireTarget();
	void LockOnTarget(CBaseEntity *pEnt);
};

LINK_ENTITY_TO_CLASS(weapon_scanvisor, CWeaponScanvisor);

PRECACHE_WEAPON_REGISTER(weapon_scanvisor);

IMPLEMENT_SERVERCLASS_ST(CWeaponScanvisor, DT_WeaponScanvisor)
END_SEND_TABLE();

BEGIN_DATADESC(CWeaponScanvisor)
END_DATADESC()

CWeaponScanvisor::CWeaponScanvisor()
{
	m_bIsCurrentlyScanning = false;
	m_pPlayer = UTIL_GetLocalPlayer();
	if (m_pPlayer)
	{
		m_nOldButtonState = m_pPlayer->m_nButtons;
	}
}

void CWeaponScanvisor::PrimaryAttack()
{
	//two possibilities. either we were scanning last frame or not. if we were, continue and check if we're done.
	//if we were not, reset the scan time.

	if (m_bIsCurrentlyScanning)
	{	
		AcquireTarget();
		
		if (m_pTarget)
		{
			LockOnTarget(m_pTarget);
			Msg(m_pTarget->GetClassname());
			//UTIL_PointAtEntity(m_pPlayer, m_pTarget);
			
			/*m_pPlayer->calc*/
		}

	
		if (m_flScanTime >= SCAN_TIME_NORMAL)
		{
			//pause and display stuff in vgui (in the future)
			//for now, spam messages to the console
			Msg("scan completed. entity info not implemented yet.");
		}
	}
	//BaseClass::PrimaryAttack();
}

//-----------------------------------------------------------------------------
// Purpose: Fires a trace forward to check if there is an object in 
//			front of the player.
//-----------------------------------------------------------------------------
void CWeaponScanvisor::AcquireTarget()
{
	trace_t result;
	Vector vecForward, vecStart, vecEnd;
	AngleVectors(EyeAngles(), &vecForward);
	vecStart = m_pPlayer->EyePosition();
	vecEnd = vecStart + (vecForward * SCAN_RANGE_NORMAL); //do traces with bigger range and check distances in primaryattack
	UTIL_TraceLine(vecStart, vecEnd, MASK_ALL, this, COLLISION_GROUP_NONE, &result);

	if (result.m_pEnt && result.DidHitNonWorldEntity())
	{
		//we've got a target!
		m_pTarget = result.m_pEnt;
	}
	else
	{
		//invalidate target
		m_pTarget = nullptr;
	}
}

void CWeaponScanvisor::LockOnTarget(CBaseEntity *pEnt)
{
	//compute vector between player and target
	Vector playerToTarget;
	VectorSubtract(pEnt->GetAbsOrigin(), m_pPlayer->EyePosition(), playerToTarget);

	QAngle viewAngles;
	VectorAngles(playerToTarget, viewAngles);
	
	m_pPlayer->SnapEyeAngles(viewAngles);
}

void CWeaponScanvisor::ItemPreFrame()
{
	//before the player moves, set the right state for primary attack if the attack button is pressed.
	if (m_pPlayer)
	{
		if (m_pPlayer->m_nButtons & IN_ATTACK && m_nOldButtonState & IN_ATTACK)
		{
			//we're still scanning, update scan time in primary attack
			m_bIsCurrentlyScanning = true;
			m_flScanTime += gpGlobals->frametime;
			Msg("m_flScanTime: %f \n", m_flScanTime);
		}
		else if (m_pPlayer->m_nButtons & IN_ATTACK)
		{
			//we're starting a scan...wait, isn't this the same as above? nope. scan time needs a reset
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = true;
		}
		else
		{
			//we're not scanning
			m_pTarget = nullptr;
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = false;
		}

		m_nOldButtonState = m_pPlayer->m_nButtons; //save old button state
	}

	BaseClass::ItemPreFrame();
}