#include "cbase.h"
#include "hl2mp_player.h"

//-----------------------------------------------------------------------------
// Crossbow Bolt
//-----------------------------------------------------------------------------
class CFOFProjectile : public CBaseCombatCharacter
{
	DECLARE_CLASS( CFOFProjectile, CBaseCombatCharacter );
	DECLARE_DATADESC();

public:
	CFOFProjectile() { };
	~CFOFProjectile();

	Class_T Classify( void ) { return CLASS_NONE; }

public:
	virtual void Spawn( void );
	void ProjectileTouch( CBaseEntity *pOther );
	bool CreateVPhysics( void );
	unsigned int PhysicsSolidMaskForEntity() const;
	static CFOFProjectile	*Create( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon, const char *pszClassname, const Vector& vecOrigin, const QAngle& angAngles, int iDamage );
	static void				GetProjectileFireSetup( CBasePlayer *pOwner, CBaseCombatWeapon *pWeapon, Vector vecOffset, Vector* vecSrc, QAngle* angForward );

protected:
	int		m_iDamage;
};