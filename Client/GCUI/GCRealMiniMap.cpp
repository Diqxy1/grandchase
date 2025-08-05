/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2012년 04월 25일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 실시간 미니맵 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "stdafx.h"
#include "GCRealMiniMap.h"


IMPLEMENT_CLASSNAME(KGCRealMiniMap);
IMPLEMENT_WND_FACTORY(KGCRealMiniMap);
IMPLEMENT_WND_FACTORY_NAME(KGCRealMiniMap, "gc_real_minimap");

KGCRealMiniMap::KGCRealMiniMap(void)
{
    m_vMyPos.Init();
    m_mapEnemyPos.clear();
    m_mapAllyPos.clear();
    m_mapETCPos.clear();
    m_vecFootHoldPos.clear();

    m_pkBackGround = NULL;
    LINK_CONTROL("background", m_pkBackGround);

    m_pkStrEnemyNum = NULL;
    LINK_CONTROL("enemy_cnt_str", m_pkStrEnemyNum);
}

KGCRealMiniMap::~KGCRealMiniMap(void)
{

}

void KGCRealMiniMap::OnCreate(void)
{
    if (m_pkBackGround)
        m_pkBackGround->InitState(true);

    if (m_pkStrEnemyNum)
    {
        m_pkStrEnemyNum->InitState(true);
        m_pkStrEnemyNum->SetAlign(DT_LEFT);
        m_pkStrEnemyNum->SetFontColor(0xffffffff);
        m_pkStrEnemyNum->SetFontOutline(true, 0xff000000);
    }
}

void KGCRealMiniMap::PostChildDraw(void)
{
    // 애니메이션 프레임 무브
    FrameMoveToAnimation();

    // 발판블럭 출력
    std::vector<D3DXVECTOR4>::iterator vecFootHoldIter = m_vecFootHoldPos.begin();
    for (; vecFootHoldIter != m_vecFootHoldPos.end(); ++vecFootHoldIter)
    {
        float fScale = OBJ_SIZE * m_sMiniMapInfo.m_fScale_FootHold;
        float fX1 = CalcResolutionX(vecFootHoldIter->x);
        float fY1 = CalcResolutionY(vecFootHoldIter->y);
        float fX2 = CalcResolutionX(vecFootHoldIter->z);
        float fY2 = CalcResolutionY(vecFootHoldIter->w);

        g_pGCDeviceManager->DrawInScreen(NULL, fX1, fY1, fX2, (fY2 + fScale), 0.0f, 0.0f, 0.0f, 0.0f, false, 10.0f, m_sMiniMapInfo.m_dxColor_FootHold, false);
    }

    // 기타 오브젝트 출력
    std::map<D3DXVECTOR2, D3DCOLOR>::iterator mapETCIter = m_mapETCPos.begin();
    for (; mapETCIter != m_mapETCPos.end(); ++mapETCIter)
    {
        float fScale = OBJ_SIZE;
        float fX1 = CalcResolutionX(mapETCIter->first.x);
        float fY1 = CalcResolutionY(mapETCIter->first.y);
        g_pGCDeviceManager->DrawInScreen(NULL, (fX1 - (fScale + 1)), (fY1 - (fScale + 1)), (fX1 + (fScale + 1)), (fY1 + (fScale + 1)), 0.0f, 0.0f, 0.0f, 0.0f, false, 10.0f, 0xff000000, false);
        g_pGCDeviceManager->DrawInScreen(NULL, (fX1 - fScale), (fY1 - fScale), (fX1 + fScale), (fY1 + fScale), 0.0f, 0.0f, 0.0f, 0.0f, false, 10.0f, mapETCIter->second, false);
    }

    // 아군출력
    std::map<std::pair<int, int>, UNITINFO>::iterator mapAllyIter = m_mapAllyPos.begin();
    for (; mapAllyIter != m_mapAllyPos.end(); ++mapAllyIter)
    {
        if (false == mapAllyIter->second.bNoBaseRender)
        {
            DrawUnitToBase(mapAllyIter->second, m_sMiniMapInfo.m_fScale_Ally, m_sMiniMapInfo.m_dxColor_Ally);
        }
        std::vector<TEXTUREINFO>::iterator vecAddTexIter = mapAllyIter->second.m_vecAddTex.begin();
        for (; vecAddTexIter != mapAllyIter->second.m_vecAddTex.end(); ++vecAddTexIter)
        {
            DrawUnitToAddTexture(mapAllyIter->second, m_sMiniMapInfo.m_fScale_Ally, m_sMiniMapInfo.m_dxColor_Ally, &(*vecAddTexIter));
        }
    }

    // 적군출력
    D3DXVECTOR2 vConvertPos;
    std::map<std::pair<int, int>, UNITINFO>::iterator mapEnemyIter = m_mapEnemyPos.begin();
    for (; mapEnemyIter != m_mapEnemyPos.end(); ++mapEnemyIter)
    {
        if (false == mapEnemyIter->second.bNoBaseRender)
        {
            DrawUnitToBase(mapEnemyIter->second, m_sMiniMapInfo.m_fScale_Enemy, m_sMiniMapInfo.m_dxColor_Enemy);
        }
        std::vector<TEXTUREINFO>::iterator vecAddTexIter = mapEnemyIter->second.m_vecAddTex.begin();
        for (; vecAddTexIter != mapEnemyIter->second.m_vecAddTex.end(); ++vecAddTexIter)
        {
            DrawUnitToAddTexture(mapEnemyIter->second, m_sMiniMapInfo.m_fScale_Enemy, m_sMiniMapInfo.m_dxColor_Enemy, &(*vecAddTexIter));
        }
    }

    // 나 출력
    if (false == m_vMyPos.bNoBaseRender)
    {
        DrawUnitToBase(m_vMyPos, m_sMiniMapInfo.m_fScale_My, m_sMiniMapInfo.m_dxColor_My);
    }
    std::vector<TEXTUREINFO>::iterator vecAddTexIter = m_vMyPos.m_vecAddTex.begin();
    for (; vecAddTexIter != m_vMyPos.m_vecAddTex.end(); ++vecAddTexIter)
    {
        DrawUnitToAddTexture(m_vMyPos, m_sMiniMapInfo.m_fScale_Enemy, m_sMiniMapInfo.m_dxColor_Enemy, &(*vecAddTexIter));
    }
}

