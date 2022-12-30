/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "electro.h"

CElectro::CElectro(CGameWorld *pGameWorld, vec2 Start, vec2 End, vec2 Offset, int Left)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Render = !Left;
	m_Pos = Start;
	m_End = End;
	m_EvalTick = Server()->Tick();
	GameWorld()->InsertEntity(this);

	if (Left > 0)
	{
		vec2 P = Start + End;
		P /= 2.0f;

		vec2 R = vec2(frandom() - frandom(), frandom() - frandom()) * 42.0f;

		GameServer()->Collision()->IntersectLine(P, P + R + Offset, 0x0, &P);

		// P += vec2(frandom()-frandom(), frandom()-frandom())*42.0f;
		// P += Offset;

		new CElectro(GameWorld(), Start, P, Offset * 0.5f, Left - 1);
		new CElectro(GameWorld(), P, End, Offset * 0.5f, Left - 1);

		/*
		float a = Angle + (frandom()-frandom()) * 1.11f;
		float l = 40.0f;

		//vec2 P2 = vec2(sin(a) * l, cos(a) * l);
		vec2 P2 = vec2(cosf(a), sinf(a)) * l;

		new CElectro(GameWorld(), P, P + P2, Angle, 0);
		*/
	}
}

void CElectro::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CElectro::Tick()
{
	if (!m_Render ||
		Server()->Tick() > m_EvalTick + (Server()->TickSpeed() * GameServer()->Tuning()->m_LaserBounceDelay) / 1000.0f)
		GameServer()->m_World.DestroyEntity(this);
}

void CElectro::TickPaused()
{
	++m_EvalTick;
}

void CElectro::Snap(int SnappingClient)
{
	if (!m_Render)
		return;

	if (NetworkClipped(SnappingClient))
		return;

	CNetObj_DDNetLaser *pObj = static_cast<CNetObj_DDNetLaser *>(Server()->SnapNewItem(NETOBJTYPE_DDNETLASER, m_ID, sizeof(CNetObj_DDNetLaser)));
	if (!pObj)
		return;

	pObj->m_ToX = (int)m_End.x;
	pObj->m_ToY = (int)m_End.y;
	pObj->m_FromX = (int)m_Pos.x;
	pObj->m_FromY = (int)m_Pos.y;
	pObj->m_StartTick = m_EvalTick;
	pObj->m_Owner = -1;
	pObj->m_Type = rand()%NUM_LASERTYPES;
}
