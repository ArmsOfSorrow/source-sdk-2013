"resource/scaninfo.res"
{
	"ScanTextLabel"
	{
		"ControlName" 	"RichText"
		"fieldname"		"ScanTextLabel"
		
		"xpos"	"0"
		"ypos"	"0"
		"proportionalToParent" "1"
		"visible" "1"
		"enabled" "1"
		"textAlignment" "north-west"
		"text"	"welcome to ltp"
		"AutoResize" "3"
		
		//LoadControlSettings treats every child as a panel, that means font
		//and probably a myriad of other options are not available...but we should
		//be able to set colors at least. for some reason it's all white :S
		
		//ApplySettings gets stuff for HudScanInfo itself, not for its children. I can still
		//load stuff manually from there if it's found in HudScanInfo.res
	}
}