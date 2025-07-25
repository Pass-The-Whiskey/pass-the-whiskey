"Resource/UI/EquipMenu.res"
{
	"equip"
	{
		"ControlName"		"CFOFEquipMenu"
		"fieldName"			"equip"
		"xpos"				"cs-0.5"
		"ypos"				"cs-0.5"
		"wide"				"380"
		"tall"				"366"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
	}

	"Header"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"Header"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"-1"
		"wide"				"380"
		"tall"				"60"
		"border"			"EquipmentBackground"
	}

	"Background"
	{
		"ControlName"		"EditablePanel"
		"fieldName"			"Background"
		"xpos"				"0"
		"ypos"				"60"
		"zpos"				"-1"
		"wide"				"380"
		"tall"				"306"
		"border"			"EquipmentBackground"
	}

	"PrimaryEquipmentContainer"
	{
		"ControlName"		"CFOFEquipmentContainer"
		"fieldName"			"PrimaryEquipmentContainer"
		"xpos"				"20"
		"ypos"				"80"
		"wide"				"100"
		"tall"				"266"
		"visible"			"1"
		"enabled"			"1"
		
		"equipment_type"	"0"
	}

	"SecondaryEquipmentContainer"
	{
		"ControlName"		"CFOFEquipmentContainer"
		"fieldName"			"SecondaryEquipmentContainer"
		"xpos"				"140"
		"ypos"				"80"
		"wide"				"100"
		"tall"				"266"
		"visible"			"1"
		"enabled"			"1"

		"equipment_type"	"1"
	}

	"HandgunSkillEquipmentContainer"
	{
		"ControlName"		"CFOFHandgunSkillEquipmentContainer"
		"fieldName"			"HandgunSkillEquipmentContainer"
		"xpos"				"260"
		"ypos"				"80"
		"wide"				"100"
		"tall"				"266"
		"visible"			"1"
		"enabled"			"1"
	}
}