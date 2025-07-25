#include "cbase.h"
#include <cdll_client_int.h>

#include "fof_cratemenu.h"
#include "fof_equipmenticon.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <hl2mp_gamerules.h>
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

CFOFCrateMenu::CFOFCrateMenu( IViewPort *pViewPort ) : Frame( NULL, PANEL_CRATE )
{
	m_pViewPort = pViewPort;

	// initialize dialog
	SetTitle( "", true );

	// load the new scheme early!!
	SetScheme( "ClientScheme" );
	SetMoveable( true );
	SetSizeable( false );

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional( true );
}

CFOFCrateMenu::~CFOFCrateMenu()
{
}

void CFOFCrateMenu::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		Activate();
	}
	else
	{
		SetVisible( false );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFCrateMenu::ApplySchemeSettings( IScheme* pScheme )
{
	LoadControlSettings( GetResFilename() );

	BaseClass::ApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFCrateMenu::OnKeyCodePressed(vgui::KeyCode code)
{
	if ( code > KEY_0 && code <= KEY_9 )
	{
		const int iButton = code - KEY_0;
		SelectItem( iButton );
	}

	BaseClass::OnKeyCodePressed( code );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFCrateMenu::SelectItem( int iItem )
{
	BaseClass::OnCommand( CFmtStr( "buy %i", iItem ).Access() );
	gViewPortInterface->ShowPanel( PANEL_CRATE, false );
}