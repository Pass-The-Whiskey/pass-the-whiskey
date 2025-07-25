//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hud_crosshair.h"
#include "iclientmode.h"
#include "view.h"
#include "vgui_controls/Controls.h"
#include "vgui/ISurface.h"
#include "ivrenderview.h"
#include "materialsystem/imaterialsystem.h"
#include "VGuiMatSurface/IMatSystemSurface.h"
#include "client_virtualreality.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar crosshair( "crosshair", "1", FCVAR_ARCHIVE );
ConVar cl_observercrosshair( "cl_observercrosshair", "1", FCVAR_ARCHIVE );

using namespace vgui;

int ScreenTransform( const Vector& point, Vector& screen );

#ifndef TF_CLIENT_DLL
DECLARE_HUDELEMENT( CHudCrosshair );
#endif

CHudCrosshair::CHudCrosshair( const char *pElementName ) :
		CHudElement( pElementName ), BaseClass( NULL, "HudCrosshair" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_clrCrosshair = Color( 0, 0, 0, 0 );
	m_vecCrossHairOffsetAngle.Init();

	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_CROSSHAIR );
}

CHudCrosshair::~CHudCrosshair()
{
}

void CHudCrosshair::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
	SetPaintBackgroundEnabled( false );

    SetSize( ScreenWidth(), ScreenHeight() );

	SetForceStereoRenderToFrameBuffer( true );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudCrosshair::ShouldDraw( void )
{
	bool bNeedsDraw;

	if ( m_bHideCrosshair )
		return false;

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return false;

	C_BaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
	if ( pWeapon && !pWeapon->ShouldDrawCrosshair() )
		return false;

	// draw a crosshair only if alive or spectating in eye
	bNeedsDraw = crosshair.GetInt() &&
		!engine->IsDrawingLoadingImage() &&
		!engine->IsPaused() && 
		g_pClientMode->ShouldDrawCrosshair() &&
		!( pPlayer->GetFlags() & FL_FROZEN ) &&
		( pPlayer->entindex() == render->GetViewEntity() ) &&
		!pPlayer->IsInVGuiInputMode() &&
		( pPlayer->IsAlive() ||	( pPlayer->GetObserverMode() == OBS_MODE_IN_EYE ) || ( cl_observercrosshair.GetBool() && pPlayer->GetObserverMode() == OBS_MODE_ROAMING ) );

	return ( bNeedsDraw && CHudElement::ShouldDraw() );
}

ConVar cl_crosshair_red( "cl_crosshair_red", "255", FCVAR_ARCHIVE, "", true, 0, true, 255 );
ConVar cl_crosshair_green( "cl_crosshair_green", "255", FCVAR_ARCHIVE, "", true, 0, true, 255 );
ConVar cl_crosshair_blue( "cl_crosshair_blue", "255", FCVAR_ARCHIVE, "", true, 0, true, 255 );
ConVar cl_crosshair_scale( "cl_crosshair_scale", "1", FCVAR_ARCHIVE, "", true, 0, true, 255 );

ConVar cl_crosshair_dot( "cl_crosshair_dot", "1", FCVAR_ARCHIVE );
ConVar cl_crosshair_thickness( "cl_crosshair_thickness", "10", FCVAR_ARCHIVE, "", true, 1, true, 10 );
ConVar cl_crosshair_distance( "cl_crosshair_distance", "100", FCVAR_ARCHIVE );

void CHudCrosshair::GetDrawPosition ( float *pX, float *pY, bool *pbBehindCamera, QAngle angleCrosshairOffset )
{
	QAngle curViewAngles = CurrentViewAngles();
	Vector curViewOrigin = CurrentViewOrigin();

	int vx, vy, vw, vh;
	vgui::surface()->GetFullscreenViewport( vx, vy, vw, vh );

	float screenWidth = vw;
	float screenHeight = vh;

	float x = screenWidth / 2;
	float y = screenHeight / 2;

	bool bBehindCamera = false;

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( ( pPlayer != NULL ) && ( pPlayer->GetObserverMode()==OBS_MODE_NONE ) )
	{
		bool bUseOffset = false;
		
		Vector vecStart;
		Vector vecEnd;

		if ( bUseOffset )
		{
			trace_t tr;
			UTIL_TraceLine( vecStart, vecEnd, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr );

			Vector screen;
			screen.Init();
			bBehindCamera = ScreenTransform(tr.endpos, screen) != 0;

			x = 0.5f * ( 1.0f + screen[0] ) * screenWidth + 0.5f;
			y = 0.5f * ( 1.0f - screen[1] ) * screenHeight + 0.5f;
		}
	}

	// MattB - angleCrosshairOffset is the autoaim angle.
	// if we're not using autoaim, just draw in the middle of the 
	// screen
	if ( angleCrosshairOffset != vec3_angle )
	{
		QAngle angles;
		Vector forward;
		Vector point, screen;

		// this code is wrong
		angles = curViewAngles + angleCrosshairOffset;
		AngleVectors( angles, &forward );
		VectorAdd( curViewOrigin, forward, point );
		ScreenTransform( point, screen );

		x += 0.5f * screen[0] * screenWidth + 0.5f;
		y += 0.5f * screen[1] * screenHeight + 0.5f;
	}

	*pX = x;
	*pY = y;
	*pbBehindCamera = bBehindCamera;
}


