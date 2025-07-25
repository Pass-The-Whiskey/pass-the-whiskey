#include "cbase.h"
#include "c_baseentity.h"
#include "c_baseanimating.h"
#include "view.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar fof_cl_crate_open_size( "fof_cl_crate_open_size", "5", FCVAR_ARCHIVE );
ConVar fof_cl_crate_close_size( "fof_cl_crate_close_size", "5", FCVAR_ARCHIVE );

ConVar fof_sv_crate_opentime( "fof_sv_crate_opentime", "2.0", FCVAR_REPLICATED );

class C_FOFCrate : public C_BaseAnimating
{
public:
	DECLARE_CLASS( C_FOFCrate, C_BaseAnimating );
	DECLARE_CLIENTCLASS();

	C_FOFCrate();
	~C_FOFCrate();

	virtual void Precache( void );
	virtual void Spawn( void );

	float GetOpenProgress() { return ( ( gpGlobals->curtime - m_flOpenStartTime ) / fof_sv_crate_opentime.GetFloat() ); }
	float GetCloseProgress() { return ( ( m_flNextOpenTime - gpGlobals->curtime ) / m_flOpenTime ); }
	bool IsOpening(void) { return !IsOpen() && m_flNextOpenTime != m_flOpenStartTime; }
	bool IsOpen(void) { return m_flNextOpenTime != -1; }

	int DrawModel( int flags );
	void DrawProgressBar( void );

	IMaterial	*m_pOpenProgressMaterial_Empty;
	IMaterial	*m_pOpenProgressMaterial_Full;

	IMaterial	*m_pCloseProgressMaterial_Empty;
	IMaterial	*m_pCloseProgressMaterial_Full;
protected:
	float	m_flOpenTime;
	float	m_flNextOpenTime;
	float	m_flOpenStartTime;
};

IMPLEMENT_CLIENTCLASS_DT( C_FOFCrate, DT_FOFCrate, CFOFCrate )
	RecvPropFloat( RECVINFO( m_flOpenTime ) ),
	RecvPropFloat( RECVINFO( m_flNextOpenTime ) ),
	RecvPropFloat( RECVINFO( m_flOpenStartTime ) ),
END_RECV_TABLE()

C_FOFCrate::C_FOFCrate( void )
{
}

C_FOFCrate::~C_FOFCrate( void )
{
}

void C_FOFCrate::Precache( void )
{
	PrecacheMaterial( "VGUI/opencrate_on" );
	PrecacheMaterial( "VGUI/opencrate_off" );

	PrecacheMaterial( "VGUI/reload_icon_6_on" );
	PrecacheMaterial( "VGUI/reload_icon_6_off" );

	m_pOpenProgressMaterial_Empty = materials->FindMaterial( "VGUI/opencrate_off", TEXTURE_GROUP_VGUI );
	m_pOpenProgressMaterial_Full = materials->FindMaterial( "VGUI/opencrate_on", TEXTURE_GROUP_VGUI );

	m_pCloseProgressMaterial_Empty = materials->FindMaterial( "VGUI/reload_icon_6_off", TEXTURE_GROUP_VGUI );
	m_pCloseProgressMaterial_Full = materials->FindMaterial( "VGUI/reload_icon_6_on", TEXTURE_GROUP_VGUI );
}

void C_FOFCrate::Spawn( void )
{
	Precache();
}

int C_FOFCrate::DrawModel( int flags )
{
	int value = BaseClass::DrawModel( flags );

	DrawProgressBar();

	return value;
}

typedef struct
{
	float maxProgress;

	float vert1x;
	float vert1y;
	float vert2x;
	float vert2y;

	int swipe_dir_x;
	int swipe_dir_y;
} progress_segment_t;

progress_segment_t Segments[8] =
{
	{ 0.125, 0.5, 0.0, 1.0, 0.0, 1, 0 },
	{ 0.25,	 1.0, 0.0, 1.0, 0.5, 0, 1 },
	{ 0.375, 1.0, 0.5, 1.0, 1.0, 0, 1 },
	{ 0.50,	 1.0, 1.0, 0.5, 1.0, -1, 0 },
	{ 0.625, 0.5, 1.0, 0.0, 1.0, -1, 0 },
	{ 0.75,	 0.0, 1.0, 0.0, 0.5, 0, -1 },
	{ 0.875, 0.0, 0.5, 0.0, 0.0, 0, -1 },
	{ 1.0,	 0.0, 0.0, 0.5, 0.0, 1, 0 },
};

