#include "cbase.h"
#include "fof_projectile_arrow.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_DATADESC( CFOFProjectileArrow )
END_DATADESC()

LINK_ENTITY_TO_CLASS( projectile_arrow, CFOFProjectileArrow ); 

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CFOFProjectileArrow *CFOFProjectileArrow::CreateArrow( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon, const Vector& vecOrigin, const QAngle& angAngles )
{
	CFOFProjectileArrow *pProjectile = static_cast<CFOFProjectileArrow*>( CFOFProjectile::Create( pPlayer, pWeapon, "projectile_arrow", vecOrigin, angAngles, 0 ) );

	Vector vecSrc;
	QAngle angForward;
	Vector vecOffset( -7, 3.7, 5 );
	//if ( pPlayer->GetFlags() & FL_DUCKING )
	//{
	//	vecOffset.z = 8.0f;
	//}

	pProjectile->GetProjectileFireSetup( ToHL2MPPlayer(pPlayer), pWeapon, vecOffset, &vecSrc, &angForward );

	trace_t trace;	
	Vector vecEye = pPlayer->EyePosition();
	CTraceFilterSimple traceFilter( pPlayer, COLLISION_GROUP_NONE );
	UTIL_TraceLine( vecEye, vecSrc, MASK_SOLID_BRUSHONLY, &traceFilter, &trace );

	Vector vecForward, vecRight, vecUp;
	AngleVectors( angAngles, &vecForward, &vecRight, &vecUp );

	pProjectile->Spawn();
	pProjectile->SetAbsOrigin( vecSrc );

	Vector vecVelocity = vecForward * 850.f;
	pProjectile->SetAbsVelocity( vecVelocity );

	// Setup the initial angles.
	QAngle angles;
	VectorAngles( vecVelocity, angles );
	pProjectile->SetAbsAngles( angles );

	return pProjectile;
}

void CFOFProjectileArrow::Precache( void ) 
{
	PrecacheModel( "models/weapons/bowarrow_bolt.mdl" );
}

void CFOFProjectileArrow::Spawn( void ) 
{
	BaseClass::Spawn();

	SetModel( "models/weapons/bowarrow_bolt.mdl" );
}