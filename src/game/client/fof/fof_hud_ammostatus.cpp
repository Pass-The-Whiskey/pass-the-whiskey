#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include "view.h"

#include "iclientmode.h"

#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui_controls/AnimationController.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ScalableImagePanel.h>
#include <fmtstr.h>

#include <vgui/ILocalize.h>

#include "fof/vgui/fof_imagepanel.h"
#include "hudelement.h"

#include "convar.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

ConVar cl_ammo_red("cl_ammo_red", "255", FCVAR_ARCHIVE, "", true, 0, true, 255);
ConVar cl_ammo_green("cl_ammo_green", "255", FCVAR_ARCHIVE, "", true, 0, true, 255);
ConVar cl_ammo_blue("cl_ammo_blue", "255", FCVAR_ARCHIVE, "", true, 0, true, 255);
ConVar cl_ammo_thickness("cl_ammo_thickness", "2", FCVAR_ARCHIVE);

ConVar cl_ammo_outline("cl_ammo_outline", "1", FCVAR_ARCHIVE);
ConVar cl_ammo_outline_red("cl_ammo_red", "0", FCVAR_ARCHIVE, "", true, 0, true, 255);
ConVar cl_ammo_outline_green("cl_ammo_green", "0", FCVAR_ARCHIVE, "", true, 0, true, 255);
ConVar cl_ammo_outline_blue("cl_ammo_blue", "0", FCVAR_ARCHIVE, "", true, 0, true, 255);
ConVar cl_ammo_outline_thickness("cl_ammo_outline_thickness", "2", FCVAR_ARCHIVE);

//-----------------------------------------------------------------------------
// Purpose: Ammo panel
//-----------------------------------------------------------------------------
class CFOFAmmoIndicator : public EditablePanel
{
public:
	DECLARE_CLASS_SIMPLE( CFOFAmmoIndicator, EditablePanel );

	CFOFAmmoIndicator( vgui::Panel *parent, const char *name );

	virtual void Paint() OVERRIDE;
	void SetWeapon( CBaseCombatWeapon* pWeapon );

	void PaintRevolver();
	void PaintGeneric();

protected:
	bool m_bVisible;

	int m_iIndicatorType;

	bool m_bInvertRotation;
	int m_iCylinderRadius;
	int m_iBulletRadius;

	int m_iMaxClip;
	int m_iClip;
};

CFOFAmmoIndicator::CFOFAmmoIndicator( vgui::Panel *parent, const char *name ) : BaseClass( parent, name )
{
	m_bVisible = false;
}

static Color g_ColorWhite = Color(255, 255, 255, 255);
static Color g_ColorRed = Color(186, 0, 0, 255);

void CFOFAmmoIndicator::Paint()
{
	if ( !m_bVisible )
		return;

	switch (m_iIndicatorType)
	{
	case 0:
		PaintRevolver();
		break;
	case 1:
		PaintGeneric();
		break;
	default:
		break;
	}
}

void CFOFAmmoIndicator::PaintRevolver()
{
	int xPos = GetWide() / 2;
	int yPos = GetTall() / 2;

	int iDrawCount = (m_iMaxClip * (m_bInvertRotation ? 2 : 1));

	if ( cl_ammo_outline.GetBool() )
	{
		for (int i = 0; i < iDrawCount; i++)
		{
			if ( !m_bInvertRotation || i % 2 != 0)
			{
				int index = (i / (m_bInvertRotation ? 2 : 1));
				bool bMissing = index >= m_iClip;

				float x = 0 + m_iCylinderRadius * sin(2 * M_PI * i / iDrawCount);
				float y = 0 + m_iCylinderRadius * cos(2 * M_PI * i / iDrawCount);

				surface()->DrawSetColor(Color(0, 0, 0, 255));

				for (int i = -cl_ammo_thickness.GetInt() + 1; i <= cl_ammo_thickness.GetInt() + cl_ammo_outline_thickness.GetInt(); i++)
				{
					surface()->DrawOutlinedCircle(x + xPos, y + yPos, m_iBulletRadius - i, !bMissing ? 32 : 8);
				}
			}
		}
	}

	for (int i = 0; i < iDrawCount; i++)
	{
		if ( !m_bInvertRotation || i % 2 != 0)
		{
			int index = (i / (m_bInvertRotation ? 2 : 1));
			bool bMissing = index >= m_iClip;

			float x = 0 + m_iCylinderRadius * sin(2 * M_PI * i / iDrawCount);
			float y = 0 + m_iCylinderRadius * cos(2 * M_PI * i / iDrawCount);

			surface()->DrawSetColor(!bMissing ? g_ColorWhite : g_ColorRed);

			for (int i = 1; i <= cl_ammo_thickness.GetInt(); i++)
			{
				surface()->DrawOutlinedCircle(x + xPos, y + yPos, m_iBulletRadius - i, !bMissing ? 32 : 8);
			}
		}
	}
}

