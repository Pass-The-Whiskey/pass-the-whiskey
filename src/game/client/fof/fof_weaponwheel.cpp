#include "cbase.h"
#include <cdll_client_int.h>

#include "fof_weaponwheel.h"
#include "fof_equipmenticon.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <filesystem.h>

#include <vgui_controls/RichText.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/HTML.h>
#include <vgui_controls/ScalableImagePanel.h>

#include "IGameUIFuncs.h" // for key bindings
#include <igameresources.h>
#include <game/client/iviewport.h>
#include <stdlib.h> // MAX_PATH define
#include <stdio.h>
#include "byteswap.h"
#include "fmtstr.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

CFOFWeaponWheel::CFOFWeaponWheel( IViewPort *pViewPort ) : Frame( NULL, PANEL_WEAPONWHEEL )
{
	m_pViewPort = pViewPort;

	// initialize dialog
	SetTitle("", true);

	// load the new scheme early!!
	SetScheme("ClientScheme");
	SetMoveable(true);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional( true );
}

CFOFWeaponWheel::~CFOFWeaponWheel()
{
}

void CFOFWeaponWheel::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		Activate();
		SetMouseInputEnabled( true );
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CFOFWeaponWheel::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	LoadControlSettings("Resource/UI/WeaponWheel.res");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFWeaponWheel::OnKeyCodePressed(vgui::KeyCode code)
{
	BaseClass::OnKeyCodePressed( code );
}

ConVar fof_weaponwheel_radius("fof_weaponwheel_radius", "150");
ConVar fof_weaponwheel_segments("fof_weaponwheel_segments", "6");

void CFOFWeaponWheel::Paint()
{
	int iCylinderRadius = YRES(fof_weaponwheel_radius.GetInt());
	int iCount = fof_weaponwheel_segments.GetInt();

	int iWide = GetWide() / 2;
	int iTall = GetTall() / 2;

	for (int i = 0; i < iCount; i++)
	{
		surface()->DrawSetColor(Color(255, 255, 255, 255));

		float x = iCylinderRadius * sin(2 * M_PI * (i) / iCount);
		float y = iCylinderRadius * cos(2 * M_PI * (i) / iCount);
		float x2 = iCylinderRadius * sin(2 * M_PI * (i+1) / iCount);
		float y2 = iCylinderRadius * cos(2 * M_PI * (i+1) / iCount);

		surface()->DrawLine(iWide, iTall, iWide + x, iTall + y);
		surface()->DrawLine(iWide, iTall, iWide + x2, iTall + y2);

		surface()->DrawOutlinedCircle(iWide, iTall, iCylinderRadius, iCount * 2);
	}

	for (int i = 0; i < iCount; i++)
	{
		float x = iCylinderRadius * sin(2 * M_PI * (i) / iCount);
		float y = iCylinderRadius * cos(2 * M_PI * (i) / iCount);
		float x2 = iCylinderRadius * sin(2 * M_PI * (i + 1) / iCount);
		float y2 = iCylinderRadius * cos(2 * M_PI * (i + 1) / iCount);

		int panelX = GetXPos();
		int panelY = GetYPos();

		if (IsMouseInTriangle(panelX + iWide + x, panelY + iTall + y, panelX + iWide, panelY + iTall, panelX + iWide + x2, panelY + iTall + y2))
		{
			surface()->DrawSetColor( Color(255, 0, 0, 255) );

			surface()->DrawLine(iWide, iTall, iWide + x, iTall + y);
			surface()->DrawLine(iWide, iTall, iWide + x2, iTall + y2);
		}
	}
}

float CFOFWeaponWheel::GetArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool CFOFWeaponWheel::IsMouseInTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	int x, y;
	input()->GetCursorPos( x, y );

	float flArea = GetArea(x1, y1, x2, y2, x3, y3);

	float flAreaA = GetArea(x, y, x2, y2, x3, y3);
	float flAreaB = GetArea(x1, y1, x, y, x3, y3);
	float flAreaC = GetArea(x1, y1, x2, y2, x, y);

	return (flArea == flAreaA + flAreaB + flAreaC);
}