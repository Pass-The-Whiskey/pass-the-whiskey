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
#define CFOFWeaponShotgun C_FOFWeaponShotgun

#define CFOFWeaponMaresLeg C_FOFWeaponMaresLeg

#define CFOFWeaponPumpShotgun C_FOFWeaponPumpShotgun
#define CFOFWeaponSawedOffShotgun C_FOFWeaponSawedOffShotgun

#define CFOFWeaponCoachgun C_FOFWeaponCoachgun
#endif

class CFOFWeaponShotgun : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CFOFWeaponShotgun, CBaseHL2MPCombatWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CFOFWeaponShotgun();
};

IMPLEMENT_NETWORKCLASS_ALIASED( FOFWeaponShotgun, DT_FOFWeaponShotgun )

BEGIN_NETWORK_TABLE( CFOFWeaponShotgun, DT_FOFWeaponShotgun )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CFOFWeaponShotgun )
END_PREDICTION_DATA()

class CFOFWeaponMaresLeg : public CFOFWeaponShotgun
{
public:
	DECLARE_CLASS( CFOFWeaponMaresLeg, CFOFWeaponShotgun );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CFOFWeaponSawedOffShotgun : public CFOFWeaponShotgun
{
public:
	DECLARE_CLASS( CFOFWeaponSawedOffShotgun, CFOFWeaponShotgun );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CFOFWeaponPumpShotgun : public CFOFWeaponShotgun
{
public:
	DECLARE_CLASS( CFOFWeaponPumpShotgun, CFOFWeaponShotgun );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CFOFWeaponCoachgun : public CFOFWeaponShotgun
{
public:
	DECLARE_CLASS( CFOFWeaponCoachgun, CFOFWeaponShotgun );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

CREATE_SIMPLE_WEAPON_TABLE( FOFWeaponMaresLeg, weapon_maresleg );
CREATE_SIMPLE_WEAPON_TABLE( FOFWeaponPumpShotgun, weapon_pump_shotgun );
CREATE_SIMPLE_WEAPON_TABLE( FOFWeaponSawedOffShotgun, weapon_sawedoff_shotgun );
CREATE_SIMPLE_WEAPON_TABLE( FOFWeaponCoachgun, weapon_coachgun );

acttable_t	CFOFWeaponShotgun::m_acttable[] =
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

IMPLEMENT_ACTTABLE( CFOFWeaponShotgun );

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CFOFWeaponShotgun::CFOFWeaponShotgun( void )
{
}