void KGCRealMiniMap::DrawUnitToBase(UNITINFO& kUnitInfo, float fBaseScale, DWORD dwBaseColor)
{
    float fTexWidth = 0.0f;
    float fTexHeight = 0.0f;

    if (kUnitInfo.m_kBaseTex.m_pTex)
    {
        fTexWidth = static_cast<float>(kUnitInfo.m_kBaseTex.m_pTex->GetWidth());
        fTexHeight = static_cast<float>(kUnitInfo.m_kBaseTex.m_pTex->GetHeight());
    }

    float fScale = OBJ_SIZE * fBaseScale * kUnitInfo.fScale * kUnitInfo.fStaticScale;
    float fX1 = CalcResolutionX(kUnitInfo.vPos.x);
    float fY1 = CalcResolutionY(kUnitInfo.vPos.y) - OBJ_SIZE;
    D3DXVECTOR4 vUV = kUnitInfo.m_kBaseTex.vUV;
    DWORD dwOutColor = (0xff000000 | D3DCOLOR_ARGB(kUnitInfo.iAlpha, 0, 0, 0));
    DWORD dwOriColor = dwBaseColor;
    if (NULL == kUnitInfo.m_kBaseTex.m_pTex)
    {
        g_pGCDeviceManager->DrawInScreen(NULL,
            (fX1 - (fScale + 1)), (fY1 - (fScale + 1)), (fX1 + (fScale + 1)), (fY1 + (fScale + 1)), 0.0f, 0.0f, 0.0f, 0.0f, false, 10.0f, dwOutColor, false);
    }
    else
        dwOriColor = kUnitInfo.m_kBaseTex.dwColor;

    if (0 < kUnitInfo.kColorAniInfo.iKeepFrame)
        dwOriColor = kUnitInfo.dwAniColor;

    DWORD dwInColor = (dwOriColor | D3DCOLOR_ARGB(kUnitInfo.iAlpha, 0, 0, 0));
    g_pGCDeviceManager->DrawInScreen(kUnitInfo.m_kBaseTex.m_pTex,
        (fX1 - fScale), (fY1 - fScale), (fX1 + fScale), (fY1 + fScale),
        vUV.x / fTexWidth, vUV.y / fTexHeight, vUV.z / fTexWidth, vUV.w / fTexHeight, false, 10.0f, dwInColor, false);
}

