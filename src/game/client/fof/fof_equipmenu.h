#ifndef FOF_EQUIPMENU_H
#define FOF_EQUIPMENU_H
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

class CFOFEquipmentContainer : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CFOFEquipmentContainer, vgui::EditablePanel );

	CFOFEquipmentContainer(Panel *parent, const char *panelName);
	~CFOFEquipmentContainer();

	virtual void	ApplySettings(KeyValues *inResourceData);

private:
	CPanelAnimationVarAliasType( int, m_iEquipPadding, "equip_padding", "2", "proportional_int" );

	CPanelAnimationVarAliasType( int, m_iEquipMinWide, "equip_min_wide", "100", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iEquipMaxWide, "equip_max_wide", "100", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iEquipMinTall, "equip_min_tall", "25", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iEquipMaxTall, "equip_max_tall", "52", "proportional_int" );

	CPanelAnimationVar( int, m_iType, "equipment_type", "0" );
};

class CFOFHandgunSkillEquipmentContainer : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CFOFHandgunSkillEquipmentContainer, vgui::EditablePanel );

	CFOFHandgunSkillEquipmentContainer(Panel *parent, const char *panelName);
	~CFOFHandgunSkillEquipmentContainer();

	virtual void	ApplySettings(KeyValues *inResourceData);

private:
	CPanelAnimationVarAliasType( int, m_iEquipPadding, "equip_padding", "2", "proportional_int" );

	CPanelAnimationVarAliasType( int, m_iEquipMinWide, "equip_min_wide", "100", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iEquipMaxWide, "equip_max_wide", "100", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iEquipMinTall, "equip_min_tall", "25", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iEquipMaxTall, "equip_max_tall", "52", "proportional_int" );
};

//-----------------------------------------------------------------------------
// Purpose: Displays the equip menu
//-----------------------------------------------------------------------------
class CFOFEquipMenu : public vgui::Frame, public IViewPortPanel
{
	DECLARE_CLASS_SIMPLE( CFOFEquipMenu, vgui::Frame );

	CFOFEquipMenu(IViewPort *pViewPort);
	~CFOFEquipMenu();

	virtual const char *GetName( void ) { return PANEL_EQUIP; }
	virtual void SetData(KeyValues *data) {};
	virtual void Reset() {};
	virtual void Update() {};
	virtual bool NeedsUpdate( void ) { return false; }
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );
	virtual void ApplySchemeSettings(vgui::IScheme* pScheme);
	virtual void OnKeyCodePressed(vgui::KeyCode code);

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	virtual GameActionSet_t GetPreferredActionSet() { return GAME_ACTION_SET_MENUCONTROLS; }

protected:
	IViewPort	*m_pViewPort;

	ButtonCode_t m_iJumpKey;
	ButtonCode_t m_iEquipMenuKey;
private:
	CFOFEquipmentContainer* m_pPrimaryEquipmentContainer;
	CFOFEquipmentContainer* m_pSecondaryEquipmentContainer;

	CFOFHandgunSkillEquipmentContainer* m_pHandgunSkillEquipmentContainer;
};

#endif