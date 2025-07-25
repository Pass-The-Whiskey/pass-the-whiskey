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
	#include "hl2mp_player.h"
	#include "ai_basenpc.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CLIENT_DLL
#define CWeaponRevolver C_WeaponRevolver

#define CWeaponWalker C_WeaponWalker
#define CWeaponPeacemaker C_WeaponPeacemaker

#define CWeaponDeringer C_WeaponDeringer
#define CWeaponVolcanic C_WeaponVolcanic

#define CWeaponColtNavy C_WeaponColtNavy
#define CWeaponSchofield C_WeaponSchofield

#define CWeaponRemingtonArmy C_WeaponRemingtonArmy

#define CWeaponHammerless C_WeaponHammerless

#define CWeaponMauser C_WeaponMauser
#endif

class CWeaponRevolver : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponRevolver, CBaseHL2MPCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponRevolver();
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponRevolver, DT_WeaponRevolver )

BEGIN_NETWORK_TABLE( CWeaponRevolver, DT_WeaponRevolver )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponRevolver )
END_PREDICTION_DATA()

class CWeaponWalker : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponWalker, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponPeacemaker : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponPeacemaker, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponDeringer : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponDeringer, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponVolcanic : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponVolcanic, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponColtNavy : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponColtNavy, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponSchofield : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponSchofield, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponRemingtonArmy : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponRemingtonArmy, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponHammerless : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponHammerless, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponMauser : public CWeaponRevolver
{
public:
	DECLARE_CLASS( CWeaponMauser, CWeaponRevolver );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

CREATE_SIMPLE_WEAPON_TABLE( WeaponWalker, weapon_walker );
CREATE_SIMPLE_WEAPON_TABLE( WeaponPeacemaker, weapon_peacemaker );

CREATE_SIMPLE_WEAPON_TABLE( WeaponDeringer, weapon_deringer );
CREATE_SIMPLE_WEAPON_TABLE( WeaponVolcanic, weapon_volcanic );

CREATE_SIMPLE_WEAPON_TABLE( WeaponColtNavy, weapon_coltnavy );
CREATE_SIMPLE_WEAPON_TABLE( WeaponSchofield, weapon_schofield );

CREATE_SIMPLE_WEAPON_TABLE( WeaponRemingtonArmy, weapon_remington_army );

CREATE_SIMPLE_WEAPON_TABLE( WeaponHammerless, weapon_hammerless );

CREATE_SIMPLE_WEAPON_TABLE( WeaponMauser, weapon_mauser );

acttable_t	CWeaponRevolver::m_acttable[] = 
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

IMPLEMENT_ACTTABLE( CWeaponRevolver );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CWeaponRevolver::CWeaponRevolver( void )
{
}