#ifndef	FOF_PLAYER_SPAWN_H
#define	FOF_PLAYER_SPAWN_H

#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "utlvector.h"
#include "items.h"

class CFOFPlayerSpawn : public CBaseEntity
{
public:
	DECLARE_CLASS( CFOFPlayerSpawn, CBaseEntity );
	//DECLARE_DATADESC();

	CFOFPlayerSpawn();
	~CFOFPlayerSpawn();
};

struct playerspawn_t
{
	Vector vecOrigin;
	QAngle angAngles;

	int iTeamNum;
};

class CFOFSpawnManager : public CBaseEntity
{
public:
	DECLARE_CLASS( CFOFSpawnManager, CBaseEntity );

	CFOFSpawnManager();
	~CFOFSpawnManager();

	void AddPlayerSpawn(playerspawn_t playerSpawn);
	playerspawn_t* GetPlayerSpawn();
private:
	CUtlVector<playerspawn_t> m_vecSpawnPoints;
};

#endif