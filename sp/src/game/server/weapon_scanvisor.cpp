#include "cbase.h"
#include "weapon_scanvisor.h"
#include "player.h"
#include "in_buttons.h"
#include "func_scan.h"



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
	if (m_bIsCurrentlyScanning)
	{	
		AcquireTarget();
		
		if (m_pTarget)
		{
			m_pTarget->UpdateScanTime(m_flScanTime, m_pPlayer);
			m_flScanTime += gpGlobals->frametime;
			//m_pPlayer->SetScannedEntity(m_pTarget); //call this in CScannable instead of here.
			//Msg("m_flScanTime: %f \n", m_flScanTime);
			//Msg(m_pTarget->GetClassname());
		}

	
		//if (m_flScanTime >= SCAN_TIME_NORMAL)
		//{
		//	//pause and display stuff in vgui (in the future)
		//	//for now, spam messages to the console
		//	Msg("scan completed. entity info not implemented yet.");

		//	//call showscaninfo here. some pause state would be needed, though.
		//}
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
		m_pTarget = dynamic_cast<CScannable*>(result.m_pEnt);
		//m_pTarget = result.m_pEnt;
	}
	else
	{
		//invalidate target
		m_pTarget = nullptr;
	}
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
		}
		else if (m_pPlayer->m_nButtons & IN_ATTACK)
		{
			//we're starting a new scan...
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = true;
		}
		else
		{
			//we're not scanning
			m_pTarget = nullptr;
			m_pPlayer->SetScannedEntity(NULL);
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = false;
		}

		m_nOldButtonState = m_pPlayer->m_nButtons; //save old button state
	}

	BaseClass::ItemPreFrame();
}

LINK_ENTITY_TO_CLASS(weapon_scanvisor, CWeaponScanvisor);

PRECACHE_WEAPON_REGISTER(weapon_scanvisor);

IMPLEMENT_SERVERCLASS_ST(CWeaponScanvisor, DT_WeaponScanvisor)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponScanvisor)
END_DATADESC()