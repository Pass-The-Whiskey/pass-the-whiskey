#include "cbase.h"
#include "fof/fof_weapon_melee.h"
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

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponMelee, DT_WeaponMelee )

BEGIN_NETWORK_TABLE( CWeaponMelee, DT_WeaponMelee )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponMelee )
END_PREDICTION_DATA()

#ifdef GAME_DLL
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
//CMeleeBolt *CWeaponMelee::CreateProjectile( CBaseEntity *pPlayer, const Vector &vecOrigin, const QAngle &vecAngles )
//{
//	CMeleeBolt *pMelee = static_cast<CMeleeBolt*>( CMeleeBolt::BoltCreate( vecOrigin, vecAngles, 0, ToHL2MPPlayer(pPlayer) ) );
//
//	Vector vecSrc;
//	QAngle angForward;
//	Vector vecOffset( -25.f, 6.f, 0.0f );
//	if ( pPlayer->GetFlags() & FL_DUCKING )
//	{
//		vecOffset.z = 8.0f;
//	}
//
//	pMelee->GetProjectileFireSetup( ToHL2MPPlayer(pPlayer), vecOffset, &vecSrc, &angForward );
//
//	trace_t trace;	
//	Vector vecEye = pPlayer->EyePosition();
//	CTraceFilterSimple traceFilter( pPlayer, COLLISION_GROUP_NONE );
//	UTIL_TraceLine( vecEye, vecSrc, MASK_SOLID_BRUSHONLY, &traceFilter, &trace );
//
//	Vector vecForward, vecRight, vecUp;
//	AngleVectors( vecAngles, &vecForward, &vecRight, &vecUp );
//
//	pMelee->Spawn();
//	pMelee->SetAbsOrigin( vecSrc );
//
//	Vector vecVelocity = vecForward * 850.f;
//	pMelee->SetAbsVelocity( vecVelocity );
//
//	// Setup the initial angles.
//	QAngle angles;
//	VectorAngles( vecVelocity, angles );
//	pMelee->SetAbsAngles( angles );
//
//	return pMelee;
//}
#endif // GAME_DLL

CWeaponMelee::CWeaponMelee()
{
	m_flThrowTime = -1.0;
}

void CWeaponMelee::SecondaryAttack()
{
	SendWeaponAnim( ACT_VM_SECONDARYATTACK );

	m_flThrowTime = m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + GetWpnData().m_flTimeMissFireDelay;
}

void CWeaponMelee::Throw()
{
#ifdef GAME_DLL
	CHL2MP_Player *pPlayer = ToHL2MPPlayer( GetPlayerOwner() );

	if (!pPlayer)
		return;

	Vector vecAiming	= pPlayer->EyeDirection3D();
	Vector vecSrc		= pPlayer->Weapon_ShootPosition();

	QAngle angAiming;
	VectorAngles( vecAiming, angAiming );

	//CreateProjectile( pPlayer, vecSrc, angAiming );
#endif
}

void CWeaponMelee::ItemPostFrame()
{
	BaseClass::ItemPostFrame();

	CheckThrow();
}

void CWeaponMelee::CheckThrow()
{
	if ( m_flThrowTime > 0.0 && gpGlobals->curtime >= m_flThrowTime )
	{
#ifdef GAME_DLL
		Throw();
#endif // GAME_DLL

		SendWeaponAnim( ACT_VM_DRAW );
		m_flThrowTime = -1.0;
	}
}

CREATE_SIMPLE_WEAPON_TABLE( WeaponAxe, weapon_axe );
CREATE_SIMPLE_WEAPON_TABLE( WeaponKnife, weapon_knife );
CREATE_SIMPLE_WEAPON_TABLE( WeaponMachete, weapon_machete );