//-----------------------------------------------------------------------------
// Purpose: Draw progress bar above the flag indicating when it will return
//-----------------------------------------------------------------------------
void C_FOFCrate::DrawProgressBar( void )
{
	// Don't draw if this flag is not going to reset
	if ( !IsOpen() && !IsOpening() )
		return;

	CMatRenderContextPtr pRenderContext( materials );

	Vector vOrigin = GetAbsOrigin();
	QAngle vAngle = vec3_angle;

	if ( IsOpening() )
		vOrigin[2] += 15 + fof_cl_crate_close_size.GetFloat();

	// Align it towards the viewer
	Vector vUp = CurrentViewUp();
	Vector vRight = CurrentViewRight();
	if ( fabs( vRight.z ) > 0.95 )	// don't draw it edge-on
		return;

	vRight.z = 0;
	VectorNormalize( vRight );

	float flSize = IsOpening() ? fof_cl_crate_open_size.GetFloat() : fof_cl_crate_close_size.GetFloat();

	unsigned char bgColor[4];
	bgColor[0] = 0;
	bgColor[1] = 0;
	bgColor[2] = 0;
	bgColor[3] = 255;

	unsigned char fgColor[4];
	fgColor[0] = 216;
	fgColor[1] = 192;
	fgColor[2] = 0;
	fgColor[3] = 255;

	// First we draw a quad of a complete icon, background
	CMeshBuilder meshBuilder;

	pRenderContext->Bind( IsOpening() ? m_pOpenProgressMaterial_Empty : m_pCloseProgressMaterial_Empty );
	IMesh *pMesh = pRenderContext->GetDynamicMesh();

	meshBuilder.Begin( pMesh, MATERIAL_QUADS, 1 );

	meshBuilder.Color4ubv( bgColor );
	meshBuilder.TexCoord2f( 0,0,0 );
	meshBuilder.Position3fv( (vOrigin + (vRight * -flSize) + (vUp * flSize)).Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ubv( bgColor );
	meshBuilder.TexCoord2f( 0,1,0 );
	meshBuilder.Position3fv( (vOrigin + (vRight * flSize) + (vUp * flSize)).Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ubv( bgColor );
	meshBuilder.TexCoord2f( 0,1,1 );
	meshBuilder.Position3fv( (vOrigin + (vRight * flSize) + (vUp * -flSize)).Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.Color4ubv( bgColor );
	meshBuilder.TexCoord2f( 0,0,1 );
	meshBuilder.Position3fv( (vOrigin + (vRight * -flSize) + (vUp * -flSize)).Base() );
	meshBuilder.AdvanceVertex();

	meshBuilder.End();

	pMesh->Draw();

	float flProgress = IsOpening() ? GetOpenProgress() : GetCloseProgress();

	pRenderContext->Bind( IsOpening() ? m_pOpenProgressMaterial_Full : m_pCloseProgressMaterial_Full );
	pMesh = pRenderContext->GetDynamicMesh();

	vRight *= flSize * 2;
	vUp *= flSize * -2;

	// Next we're drawing the circular progress bar, in 8 segments
	// For each segment, we calculate the vertex position that will draw
	// the slice.
	int i;
	for ( i = 0; i < 8; i++ )
	{
		if ( flProgress < Segments[i].maxProgress )
		{
			CMeshBuilder meshBuilder_Full;

			meshBuilder_Full.Begin( pMesh, MATERIAL_TRIANGLES, 3 );

			// vert 0 is ( 0.5, 0.5 )
			meshBuilder_Full.Color4ubv( fgColor );
			meshBuilder_Full.TexCoord2f( 0, 0.5, 0.5 );
			meshBuilder_Full.Position3fv( vOrigin.Base() );
			meshBuilder_Full.AdvanceVertex();

			// Internal progress is the progress through this particular slice
			float internalProgress = RemapVal( flProgress, Segments[i].maxProgress - 0.125, Segments[i].maxProgress, 0.0, 1.0 );
			internalProgress = clamp( internalProgress, 0.0, 1.0 );

			// Calculate the x,y of the moving vertex based on internal progress
			float swipe_x = Segments[i].vert2x - ( 1.0 - internalProgress ) * 0.5 * Segments[i].swipe_dir_x;
			float swipe_y = Segments[i].vert2y - ( 1.0 - internalProgress ) * 0.5 * Segments[i].swipe_dir_y;

			// vert 1 is calculated from progress
			meshBuilder_Full.Color4ubv( fgColor );
			meshBuilder_Full.TexCoord2f( 0, swipe_x, swipe_y );
			meshBuilder_Full.Position3fv( ( vOrigin + ( vRight * ( swipe_x - 0.5 ) ) + ( vUp *( swipe_y - 0.5 ) ) ).Base() );
			meshBuilder_Full.AdvanceVertex();

			// vert 2 is ( Segments[i].vert1x, Segments[i].vert1y )
			meshBuilder_Full.Color4ubv( fgColor );
			meshBuilder_Full.TexCoord2f( 0, Segments[i].vert2x, Segments[i].vert2y );
			meshBuilder_Full.Position3fv( ( vOrigin + ( vRight * ( Segments[i].vert2x - 0.5 ) ) + ( vUp *( Segments[i].vert2y - 0.5 ) ) ).Base() );
			meshBuilder_Full.AdvanceVertex();

			meshBuilder_Full.End();

			pMesh->Draw();
		}
	}
}