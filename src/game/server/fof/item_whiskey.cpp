#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "items.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CItemWhiskey : public CItem
{
public:
	DECLARE_CLASS( CItemWhiskey, CItem );
	DECLARE_DATADESC();

	void Spawn( void )
	{ 
		Precache( );
		SetModel( "models/items_fof/whiskey_world.mdl" );
		BaseClass::Spawn( );

		m_takedamage = DAMAGE_NO;

		SetMoveType( MOVETYPE_NONE );
		AddEffects( EF_ITEM_BLINK );
	}
	void Precache( void )
	{
		PrecacheModel ( "models/items_fof/whiskey_world.mdl" );
	}
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
	{
		CBasePlayer *pPlayer = ToBasePlayer( pActivator );

		if ( IsEffectActive( EF_NODRAW ) )
			return;

		if ( pPlayer && ( pPlayer->TakeHealth( m_nHealth, DMG_GENERIC ) > 0 ) )
		{
			m_OnPlayerTouch.FireOutput( pActivator, this );

			SetTouch( NULL );
			SetThink( NULL );

			// player grabbed the item. 
			g_pGameRules->PlayerGotItem( pPlayer, this );
			if ( g_pGameRules->ItemShouldRespawn( this ) == GR_ITEM_RESPAWN_YES )
			{
				Respawn(); 
				AddSolidFlags( FSOLID_NOT_SOLID );
			}
			else
			{
				UTIL_Remove( this );
				HL2MPRules()->RemoveLevelDesignerPlacedObject( this );
			}
		}
	}
	void Materialize( void )
	{
		CreateItemVPhysicsObject();

		if ( IsEffectActive( EF_NODRAW ) )
		{
			SetMoveType( MOVETYPE_NONE );
			RemoveEffects( EF_NODRAW );
			RemoveSolidFlags( FSOLID_NOT_SOLID );
		}
	}
private:
	int m_nHealth;
};

BEGIN_DATADESC( CItemWhiskey )
	DEFINE_KEYFIELD( m_nHealth, FIELD_INTEGER, "Health"),
END_DATADESC()

LINK_ENTITY_TO_CLASS( item_whiskey, CItemWhiskey );
