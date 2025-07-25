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

CFOFEquipMenu::CFOFEquipMenu( IViewPort *pViewPort ) : Frame( NULL, PANEL_EQUIP )
{
	m_pViewPort = pViewPort;
	m_iJumpKey = BUTTON_CODE_INVALID; // this is looked up in Activate()
	m_iEquipMenuKey = BUTTON_CODE_INVALID; // this is looked up in Activate()

	// initialize dialog
	SetTitle("", true);

	// load the new scheme early!!
	SetScheme("ClientScheme");
	SetMoveable(true);
	SetSizeable(false);

	// hide the system buttons
	SetTitleBarVisible( false );
	SetProportional(true);

	m_pPrimaryEquipmentContainer = new CFOFEquipmentContainer( this, "PrimaryEquipmentContainer" );
	m_pSecondaryEquipmentContainer = new CFOFEquipmentContainer( this, "SecondaryEquipmentContainer" );

	m_pHandgunSkillEquipmentContainer = new CFOFHandgunSkillEquipmentContainer( this, "HandgunSkillEquipmentContainer" );
}

CFOFEquipMenu::~CFOFEquipMenu()
{
}

extern ConVar fof_sv_weaponmenu;

void CFOFEquipMenu::ShowPanel(bool bShow)
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if (!fof_sv_weaponmenu.GetBool())
		bShow = false;

	if ( bShow )
	{
		Activate();

		SetMouseInputEnabled( true );

		// get key bindings if shown

		if( m_iJumpKey == BUTTON_CODE_INVALID ) // you need to lookup the jump key AFTER the engine has loaded
		{
			m_iJumpKey = gameuifuncs->GetButtonCodeForBind( "jump" );
		}

		if ( m_iEquipMenuKey == BUTTON_CODE_INVALID ) 
		{
			m_iEquipMenuKey = gameuifuncs->GetButtonCodeForBind( "equipmenu" );
		}
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CFOFEquipMenu::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	LoadControlSettings("Resource/UI/EquipMenu.res");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFEquipMenu::OnKeyCodePressed(vgui::KeyCode code)
{
	if ( m_iJumpKey != BUTTON_CODE_INVALID && m_iJumpKey == code )
	{
		gViewPortInterface->ShowPanel( PANEL_EQUIP, false );
	}
	else if ( m_iEquipMenuKey != BUTTON_CODE_INVALID && m_iEquipMenuKey == code )
	{
		gViewPortInterface->ShowPanel( PANEL_EQUIP, false );
	}
	else
	{
		BaseClass::OnKeyCodePressed( code );
	}
}

CFOFEquipmentContainer::CFOFEquipmentContainer( Panel *parent, const char *panelName ) : EditablePanel( parent, panelName )
{
}

CFOFEquipmentContainer::~CFOFEquipmentContainer()
{
}

void CFOFEquipmentContainer::ApplySettings(KeyValues *inResourceData)
{
	while ( vgui::ipanel()->GetChildCount( GetVPanel() ) )
	{
		vgui::VPANEL child = vgui::ipanel()->GetChild( GetVPanel(), 0 );
		vgui::ipanel()->DeletePanel( child );
	}

	BaseClass::ApplySettings(inResourceData);

	int iEquipmentCount = 10;
	int iTotalPadding = ( ( iEquipmentCount - 1 ) * m_iEquipPadding );
	int iPanelTall = Clamp(( ( GetTall() / iEquipmentCount ) - iTotalPadding ), m_iEquipMinTall, m_iEquipMaxTall);

	for (int i = 0; i < iEquipmentCount; i++)
	{
		CFOFEquipmentIcon* pImage = new CFOFEquipmentIcon(this, CFmtStr("Icon%d", i), m_iType);
		pImage->SetSize(m_iEquipMaxWide, iPanelTall);
		pImage->SetPos(0, (iPanelTall * i) + (m_iEquipPadding * i));
		pImage->SetVisible(true);
	}
}

CFOFHandgunSkillEquipmentContainer::CFOFHandgunSkillEquipmentContainer( Panel *parent, const char *panelName ) : EditablePanel( parent, panelName )
{
}

CFOFHandgunSkillEquipmentContainer::~CFOFHandgunSkillEquipmentContainer()
{
}

void CFOFHandgunSkillEquipmentContainer::ApplySettings(KeyValues *inResourceData)
{
	BaseClass::ApplySettings(inResourceData);

	int iEquipmentCount = 10;
	int iTotalPadding = ( ( iEquipmentCount - 1 ) * m_iEquipPadding );
	int iPanelTall = Clamp(( ( GetTall() / iEquipmentCount ) - iTotalPadding ), m_iEquipMinTall, m_iEquipMaxTall);

	CFOFEquipmentIcon* pNoSkillIcon = new CFOFEquipmentIcon(this, "Icon0", EQUIPMENTTYPE_HANDGUN_SKILL);
	pNoSkillIcon->SetSize(m_iEquipMaxWide, iPanelTall);
	pNoSkillIcon->SetPos(0, 0);
	pNoSkillIcon->SetVisible(true);

	CFOFEquipmentIcon* pLeftHandedSkillIcon = new CFOFEquipmentIcon(this, "Icon1", EQUIPMENTTYPE_HANDGUN_SKILL);
	pLeftHandedSkillIcon->SetSize((m_iEquipMaxWide / 2) - (m_iEquipPadding / 2), (iPanelTall * 2) + m_iEquipPadding);
	pLeftHandedSkillIcon->SetPos(0, (iPanelTall * 1) + (m_iEquipPadding * 1));
	pLeftHandedSkillIcon->SetVisible(true);

	CFOFEquipmentIcon* pRightHandedSkillIcon = new CFOFEquipmentIcon(this, "Icon2", EQUIPMENTTYPE_HANDGUN_SKILL);
	pRightHandedSkillIcon->SetSize((m_iEquipMaxWide / 2) - (m_iEquipPadding / 2), (iPanelTall * 2) + m_iEquipPadding);
	pRightHandedSkillIcon->SetPos(GetWide() - (pRightHandedSkillIcon->GetWide()), (iPanelTall * 1) + (m_iEquipPadding * 1));
	pRightHandedSkillIcon->SetVisible(true);

	CFOFEquipmentIcon* pAmbidextrousSkillIcon = new CFOFEquipmentIcon(this, "Icon3", EQUIPMENTTYPE_HANDGUN_SKILL);
	pAmbidextrousSkillIcon->SetSize(m_iEquipMaxWide, (iPanelTall * 2) + m_iEquipPadding);
	pAmbidextrousSkillIcon->SetPos(0, (iPanelTall * 3) + (m_iEquipPadding * 3));
	pAmbidextrousSkillIcon->SetVisible(true);

	CFOFEquipmentIcon* pFanningSkillIcon = new CFOFEquipmentIcon(this, "Icon4", EQUIPMENTTYPE_HANDGUN_SKILL);
	pFanningSkillIcon->SetSize(m_iEquipMaxWide, (iPanelTall * 2) + m_iEquipPadding);
	pFanningSkillIcon->SetPos(0, (iPanelTall * 5) + (m_iEquipPadding * 5));
	pFanningSkillIcon->SetVisible(true);
}