void CHudCrosshair::Paint( void )
{
	if ( !IsCurrentViewAccessAllowed() )
		return;

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	float flX, flY;
	bool bBehindCamera;
	GetDrawPosition ( &flX, &flY, &bBehindCamera, m_vecCrossHairOffsetAngle );

	int x = (int)(flX);
	int y = (int)(flY);

	if( bBehindCamera )
		return;

	int iTextureW = cl_crosshair_thickness.GetInt() * 2;
	int iTextureH = cl_crosshair_thickness.GetInt() * 2;

	float flPlayerScale = 1.0f;
	Color clr( cl_crosshair_red.GetInt(), cl_crosshair_green.GetInt(), cl_crosshair_blue.GetInt(), 255 );
	flPlayerScale = cl_crosshair_scale.GetFloat();  // the player can change the scale in the options/multiplayer tab

	float flWidth = flPlayerScale * iTextureW;
	float flHeight = flPlayerScale * iTextureH;

	int iDistance = cl_crosshair_distance.GetInt();

	surface()->DrawSetColor(clr);

	if ( cl_crosshair_dot.GetBool() )
		surface()->DrawFilledRect(x - (flWidth / 2), y - (flHeight / 2), x + (flWidth / 2), y + (flHeight / 2));

	clr[3] = (100 - iDistance);
	surface()->DrawSetColor(clr);

	surface()->DrawFilledRect(x - iDistance, y - (flHeight / 2), x - iDistance + 12, y + (flHeight / 2)); // left stem
	surface()->DrawFilledRect(x + iDistance - 12, y - (flHeight / 2), x + iDistance, y + (flHeight / 2)); // right stem

	surface()->DrawFilledRect(x - (flWidth / 2), y - iDistance, x + (flWidth / 2), y - iDistance + 12); // top stem
	surface()->DrawFilledRect(x - (flWidth / 2), y + iDistance - 12, x + (flWidth / 2), y + iDistance); // bottom stem

	surface()->DrawLine(x - iDistance, y - iDistance, x - iDistance + 12, y - iDistance + 12); // top left stem
	surface()->DrawLine(x + iDistance - 12, y - iDistance + 12 - 1, x + iDistance, y - iDistance - 1); // top right stem

	surface()->DrawLine(x - iDistance, y + iDistance - 1, x - iDistance + 12, y + iDistance - 12 - 1); // bottom left stem
	surface()->DrawLine(x + iDistance - 12, y + iDistance - 12, x + iDistance, y + iDistance); // bottom right stem

	for (int i = 1; i <= cl_crosshair_thickness.GetInt(); i++)
	{
		// top left stem
		surface()->DrawLine(x - iDistance + i, y - iDistance, x - iDistance + 12, y - iDistance + 12 - i);
		surface()->DrawLine(x - iDistance, y - iDistance + i, x - iDistance + 12 - i, y - iDistance + 12);

		// top right stem
		surface()->DrawLine(x + iDistance - 12, y - iDistance + 12 - 1 - i, x + iDistance - i, y - iDistance - 1);
		surface()->DrawLine(x + iDistance - 12 + i, y - iDistance + 12 - 1, x + iDistance, y - iDistance - 1 + i);

		// bottom left stem
		surface()->DrawLine(x - iDistance, y + iDistance - 1 - i, x - iDistance + 12 - i, y + iDistance - 12 - 1);
		surface()->DrawLine(x - iDistance + i, y + iDistance - 1, x - iDistance + 12, y + iDistance - 12 - 1 + i);

		// bottom right stem
		surface()->DrawLine(x + iDistance - 12 + i, y + iDistance - 12, x + iDistance, y + iDistance - i);
		surface()->DrawLine(x + iDistance - 12, y + iDistance - 12 + i, x + iDistance - i, y + iDistance);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCrosshair::SetCrosshairAngle( const QAngle& angle )
{
	VectorCopy( angle, m_vecCrossHairOffsetAngle );
}