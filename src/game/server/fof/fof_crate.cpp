#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "items.h"
#include "hl2mp_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar fof_sv_crate_opentime( "fof_sv_crate_opentime", "2.0", FCVAR_REPLICATED );

class CFOFCrate : public CBaseAnimating
{
public:
	DECLARE_CLASS( CFOFCrate, CBaseAnimating );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	void Spawn( void )
	{ 
		Precache( );
		SetModel( "models/items_fof/safe_crate.mdl" );
		BaseClass::Spawn( );

		m_takedamage = DAMAGE_NO;

		SetMoveType( MOVETYPE_NONE );
		AddEffects( EF_ITEM_BLINK );

		SetCollisionGroup( COLLISION_GROUP_INTERACTIVE );
		AddSolidFlags( FSOLID_NOT_STANDABLE );
		SetSolid( SOLID_BBOX );

		SetBlocksLOS( false );
		AddEFlags( EFL_NO_ROTORWASH_PUSH );

		//AddGlowEffect();
		ToggleGlow( true );
		SetGlowMode( 2 );
		SetGlowColor( GetGlowColor().r(), GetGlowColor().g(), GetGlowColor().b(), GetGlowColor().a() );

		m_flOpenTime = 5;
		m_flNextOpenTime = -1;
		m_flOpenStartTime = -1;
	}
	void Precache( void )
	{
		PrecacheModel( "models/items_fof/safe_crate.mdl" );
		PrecacheModel( "models/items_fof/safe_crate_small.mdl" );

		PrecacheScriptSound( "MoneyCrate.Open" );
		PrecacheScriptSound( "MoneyCrate.Close" );
	}
	bool IsOpen( void )
	{
		return m_flNextOpenTime != -1;
	}
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
	{
		if ( !IsOpen() )
		{
			SetNextThink( gpGlobals->curtime ); // this is surely gross ? 

			if ( value >= fof_sv_crate_opentime.GetFloat() )
			{
				Open( (CBasePlayer*)pActivator );
				return;
			}

			if ( m_flOpenStartTime == -1 )
			{
				m_flOpenStartTime = gpGlobals->curtime;
			}
		}
	}
	void CancelUse( void )
	{
		m_flOpenStartTime = -1;
	}
	void Open( CBasePlayer* pPlayer )
	{
		CancelUse();

		ResetSequence( LookupSequence( "Close" ) ); // i really like that this is the correct animation name
		EmitSound( "MoneyCrate.Open" );

		m_OnOpen.FireOutput( pPlayer, this );

		m_flNextOpenTime = gpGlobals->curtime + m_flOpenTime;
		SetNextThink( m_flNextOpenTime );
	}
	void Close( void )
	{
		ResetSequence( LookupSequence( "Open" ) );
		EmitSound( "MoneyCrate.Close" );

		m_flNextOpenTime = -1;
	}
	void Think() {
		if ( IsOpen() )
		{
			Close();
		}
		else
		{
			CancelUse();
		}
		
	}
	virtual Color GetGlowColor( void )
	{
		return Color( 255, 216, 0, 255 );
	}
	virtual int	ObjectCaps() { return BaseClass::ObjectCaps() | (IsOpen() ? (FCAP_WCEDIT_POSITION) : FCAP_CONTINUOUS_USE | FCAP_USE_IN_RADIUS | FCAP_WCEDIT_POSITION); };
protected:
	CNetworkVar( float, m_flOpenTime );
	CNetworkVar( float, m_flNextOpenTime );
	CNetworkVar( float, m_flOpenStartTime );

	COutputEvent	m_OnOpen;
	COutputEvent	m_OnClosed;
};

LINK_ENTITY_TO_CLASS( fof_crate, CFOFCrate );

BEGIN_DATADESC( CFOFCrate )
	DEFINE_OUTPUT( m_OnOpen, "OnOpen" ),
	DEFINE_OUTPUT( m_OnClosed, "OnClose" ),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CFOFCrate, DT_FOFCrate )
	SendPropFloat( SENDINFO( m_flOpenTime ), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO( m_flNextOpenTime ), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO( m_flOpenStartTime ), 0, SPROP_NOSCALE ),
END_SEND_TABLE()

class CFOFCrateMed : public CFOFCrate // red
{
public:
	DECLARE_CLASS( CFOFCrateMed, CFOFCrate );

	void Spawn( void )
	{ 
		BaseClass::Spawn( );
		SetModel( "models/items_fof/safe_crate_small.mdl" );

		m_nSkin = 1;
	}
	Color GetGlowColor( void )
	{
		return Color( 255, 63, 63, 255 );
	}
};

class CFOFCrateLow : public CFOFCrate // blue
{
public:
	DECLARE_CLASS( CFOFCrateLow, CFOFCrate );

	void Spawn( void )
	{ 
		BaseClass::Spawn( );
		SetModel( "models/items_fof/safe_crate_small.mdl" );

		m_nSkin = 2;
	}
	Color GetGlowColor( void )
	{
		return Color( 63, 63, 255, 255 );
	}
};

LINK_ENTITY_TO_CLASS( fof_crate_med, CFOFCrateMed );
LINK_ENTITY_TO_CLASS( fof_crate_low, CFOFCrateLow );
