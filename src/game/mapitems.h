/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_MAPITEMS_H
#define GAME_MAPITEMS_H

// layer types
enum
{
	LAYERTYPE_INVALID=0,
	LAYERTYPE_GAME,
	LAYERTYPE_TILES,
	LAYERTYPE_QUADS,

	MAPITEMTYPE_VERSION=0,
	MAPITEMTYPE_INFO,
	MAPITEMTYPE_IMAGE,
	MAPITEMTYPE_ENVELOPE,
	MAPITEMTYPE_GROUP,
	MAPITEMTYPE_LAYER,
	MAPITEMTYPE_ENVPOINTS,


	CURVETYPE_STEP=0,
	CURVETYPE_LINEAR,
	CURVETYPE_SLOW,
	CURVETYPE_FAST,
	CURVETYPE_SMOOTH,
	NUM_CURVETYPES,

	// game layer tiles
	ENTITY_NULL=0,
	ENTITY_SPAWN,
	ENTITY_SPAWN_RED,
	ENTITY_SPAWN_BLUE,
	ENTITY_FLAGSTAND_RED,
	ENTITY_FLAGSTAND_BLUE,
	ENTITY_ARMOR_1,
	ENTITY_HEALTH_1,
	ENTITY_WEAPON_SHOTGUN,
	ENTITY_WEAPON_GRENADE,
	ENTITY_POWERUP_NINJA,
	ENTITY_WEAPON_RIFLE,
	ENTITY_ARMOR_2,
	ENTITY_HEALTH_2,
	ENTITY_WEAPON_DROP,
	ENTITY_TELEPORT_IN,
	ENTITY_TELEPORT_OUT,
	ENTITY_LOSEPOINT,
	ENTITY_LASERWALL,

	ENTITY_AMMO_1,

	ENTITY_KIT,
	ENTITY_RANDOM_WEAPON,
	ENTITY_BALL,
	ENTITY_RED_AREA,
	ENTITY_BLUE_AREA,
	
	ENTITY_SWITCH = 16*1 + 1,
	ENTITY_DOOR1,
	ENTITY_GENERATOR,
	ENTITY_BARREL,
	ENTITY_POWERBARREL,
	ENTITY_LIGHTNINGWALL,
	ENTITY_REACTOR,
	ENTITY_TESLACOIL,
	ENTITY_SCREEN,
	ENTITY_STAND,
	ENTITY_FLAMETRAP_RIGHT,
	ENTITY_FLAMETRAP_LEFT,
	ENTITY_EMPTY,
	ENTITY_SAWBLADE,
	ENTITY_LAZER,
	ENTITY_POWERUPPER,
	
	// row 3
	ENTITY_SHOP = 16*2 + 1,
	ENTITY_BLOCK1,
	ENTITY_BLOCK2,
	
	// row 4
	ENTITY_ENEMYSPAWN = 16*3 + 1,
	ENTITY_TURRET,
	ENTITY_DROID_WALKER,
	ENTITY_DROID_STAR,
	ENTITY_DROID_CRAWLER,
	ENTITY_DROID_BOSSCRAWLER,

	TILE_AIR=0,
	TILE_SOLID,
	TILE_DEATH,
	TILE_NOHOOK,
	TILE_AILEFT,
	TILE_AIRIGHT,
	TILE_AIUP,
	TILE_NOGO,
	TILE_RELEASENOGO,
	
	TILE_INSTADEATH,
	TILE_RAMP_LEFT,
	TILE_RAMP_RIGHT,
	TILE_ROOFSLOPE_LEFT,
	TILE_ROOFSLOPE_RIGHT,
	TILE_DAMAGEFLUID,

	TILEFLAG_VFLIP=1,
	TILEFLAG_HFLIP=2,
	TILEFLAG_OPAQUE=4,
	TILEFLAG_ROTATE=8,

	LAYERFLAG_DETAIL=1,
	TILESLAYERFLAG_GAME=1,

	ENTITY_OFFSET=255-16*4,
};

struct CPoint
{
	int x, y; // 22.10 fixed point
};

struct CColor
{
	int r, g, b, a;
};

struct CQuad
{
	CPoint m_aPoints[5];
	CColor m_aColors[4];
	CPoint m_aTexcoords[4];

	int m_PosEnv;
	int m_PosEnvOffset;

	int m_ColorEnv;
	int m_ColorEnvOffset;
};

class CTile
{
public:
	unsigned char m_Index;
	unsigned char m_Flags;
	unsigned char m_Skip;
	unsigned char m_Reserved;
};

struct CMapItemInfo
{
	int m_Version;
	int m_Author;
	int m_MapVersion;
	int m_Credits;
	int m_License;
} ;

struct CMapItemImage_v1
{
	int m_Version;
	int m_Width;
	int m_Height;
	int m_External;
	int m_ImageName;
	int m_ImageData;
} ;

struct CMapItemImage : public CMapItemImage_v1
{
	enum { CURRENT_VERSION=2 };
	int m_Format;
};

struct CMapItemGroup_v1
{
	int m_Version;
	int m_OffsetX;
	int m_OffsetY;
	int m_ParallaxX;
	int m_ParallaxY;

	int m_StartLayer;
	int m_NumLayers;
} ;


struct CMapItemGroup : public CMapItemGroup_v1
{
	enum { CURRENT_VERSION=3 };

	int m_UseClipping;
	int m_ClipX;
	int m_ClipY;
	int m_ClipW;
	int m_ClipH;

	int m_aName[3];
} ;

struct CMapItemLayer
{
	int m_Version;
	int m_Type;
	int m_Flags;
} ;

struct CMapItemLayerTilemap
{
	CMapItemLayer m_Layer;
	int m_Version;

	int m_Width;
	int m_Height;
	int m_Flags;

	CColor m_Color;
	int m_ColorEnv;
	int m_ColorEnvOffset;

	int m_Image;
	int m_Data;

	int m_aName[3];
} ;

struct CMapItemLayerQuads
{
	CMapItemLayer m_Layer;
	int m_Version;

	int m_NumQuads;
	int m_Data;
	int m_Image;

	int m_aName[3];
} ;

struct CMapItemVersion
{
	int m_Version;
} ;

struct CEnvPoint
{
	int m_Time; // in ms
	int m_Curvetype;
	int m_aValues[4]; // 1-4 depending on envelope (22.10 fixed point)

	bool operator<(const CEnvPoint &Other) { return m_Time < Other.m_Time; }
} ;

struct CMapItemEnvelope_v1
{
	int m_Version;
	int m_Channels;
	int m_StartPoint;
	int m_NumPoints;
	int m_aName[8];
} ;

struct CMapItemEnvelope : public CMapItemEnvelope_v1
{
	enum { CURRENT_VERSION=2 };
	int m_Synchronized;
};

#endif
