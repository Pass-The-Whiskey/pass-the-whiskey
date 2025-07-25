#include "cbase.h"
#include "fof_projectile.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

BEGIN_DATADESC( CFOFProjectile )
DEFINE_FUNCTION( ProjectileTouch ),
END_DATADESC()

CFOFProjectile* CFOFProjectile::Create( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon, const char *pszClassname, const Vector& vecOrigin, const QAngle& angAngles, int iDamage )
{
	CFOFProjectile* pProjectile = (CFOFProjectile*)CreateEntityByName( pszClassname );
	UTIL_SetOrigin( pProjectile, vecOrigin );

	pProjectile->SetAbsAngles( angAngles );
	pProjectile->Spawn();
	pProjectile->SetOwnerEntity( pPlayer );
	pProjectile->SetGravity( pWeapon->GetWpnData().m_flProjectileGravity );

	pProjectile->m_iDamage = iDamage;

	return pProjectile;
}

//-----------------------------------------------------------------------------
// Purpose: Return the origin & angles for a projectile fired from the player's gun
//-----------------------------------------------------------------------------
void CFOFProjectile::GetProjectileFireSetup( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon, Vector vecOffset, Vector* vecSrc, QAngle* angForward )
{
	// Flip the firing offset if our view model is flipped.
	if ( pWeapon->IsSecondary() )
	{
		vecOffset.y *= -1;
	}

	QAngle angSpread = pPlayer->EyeAngles();
	Vector vecForward, vecRight, vecUp;
	AngleVectors(angSpread, &vecForward, &vecRight, &vecUp);

	Vector vecShootPos = pPlayer->Weapon_ShootPosition();

	// Estimate end point
	Vector endPos = vecShootPos + vecForward * pWeapon->GetWpnData().m_flProjectileSpeed;

	// Trace forward and find what's in front of us, and aim at that
	trace_t tr;

	CTraceFilterNoNPCsOrPlayer filter( pPlayer, COLLISION_GROUP_NONE );
	UTIL_TraceLine( vecShootPos, endPos, MASK_SOLID, &filter, &tr );

	// Offset actual start point
	*vecSrc = vecShootPos + (vecForward * vecOffset.x) + (vecRight * vecOffset.y) + (vecUp * vecOffset.z);

	// Find angles that will get us to our desired end point
	// Only use the trace end if it wasn't too close, which results
	// in visually bizarre forward angles
	if (tr.fraction > 0.1)
	{
		VectorAngles( tr.endpos - *vecSrc, *angForward );
	}
	else
	{
		VectorAngles( endPos - *vecSrc, *angForward );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFOFProjectile::~CFOFProjectile( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CFOFProjectile::CreateVPhysics( void )
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_VPHYSICS, FSOLID_NOT_STANDABLE, false );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
unsigned int CFOFProjectile::PhysicsSolidMaskForEntity() const
{
	return ( BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX ) & ~CONTENTS_GRATE;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFProjectile::Spawn( void )
{
	Precache();

	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
	SetSolid( SOLID_VPHYSICS );

	// Make sure we're updated if we're underwater
	UpdateWaterState();

	SetTouch( &CFOFProjectile::ProjectileTouch );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CFOFProjectile::ProjectileTouch(CBaseEntity* pOther)
{
	if ( !pOther->IsSolid() || pOther->IsSolidFlagSet( FSOLID_VOLUME_CONTENTS ) )
		return;

	if ( pOther->m_takedamage != DAMAGE_NO )
	{
		trace_t	tr, tr2;
		tr = BaseClass::GetTouchTrace();
		Vector	vecNormalizedVel = GetAbsVelocity();

		ClearMultiDamage();
		VectorNormalize( vecNormalizedVel );

		if (GetOwnerEntity() && GetOwnerEntity()->IsPlayer() && pOther->IsNPC())
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), m_iDamage, DMG_NEVERGIB);
			dmgInfo.AdjustPlayerDamageInflictedForSkillLevel();
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);
		}
		else
		{
			CTakeDamageInfo	dmgInfo(this, GetOwnerEntity(), m_iDamage, DMG_BULLET | DMG_NEVERGIB);
			CalculateMeleeDamageForce(&dmgInfo, vecNormalizedVel, tr.endpos, 0.7f);
			dmgInfo.SetDamagePosition(tr.endpos);
			pOther->DispatchTraceAttack(dmgInfo, vecNormalizedVel, &tr);
		}

		ApplyMultiDamage();

		// Adrian: keep going through the glass.
		if ( pOther->GetCollisionGroup() == COLLISION_GROUP_BREAKABLE_GLASS )
			return;

		// play body "thwack" sound
		EmitSound( "Weapon_Crossbow.BoltHitBody" );

		SetTouch( NULL );
		SetThink( NULL );
	}
	else
	{
		trace_t	tr;
		tr = BaseClass::GetTouchTrace();

		// See if we struck the world
		if (pOther->GetMoveType() == MOVETYPE_NONE && !(tr.surface.flags & SURF_SKY))
		{
			EmitSound( "Weapon_Crossbow.BoltHitWorld" );
			UTIL_ImpactTrace( &tr, DMG_BULLET );

			SetTouch( NULL );
			SetThink( NULL );
		}
		else
		{
			// Put a mark unless we've hit the sky
			if ( ( tr.surface.flags & SURF_SKY ) == false )
			{
				UTIL_ImpactTrace( &tr, DMG_BULLET );
			}

			// UTIL_Remove( this );
		}
	}
}