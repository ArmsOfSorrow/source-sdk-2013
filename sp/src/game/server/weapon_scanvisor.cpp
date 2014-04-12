#include "cbase.h"
#include "weapon_scanvisor.h"



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
		}

	
		if (m_flScanTime >= SCAN_TIME_NORMAL)
		{
			//pause and display stuff in vgui (in the future)
			//for now, spam messages to the console
			Msg("scan completed. entity info not implemented yet.");
		}
	}
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
		m_hTarget = m_pTarget->GetRefEHandle();
	}
	else
	{
		//invalidate target
		m_pTarget = nullptr;
		m_hTarget = nullptr;
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
	//UpdateClientData(m_pPlayer);
}

void CWeaponScanvisor::ItemPreFrame()
{
	//before the player moves, set the right state for primary attack if the attack button is pressed.
	if (m_pPlayer)
	{
		if (m_pPlayer->m_nButtons & IN_ATTACK && m_nOldButtonState & IN_ATTACK)
		{
			//we're still scanning, update scan time in primary attack
			m_bIsCurrentlyScanning = true; //FIXME: set isCurrentlyScanning only if we actually have a target
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
			m_hTarget = nullptr;
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = false;
		}

		m_nOldButtonState = m_pPlayer->m_nButtons; //save old button state
	}

	BaseClass::ItemPreFrame();
}

int CWeaponScanvisor::UpdateTransmitState()
{
	return SetTransmitState(FL_EDICT_ALWAYS);
}

LINK_ENTITY_TO_CLASS(weapon_scanvisor, CWeaponScanvisor);

PRECACHE_WEAPON_REGISTER(weapon_scanvisor);

IMPLEMENT_SERVERCLASS_ST(CWeaponScanvisor, DT_WeaponScanvisor)
SendPropBool(SENDINFO(m_bIsCurrentlyScanning)),
SendPropEHandle(SENDINFO(m_hTarget))
END_SEND_TABLE();

BEGIN_DATADESC(CWeaponScanvisor)
END_DATADESC()
