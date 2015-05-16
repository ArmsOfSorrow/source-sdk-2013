#include "cbase.h"
#include "weapon_scanvisor.h"
#include "player.h"
#include "in_buttons.h"
#include "func_scan.h"
#include "convar.h"
#include "ai_basenpc.h"


CWeaponScanvisor::CWeaponScanvisor()
{
	m_bIsCurrentlyScanning = false;
	m_nOldButtonState = 0;
}

void CWeaponScanvisor::OnPickedUp(CBaseCombatCharacter *pNewOwner)
{
	//m_pPlayer = ToBasePlayer(pNewOwner); 
}

void CWeaponScanvisor::PrimaryAttack()
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer && m_bIsCurrentlyScanning)
	{	
		AcquireTarget(pPlayer);
		
		if (dynamic_cast<CScannable*>(m_pTarget))
		{
			dynamic_cast<CScannable*>(m_pTarget)->UpdateScanTime(m_flScanTime, pPlayer, m_pTarget);
			m_flScanTime += gpGlobals->frametime;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Fires a trace forward to check if there is an object in 
//			front of the player.
//-----------------------------------------------------------------------------
void CWeaponScanvisor::AcquireTarget(CBasePlayer *pPlayer)
{

	trace_t result;
	Vector vecForward, vecStart, vecEnd;
	AngleVectors(EyeAngles(), &vecForward);
	vecStart = pPlayer->EyePosition();
	vecEnd = vecStart + (vecForward * SCAN_RANGE_NORMAL); //do traces with bigger range and check distances in primaryattack
	UTIL_TraceLine(vecStart, vecEnd, MASK_ALL, this, COLLISION_GROUP_NONE, &result);

	if (result.m_pEnt && result.DidHitNonWorldEntity())
	{
		m_pTarget = result.m_pEnt;
	}
	else
	{
		//invalidate target
		m_pTarget = nullptr;
	}
}

void CWeaponScanvisor::ItemPreFrame()
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
	
	//before the player moves, set the right state for primary attack if the attack button is pressed.
	if (pPlayer)
	{
		if (pPlayer->m_nButtons & IN_ATTACK && m_nOldButtonState & IN_ATTACK)
		{
			m_bIsCurrentlyScanning = true;
		}
		else if (pPlayer->m_nButtons & IN_ATTACK)
		{
			//we're starting a new scan...
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = true;
		}
		else
		{
			//we're not scanning
			m_pTarget = nullptr;
			pPlayer->SetScannedEntity(NULL);
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = false;
		}

		m_nOldButtonState = pPlayer->m_nButtons; //save old button state
	}

	BaseClass::ItemPreFrame();
}

LINK_ENTITY_TO_CLASS(weapon_scanvisor, CWeaponScanvisor);

PRECACHE_WEAPON_REGISTER(weapon_scanvisor);

IMPLEMENT_SERVERCLASS_ST(CWeaponScanvisor, DT_WeaponScanvisor)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponScanvisor)
END_DATADESC()

//void PauseEntities(IConVar *pVar, const char *pOldValue, float flOldValue);

//ConVar pause_entities("ltp_pause", "0", FCVAR_CHEAT, "put all entities into a dormant state", PauseEntities);
//ConCommand pause_ents("ltp_pause_entities", PauseEntities, "puts all entities into a dormant state.", 0);

//void PauseEntities(IConVar *pVar, const char *pOldValue, float flOldValue)
//{
//
//	//CBaseEntity *pEnt = gEntList.FindEntityByClassname(nullptr, "npc_*");
//	CBaseEntity *pEnt = gEntList.FirstEnt();
//
//	if (pause_entities.GetBool())
//	{
//		while (pEnt)
//		{
//			/*CAI_BaseNPC *pNPC = dynamic_cast<CAI_BaseNPC*>(pEnt);
//			if (pNPC)
//			{
//				pNPC->MakeDormant();
//			}
//			pEnt = gEntList.FindEntityByClassname(pEnt, "npc_*");*/
//
//			pEnt->MakeDormant();
//			pEnt = gEntList.NextEnt(pEnt);
//		}
//	}
//	else
//	{
//
//	}
//
//	//so...basic freezing does work, although there are several limitations which will
//	//inevitably lead to me writing my own function(s):
//	//1) dormant objects are not drawn
//	//2) while player movement is frozen, mouse movement is not
//	//3) there is no method to reactivate dormant entities
//	
//	//also, i should move this out of weapon_scanvisor.cpp
//}