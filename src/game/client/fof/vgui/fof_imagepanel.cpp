#include "cbase.h"
#include <KeyValues.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/IScheme.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui/ISurface.h>
#include <vgui/IImage.h>
#include <vgui_controls/Label.h>
#include <c_hl2mp_player.h>

#include "fof_imagepanel.h"

using namespace vgui;

DECLARE_BUILD_FACTORY( CFOFImagePanel );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CFOFImagePanel::CFOFImagePanel( Panel *parent, const char *name ) : ScalableImagePanel( parent, name )
{
	for ( int i = 0; i < TEAM_COUNT; i++ )
	{
		m_szTeamBG[i][0] = '\0';
		m_szTeamFG[i][0] = '\0';

		m_pBGImage[i] = NULL;
		m_pFGImage[i] = NULL;
	}

	C_HL2MP_Player *pPlayer = ToHL2MPPlayer( C_BasePlayer::GetLocalPlayer() );
	m_iTeam = pPlayer ? pPlayer->GetTeamNumber() : TEAM_UNASSIGNED;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFImagePanel::PaintBackground()
{
	int iTextureID = m_iTextureID;
	if ( m_pBGImage[m_iTeam] && m_pBGImage[m_iTeam]->GetID() != -1 )
	{
		m_iTextureID = m_pBGImage[m_iTeam]->GetID();
		BaseClass::PaintBackground();
	}
	else
	{
		BaseClass::PaintBackground();
	}
	if ( m_pFGImage[m_iTeam] && m_pFGImage[m_iTeam]->GetID() != -1)
	{
		m_iTextureID = m_pFGImage[m_iTeam]->GetID();
		BaseClass::PaintBackground();
	}
	m_iTextureID = iTextureID;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFImagePanel::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	for ( int i = 0; i < TEAM_COUNT; i++ )
	{
		Q_strncpy( m_szTeamBG[i], inResourceData->GetString( VarArgs("teambg_%d", i), "" ), sizeof( m_szTeamBG[i] ) );
		Q_strncpy( m_szTeamFG[i], inResourceData->GetString( VarArgs("teamfg_%d", i), "" ), sizeof( m_szTeamFG[i] ) );

		if ( m_szTeamBG[i] && m_szTeamBG[i][0] ) {
			auto pImage = scheme()->GetImage(m_szTeamBG[i], true);
			if (pImage)
				m_pBGImage[i] = pImage;
		}
		if ( m_szTeamFG[i] && m_szTeamFG[i][0] ) {
			auto pImage = scheme()->GetImage(m_szTeamFG[i], true);
			if (pImage)
				m_pFGImage[i] = pImage;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFImagePanel::OnThink()
{
	C_HL2MP_Player *pPlayer = ToHL2MPPlayer( C_BasePlayer::GetLocalPlayer() );

	if (pPlayer)
	{
		int iPlayerTeam = pPlayer->GetPlayerModelType() + FIRST_GAME_TEAM;

		if ((m_iTeam != iPlayerTeam))
			SetTeam(iPlayerTeam);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFOFImagePanel::SetTeam( int iTeam )
{
	m_iTeam = iTeam;
}