void KGCRealMiniMap::DrawUnitToAddTexture(UNITINFO& kUnitInfo, float fBaseScale, DWORD dwBaseColor, TEXTUREINFO* pAddTex)
{
    if ((NULL == pAddTex) || (NULL == pAddTex->m_pTex))
        return;

    float fTexWidth = static_cast<float>(pAddTex->m_pTex->GetWidth());
    float fTexHeight = static_cast<float>(pAddTex->m_pTex->GetHeight());

    float fScale = OBJ_SIZE * fBaseScale * kUnitInfo.fScale * kUnitInfo.fStaticScale;
    float fX1 = CalcResolutionX(kUnitInfo.vPos.x);
    float fY1 = CalcResolutionY(kUnitInfo.vPos.y) - OBJ_SIZE;
    D3DXVECTOR4 vUV = pAddTex->vUV;
    DWORD dwOriColor = pAddTex->dwColor;

    if (0 < kUnitInfo.kColorAniInfo.iKeepFrame)
        dwOriColor = kUnitInfo.dwAniColor;

    DWORD dwInColor = (dwOriColor | D3DCOLOR_ARGB(kUnitInfo.iAlpha, 0, 0, 0));
    g_pGCDeviceManager->DrawInScreen(pAddTex->m_pTex,
        (fX1 - fScale), (fY1 - fScale), (fX1 + fScale), (fY1 + fScale),
        vUV.x / fTexWidth, vUV.y / fTexHeight, vUV.z / fTexWidth, vUV.w / fTexHeight, false, 10.0f, dwInColor, false);
}

