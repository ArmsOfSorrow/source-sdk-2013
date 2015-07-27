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
	m_bShowingScannables = false;
}

void CWeaponScanvisor::Activate()
{
	BaseClass::Activate();
	CBasePlayer *pPlayer = ToBasePlayer(this->GetOwner());

	if (pPlayer && dynamic_cast<CWeaponScanvisor*>(pPlayer->GetActiveWeapon()))
	{
		//if the player owns us from the start and we're actively carried
		ShowScannablesInLevel(true);
		m_bShowingScannables = true;
	}
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


/*
Fires a trace forward to check if there is an object in 
front of the player and sets the target member of this class.
If a valid target is already set, it doesn't do anything.
*/
void CWeaponScanvisor::AcquireTarget(CBasePlayer *pPlayer)
{
	if (m_pTarget)
		return;

	trace_t result;
	Vector vecForward, vecStart, vecEnd;
	AngleVectors(EyeAngles(), &vecForward);
	vecStart = pPlayer->EyePosition();
	vecEnd = vecStart + (vecForward * SCAN_RANGE_NORMAL); //do traces with bigger range and check distances in primaryattack
	UTIL_TraceLine(vecStart, vecEnd, MASK_ALL, this, COLLISION_GROUP_NONE, &result);

	if (result.m_pEnt && result.DidHitNonWorldEntity())
	{
		m_pTarget = result.m_pEnt;
		pPlayer->SetScannedEntity(m_pTarget);
	}
}

void CWeaponScanvisor::ItemPreFrame()
{
	BaseClass::ItemPreFrame();

	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
	
	//before the player moves, set the right state for primary attack if the attack button is pressed.
	if (pPlayer)
	{

		if (pPlayer->m_nButtons & IN_ATTACK)
		{
			m_bIsCurrentlyScanning = true;
		}
		else
		{
			//we're not scanning, invalidate target
			m_bIsCurrentlyScanning = false;
			m_pTarget = nullptr;
			pPlayer->SetScannedEntity(nullptr);
			m_flScanTime = 0;
		}
	}
}

bool CWeaponScanvisor::Holster(CBaseCombatWeapon *pSwitchingTo)
{
	ShowScannablesInLevel(false);
	return BaseClass::Holster(pSwitchingTo);
}

bool CWeaponScanvisor::Deploy()
{
	ShowScannablesInLevel(true);
	return BaseClass::Deploy();
}

/*
Finds scannable entities in the global entity list and sets their state
according to the passed parameter value.
*/
void CWeaponScanvisor::ShowScannablesInLevel(bool show)
{
	/* when defining new scannable entities, make sure they begin with "scannable_", otherwise
	we won't be able to find them */
	CBaseEntity *pEnt = gEntList.FindEntityByClassname(nullptr, "scannable_*");
	CScannable *pScan;

	/* TODO: this might become a performance problem when there are lots of
	   scannables in the level. try to limit this to the PVS. */
	while (pEnt)
	{
		Msg("found ent with classname %s, entity name %s, setting state to %d\n", pEnt->GetClassname(), pEnt->GetEntityName(), show);
		
		pScan = dynamic_cast<CScannable*>(pEnt);
		if (pScan)
		{
			pScan->SetSpriteVisibility(show);
			//TODO: set alpha on models
		}

		pEnt = gEntList.FindEntityByClassname(pEnt, "scannable_*");

	}
}

LINK_ENTITY_TO_CLASS(weapon_scanvisor, CWeaponScanvisor);

PRECACHE_WEAPON_REGISTER(weapon_scanvisor);

IMPLEMENT_SERVERCLASS_ST(CWeaponScanvisor, DT_WeaponScanvisor)
END_SEND_TABLE()

BEGIN_DATADESC(CWeaponScanvisor)
END_DATADESC()