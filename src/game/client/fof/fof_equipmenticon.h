#ifndef FOF_EQUIPMENTICON_H
#define FOF_EQUIPMENTICON_H
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

enum
{
	EQUIPMENTTYPE_PRIMARY = 0,
	EQUIPMENTTYPE_SECONDARY,
	EQUIPMENTTYPE_HANDGUN_SKILL,
};

class CFOFEquipmentIcon : public vgui::Button
{
	DECLARE_CLASS_SIMPLE(CFOFEquipmentIcon, vgui::Button);

	CFOFEquipmentIcon(Panel* parent, const char* panelName, int type);
	~CFOFEquipmentIcon();

	virtual void	ApplySchemeSettings(vgui::IScheme* pScheme);
	virtual void	ApplySettings(KeyValues* inResourceData);

private:
	int			m_iType;
};

#endif