// 프레임무브에서 유닛들의 위치를 갱신합니다.
void KGCRealMiniMap::FrameMoveInEnabledState(void)
{
    // 갱신 타임처리
    if (0 != (g_pGameBoard->GetPlayTimeInFrame() % m_sMiniMapInfo.m_iRenewFrame))
        return;

    UNITINFO vUnitInfo;
    int iMyPlayer = g_MyD3D->Get_MyPlayer();
    if (g_MyD3D->IsPlayerIndex(iMyPlayer))
    {
        PLAYER* pMyPlayer = g_MyD3D->MyPlayer[iMyPlayer];

        // 내 캐릭터 체크
        if (-1 == m_vMyPos.iType)
        {
            m_vMyPos.Init();
            m_vMyPos.iType = TYPE_PLAYER;
            m_vMyPos.iIndex = iMyPlayer;
            m_vMyPos.vPos = ConverttingCoordinates(pMyPlayer->vPos.x, pMyPlayer->vPos.y);
            if (0 < static_cast<int>(strlen(m_sMiniMapInfo.m_kTexInfo_My.m_strFileName.c_str())))
            {
                m_vMyPos.m_kBaseTex.m_pTex = g_pGCDeviceManager2->CreateTexture(m_sMiniMapInfo.m_kTexInfo_My.m_strFileName.c_str());
                m_vMyPos.m_kBaseTex.vUV = m_sMiniMapInfo.m_kTexInfo_My.m_vUV;
                m_vMyPos.m_kBaseTex.dwColor = m_sMiniMapInfo.m_kTexInfo_My.m_dwColor;
            }
        }
        else
        {
            m_vMyPos.vPos = ConverttingCoordinates(pMyPlayer->vPos.x, pMyPlayer->vPos.y);
        }

        // 플레이어 체크
        for (int iPlayerLoop = 0; iPlayerLoop < MAX_PLAYER_NUM; ++iPlayerLoop)
        {
            PLAYER* pPlayer = g_MyD3D->MyPlayer[iPlayerLoop];
            std::map<std::pair<int, int>, UNITINFO>::iterator mapAllyIter = m_mapAllyPos.find(make_pair(TYPE_PLAYER, iPlayerLoop));
            std::map<std::pair<int, int>, UNITINFO>::iterator mapEnemyIter = m_mapEnemyPos.find(make_pair(TYPE_PLAYER, iPlayerLoop));

            bool bDeleteAndContinue = false;
            if (false == pPlayer->m_kUserInfo.bLive)
                bDeleteAndContinue = true;

            else if (pPlayer->m_cLife == 0)
                bDeleteAndContinue = true;

            else if (pPlayer->m_ePlayerState == PS_DEAD)
                bDeleteAndContinue = true;

            else if (pPlayer->IsLocalPlayer())
                bDeleteAndContinue = true;

            if (bDeleteAndContinue)
            {
                if (mapAllyIter != m_mapAllyPos.end())     m_mapAllyPos.erase(mapAllyIter);
                if (mapEnemyIter != m_mapEnemyPos.end())   m_mapEnemyPos.erase(mapEnemyIter);
                continue;
            }

            vUnitInfo.Init();
            vUnitInfo.iType = TYPE_PLAYER;
            vUnitInfo.iIndex = pPlayer->m_iPlayerIndex;
            vUnitInfo.vPos = ConverttingCoordinates(pPlayer->vPos.x, pPlayer->vPos.y);

            if (g_MyD3D->IsSameTeam(pMyPlayer->m_iPlayerIndex, pPlayer->m_iPlayerIndex))
            {
                vUnitInfo.fScale = m_sMiniMapInfo.m_fScale_Ally;
                if (mapAllyIter == m_mapAllyPos.end())
                {
                    std::map<std::pair<int, int>, UNITINFO>::iterator insIter = m_mapAllyPos.insert(m_mapAllyPos.end(), std::make_pair(std::make_pair(vUnitInfo.iType, vUnitInfo.iIndex), vUnitInfo));
                    if (0 < static_cast<int>(strlen(m_sMiniMapInfo.m_kTexInfo_Ally.m_strFileName.c_str())))
                    {
                        insIter->second.m_kBaseTex.m_pTex = g_pGCDeviceManager2->CreateTexture(m_sMiniMapInfo.m_kTexInfo_Ally.m_strFileName.c_str());
                        insIter->second.m_kBaseTex.vUV = m_sMiniMapInfo.m_kTexInfo_Ally.m_vUV;
                        insIter->second.m_kBaseTex.dwColor = m_sMiniMapInfo.m_kTexInfo_Ally.m_dwColor;
                    }
                }
                else
                {
                    mapAllyIter->second.vPos = vUnitInfo.vPos;
                }
            }
            else
            {
                vUnitInfo.fScale = m_sMiniMapInfo.m_fScale_Enemy;
                if (mapEnemyIter == m_mapEnemyPos.end())
                {
                    std::map<std::pair<int, int>, UNITINFO>::iterator insIter = m_mapEnemyPos.insert(m_mapEnemyPos.end(), std::make_pair(std::make_pair(vUnitInfo.iType, vUnitInfo.iIndex), vUnitInfo));
                    if (0 < static_cast<int>(strlen(m_sMiniMapInfo.m_kTexInfo_Enemy.m_strFileName.c_str())))
                    {
                        insIter->second.m_kBaseTex.m_pTex = g_pGCDeviceManager2->CreateTexture(m_sMiniMapInfo.m_kTexInfo_Enemy.m_strFileName.c_str());
                        insIter->second.m_kBaseTex.vUV = m_sMiniMapInfo.m_kTexInfo_Enemy.m_vUV;
                        insIter->second.m_kBaseTex.dwColor = m_sMiniMapInfo.m_kTexInfo_Enemy.m_dwColor;
                    }
                }
                else
                {
                    mapEnemyIter->second.vPos = vUnitInfo.vPos;
                }
            }
        }

        // 몬스터 체크
        for (MAP_INT_PMONSTER::iterator mit = g_kMonsterManager.m_mapMonster.begin(); mit != g_kMonsterManager.m_mapMonster.end(); ++mit) {
            MONSTER* pMonster = mit->second;

            if (pMonster == NULL)
                continue;

            std::map<std::pair<int, int>, UNITINFO>::iterator mapMonIter;
            std::map<std::pair<int, int>, UNITINFO>::iterator mapAllyIter = m_mapAllyPos.find(make_pair(TYPE_MONSTER, mit->first));
            std::map<std::pair<int, int>, UNITINFO>::iterator mapEnemyIter = m_mapEnemyPos.find(make_pair(TYPE_MONSTER, mit->first));

            bool bDeleteAndContinue = false;
            if (false == pMonster->IsLive())
                bDeleteAndContinue = true;

            if (true == pMonster->IsDie())
                bDeleteAndContinue = true;

            if (NPC_NONE != pMonster->m_eNPC)
                bDeleteAndContinue = true;

            if (pMonster->m_bMonsterTarget == false)
                bDeleteAndContinue = true;

            if ((0 > pMonster->GetMonsterType()) ||
                (pMonster->GetMonsterType() >= MONSTER_MODEL_NUM) ||
                (pMonster->GetMonsterType() >= int(g_kMonsterManager.m_vecMonsterTable.size())))
                bDeleteAndContinue = true;

            const SMonsterTemplate& sMonsterTable = g_kMonsterManager.m_vecMonsterTable[pMonster->GetMonsterType()];
            if (sMonsterTable.bNoRealMiniMap)
                bDeleteAndContinue = true;

            if (bDeleteAndContinue)
            {
                if (mapAllyIter != m_mapAllyPos.end())     m_mapAllyPos.erase(mapAllyIter);
                if (mapEnemyIter != m_mapEnemyPos.end())   m_mapEnemyPos.erase(mapEnemyIter);
                continue;
            }

            vUnitInfo.Init();
            vUnitInfo.iType = TYPE_MONSTER;
            vUnitInfo.iIndex = pMonster->m_iMonsterIndex;
            vUnitInfo.vPos = ConverttingCoordinates(pMonster->GetX(), pMonster->GetY());
            vUnitInfo.SetScaleAniInfo(pMonster->m_kRealMiniMapScaleAniInfo);
            pMonster->m_kRealMiniMapScaleAniInfo.Init();
            vUnitInfo.SetAlphaAniInfo(pMonster->m_kRealMiniMapAlphaAniInfo);
            pMonster->m_kRealMiniMapAlphaAniInfo.Init();
            vUnitInfo.SetColorAniInfo(pMonster->m_kRealMiniMapColorAniInfo);
            pMonster->m_kRealMiniMapColorAniInfo.Init();

            if (1.0f != pMonster->m_fRealMiniMapStaticScale)
            {
                vUnitInfo.SetStaticScale(pMonster->m_fRealMiniMapStaticScale);
                pMonster->m_fRealMiniMapStaticScale = 1.0f;
            }

            if (-1 != pMonster->m_iBaseRender)
            {
                vUnitInfo.SetNoBaseRender((0 == pMonster->m_iBaseRender) ? true : false);
                pMonster->m_iBaseRender = -1;
            }

            if (g_MyD3D->IsSameTeam_PlayerAndMonster(pMyPlayer->m_iPlayerIndex, pMonster->m_iMonsterIndex))
            {
                vUnitInfo.fScale = m_sMiniMapInfo.m_fScale_Ally;
                if (mapAllyIter == m_mapAllyPos.end())
                {
                    mapAllyIter = m_mapAllyPos.insert(m_mapAllyPos.end(), std::make_pair(std::make_pair(vUnitInfo.iType, vUnitInfo.iIndex), vUnitInfo));
                    if (0 < static_cast<int>(strlen(m_sMiniMapInfo.m_kTexInfo_Ally.m_strFileName.c_str())))
                    {
                        mapAllyIter->second.m_kBaseTex.m_pTex = g_pGCDeviceManager2->CreateTexture(m_sMiniMapInfo.m_kTexInfo_Ally.m_strFileName.c_str());
                        mapAllyIter->second.m_kBaseTex.vUV = m_sMiniMapInfo.m_kTexInfo_Ally.m_vUV;
                        mapAllyIter->second.m_kBaseTex.dwColor = m_sMiniMapInfo.m_kTexInfo_Ally.m_dwColor;
                    }
                }
                else
                {
                    mapAllyIter->second.vPos = vUnitInfo.vPos;
                    mapAllyIter->second.SetScaleAniInfo(vUnitInfo.kScaleAniInfo);
                    mapAllyIter->second.SetAlphaAniInfo(vUnitInfo.kAlphaAniInfo);
                    mapAllyIter->second.SetColorAniInfo(vUnitInfo.kColorAniInfo);
                }
                mapMonIter = mapAllyIter;
            }
            else
            {
                vUnitInfo.fScale = m_sMiniMapInfo.m_fScale_Enemy;
                if (mapEnemyIter == m_mapEnemyPos.end())
                {
                    mapEnemyIter = m_mapEnemyPos.insert(m_mapEnemyPos.end(), std::make_pair(std::make_pair(vUnitInfo.iType, vUnitInfo.iIndex), vUnitInfo));
                    if (0 < static_cast<int>(strlen(m_sMiniMapInfo.m_kTexInfo_Enemy.m_strFileName.c_str())))
                    {
                        mapEnemyIter->second.m_kBaseTex.m_pTex = g_pGCDeviceManager2->CreateTexture(m_sMiniMapInfo.m_kTexInfo_Enemy.m_strFileName.c_str());
                        mapEnemyIter->second.m_kBaseTex.vUV = m_sMiniMapInfo.m_kTexInfo_Enemy.m_vUV;
                        mapEnemyIter->second.m_kBaseTex.dwColor = m_sMiniMapInfo.m_kTexInfo_Enemy.m_dwColor;
                    }
                }
                else
                {
                    mapEnemyIter->second.vPos = vUnitInfo.vPos;
                    mapEnemyIter->second.SetScaleAniInfo(vUnitInfo.kScaleAniInfo);
                    mapEnemyIter->second.SetAlphaAniInfo(vUnitInfo.kAlphaAniInfo);
                    mapEnemyIter->second.SetColorAniInfo(vUnitInfo.kColorAniInfo);
                }
                mapMonIter = mapEnemyIter;
            }

            if (false == pMonster->m_kRealMiniMapAccTexInfo.m_strFileName.empty())
            {
                TEXTUREINFO m_kBaseTex;
                m_kBaseTex.dwColor = pMonster->m_kRealMiniMapAccTexInfo.m_dwColor;
                m_kBaseTex.vUV = pMonster->m_kRealMiniMapAccTexInfo.m_vUV;
                m_kBaseTex.m_pTex = g_pGCDeviceManager2->CreateTexture(pMonster->m_kRealMiniMapAccTexInfo.m_strFileName.c_str());
                mapMonIter->second.m_vecAddTex.push_back(m_kBaseTex);
                pMonster->m_kRealMiniMapAccTexInfo.Init();
            }
        }

        // 기타 오브젝트 체크
        std::map<std::pair<int, DWORD>, D3DCOLOR>::const_iterator mapOBJIter = m_sMiniMapInfo.m_mapETCColor.begin();
        for (; mapOBJIter != m_sMiniMapInfo.m_mapETCColor.end(); ++mapOBJIter)
        {
            switch (mapOBJIter->first.first)
            {
            case MINIMAP_ETC_SYNCOBJECT:
            {
                const std::map<DWORD, SyncObjectInstance*>* pMapSyncObj = SiKGCSyncObjectManager()->GetObjectInstancePool();
                if (pMapSyncObj)
                {
                    m_mapETCPos.clear();

                    std::map<DWORD, SyncObjectInstance*>::const_iterator mapSyncIter = pMapSyncObj->begin();
                    for (; mapSyncIter != pMapSyncObj->end(); ++mapSyncIter)
                    {
                        if (mapSyncIter->second && mapSyncIter->second->m_iObjectID == mapOBJIter->first.second)
                        {
                            m_mapETCPos.insert(std::make_pair(ConverttingCoordinates(mapSyncIter->second->m_vPos.x + 1.0f, mapSyncIter->second->m_vPos.y + 0.5f), mapOBJIter->second));
                        }
                    }
                }
            }
            break;
            }
        }

        // 적군수 출력
        if (m_pkStrEnemyNum)
        {
            std::wostringstream stm;
            stm << L"X" << int(m_mapEnemyPos.size());
            m_pkStrEnemyNum->SetText(stm.str());
        }
    }
}

