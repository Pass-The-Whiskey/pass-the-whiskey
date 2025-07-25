#include "cbase.h"
#include "npcevent.h"
#include "weapon_hl2mpbasebasebludgeon.h"
#include "IEffects.h"
#include "debugoverlay_shared.h"

#ifdef CLIENT_DLL
#define CWeaponMelee C_WeaponMelee

#define CWeaponAxe C_WeaponAxe
#define CWeaponKnife C_WeaponKnife
#define CWeaponMachete C_WeaponMachete
#endif

class CWeaponMelee : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponMelee, CBaseHL2MPBludgeonWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

#ifdef GAME_DLL
	//static CMeleeBolt *CreateProjectile( CBaseEntity *pThrower, const Vector &vecOrigin, const QAngle &vecAngles );
#endif // GAME_DLL

	CWeaponMelee();

	virtual void SecondaryAttack( void );
	virtual void Throw();

	virtual void ItemPostFrame();

	virtual	bool CanDualWield( void ) { return false; };

	void CheckThrow();
private:
	float		m_flThrowTime;
};

class CWeaponAxe : public CWeaponMelee
{
public:
	DECLARE_CLASS( CWeaponAxe, CWeaponMelee );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponKnife : public CWeaponMelee
{
public:
	DECLARE_CLASS( CWeaponKnife, CWeaponMelee );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};

class CWeaponMachete : public CWeaponMelee
{
public:
	DECLARE_CLASS( CWeaponMachete, CWeaponMelee );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
};