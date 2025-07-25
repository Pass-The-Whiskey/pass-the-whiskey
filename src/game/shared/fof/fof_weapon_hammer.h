#include "cbase.h"
#include "npcevent.h"
#include "fof_weapon_melee.h"
#include "IEffects.h"
#include "debugoverlay_shared.h"

#ifdef CLIENT_DLL
#define CWeaponHammer C_WeaponHammer
#endif

class CWeaponHammer : public CWeaponMelee
{
public:
	DECLARE_CLASS( CWeaponHammer, CWeaponMelee );

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CWeaponHammer();

	virtual	void	StartPrimaryAttack( void );
	virtual	void	PrimaryAttack( void );

	virtual void	ItemPostFrame( void );
private:

	bool			m_bStartedPrimaryAttack;
};