float KGCRealMiniMap::Animation(float fNowValue, float fDefaultValue, ANIINFO& kAniInfo)
{
    float fReturnValue = fDefaultValue;
    if (0 < kAniInfo.iKeepFrame)
    {
        fNowValue += kAniInfo.fAniSpeed;
        if (fNowValue < kAniInfo.fMinLimit)
        {
            if (kAniInfo.bReturnAni)
            {
                fNowValue = kAniInfo.fMinLimit;
                kAniInfo.fAniSpeed *= -1;
            }
            else
                fNowValue = kAniInfo.fMaxLimit;

            if (-1 != kAniInfo.iSoundID)
                g_KDSound.Play(kAniInfo.iSoundID, 0, LATENCY_BUFFER_SIZE);
        }
        if (fNowValue > kAniInfo.fMaxLimit)
        {
            if (kAniInfo.bReturnAni)
            {
                fNowValue = kAniInfo.fMaxLimit;
                kAniInfo.fAniSpeed *= -1;
            }
            else
                fNowValue = kAniInfo.fMinLimit;

            if (-1 != kAniInfo.iSoundID)
                g_KDSound.Play(kAniInfo.iSoundID, 0, LATENCY_BUFFER_SIZE);
        }

        fReturnValue = fNowValue;
        kAniInfo.iKeepFrame--;
    }

    return fReturnValue;
}

