"Resource/UI/TeamMenu.res"
{
	"team"
	{
		"ControlName"		"CTeamMenu"
		"fieldName"			"team"
		"xpos"				"cs-0.5"
		"ypos"				"330"
		"wide"				"400"
		"tall"				"100"
		"autoResize"		"0"
		"pinCorner"			"0"
		"visible"			"1"
		"enabled"			"1"
		"tabPosition"		"0"
	}
	"Background"
	{
		"ControlName"		"ScalableImagePanel"
		"fieldName"			"Background"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"-1"
		"wide"				"400"
		"tall"				"100"
		"image"				"teambg"
	}
	"cancel"
	{
		"ControlName"		"ImageButton"
		"fieldName"			"cancel"
		"xpos"				"65"
		"ypos"				"35"
		"wide"				"70"
		"tall"				"35"
		"autoResize"		"0"
		"visible"			"1"
		"enabled"			"1"
		"labelText"			""
		"command"			"vguicancel"
		"Default"			"1"

		"image_drawcolor"		"255 255 255 255"
		
		"SubImage"
		{
			"ControlName"		"ImagePanel"
			"fieldName"			"SubImage"
			"xpos"				"0"
			"ypos"				"0"
			"wide"				"70"
			"tall"				"35"
			"visible"			"1"
			"enabled"			"1"
			"scaleImage"		"1"
			"image"				"tm_cancel"
		}
	}
	"autojoin"
	{
		"ControlName"		"ImageButton"
		"fieldName"			"autojoin"
		"xpos"				"cs-0.5"
		"ypos"				"35"
		"wide"				"70"
		"tall"				"35"
		"autoResize"		"0"
		"visible"			"1"
		"enabled"			"1"
		"labelText"			""
		"command"			"jointeam 0"
		"Default"			"1"
		"proportionaltoparent"			"1"
		
		"image_drawcolor"		"255 255 255 255"
		
		"SubImage"
		{
			"ControlName"		"ImagePanel"
			"fieldName"			"SubImage"
			"xpos"				"0"
			"ypos"				"0"
			"wide"				"70"
			"tall"				"35"
			"visible"			"1"
			"enabled"			"1"
			"scaleImage"		"1"
			"image"				"tm_auto2"
		}
	}
	"spectate"
	{
		"ControlName"		"ImageButton"
		"fieldName"			"spectate"
		"xpos"				"265"
		"ypos"				"35"
		"wide"				"70"
		"tall"				"35"
		"autoResize"		"0"
		"visible"			"1"
		"enabled"			"1"
		"labelText"			""
		"command"			"jointeam 1"
		"Default"			"1"
		"proportionaltoparent"			"1"
		
		"image_drawcolor"		"255 255 255 255"
		
		"SubImage"
		{
			"ControlName"		"ImagePanel"
			"fieldName"			"SubImage"
			"xpos"				"0"
			"ypos"				"0"
			"wide"				"70"
			"tall"				"35"
			"visible"			"1"
			"enabled"			"1"
			"scaleImage"		"1"
			"image"				"tm_spect"
		}
	}
}