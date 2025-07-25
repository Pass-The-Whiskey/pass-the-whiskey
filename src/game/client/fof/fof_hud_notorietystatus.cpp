#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"
#include "c_hl2mp_player.h"
#include "c_playerresource.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ScalableImagePanel.h>
#include <fmtstr.h>

#include <vgui/ILocalize.h>

#include "hudelement.h"

#include "convar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

struct playerentry_t
{
	int	index;
	int score;

	bool operator == (const playerentry_t &rhs) const
	{
		return ( *((int *)this) == *((int *)&rhs) );
	}
};

//-----------------------------------------------------------------------------
// Purpose: Health panel
//-----------------------------------------------------------------------------
class CHudNotorietyStatus : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudNotorietyStatus, EditablePanel );

public:
	CHudNotorietyStatus( const char *pElementName );
	virtual void ApplySchemeSettings( IScheme* pScheme );
	virtual void OnThink();

	void Update();
	void SetNotorietyDialog(const char* varName, int ranking, int score, int frags);

	virtual const char *GetResFilename( void ) { return "resource/UI/HudNotorietyStatus.res"; }
};

DECLARE_HUDELEMENT( CHudNotorietyStatus );

CHudNotorietyStatus::CHudNotorietyStatus( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, pElementName )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudNotorietyStatus::ApplySchemeSettings( IScheme* pScheme )
{
	LoadControlSettings( GetResFilename() );

	BaseClass::ApplySchemeSettings( pScheme );
}

void CHudNotorietyStatus::OnThink()
{
	Update();
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting players
//-----------------------------------------------------------------------------
int __cdecl StaticPlayerSortFunc(const playerentry_t* pLeft, const playerentry_t* pRight)
{
	int v1 = pLeft->score;
	int v2 = pRight->score;
	if (v1 > v2)
		return false;
	else if (v1 < v2)
		return true;

	return pLeft->index < pRight->index;
}

void CHudNotorietyStatus::Update()
{
	CUtlVector<playerentry_t> vecScores;
	IGameResources* pResource = GameResources();
	playerentry_t localPlayer;
	bool bFoundLocalPlayer = false;;

	if (!pResource)
		return;

	for ( int i = 1; i <= gpGlobals->maxClients; ++i )
	{
		if (pResource->IsConnected(i)) {
			playerentry_t entry = playerentry_t();
			entry.index = i;
			entry.score = pResource->GetPlayerScore(i);
			vecScores.AddToTail(entry);
			if (i == GetLocalPlayerIndex())
			{
				bFoundLocalPlayer = true;
				localPlayer = entry;
			}
		}
	}

	if (!bFoundLocalPlayer)
		return;

	vecScores.Sort(StaticPlayerSortFunc);

	//%d%s/ %d x%d

	int iPlayerScoreIndex = vecScores.Find(localPlayer);

	SetNotorietyDialog("second", iPlayerScoreIndex + 1, vecScores[iPlayerScoreIndex].score, 0);

	if (iPlayerScoreIndex > 0)
	{
		SetNotorietyDialog("first", iPlayerScoreIndex, vecScores[iPlayerScoreIndex - 1].score, 0);
	}
	else
	{
		SetDialogVariable("first", "");
	}

	if (iPlayerScoreIndex + 1 < vecScores.Size())
	{
		SetNotorietyDialog("third", iPlayerScoreIndex + 2, vecScores[iPlayerScoreIndex + 1].score, 0);
	}
	else
	{
		SetDialogVariable("third", "");
	}
}

void CHudNotorietyStatus::SetNotorietyDialog(const char* varName, int ranking, int score, int frags)
{
	char* szFormat = "FoF_xth";

	if (ranking < 10 || ranking > 20)
	{
		switch (ranking % 10)
		{
		case 1:
			szFormat = "FoF_1st";
			break;
		case 2:
			szFormat = "FoF_2nd";
			break;
		case 3:
			szFormat = "FoF_3rd";
			break;
		}
	}

	wchar_t *loc = g_pVGuiLocalize->Find(szFormat);

	wchar_t unicode[16];
	char ansi[16];
	g_pVGuiLocalize->ConstructString(unicode, sizeof(unicode), loc, 0);
	g_pVGuiLocalize->ConvertUnicodeToANSI(unicode, ansi, sizeof(ansi));

	SetDialogVariable(varName, CFmtStr("%d%s/ %d x%d", ranking, ansi, score, 0));
}