DWORD KGCRealMiniMap::ColorAnimation(DWORD dwNowValue, COLOR_ANIINFO& kColorInfo)
{
    DWORD dwReturnValue = 0;
    if (0 < kColorInfo.iKeepFrame)
    {
        ANIINFO kTempAniInfo;
        kTempAniInfo.iKeepFrame = 10;
        kTempAniInfo.bReturnAni = kColorInfo.bReturnAni;
        kTempAniInfo.fMinLimit = kColorInfo.vMinLimit.x;
        kTempAniInfo.fMaxLimit = kColorInfo.vMaxLimit.x;
        kTempAniInfo.fAniSpeed = kColorInfo.vAniSpeed.x;
        int iR = static_cast<int>(Animation(static_cast<float>(GetBValue(dwNowValue)), 0.0f, kTempAniInfo));// R보간

        kTempAniInfo.fMinLimit = kColorInfo.vMinLimit.y;
        kTempAniInfo.fMaxLimit = kColorInfo.vMaxLimit.y;
        kTempAniInfo.fAniSpeed = kColorInfo.vAniSpeed.y;
        int iG = static_cast<int>(Animation(static_cast<float>(GetGValue(dwNowValue)), 0.0f, kTempAniInfo));// G보간

        kTempAniInfo.fMinLimit = kColorInfo.vMinLimit.z;
        kTempAniInfo.fMaxLimit = kColorInfo.vMaxLimit.z;
        kTempAniInfo.fAniSpeed = kColorInfo.vAniSpeed.z;
        int iB = static_cast<int>(Animation(static_cast<float>(GetRValue(dwNowValue)), 0.0f, kTempAniInfo));// B보간

        dwReturnValue = D3DCOLOR_ARGB(0, iR, iG, iB);
        kColorInfo.iKeepFrame--;
    }

    return dwReturnValue;
}