void CFOFAmmoIndicator::PaintGeneric()
{
	int iCenterX = GetWide() / 2;
	int iCenterY = GetTall() / 2;

	int iDrawCount = m_iMaxClip;
	int iTotalWidth = m_iCylinderRadius * iDrawCount;

	for (int i = 0; i < iDrawCount; i++)
	{
		bool bMissing = i >= m_iClip;
		bool bInvertRotation = i % 2 == 0;

		if (m_bInvertRotation)
			bInvertRotation = !bInvertRotation;

		surface()->DrawSetColor(!bMissing ? g_ColorWhite : g_ColorRed);

		surface()->DrawOutlinedCircle((iCenterX - (iTotalWidth / 2) + (m_iCylinderRadius * i)), (iCenterY - (bInvertRotation ? (m_iCylinderRadius / 2) : -(m_iCylinderRadius / 2))), m_iBulletRadius, !bMissing ? 32 : 8);
	}
}

void CFOFAmmoIndicator::SetWeapon( CBaseCombatWeapon* pWeapon )
{
	if (pWeapon)
	{
		m_bVisible = true;

		m_iIndicatorType = pWeapon->GetWpnData().m_iIndicatorType;

		m_bInvertRotation = pWeapon->GetWpnData().m_bInvertRotation;
		m_iCylinderRadius = YRES(pWeapon->GetWpnData().m_iCylinderRadius);
		m_iBulletRadius = YRES(pWeapon->GetWpnData().m_iBulletRadius);

		m_iMaxClip = pWeapon->GetMaxClip1();
		m_iClip = pWeapon->Clip1();
	}
	else
	{
		m_bVisible = false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Ammo status panel
//-----------------------------------------------------------------------------
class CHudAmmoStatus : public CHudElement, public EditablePanel
{
	DECLARE_CLASS_SIMPLE( CHudAmmoStatus, EditablePanel );

public:
	CHudAmmoStatus( const char *pElementName );
	virtual void ApplySchemeSettings( IScheme* pScheme );
	virtual void OnThink();

	virtual const char *GetResFilename( void ) { return "resource/UI/HudAmmoStatus.res"; }
private:
	CFOFAmmoIndicator* m_pWeaponIndicator1;
	CFOFAmmoIndicator* m_pWeaponIndicator2;
};

DECLARE_HUDELEMENT( CHudAmmoStatus );

CHudAmmoStatus::CHudAmmoStatus( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, pElementName )
{
	Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_PLAYERDEAD );

	m_pWeaponIndicator1 = new CFOFAmmoIndicator(this, "WeaponIndicator1");
	m_pWeaponIndicator2 = new CFOFAmmoIndicator(this, "WeaponIndicator2");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudAmmoStatus::ApplySchemeSettings( IScheme* pScheme )
{
	LoadControlSettings( GetResFilename() );

	BaseClass::ApplySchemeSettings( pScheme );
}

void CHudAmmoStatus::OnThink()
{
	m_pWeaponIndicator1->SetWeapon( GetActiveWeapon() );
	m_pWeaponIndicator2->SetWeapon( GetActiveWeapon2() );
}