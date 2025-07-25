#include "cbase.h"
#include "fof/fof_weapon_hammer.h"
#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "gamerules.h"
#include "ammodef.h"
#include "mathlib/mathlib.h"
#include "in_buttons.h"
#include "vstdlib/random.h"
#include "npcevent.h"

#if defined( CLIENT_DLL )
#include "c_hl2mp_player.h"
#else
#include "hl2mp_player.h"
#include "ai_basenpc.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponHammer, DT_WeaponHammer )

BEGIN_NETWORK_TABLE( CWeaponHammer, DT_WeaponHammer )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponHammer )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_hammer, CWeaponHammer );
PRECACHE_WEAPON_REGISTER( weapon_hammer );

CWeaponHammer::CWeaponHammer()
{
	m_bStartedPrimaryAttack = false;
}

void CWeaponHammer::StartPrimaryAttack( void )
{
	SendWeaponAnim( ACT_VM_HAULBACK );
	ResetViewmodelAnimation();

	m_bStartedPrimaryAttack = true;
}

void CWeaponHammer::PrimaryAttack( void )
{
	BaseClass::PrimaryAttack();

	m_bStartedPrimaryAttack = false;
}

void CWeaponHammer::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	
	if ( pOwner == NULL )
		return;

	CheckThrow();

	if ( (m_flNextPrimaryAttack <= gpGlobals->curtime) )
	{
		if ( (pOwner->m_nButtons & IN_ATTACK) )
		{
			if ( !m_bStartedPrimaryAttack )
			{
				StartPrimaryAttack();
			}
		}
		else
		{
			if ( m_bStartedPrimaryAttack )
			{
				PrimaryAttack();
			}
			else
			{
				if ( (pOwner->m_nButtons & IN_ATTACK2) && (m_flNextSecondaryAttack <= gpGlobals->curtime) )
				{
					SecondaryAttack();
				}
				else 
				{
					WeaponIdle();
					return;
				}
			}
		}
	}
}