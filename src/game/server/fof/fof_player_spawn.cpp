#include "cbase.h"
#include "fof_player_spawn.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( info_player_fof, CFOFPlayerSpawn );
LINK_ENTITY_TO_CLASS( info_player_deathmatch, CFOFPlayerSpawn );

LINK_ENTITY_TO_CLASS( info_player_counterterrorist, CFOFPlayerSpawn );
LINK_ENTITY_TO_CLASS( info_player_terrorist, CFOFPlayerSpawn );

CFOFPlayerSpawn::CFOFPlayerSpawn()
{

}

CFOFPlayerSpawn::~CFOFPlayerSpawn()
{

}

LINK_ENTITY_TO_CLASS( spawn_manager, CFOFSpawnManager );

CFOFSpawnManager::CFOFSpawnManager()
{

}

CFOFSpawnManager::~CFOFSpawnManager()
{

}

void CFOFSpawnManager::AddPlayerSpawn(playerspawn_t playerSpawn)
{
	m_vecSpawnPoints.AddToTail(playerSpawn);
}

playerspawn_t* CFOFSpawnManager::GetPlayerSpawn()
{
	return &m_vecSpawnPoints.Random();
}