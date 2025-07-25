#include "cbase.h"
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
	#include "fof/fof_projectile_arrow.h"
	#include "hl2mp_player.h"
	#include "ai_basenpc.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CLIENT_DLL
#define CWeaponBow C_WeaponBow
#define CWeaponBowBlack C_WeaponBowBlack
#define CWeaponBowExplosive C_WeaponBowExplosive
#endif

class CWeaponBow : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponBow, CBaseHL2MPCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponBow();

	virtual CBaseEntity* FireProjectile() OVERRIDE;
};

CBaseEntity *CWeaponBow::FireProjectile()
{
#ifdef GAME_DLL
	CHL2MP_Player *pPlayer = GetHL2MPPlayerOwner();

	if (!pPlayer)
		return NULL;

	return CFOFProjectileArrow::CreateArrow( pPlayer, this, pPlayer->Weapon_ShootPosition(), pPlayer->EyeAngles() ); // GTODO : when you do custom weapons and the like , make this an actual projectile check
#else
	return NULL;
#endif // GAME_DLL
}

class CWeaponBowBlack : public CWeaponBow
{
public:
	DECLARE_CLASS( CWeaponBowBlack, CWeaponBow );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponBowExplosive : public CWeaponBow
{
public:
	DECLARE_CLASS( CWeaponBowExplosive, CWeaponBow );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

CREATE_SIMPLE_WEAPON_TABLE( WeaponBow, weapon_bow );
CREATE_SIMPLE_WEAPON_TABLE( WeaponBowBlack, weapon_bow_black );
CREATE_SIMPLE_WEAPON_TABLE( WeaponBowExplosive, weapon_xbow );

acttable_t	CWeaponBow::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_PISTOL,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_PISTOL,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_PISTOL,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_PISTOL,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_PISTOL,					false },
};

IMPLEMENT_ACTTABLE( CWeaponBow );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponBow::CWeaponBow( void )
{
}