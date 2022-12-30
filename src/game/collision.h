/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_COLLISION_H
#define GAME_COLLISION_H

#include <vector>
#include <base/vmath.h>
#include "pathfinding.h"

enum BlockType
{
	BLOCKTYPE_SOLID,
	BLOCKTYPE_UNHOOKABLE,

	NUM_BLOCKTYPE,
};

class CBlockSolid
{
public:
	vec2 m_Pos;
	int m_Type;
};

class CCollision
{
	class CTile *m_pTiles;
	int m_Width;
	int m_Height;
	class CLayers *m_pLayers;
	std::vector<CBlockSolid> m_pBlockSolid;
	CBlockSolid *FindBlock(vec2 Pos);

	int GetTile(int x, int y);

	
	int m_WaypointCount;
	int m_ConnectionCount;
	
	void ClearWaypoints();
	void AddWaypoint(vec2 Position, bool InnerCorner = false);
	CWaypoint *GetWaypointAt(int x, int y);
	void ConnectWaypoints();
	CWaypoint *GetClosestWaypoint(vec2 Pos);

	CWaypoint *m_apWaypoint[MAX_WAYPOINTS];
	CWaypoint *m_pCenterWaypoint;
	
	CWaypointPath *m_pPath;
	
public:
	enum
	{
		COLFLAG_SOLID=1,
		COLFLAG_DEATH=2,
		COLFLAG_NOHOOK=4,
		COLFLAG_RAMP_LEFT=8,
		COLFLAG_RAMP_RIGHT=16,
		COLFLAG_ROOFSLOPE_LEFT=32,
		COLFLAG_ROOFSLOPE_RIGHT=64,
		COLFLAG_DAMAGEFLUID=128,
		COLFLAG_INSTADEATH=256,
	};

	void CreateBlock(vec2 Pos, int Type);

	bool IsTileSolid(int x, int y, bool IncludeDeath = false);
	
	void GenerateWaypoints();
	bool GenerateSomeMoreWaypoints();
	int WaypointCount() { return m_WaypointCount; }
	int ConnectionCount() { return m_ConnectionCount; }
	
	void SetWaypointCenter(vec2 Position);
	void AddWeight(vec2 Pos, int Weight);
	bool FindWaypointPath(vec2 TargetPos);
	
	//CWaypointPath *AStar(vec2 Start, vec2 End);
	bool AStar(vec2 Start, vec2 End);
	
	CWaypointPath *GetPath(){ return m_pPath; }
	void ForgetAboutThePath(){ m_pPath = 0; }
		

	// for testing
	vec2 m_aPath[99];
	
	CCollision();
	void Init(class CLayers *pLayers);
	bool CheckPoint(float x, float y, bool IncludeDeath = false) { return IsTileSolid(round(x), round(y), IncludeDeath); }
	bool CheckPoint(vec2 Pos) { return CheckPoint(Pos.x, Pos.y); }
	int GetCollisionAt(float x, float y) { return GetTile(round(x), round(y)); }
	int GetWidth() { return m_Width; };
	int GetHeight() { return m_Height; };
	int FastIntersectLine(vec2 Pos0, vec2 Pos1);
	int IntersectLine(vec2 Pos0, vec2 Pos1, vec2 *pOutCollision, vec2 *pOutBeforeCollision, bool IncludeDeath = false);
	void MovePoint(vec2 *pInoutPos, vec2 *pInoutVel, float Elasticity, int *pBounces);
	void MoveBox(vec2 *pInoutPos, vec2 *pInoutVel, vec2 Size, float Elasticity);
	bool TestBox(vec2 Pos, vec2 Size);

	// MapGen
	bool ModifTile(ivec2 pos, int group, int layer, int tile, int flags, int reserved);

	vec2 RoundPos(vec2 Pos)
	{
		Pos.x -= (int)Pos.x % 32 - 16;
		Pos.y -= (int)Pos.y % 32 - 16;
		return Pos;
	}
};

#endif
