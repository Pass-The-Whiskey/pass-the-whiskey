#include "cbase.h"
#include <cdll_client_int.h>

#include "fof_equipmenticon.h"

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/Button.h>
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

CFOFEquipmentIcon::CFOFEquipmentIcon( Panel* parent, const char* panelName, int type ) : Button( parent, panelName, "" )
{
	m_iType = type;
}

CFOFEquipmentIcon::~CFOFEquipmentIcon()
{
}

void CFOFEquipmentIcon::ApplySchemeSettings(vgui::IScheme* pScheme)
{
	switch (m_iType)
	{
	default:
	case EQUIPMENTTYPE_PRIMARY:
		SetBorder(pScheme->GetBorder("EquipmentPrimary"));
		SetDefaultBorder(pScheme->GetBorder("EquipmentPrimary"));
		SetCommand("lol");
		break;
	case EQUIPMENTTYPE_SECONDARY:
		SetBorder(pScheme->GetBorder("EquipmentSecondary"));
		SetDefaultBorder(pScheme->GetBorder("EquipmentSecondary"));
		SetCommand("lol");
		break;
	case EQUIPMENTTYPE_HANDGUN_SKILL:
		SetBorder(pScheme->GetBorder("EquipmentHandgunSkill"));
		SetDefaultBorder(pScheme->GetBorder("EquipmentHandgunSkill"));
		SetCommand("lol");
		break;
	}
}

void CFOFEquipmentIcon::ApplySettings(KeyValues* inResourceData)
{
	
}