void KGCRealMiniMap::FrameMoveToAnimation()
{
    std::map<std::pair<int, int>, UNITINFO>::iterator mapEnemyIter = m_mapEnemyPos.begin();
    for (; mapEnemyIter != m_mapEnemyPos.end(); ++mapEnemyIter)
    {
        mapEnemyIter->second.iAlpha = static_cast<int>(Animation(static_cast<float>(mapEnemyIter->second.iAlpha), 255.0f, mapEnemyIter->second.kAlphaAniInfo));
        mapEnemyIter->second.fScale = Animation(mapEnemyIter->second.fScale, m_sMiniMapInfo.m_fScale_Enemy, mapEnemyIter->second.kScaleAniInfo);
        mapEnemyIter->second.dwAniColor = ColorAnimation(mapEnemyIter->second.dwAniColor, mapEnemyIter->second.kColorAniInfo);
    }

    std::map<std::pair<int, int>, UNITINFO>::iterator mapAllyIter = m_mapAllyPos.begin();
    for (; mapAllyIter != m_mapAllyPos.end(); ++mapAllyIter)
    {
        mapAllyIter->second.iAlpha = static_cast<int>(Animation(static_cast<float>(mapAllyIter->second.iAlpha), 255.0f, mapAllyIter->second.kAlphaAniInfo));
        mapAllyIter->second.fScale = Animation(mapAllyIter->second.fScale, m_sMiniMapInfo.m_fScale_Ally, mapAllyIter->second.kScaleAniInfo);
        mapAllyIter->second.dwAniColor = ColorAnimation(mapAllyIter->second.dwAniColor, mapAllyIter->second.kColorAniInfo);
    }
}

/*  2012.04.26, shmhlove, 이 함수에서 발판블럭들을 미니맵에 출력할 블럭 정보로 만듭니다.
    스테이지가 시작되면
    모든 발판블럭을 월드좌표계에서 스크린좌표계로 이동시킨 후
    실시간미니맵UI 크기에 맞게 스케일을 컨버팅 한 후
    출력블럭을 만드는 함수입니다.

    블럭 하나에 하나의 출력블럭을 만들어버리면 DrawInScreen함수호출이 너무 많아지므로
    X축으로 이어진 블럭들을 묶어 하나의 출력블럭으로 구성했습니다.
*/
void KGCRealMiniMap::SetFootHoldInfo()
{
    int iBlockWidth = SiKGCFantasticMap()->GetBlockWidth();
    int iBlockHeight = SiKGCFantasticMap()->GetBlockHeight();
    KGCFootHold** ppFHWorld = SiKGCFantasticMap()->GetFHWorld();

    if (ppFHWorld)
    {
        m_vecFootHoldPos.clear();
        D3DXVECTOR4 v4BlockPos(0.0f, 0.0f, 0.0f, 0.0f);
        for (int iHeightLoop = 0; iHeightLoop < iBlockHeight; ++iHeightLoop)
        {
            for (int iWidthLoop = 0; iWidthLoop < iBlockWidth; ++iWidthLoop)
            {
                if (ppFHWorld[iWidthLoop][iHeightLoop].GetFootHoldType() == KGCFootHold::FHT_INIT &&
                    ppFHWorld[iWidthLoop][iHeightLoop].GetFootHoldProp() != KGCFootHold::FHP_NO_ACTION)
                {
                    PushFoolHoldData(v4BlockPos);
                    continue;
                }

                if (v4BlockPos == D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))
                {
                    float fX = ppFHWorld[iWidthLoop][iHeightLoop].GetX();
                    float fY = ppFHWorld[iWidthLoop][iHeightLoop].GetY();
                    D3DXVECTOR3 vLeftTopPos = ConverttingCoordinates(fX, fY);
                    D3DXVECTOR3 vRightBottomPos = ConverttingCoordinates((fX + FOOTHOLDSIZE), (fY - FOOTHOLDSIZE));

                    v4BlockPos.x = vLeftTopPos.x;
                    v4BlockPos.y = vLeftTopPos.y;
                    v4BlockPos.z = vRightBottomPos.x;
                    v4BlockPos.w = vRightBottomPos.y;
                }
                else
                    v4BlockPos.z += CONVERT_WIDTH((FOOTHOLDSIZE * 400.0f));
            }
            PushFoolHoldData(v4BlockPos);
        }
    }
}

