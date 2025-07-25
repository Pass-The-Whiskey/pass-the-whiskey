#include "cbase.h"
#include "npcevent.h"
#include "weapon_hl2mpbasebasebludgeon.h"
#include "IEffects.h"
#include "debugoverlay_shared.h"

#ifdef CLIENT_DLL
#define CWeaponFists C_WeaponFists
#endif

class CWeaponFists : public CBaseHL2MPBludgeonWeapon
{
public:
	DECLARE_CLASS( CWeaponFists, CBaseHL2MPBludgeonWeapon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();

	CWeaponFists();

	float		GetRange( void );
	float		GetFireRate( void );

	void		AddViewKick( void );
	float		GetDamageForActivity( Activity hitActivity );
};