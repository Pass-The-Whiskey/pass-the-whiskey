#ifndef FOF_WEAPONWHEEL_H
#define FOF_WEAPONWHEEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/EditablePanel.h>
#include "vgui/IInput.h"
#include <vgui/IVGui.h>

#include <game/client/iviewport.h>

#include <vgui/KeyCode.h>
#include <utlvector.h>

//-----------------------------------------------------------------------------
// Purpose: Displays the equip menu
//-----------------------------------------------------------------------------
class CFOFWeaponWheel : public vgui::Frame, public IViewPortPanel
{
	DECLARE_CLASS_SIMPLE( CFOFWeaponWheel, vgui::Frame );

	CFOFWeaponWheel(IViewPort *pViewPort);
	~CFOFWeaponWheel();

	virtual const char *GetName( void ) { return PANEL_WEAPONWHEEL; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset() {};
	virtual void Update() {};
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);
	virtual void OnKeyCodePressed(vgui::KeyCode code);

	void Paint();

	float GetArea(int x1, int y1, int x2, int y2, int x3, int y3);
	bool IsMouseInTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_MENUCONTROLS; }

protected:
	IViewPort	*m_pViewPort;
};

#endif