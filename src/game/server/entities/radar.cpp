/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "radar.h"

CRadar::CRadar(CGameWorld *pGameWorld)
	: CEntity(pGameWorld, CGameWorld::ENTTYPE_LASER)
{
	m_Hide = true;
	GameWorld()->InsertEntity(this);
	m_Range = 0;
}

void CRadar::Reset()
{
}

void CRadar::Tick()
{
	// m_Range += 20;
	// if (m_Range > 600)
	//	m_Range = 0;

	m_Range = 350;

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		m_aTipPos[i].x += (m_aTipTargetPos[i].x - m_aTipPos[i].x) / 4.0f;
		m_aTipPos[i].y += (m_aTipTargetPos[i].y - m_aTipPos[i].y) / 4.0f;

		m_aBottomPos[i].x += (m_aBottomTargetPos[i].x - m_aBottomPos[i].x) / 4.0f;
		m_aBottomPos[i].y += (m_aBottomTargetPos[i].y - m_aBottomPos[i].y) / 4.0f;
		// m_aTipPos[i] = m_aTipTargetPos[i];
		// m_aBottomPos[i] = m_aBottomTargetPos[i];
	}
}

void CRadar::TickPaused()
{
}

void CRadar::Snap(int SnappingClient)
{
	if (SnappingClient < 0 || SnappingClient >= MAX_CLIENTS)
		return;

	CPlayer *pPlayer = GameServer()->m_apPlayers[SnappingClient];
	if (!pPlayer)
		return;

	if (!pPlayer->GotAbility(BOMBRADAR))
		return;

	CCharacter *pCharacter = pPlayer->GetCharacter();
	if (!pCharacter)
		return;

	CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if (!pObj)
		return;

	// if (distance(pCharacter->m_Pos, m_Target) < 400)
	//	return;

	float Angle = atan2(m_Target.x - pCharacter->m_Pos.x, m_Target.y - pCharacter->m_Pos.y);

	float Range = min((int)distance(m_Target, pCharacter->m_Pos), m_Range);

	vec2 Pos = pCharacter->m_Pos;
	vec2 TargetPos = vec2(pCharacter->m_Pos.x + sin(Angle) * Range, pCharacter->m_Pos.y + cos(Angle) * Range);

	GameServer()->Collision()->IntersectLine(Pos, TargetPos, &Pos, 0);

	m_aTipTargetPos[pPlayer->GetCID()] = Pos;

	/*
	pObj->m_X = (int)(pCharacter->m_Pos.x + sin(Angle) * (m_Range+100));
	pObj->m_Y = (int)(pCharacter->m_Pos.y + cos(Angle) * (m_Range+100));
	pObj->m_FromX = (int)(pCharacter->m_Pos.x + sin(Angle) * (m_Range));
	pObj->m_FromY = (int)(pCharacter->m_Pos.y + cos(Angle) * (m_Range));
	*/
	pObj->m_X = (int)(m_aTipPos[pPlayer->GetCID()].x);
	pObj->m_Y = (int)(m_aTipPos[pPlayer->GetCID()].y);
	pObj->m_FromX = (int)(m_aTipPos[pPlayer->GetCID()].x + 1);
	pObj->m_FromY = (int)(m_aTipPos[pPlayer->GetCID()].y + 1);
	pObj->m_StartTick = Server()->Tick();
}