// 2012.04.26, shmhlove, 이 함수에서 출력블럭을 만듭니다.
void KGCRealMiniMap::PushFoolHoldData(D3DXVECTOR4& v4BlockPos)
{
    if (0.0f != v4BlockPos.x || 0.0f != v4BlockPos.y || 0.0f != v4BlockPos.z || 0.0f != v4BlockPos.w)
    {
        m_vecFootHoldPos.push_back(v4BlockPos);
        v4BlockPos = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

// 월드좌표계에서 스크린좌표계로 이동시킨 후 스케일 컨버팅합니다.
D3DXVECTOR2 KGCRealMiniMap::ConverttingCoordinates(float fWorldX, float fWorldY)
{
    D3DXVECTOR2 vConvertting;

    // 월드에서 스크린좌표계로
    float fScreenX = fWorldX * 400.0f;
    float fScreenY = fWorldY * 400.0f;

    // 스케일 컨버팅
    vConvertting.x = float(m_sMiniMapInfo.m_iPosUI_X + 5.0f) + CONVERT_WIDTH(fScreenX);
    vConvertting.y = float(m_sMiniMapInfo.m_iPosUI_Y) + CONVERT_HEIGHT(fScreenY);

    // 상하 반전
    vConvertting.y = ((m_sMiniMapInfo.m_iHeightUI + GAP_TAGANDBOX) + m_sMiniMapInfo.m_iPosUI_Y) + (m_sMiniMapInfo.m_iPosUI_Y - vConvertting.y);

    return vConvertting;
}

// 미니맵 정보를 셋팅합니다.
void KGCRealMiniMap::SetRealMiniMapInfo(SMiniMapInfo* pMiniMapInfo)
{
    if (pMiniMapInfo)
    {
        m_sMiniMapInfo = (*pMiniMapInfo);

        m_vQuestSize.x = SiKGCFantasticMap()->GetQuestWidth();
        m_vQuestSize.y = SiKGCFantasticMap()->GetQuestHeight();
        this->SetWindowPosX(static_cast<float>(m_sMiniMapInfo.m_iPosUI_X));
        this->SetWindowPosY(static_cast<float>((m_sMiniMapInfo.m_iPosUI_Y + GAP_TAGANDBOX)));
        {
            float fWidthRadio = m_sMiniMapInfo.m_iWidthUI / ORI_WIDTH;
            float fHeightRadio = (m_sMiniMapInfo.m_iHeightUI + GAP_TAGANDBOX) / ORI_HEIGHT;
            this->SetWidth(int(ORI_WIDTH * fWidthRadio));
            this->SetHeight(int(ORI_HEIGHT * fHeightRadio));
            m_pkBackGround->OnSetWidth(int(ORI_WIDTH * fWidthRadio));
            m_pkBackGround->OnSetHeight(int((ORI_HEIGHT - GAP_TAGANDBOX) * fHeightRadio));
        }

        SetFootHoldInfo();
    }
    else
        m_sMiniMapInfo.Init();

    m_vMyPos.Init();
    m_mapEnemyPos.clear();
    m_mapAllyPos.clear();
    m_mapETCPos.clear();
}