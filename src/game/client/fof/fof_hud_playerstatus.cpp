#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ScalableImagePanel.h>
#include <hl2mp_gamerules.h>
#include <fmtstr.h>

#include <vgui/ILocalize.h>

#include "fof/vgui/fof_imagepanel.h"
#include "hudelement.h"

#include "convar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose:  Clips the health image to the appropriate percentage
//-----------------------------------------------------------------------------
class CFOFHealthMeter : public CFOFImagePanel
{
public:
	DECLARE_CLASS_SIMPLE( CFOFHealthMeter, CFOFImagePanel );

	CFOFHealthMeter( vgui::Panel *parent, const char *name );
	virtual void PaintBackground() OVERRIDE;
	virtual void SetHealth(float flHealth);
	virtual void Reset();

private:
	float	m_flSpeed;
	float	m_flLastHealth;
	float	m_flHealth;

	float	m_flDrawHealth;
	float	m_flStartTime;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFOFHealthMeter::CFOFHealthMeter( Panel *parent, const char *name ) : CFOFImagePanel( parent, name )
{
	Reset();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFHealthMeter::PaintBackground()
{
	int iTextureID = m_iTextureID;

	float pos = clamp( ( ( ( gpGlobals->curtime - m_flStartTime ) * m_flSpeed ) ), 0.0, 1.0 ) - 1;
	m_flDrawHealth = m_flLastHealth + ((m_flHealth - m_flLastHealth) * (sqrtf(1 - pos * pos)));

	int x, y, w, h;
	GetBounds( x, y, w, h );

	Vertex_t vert[4];	
	float uv1 = 0.0f;
	float uv2 = 1.0f;
	int xpos = 0, ypos = 0;

	float flDamageY = h * ( 1.0f - m_flDrawHealth );

	if ( m_pBGImage[m_iTeam] && m_pBGImage[m_iTeam]->GetID() != -1 )
	{
		m_iTextureID = m_pBGImage[m_iTeam]->GetID();
	}
	else
	{
		m_iTextureID = iTextureID;
	}

	// blend in the red "damage" part
	surface()->DrawSetColor( m_DrawColor.r(), m_DrawColor.g(), m_DrawColor.b(), GetAlpha() );
	surface()->DrawSetTexture( m_iTextureID );

	Vector2D uv11( uv1, uv2 - m_flDrawHealth );
	Vector2D uv21( uv2, uv2 - m_flDrawHealth );
	Vector2D uv22( uv2, uv2 );
	Vector2D uv12( uv1, uv2 );

	vert[0].Init( Vector2D( xpos, flDamageY ), uv11 );
	vert[1].Init( Vector2D( xpos + w, flDamageY ), uv21 );
	vert[2].Init( Vector2D( xpos + w, ypos + h ), uv22 );				
	vert[3].Init( Vector2D( xpos, ypos + h ), uv12 );

	surface()->DrawTexturedPolygon( 4, vert );
	surface()->DrawSetTexture( 0 );

	m_iTextureID = iTextureID;
}

void CFOFHealthMeter::SetHealth( float flHealth )
{ 
	m_flHealth = ( flHealth < 1.0 ) ? flHealth : 1.0f; 
	m_flStartTime = gpGlobals->curtime;
	m_flLastHealth = m_flDrawHealth;

	m_flSpeed = 1.f / ( max(m_flHealth, m_flLastHealth) - min(m_flHealth, m_flLastHealth) );
}

void CFOFHealthMeter::Reset()
{
	m_flLastHealth = 1.0;
	m_flHealth = 1.0;

	m_flDrawHealth = 1.0;

	m_flStartTime = gpGlobals->curtime;
}

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CHudPlayerStatus : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudPlayerStatus, EditablePanel );

public:
	CHudPlayerStatus( const char *pElementName );
	virtual void ApplySchemeSettings( IScheme* pScheme );
	virtual void OnThink();

	virtual const char *GetResFilename( void ) { return "resource/UI/HudPlayerStatus.res"; }
private:
	bool	m_bAccountVisible;
	
	int		m_iHealth;
	int		m_iAccount;
protected:

	CFOFHealthMeter* m_pHealthMeter;
};

DECLARE_HUDELEMENT( CHudPlayerStatus );

CHudPlayerStatus::CHudPlayerStatus( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, pElementName )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );

	m_pHealthMeter = new CFOFHealthMeter( this, "HealthMeter" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudPlayerStatus::ApplySchemeSettings( IScheme* pScheme )
{
	KeyValues *pConditions = new KeyValues( "conditions" );

	if ( m_bAccountVisible )
	{
		pConditions->AddSubKey( new KeyValues( "if_account" ) );
	}

	LoadControlSettings( GetResFilename(), NULL, NULL, pConditions );

	BaseClass::ApplySchemeSettings( pScheme );
}

extern ConVar fof_sv_enablecurrency;

void CHudPlayerStatus::OnThink()
{
	if ( m_bAccountVisible != fof_sv_enablecurrency.GetBool() )
	{
		m_bAccountVisible = fof_sv_enablecurrency.GetBool();
		InvalidateLayout( false, true );
	}

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( pPlayer )
	{
		int iHealth = MAX( pPlayer->GetHealth(), 0 );
		if (iHealth == m_iHealth)
			return;

		m_iHealth = iHealth;

		m_pHealthMeter->SetHealth( (float)( m_iHealth ) / (float)( pPlayer->GetMaxHealth() ) );
		SetDialogVariable( "health", CFmtStr("@%d", m_iHealth ) );
		SetDialogVariable( "account", "$100" );
	}
}