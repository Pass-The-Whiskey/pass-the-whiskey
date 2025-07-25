#ifndef FOF_IMAGEPANEL_H
#define FOF_IMAGEPANEL_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui/IScheme.h>
#include "vgui_controls/ScalableImagePanel.h"
#include "GameEventListener.h"
#include "hl2mp_gamerules.h"

#define MAX_BG_LENGTH		128

class CFOFImagePanel : public vgui::ScalableImagePanel
{
public:
	DECLARE_CLASS_SIMPLE( CFOFImagePanel, vgui::ScalableImagePanel );

	CFOFImagePanel( vgui::Panel *parent, const char *name );

	virtual void PaintBackground( void );
	virtual void ApplySettings( KeyValues *inResourceData );
	virtual void OnThink();

public: // IGameEventListener Interface
	virtual void SetTeam( int iTeam );

public:
	vgui::IImage* m_pBGImage[TEAM_COUNT];
	vgui::IImage* m_pFGImage[TEAM_COUNT];

	char	m_szTeamBG[TEAM_COUNT][MAX_BG_LENGTH];
	char	m_szTeamFG[TEAM_COUNT][MAX_BG_LENGTH];

	int		m_iTeam;
};


#endif // FOF_IMAGEPANEL_H
