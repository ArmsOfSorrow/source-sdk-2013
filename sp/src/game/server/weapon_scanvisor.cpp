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


#include "memdbgon.h"

#define SCAN_TIME_NORMAL 3.0f

class CWeaponScanvisor : public CBaseHLCombatWeapon
{
	DECLARE_CLASS(CWeaponScanvisor, CBaseHLCombatWeapon);

public:
	CWeaponScanvisor();
	void PrimaryAttack();
	void ItemPreFrame();

	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

private:
	float m_flScanTime;
	int m_nOldButtonState;
	bool m_bIsCurrentlyScanning;
	CBasePlayer *pPlayer;
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
	pPlayer = UTIL_GetLocalPlayer();
	if (pPlayer)
	{
		m_nOldButtonState = pPlayer->m_nButtons;
	}
}

void CWeaponScanvisor::PrimaryAttack()
{
	//two possibilities. either we were scanning last frame or not. if we were, continue and check if we're done.
	//if we were not, reset the scan time.
	//Msg("ATTACK!");

	if (m_bIsCurrentlyScanning)
	{
		if (m_flScanTime >= SCAN_TIME_NORMAL)
		{
			//pause and display stuff in vgui (in the future)
			//for now, spam messages to the console
			Msg("scan completed. entity info not implemented yet.");
		}
	}
	//BaseClass::PrimaryAttack();
}

void CWeaponScanvisor::ItemPreFrame()
{
	//before the player moves, set the right state for primary attack if the attack button is pressed.
	if (pPlayer)
	{
		if (pPlayer->m_nButtons & IN_ATTACK && m_nOldButtonState & IN_ATTACK)
		{
			//we're still scanning, update scan time in primary attack
			m_bIsCurrentlyScanning = true;
			m_flScanTime += gpGlobals->frametime;
			Msg("m_flScanTime: %f \n", m_flScanTime);
		}
		else if (pPlayer->m_nButtons & IN_ATTACK)
		{
			//we're starting a scan...wait, isn't this the same as above? nope. scan time needs a reset
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = true;
		}
		else
		{
			//we're not scanning
			m_flScanTime = 0;
			m_bIsCurrentlyScanning = false;
		}

		m_nOldButtonState = pPlayer->m_nButtons; //save old button state
	}

	BaseClass::ItemPreFrame();
}