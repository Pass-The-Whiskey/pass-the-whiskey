"Resource/UI/HudPlayerStatus.res"
{
	"HealthMeter"
	{
		"ControlName"		"CFOFHealthMeter"
		"fieldName"			"HealthMeter"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"1"
		"wide"				"90"
		"tall"				"90"
		"image"				"hp_team2_full"
		"teambg_2"			"hp_team2_full"
		"teambg_3"			"hp_team3_full"
		"teambg_4"			"hp_team4_full"
		"teambg_5"			"hp_team5_full"
	}
	"HealthMeterBackground"
	{
		"ControlName"		"CFOFImagePanel"
		"fieldName"			"HealthMeterBackground"
		"xpos"				"0"
		"ypos"				"0"
		"zpos"				"0"
		"wide"				"90"
		"tall"				"90"
		"image"				"hp_team2_empty"
		"teambg_2"			"hp_team2_empty"
		"teambg_3"			"hp_team3_empty"
		"teambg_4"			"hp_team4_empty"
		"teambg_5"			"hp_team5_empty"
	}
	"HealthLabel"
	{
		"ControlName"		"Label"
		"fieldName"			"HealthLabel"
		"xpos"				"90"
		"ypos"				"0"
		"zpos"				"0"
		"wide"				"90"
		"tall"				"90"
		"textAlignment"		"center"
		"labelText"			"%health%"
		"font"				"MenuFont"
		"visible"			"1"

		"if_account"
		{
			"textAlignment"		"south"
			"tall"				"45"
		}
	}
	"AccountLabel"
	{
		"ControlName"		"Label"
		"fieldName"			"AccountLabel"
		"xpos"				"90"
		"ypos"				"45"
		"zpos"				"0"
		"wide"				"90"
		"tall"				"45"
		"textAlignment"		"center"
		"labelText"			"%account%"
		"font"				"MenuFont"
		"visible"			"0"

		"if_account"
		{
			"textAlignment"		"north"
			"visible"			"1"
		}
	}
}