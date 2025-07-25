#include "cbase.h"
#include "fof/fof_weapon_fists.h"
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

#define	FISTS_RANGE		65.0f
#define	FISTS_REFIRE	0.8f

//-----------------------------------------------------------------------------
// CWeaponFists
//-----------------------------------------------------------------------------

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponFists, DT_WeaponFists )

BEGIN_NETWORK_TABLE( CWeaponFists, DT_WeaponFists )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponFists )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_fists, CWeaponFists );
PRECACHE_WEAPON_REGISTER( weapon_fists );

acttable_t	CWeaponFists::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,					ACT_HL2MP_IDLE_FISTS,					false },
	{ ACT_MP_CROUCH_IDLE,					ACT_HL2MP_IDLE_CROUCH_FISTS,			false },
	{ ACT_MP_RUN,							ACT_HL2MP_RUN_FISTS,					false },
	{ ACT_MP_CROUCHWALK,					ACT_HL2MP_WALK_CROUCH_FISTS,			false },
	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,		ACT_HL2MP_GESTURE_RANGE_ATTACK_FISTS,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,		ACT_HL2MP_GESTURE_RANGE_ATTACK_FISTS,	false },
	{ ACT_MP_ATTACK_STAND_SECONDARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_FISTS2,	false },
	{ ACT_MP_ATTACK_CROUCH_SECONDARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_FISTS2,	false },
	{ ACT_MP_RELOAD_STAND,					ACT_HL2MP_GESTURE_RELOAD_FISTS,			false },
	{ ACT_MP_RELOAD_CROUCH,					ACT_HL2MP_GESTURE_RELOAD_FISTS,			false },
	{ ACT_MP_JUMP,							ACT_HL2MP_JUMP_FISTS,					false },
};

IMPLEMENT_ACTTABLE(CWeaponFists);

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponFists::CWeaponFists( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Get the damage amount for the animation we're doing
// Input  : hitActivity - currently played activity
// Output : Damage amount
//-----------------------------------------------------------------------------
float CWeaponFists::GetDamageForActivity( Activity hitActivity )
{
	return 25.0f;
}

//-----------------------------------------------------------------------------
// Purpose: Add in a view kick for this weapon
//-----------------------------------------------------------------------------
void CWeaponFists::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle punchAng;

	punchAng.x = SharedRandomFloat( "fistspax", 4.0f, 6.0f );
	punchAng.y = SharedRandomFloat( "fistspay", -2.0f, -1.0f );
	punchAng.z = 0.0f;
	
	pPlayer->ViewPunch( punchAng ); 
}

float CWeaponFists::GetRange( void )
{
	return	FISTS_RANGE;	
}

float CWeaponFists::GetFireRate( void )
{
	return	FISTS_REFIRE;	
}