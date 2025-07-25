#include "cbase.h"
#include "hl2mp_player.h"
#include "fof_projectile.h"

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CFOFProjectileArrow : public CFOFProjectile
{
	DECLARE_CLASS( CFOFProjectileArrow, CFOFProjectile );
	DECLARE_DATADESC();

public:
	CFOFProjectileArrow() {};

	virtual void Precache() OVERRIDE;
	virtual void Spawn() OVERRIDE;

	static CFOFProjectileArrow	*CreateArrow( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon, const Vector& vecOrigin, const QAngle& angAngles );
};