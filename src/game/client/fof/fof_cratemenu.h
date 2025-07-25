#ifndef FOF_CRATEMENU_H
#define FOF_CRATEMENU_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include <cdll_client_int.h>

#include "fof_equipmenu.h"
#include "fof_equipmenticon.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <hl2mp_gamerules.h>
#include <filesystem.h>
#include <hudelement.h>

#include <vgui_controls/RichText.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/HTML.h>
#include <vgui_controls/ScalableImagePanel.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/EditablePanel.h>
#include "vgui/IInput.h"
#include <vgui/IVGui.h>

#include <game/client/iviewport.h>

#include <vgui/KeyCode.h>
#include <utlvector.h>

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

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CFOFCrateMenu : public vgui::Frame, public IViewPortPanel
{
	DECLARE_CLASS_SIMPLE( CFOFCrateMenu, EditablePanel );

public:
	CFOFCrateMenu(IViewPort *pViewPort);
	~CFOFCrateMenu();

	virtual const char *GetName( void ) { return PANEL_CRATE; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset() {};
	virtual void Update() {};
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);
	virtual void OnKeyCodePressed(vgui::KeyCode code);

	void SelectItem( int iItem );

	virtual const char *GetResFilename( void ) { return "resource/UI/cratemenu.res"; }

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_MENUCONTROLS; }

protected:
	IViewPort	*m_pViewPort;
};

#endif