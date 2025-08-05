#include "stdafx.h"
#include "Replay.h"

#include "KDInput.h"
#include "Headup Display.h"
#include "Item.h"
#include "GCUI/GCGameOverlayUI.h"


#include "Monster.h"
#include "Stage.h"
#include "KGCGameBoard.h"
#include "zlib.h"

//
#include "GCStateMachine.h"

#include "KGCRoomManager.h"
#include "gcui/KGCDeathMatchScoreBoard.h"


Replay::Replay()
{
    m_fVersion = REPLAY_VERSION;
    m_bEndOfFile = false;
    m_iCurrState = RP_NONE;
    swprintf(m_pFileName, MAX_PATH, L"Replay\\SLOT00.first");

    m_hFile = NULL;
    m_iFeed = 0;
    m_eReplay_Speed = RS_NORMAL;

    m_iFileNum = 0;
    m_vecFileList.clear();

    m_bItemDataEntered = false;

    for( int i = 0 ; i < (int)m_iDeath.size() ; ++i )
    {
        m_iDeath[i] = 0;
    }
    for( int i = 0 ; i < (int)m_iKills.size() ; ++i )
    {
        m_iKills[i] = 0;
    }

    m_bUsePetSkill = false;
    m_bTagCharacter = false;
    m_bUpdateGameboard = false;

    memset( &m_TagMatchFrameTemp, -1, sizeof( m_TagMatchFrameTemp ) );
    memset( &m_GameBoardTemp, -1, sizeof( m_GameBoardTemp ) );
}

int Replay::GetFileNum()
{
    GetList();
    return m_iFileNum;
}

void Replay::GetList()
{
    HANDLE hSrch,hFile;
    WIN32_FIND_DATA wfd;
    BOOL bResult = TRUE;
    m_iFileNum = 0;
    m_vecFileList.clear();

    hSrch = FindFirstFile( L"Replay\\*.first" , &wfd );
    while(bResult && hSrch != INVALID_HANDLE_VALUE)
    {
        std::wstring FileName = L"Replay\\";
        FileName += wfd.cFileName;

        swprintf( m_pFileName, MAX_PATH, FileName.c_str());

        hFile = CreateFile(m_pFileName, GENERIC_READ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
        {
            hFile = NULL;
        }
        else
        {
            float fVersion;
            DWORD size;
            DWORD temp;
            //파일의 버전을 기록한다.
            ReadFile(hFile, &fVersion, sizeof(float),&temp,NULL);
            ReadFile(hFile, &size , sizeof(DWORD),&temp,NULL);
            
            if ( fVersion >= REPLAY_MINOR_VERSION && fVersion <= REPLAY_VERSION )
            {
                if ( temp >= sizeof(DWORD) )
                {
                    m_vecFileList.push_back(FileName); //등록한다.
                    m_iFileNum++;                    
                }
            }
        }
        CloseHandle(hFile);
        bResult = FindNextFile(hSrch , &wfd);
    }
    FindClose(hSrch);
}


void Replay::BlankToZero( WCHAR* pFileName )
{
    UINT iTexLen = (UINT)wcslen(pFileName);
    for( UINT i = 0 ; i < iTexLen ; ++i )
    {
        if( pFileName[i] == L' ' )
        {
            if( i != 10 )
                pFileName[i] = 48;
        }
    }
}

// 파일을 압축해서 저장한다.
bool Replay::Replay_Compress()
{
    char pBuffer[1024];
    DWORD dwSourceSize = 0;
    DWORD dwDestSize = 0;
    HANDLE hSource = NULL;
    HANDLE hDest = NULL;
    DWORD temp;

    // 읽을 파일 열기
    hSource = CreateFile( L"Replay.tmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if( hSource == INVALID_HANDLE_VALUE )
    {
        hSource = NULL;
        return false;
    }

    while(1)// 필요한 버퍼 사이즈를 알아내기 위해 한번 주욱 읽는다.
    {
        ReadFile(hSource, pBuffer, 1024, &temp, NULL);

        if(temp<1024)
        {
            dwSourceSize += temp;
            break;
        }
        dwSourceSize += 1024;
    }

    dwDestSize = (DWORD)((float)(dwSourceSize + 12) * 1.1f);
    // 18699
    // Size를 알아내었으니 버퍼를 만들자.
    BYTE* pSourceBuffer = new BYTE[dwSourceSize];
    BYTE* pDestBuffer = new BYTE[dwDestSize];

    // 소스 버퍼에 채워넣는다.
    SetFilePointer( hSource, 0, 0, 0 );

    ReadFile( hSource, pSourceBuffer, dwSourceSize, &temp, NULL );

    if( temp != dwSourceSize )
    {
        SAFE_DELETE_ARRAY(pSourceBuffer);
        SAFE_DELETE_ARRAY(pDestBuffer);
        return false; // 이전에 측정했던 사이즈보다 작으므로 실패
    }

    CloseHandle(hSource);

    int Result = compress2( pDestBuffer, &dwDestSize, pSourceBuffer, dwSourceSize, 9 );

    if(Result != Z_OK)
    {
        SAFE_DELETE_ARRAY(pSourceBuffer);
        SAFE_DELETE_ARRAY(pDestBuffer);
        return false;
    }

    hDest = CreateFile( m_pFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hDest == INVALID_HANDLE_VALUE )
    {
        hDest = NULL;

        SAFE_DELETE_ARRAY(pSourceBuffer);
        SAFE_DELETE_ARRAY(pDestBuffer);

        return false;
    }

    //파일의 첫 머리에 파일의 버전을 쓴다.
    m_fVersion = REPLAY_VERSION;
    WriteFile(hDest, &m_fVersion, sizeof(float), &temp, NULL);

    // 파일을 저장한다.
    WriteFile( hDest, &dwSourceSize, sizeof(DWORD), &temp, NULL );
    WriteFile( hDest, pDestBuffer, dwDestSize, &temp, NULL);

    if(temp != dwDestSize)
    {
        CloseHandle(hDest);

        SAFE_DELETE_ARRAY(pSourceBuffer);
        SAFE_DELETE_ARRAY(pDestBuffer);

        return false;
    }

    SAFE_DELETE_ARRAY(pSourceBuffer);
    SAFE_DELETE_ARRAY(pDestBuffer);
    CloseHandle(hDest);
    DeleteFile(L"Replay.tmp");
    return true;

}

// bool Replay::Replay_Uncompress()
// {
//     DWORD dwSource = 0;
//     DWORD dwDest = 0;
//     HANDLE hSource = NULL;
//     HANDLE hDest = NULL;
//     DWORD temp;
// 
//     // 읽을 파일 열기
//     hSource = CreateFile(m_pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//     if(hSource == INVALID_HANDLE_VALUE)
//     {
//         hSource = NULL;
//         return false;
//     }
// 
//     dwSource = GetFileSize(hSource,&dwSource);
// 
//     // 버전을 본다.
//     m_fVersion = 0;
//     ReadFile( hSource, &m_fVersion, sizeof(float), &temp, NULL );
// 
//     if( temp < sizeof(float) || m_fVersion != REPLAY_VERSION )
//     {
//         CloseHandle( hSource );
//         m_iCurrState = RP_NONE;
//         return false;
//     }
// 
//     ReadFile(hSource, &dwDest, sizeof(DWORD), &temp, NULL);
// 
//     dwSource -= sizeof(float) + /*sizeof(m_pFileInfo) +*/ sizeof(dwDest);
// 
//     // Size를 알아내었으니 버퍼를 만들자.
//     BYTE* pSourceBuffer = new BYTE[dwSource];
//     BYTE* pDestBuffer = new BYTE[dwDest];
// 
//     ReadFile(hSource, pSourceBuffer, dwSource, &temp, NULL);
// 
//     if( temp != dwSource )
//     {
//         SAFE_DELETE_ARRAY(pSourceBuffer);
//         SAFE_DELETE_ARRAY(pDestBuffer);
//         return false; // 이전에 측정했던 사이즈보다 작으므로 실패
//     }
// 
//     CloseHandle(hSource);
// 
//     int Result = Z_BUF_ERROR;
// 
//     while(Result == Z_BUF_ERROR)
//     {
//         Result = uncompress(pDestBuffer, &dwDest, pSourceBuffer, dwSource);
//         if( Result == Z_BUF_ERROR )
//         {
//             SAFE_DELETE_ARRAY(pDestBuffer);
//             dwDest *= 2;
//             pDestBuffer = new BYTE[dwDest];
//         }
//     }
// 
//     if(Result!=Z_OK)
//     {
//         SAFE_DELETE_ARRAY(pSourceBuffer);
//         SAFE_DELETE_ARRAY(pDestBuffer);
//         return false;
//     }
// 
//     hDest = CreateFile( L"Replay.tmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
// 
//     if(hDest == INVALID_HANDLE_VALUE)
//     {
//         hDest = NULL;
//         SAFE_DELETE_ARRAY(pSourceBuffer);
//         SAFE_DELETE_ARRAY(pDestBuffer);
//         return false;
//     }
// 
//     // 파일을 저장한다.
//     WriteFile(hDest, pDestBuffer, dwDest, &temp, NULL);
// 
//     if(temp != dwDest)
//     {
//         CloseHandle(hDest);
//         SAFE_DELETE_ARRAY(pSourceBuffer);
//         SAFE_DELETE_ARRAY(pDestBuffer);
//         return false;
//     }
// 
//     SAFE_DELETE_ARRAY(pSourceBuffer);
//     SAFE_DELETE_ARRAY(pDestBuffer);
//     CloseHandle(hDest);
// 
//     return true;
// }

void Replay::Set_PlayerData(int PlayerNum)
{
    if( !g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.bLive )
    {
        m_HeadData.m_pUserInfo[PlayerNum].m_bIsLive = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.bLive;
        return;
    }

    swprintf(m_HeadData.m_pUserInfo[PlayerNum].m_pPlayerName, 25, g_MyD3D->MyPlayer[PlayerNum]->GetStrUserName().c_str());
    m_HeadData.m_pUserInfo[PlayerNum].m_bIsLive = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.bLive;
    m_HeadData.m_pUserInfo[PlayerNum].m_bIsBlueTeam = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.IsBlueTeam();
    m_HeadData.m_pUserInfo[PlayerNum].m_fStartX = g_MyD3D->MyPlayer[PlayerNum]->vPos.x;
    m_HeadData.m_pUserInfo[PlayerNum].m_fStartY = g_MyD3D->MyPlayer[PlayerNum]->vPos.y;
    m_HeadData.m_pUserInfo[PlayerNum].m_bIsRight = g_MyD3D->MyPlayer[PlayerNum]->bIsRight;
    m_HeadData.m_pUserInfo[PlayerNum].m_iNowMotion = g_MyD3D->MyPlayer[PlayerNum]->uiMotion;
    m_HeadData.m_pUserInfo[PlayerNum].m_iSelectChar = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.GetCurrentChar().iCharType;

    for( int i = 0 ; i < (int)m_HeadData.m_pUserInfo[PlayerNum].m_aiTagCharType.size() ; ++i )
    {
        m_HeadData.m_pUserInfo[PlayerNum].m_aiTagCharType[i] = -1;
    }
    for( int i = 0 ; i < (int)m_HeadData.m_pUserInfo[PlayerNum].m_aiIndigoCharType.size() ; ++i )
    {
        m_HeadData.m_pUserInfo[PlayerNum].m_aiIndigoCharType[i] = -1;
    }

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
    {
        std::vector< int >& vecTagCharNum = g_MyD3D->MyPlayer[PlayerNum]->VecTagCharNum();
        std::vector< int >::reverse_iterator vitTag;
        int it = 0;
        for( vitTag = vecTagCharNum.rbegin(); vitTag != vecTagCharNum.rend(); ++vitTag, ++it )
            m_HeadData.m_pUserInfo[PlayerNum].m_aiTagCharType[it] = (*vitTag);
    }
    else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO )
    {
        for( int i = 0; i < 3; ++i )
            m_HeadData.m_pUserInfo[PlayerNum].m_aiIndigoCharType[i] = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.aiTagSlot[i];
    }

    for( int i = 0; i < GC_CHAR_NUM; i++ )
    {
        for( int j = 0 ; j < (int)g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo.size(); j++ )
        {
            if( i == g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[j].iCharType )
            {
                KUserItemVector& vecItem = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[j].vecItemInfo;
                for( int k = 0; k < (int)vecItem.size(); k++ )
                {
                    m_HeadData.m_pUserInfo[PlayerNum].m_ppItemNum[i][k] = vecItem[k].iItemID;
                    m_HeadData.m_pUserInfo[PlayerNum].m_ppItemPatten[i][k] = 0;
                }

                KUserItemVector& vecLookItem = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[j].vecLookInfo;
                for( int k = 0; k < (int)vecLookItem.size(); k++ )
                {
                    m_HeadData.m_pUserInfo[PlayerNum].m_ppLookItemNum[i][k] = vecLookItem[k].iItemID;                    
                }

            }
        }
    }

    // 펫정보 세팅
    for( int i = 0; i < GC_CHAR_NUM; i++ )
    {
        for( int j = 0; j < (int)g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo.size(); j++ )
        {
            if( i == g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[j].iCharType )
            {
                KPetInfo kPet = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[j].kPetInfo;

                m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwUID = kPet.m_dwUID;
                m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwID = kPet.m_dwID;
				m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwCostumeID = kPet.m_kCostume.m_dwID;
                m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_cPromotion = kPet.m_cPromotion;
                m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwEXP = kPet.m_dwEXP;
                m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_iLevel = kPet.m_iLevel;
                m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_iSatiation = kPet.m_iSatiation;
                swprintf(m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_strName, PETNICKNAME, kPet.m_strName.c_str());

                for( int k = 0; k < (int)kPet.m_vecEquipItem.size(); k++ )
                {
                    m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_akEquipItem[k] = kPet.m_vecEquipItem[k];
                }
            }
        }
    }

    for( int i = 0; i < GC_CHAR_NUM; i++ )
    {
        SCharInfo& kChar = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.GetCurrentChar(i);
        for( int j = 0; j < NUM_JOB_LEVEL; ++j )
        {
            for( int k = 0; k < (int)m_HeadData.m_pUserInfo[PlayerNum].m_pppSkill[i][j].size(); ++k )
            {
                m_HeadData.m_pUserInfo[PlayerNum].m_pppSkill[i][j][k] = -1;
            }

            std::vector<KSkillSlot>& vecSkill = kChar.m_mapEquipSkill[j];
            for( int k=0 ; k < MAX_EQUIP_NUM ; ++k ) { 
                if( k < static_cast<int>( vecSkill.size() ) ) { 
                    KSkillSlot& kSkill = vecSkill[ k ];
                    m_HeadData.m_pUserInfo[ PlayerNum ].m_pppSkill[ i ][ j ][ k ] = kSkill.m_nSkillID;
                }
            }
        }
    }
}

void Replay::Get_PlayerData(int PlayerNum)
{
    if (!m_HeadData.m_pUserInfo[PlayerNum].m_bIsLive)
        return;

    g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.ClearChar();
    for( int i=0 ; i<GC_CHAR_NUM ; ++i )
    {
        SCharInfo CharInfo;
        CharInfo.iCharType = i;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.AddChar( CharInfo );
    }

    g_MyD3D->MyPlayer[PlayerNum]->SetPlayerName( m_HeadData.m_pUserInfo[PlayerNum].m_pPlayerName, L"FFFFFF");
    g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.bLive = m_HeadData.m_pUserInfo[PlayerNum].m_bIsLive;
    g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.iTeam = m_HeadData.m_pUserInfo[PlayerNum].m_bIsBlueTeam;
    g_MyD3D->MyPlayer[PlayerNum]->vPos.x = m_HeadData.m_pUserInfo[PlayerNum].m_fStartX;
    g_MyD3D->MyPlayer[PlayerNum]->vPos.y = m_HeadData.m_pUserInfo[PlayerNum].m_fStartY;
    g_MyD3D->MyPlayer[PlayerNum]->bIsRight = m_HeadData.m_pUserInfo[PlayerNum].m_bIsRight;
    g_MyD3D->MyPlayer[PlayerNum]->uiMotion = m_HeadData.m_pUserInfo[PlayerNum].m_iNowMotion;
    g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.cIsNetmarblePCRoom = false;
    g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.clsUserBenefitType = false;
    g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.SetCurrentChar( m_HeadData.m_pUserInfo[PlayerNum].m_iSelectChar );
    g_MyD3D->MyPlayer[PlayerNum]->Calcualte_Point();
    g_MyD3D->MyPlayer[PlayerNum]->LoadPlayerTemplate();
    //↓이 함수 잘못짜여져 있어 수정했음 아래꺼 쓸일 없을것임 wony
    //g_MyD3D->MyPlayer[PlayerNum]->SetSkillType(0);

    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
    {
        for( int i = 0 ; i < TAG_CHAR_NUM ; ++i )
        {
            g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.aiTagSlot[i] = m_HeadData.m_pUserInfo[PlayerNum].m_aiTagCharType[i];
        }
    }
    else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO )
    {
        for( int i = 0 ; i < 3 ; ++i )
        {
           g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.aiTagSlot[i] = m_HeadData.m_pUserInfo[PlayerNum].m_aiIndigoCharType[i];
        }
    }
    else
    {
        for( int i = 0 ; i < 3 ; ++i )
        {
            g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.aiTagSlot[i] = -1;
        }
    }

    SUserItem ItemInfo;
    for( int i=0 ; i<GC_CHAR_NUM ; ++i )
    {
        for( int j=0 ; j<MAX_EQUIP_NUM ; ++j )
        {
            if( m_HeadData.m_pUserInfo[PlayerNum].m_ppItemNum[i][j] == -1 )
                break;

            ItemInfo.dwItemUID = 0;
            ItemInfo.iItemID = m_HeadData.m_pUserInfo[PlayerNum].m_ppItemNum[i][j];
            g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].vecItemInfo.push_back( ItemInfo );
        }
    }

    for( int i=0 ; i<GC_CHAR_NUM ; ++i )
    {
        for( int j=0 ; j<MAX_EQUIP_NUM ; ++j )
        {
            if( m_HeadData.m_pUserInfo[PlayerNum].m_ppLookItemNum[i][j] == -1 )
                break;

            ItemInfo.dwItemUID = 0;
            ItemInfo.iItemID = m_HeadData.m_pUserInfo[PlayerNum].m_ppLookItemNum[i][j];
            g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].vecLookInfo.push_back( ItemInfo );
        }
    }

    if(g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH || g_MyD3D->MyStg->GetStage() == GC_GS_BABEL_OF_X_MAS || g_MyD3D->MyStg->GetStage() == GC_GS_VALLEY_OF_OATH_REVERS )
    {
        g_MyD3D->MyPlayer[PlayerNum]->m_cLife = 5;
    }
    else if( SiKGCRoomManager()->GetGameModeCategory() != GC_GMC_TAG_MATCH )
    {
        g_MyD3D->MyPlayer[PlayerNum]->m_cLife = 3;
    }

    g_MyD3D->MyHead->Old_Life[PlayerNum] = g_MyD3D->MyHead->Change_Life[PlayerNum] = g_MyD3D->MyPlayer[PlayerNum]->m_cLife;

    // 펫정보 세팅
    for( int i = 0; i < GC_CHAR_NUM; i++ )
    {
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_dwUID = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwUID;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_dwID = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwID;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_cPromotion = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_cPromotion;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_dwEXP = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_dwEXP;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_iLevel = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_iLevel;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_iSatiation = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_iSatiation;
        g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_strName = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_strName;

        for( int k = 0; k < PET_EQUIP_ITEM_NUM; k++ )
        {
            if( m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_akEquipItem[k].m_dwUID != 0 )
            {
                KSimpleItem kSimpleItem = m_HeadData.m_pUserInfo[PlayerNum].m_akPet[i].m_akEquipItem[k];

                g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.vecCharInfo[i].kPetInfo.m_vecEquipItem.push_back( kSimpleItem );
            }
        }
    }

    for( int i = 0; i < GC_CHAR_NUM; i++ )
    {
        SCharInfo& kChar = g_MyD3D->MyPlayer[PlayerNum]->m_kUserInfo.GetCurrentChar(i);
        for( int j = 0; j < NUM_JOB_LEVEL; ++j )
        {
            std::vector<KSkillSlot>& vecSkill = kChar.m_mapEquipSkill[j];
            for( int k = 0; k < MAX_EQUIP_NUM; ++k )
            {
                KSkillSlot kSlot;
                kSlot.m_nSlotID = -1;
                kSlot.m_nSkillID = m_HeadData.m_pUserInfo[PlayerNum].m_pppSkill[i][j][k];
                if( kSlot.m_nSkillID == -1 )
                    break;
                vecSkill.push_back( kSlot );
            }
        }
    }
}

void Replay::SetDummyReplayHeadData()
{
    // 파일 헤더를 저장한다.
    m_HeadData.Init();
    m_HeadData.m_sGameModeCategory = (short)SiKGCRoomManager()->GetGameModeCategory();
    m_HeadData.m_sGameMode = (short)SiKGCRoomManager()->GetGameMode();
    m_HeadData.m_sSubGameMode = (short)SiKGCRoomManager()->GetSubGameMode();
    m_HeadData.m_ucStage = SiKGCRoomManager()->GetRoomInfo().ucStage;
    m_HeadData.m_iStory = g_MyD3D->Story;

    for ( int i = 0 ; i < MAX_NUM_MON_SLOT ; i++ )
    {
        m_HeadData.m_sMonsterSlot[i] = -1;
    }

    for( int i = 0 ; i < MAX_PLAYER_NUM; i++)
    {
        Set_PlayerData(i);
    }
}

bool Replay::Save_HeadData()
{
    if(m_iCurrState != RP_NONE)
        return false;

    if ( m_hFile ) 
        return false;

    CreateDirectoryA("./Replay",NULL);
    // 저장용 파일 열기
    m_hFile = CreateFile(L"Replay.tmp",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

    if(m_hFile == INVALID_HANDLE_VALUE)
    {
        m_hFile = NULL;
        m_iCurrState = RP_NONE;
        return false;
    }

    SetDummyReplayHeadData();

    DWORD temp;
    WriteFile(m_hFile, &m_HeadData, sizeof(Replay_Head_Data), &temp, NULL);

    m_iFeed=0;
    if(temp==sizeof(Replay_Head_Data))
    {
        m_iCurrState = RP_RECORD;
        return true;
    }
    else
    {
        m_iCurrState = RP_NONE;
        return false;
    }
}

void Replay::SetPlayerFrame( Replay_PlayerFrameData& PlayerFrame, int iPlayerIndex )
{
    PlayerFrame.MyNumber =iPlayerIndex;
    PlayerFrame.Life = g_MyD3D->MyPlayer[iPlayerIndex]->m_cLife;
    PlayerFrame.HP = g_MyD3D->MyPlayer[iPlayerIndex]->GetHP();
    PlayerFrame.Now_Motion = g_MyD3D->MyPlayer[iPlayerIndex]->uiMotion;
    PlayerFrame.Frame = g_MyD3D->MyPlayer[iPlayerIndex]->cFrame;
    PlayerFrame.Body_Angle = g_MyD3D->MyPlayer[iPlayerIndex]->Body_Angle;
    PlayerFrame.IsRight = g_MyD3D->MyPlayer[iPlayerIndex]->bIsRight;
    PlayerFrame.x = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.x;
    PlayerFrame.y = g_MyD3D->MyPlayer[iPlayerIndex]->vPos.y;
    PlayerFrame.Super = g_MyD3D->MyPlayer[iPlayerIndex]->Super;
    //PlayerFrame.ucNotSpecialSuper = unsigned char( g_MyD3D->MyPlayer[iPlayerIndex]->GetNotSpecialSuper() );
    PlayerFrame.Shield = g_MyD3D->MyPlayer[iPlayerIndex]->Shield;
    PlayerFrame.iKills = g_pGameBoard->m_pStartingMember[iPlayerIndex].How_Kill;
    PlayerFrame.iDeaths = g_pGameBoard->m_pStartingMember[iPlayerIndex].How_Die; //아레나 배틀용-죽은 수
    PlayerFrame.m_iSelectChar = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentChar().iCharType;
    PlayerFrame.m_bRender = g_MyD3D->MyPlayer[iPlayerIndex]->m_bRender;
    PlayerFrame.MetamorphosisForm = g_MyD3D->MyPlayer[iPlayerIndex]->MetamorphosisForm;
    PlayerFrame.m_bRenderWeapon = g_MyD3D->MyPlayer[iPlayerIndex]->RenderWeapon();
    PlayerFrame.m_bRenderMergeWeapon = g_MyD3D->MyPlayer[iPlayerIndex]->RenderMergeWeapon();
    PlayerFrame.SetSlashOnOff( g_MyD3D->MyPlayer[iPlayerIndex]->DrawSlashLeft(), g_MyD3D->MyPlayer[iPlayerIndex]->DrawSlashLegLeft(), &PlayerFrame.m_cDrawSlashLeft );
    PlayerFrame.SetSlashOnOff( g_MyD3D->MyPlayer[iPlayerIndex]->DrawSlashRight(), g_MyD3D->MyPlayer[iPlayerIndex]->DrawSlashLegRight(), &PlayerFrame.m_cDrawSlashRight );
}

void Replay::SetMonsterFrame( Replay_MonsterFrameData& MonsterFrame, int iMonIndex, int RIndex )
{
	MONSTER* pMonster = g_kMonsterManager.GetMonster(iMonIndex);
	if( pMonster == NULL) return;

    MonsterFrame.MyNumber = iMonIndex;
    MonsterFrame.m_fAtk_Point = pMonster->m_fAttackPoint;
    //MonsterFrame.m_fDef_Point = pMonster->m_fDef_Point;
    MonsterFrame.Now_Monster = pMonster->m_iMonsterType;// Now_Monster;
    MonsterFrame.HP = pMonster->m_fHP;

    MonsterFrame.IsRight = pMonster->m_abIsRight[RIndex];
    MonsterFrame.Now_Motion = pMonster->m_aiMotion[RIndex];
    MonsterFrame.Frame = pMonster->m_aiFrame[RIndex];
    MonsterFrame.x = pMonster->m_afX[RIndex];
    MonsterFrame.y = pMonster->m_afY[RIndex];

    MonsterFrame.Render_IsRight = pMonster->m_abIsRight[0];
    MonsterFrame.Render_Now_Motion= pMonster->m_aiMotion[0];
    MonsterFrame.Render_Frame = pMonster->m_aiFrame[0];
    MonsterFrame.Render_x = pMonster->m_afX[0];
    MonsterFrame.Render_y = pMonster->m_afY[0];
}

void Replay::MakePlayerFrameData( bool bReplay )
{
    //// [8/3/2007 breadceo] Replay 가 아닌곳에서 불렀는데 RP_RECORD 상태이면 계산된 상태임
    //if( bReplay == false && m_iCurrState == RP_RECORD )
    //    return;

    // 현재 플레이어 수를 센다.
    Num_Player[m_iFeed] = 0;

    for( int i = 0 ; i < MAX_PLAYER_NUM ; ++i )
    {
        bool bLifeCheck;
        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
            bLifeCheck = (g_MyD3D->MyPlayer[i]->GetNextTagChar() != GC_CHAR_INVALID);
        else
            bLifeCheck = (g_MyD3D->MyPlayer[i]->m_cLife > 0);

        if(g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive && bLifeCheck)
        {
            Replay_PlayerFrameData PlayerFrame;
            PlayerFrame.Init();
            SetPlayerFrame( PlayerFrame, i );
            m_PlayerFrame[m_iFeed][Num_Player[m_iFeed]] = PlayerFrame;
            Num_Player[m_iFeed]++;
        }
    }
}

void Replay::MakeMonsterFrameData( bool bReplay )
{
    //// [8/3/2007 breadceo] Replay 가 아닌곳에서 불렀는데 RP_RECORD 상태이면 계산된 상태임
    //if( bReplay == false && m_iCurrState == RP_RECORD )
    //    return;

    // 현재 몬스터 수를 센다.
    Num_Monster[m_iFeed] = 0;
    int RIndex = LATENCY+SHOWLATENCY-1;

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		if( mit->second->m_bLive )
		{
			Replay_MonsterFrameData MonsterFrame;
			MonsterFrame.Init();
			SetMonsterFrame( MonsterFrame, mit->first, RIndex );
			m_MonsterFrame[m_iFeed][Num_Monster[m_iFeed]]=MonsterFrame;
			Num_Monster[m_iFeed]++;
		}
	}

}

void Replay::MakeReplayChatData( bool bReplay )
{
    // [8/3/2007 breadceo] Replay 가 아닌곳에서 불렀는데 RP_RECORD 상태이면 계산된 상태임
    //if( bReplay == false && m_iCurrState == RP_RECORD )
    //    return;

    if ( g_pkGameOverlayUI->ReplayChatEntered() )
    {
        Replay_ChatFrameData ChatFrame;
        ChatFrame.m_TimeFrame = m_iFeed; //채팅이 들어온 순간.

        std::wstring strText = L"";
        strText = g_pkGameOverlayUI->GetChatMsg();
        _snwprintf(ChatFrame.m_strChatData , 300, L"%s", strText.c_str() ); //문제가 될수도 있음
        m_ChatFrame[m_iFeed] = ChatFrame;
    }
    else
    {
        m_ChatFrame[m_iFeed].m_TimeFrame = -1;
        swprintf(m_ChatFrame[m_iFeed].m_strChatData, 300, L"");
    }
}

void Replay::MakeItemData( bool bReplay )
{
    //// [8/3/2007 breadceo] Replay 가 아닌곳에서 불렀는데 RP_RECORD 상태이면 계산된 상태임
    //if( bReplay == false && m_iCurrState == RP_RECORD )
    //    return;

    if ( m_bItemDataEntered )
    {
        for( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
        {
            m_ItemFrame[m_iFeed].Eff_Item[i].cWhat = m_ItemFrameTemp.Eff_Item[i].cWhat;
            m_ItemFrame[m_iFeed].Eff_Item[i].sTime = m_ItemFrameTemp.Eff_Item[i].sTime;
        }
        m_ItemFrame[m_iFeed].m_cUser = m_ItemFrameTemp.m_cUser;
        m_ItemFrame[m_iFeed].m_TimeFrame = m_iFeed;
        m_bItemDataEntered = false;
    }
    else
    {
        ZeroMemory( &m_ItemFrame[m_iFeed] , sizeof(Replay_ItemFrameData) );
        m_ItemFrame[m_iFeed].m_TimeFrame = -1;
    }
}

void Replay::MakePetSkillData( bool bReplay )
{
    //// [8/3/2007 breadceo] Replay 가 아닌곳에서 불렀는데 RP_RECORD 상태이면 계산된 상태임
    //if( bReplay == false && m_iCurrState == RP_RECORD )
    //    return;

    if( m_bUsePetSkill )
    {
        m_PetSKillFrame[m_iFeed].m_cPlayer = m_PetSkillFrameTemp.m_cPlayer;
        m_PetSKillFrame[m_iFeed].m_fX = m_PetSkillFrameTemp.m_fX;
        m_PetSKillFrame[m_iFeed].m_fY = m_PetSkillFrameTemp.m_fY;
        m_PetSKillFrame[m_iFeed].m_bIsRight = m_PetSkillFrameTemp.m_bIsRight;
        m_PetSKillFrame[m_iFeed].m_cMotion = m_PetSkillFrameTemp.m_cMotion;
        m_PetSKillFrame[m_iFeed].m_TimeFrame = m_iFeed;

        m_bUsePetSkill = false;
    }
    else
    {
        ZeroMemory( &m_PetSKillFrame[m_iFeed] , sizeof(Replay_PetSkillFrameData) );
        m_PetSKillFrame[m_iFeed].m_TimeFrame = -1;
    }
}

void Replay::MakeTagData( bool bReplay )
{
    //// [8/3/2007 breadceo] Replay 가 아닌곳에서 불렀는데 RP_RECORD 상태이면 계산된 상태임
    //if( bReplay == false && m_iCurrState == RP_RECORD )
    //    return;

    if( m_bTagCharacter )
    {
        m_TagMatchFrame[m_iFeed].m_bArena = m_TagMatchFrameTemp.m_bArena;
        m_TagMatchFrame[m_iFeed].m_cPlayer = m_TagMatchFrameTemp.m_cPlayer;
        m_TagMatchFrame[m_iFeed].m_TimeFrame = m_iFeed;

        m_bTagCharacter = false;
    }
    else
    {
        ZeroMemory( &m_TagMatchFrame[m_iFeed], sizeof(Replay_TagMatchFrameData) );
        m_TagMatchFrame[m_iFeed].m_TimeFrame = -1;
        m_TagMatchFrame[m_iFeed].m_cPlayer = -1;
    }
}

void Replay::MakeGameBoardData( bool bReplay )
{
    if( m_bUpdateGameboard )
    {
        m_GameBoardFrame[m_iFeed].m_TimeFrame = m_iFeed;
        //memcpy( m_GameBoardFrame[m_iFeed].aHow_Die , m_GameBoardTemp.aHow_Die, sizeof(m_GameBoardFrame[m_iFeed].aHow_Die) );
        //memcpy( m_GameBoardFrame[m_iFeed].aHow_Kill , m_GameBoardTemp.aHow_Kill, sizeof(m_GameBoardFrame[m_iFeed].aHow_Kill) );
        for( int i = 0 ; i < (int)m_GameBoardFrame[m_iFeed].aTeamScore.size() ; ++i )
        {
            m_GameBoardFrame[m_iFeed].aTeamScore[i] = m_GameBoardTemp.aTeamScore[i];
        }

        m_bUpdateGameboard = false;
    }
    else
    {
        ZeroMemory( &m_GameBoardFrame[m_iFeed], sizeof(Replay_GameBoardData) );
        m_GameBoardFrame[m_iFeed].m_TimeFrame = -1;        
    }
}

void Replay::MakeDummyData()
{
    MakePlayerFrameData( false );
    MakeMonsterFrameData( false );
    MakeReplayChatData( false );
    MakeItemData( false );
    MakePetSkillData( false );
    MakeTagData( false );
    MakeGameBoardData( false );
}

void Replay::WriteStream( RakNet::BitStream& stmReplay )
{
    SavePlayerFrameToStream( stmReplay );
    SaveMonFrameToStream( stmReplay );
    SaveChatFrameToStream( stmReplay );
    SaveItemFrameToStream( stmReplay );
    SavePetSkillFrameToStream( stmReplay );
    SaveTagMatchFrameToStream( stmReplay );
    SaveGameBoardFrameToStream( stmReplay );
}

void Replay::SavePetSkillFrameToStream( RakNet::BitStream& stmReplay )
{
    for( int i = 0; i < GC_FPS_LIMIT; i++ )
    {
        if( m_PetSKillFrame[i].m_TimeFrame != -1 )
            stmReplay.Write( (char*)&m_PetSKillFrame[i], sizeof(Replay_PetSkillFrameData) );
        else
            stmReplay.Write( (char*)&m_PetSKillFrame[i].m_TimeFrame, sizeof(int) );
    }
}

void Replay::SaveTagMatchFrameToStream( RakNet::BitStream& stmReplay )
{
    for( int i = 0; i < GC_FPS_LIMIT; i++ )
    {
        if( m_TagMatchFrame[i].m_TimeFrame != -1 )
            stmReplay.Write( (char*)&m_TagMatchFrame[i], sizeof(Replay_TagMatchFrameData) );
        else
            stmReplay.Write( (char*)&m_TagMatchFrame[i].m_TimeFrame, sizeof(int) );
    }
}

void Replay::SaveGameBoardFrameToStream( RakNet::BitStream& stmReplay )
{
    for( int i = 0; i < GC_FPS_LIMIT; i++ )
    {
        if( m_GameBoardFrame[i].m_TimeFrame != -1 )
            stmReplay.Write( (char*)&m_GameBoardFrame[i], sizeof(Replay_GameBoardData) );
        else
            stmReplay.Write( (char*)&m_GameBoardFrame[i].m_TimeFrame, sizeof(int) );
    }
}

void Replay::SaveItemFrameToStream( RakNet::BitStream& stmReplay )
{
    for( int i = 0 ; i < GC_FPS_LIMIT ; i++ )
    {
        if ( m_ItemFrame[i].m_TimeFrame != -1 )
            stmReplay.Write( (char*)&m_ItemFrame[i], sizeof(Replay_ItemFrameData) );
        else
            stmReplay.Write( (char*)&m_ItemFrame[i].m_TimeFrame, sizeof(int) );
    }
}

void Replay::SaveChatFrameToStream( RakNet::BitStream& stmReplay )
{
    for( int i = 0 ; i < GC_FPS_LIMIT ; i++ )
    {
        if ( m_ChatFrame[i].m_TimeFrame != -1 )
            stmReplay.Write( (char*)&m_ChatFrame[i], sizeof(Replay_ChatFrameData) );
        else
            stmReplay.Write( (char*)&m_ChatFrame[i].m_TimeFrame, sizeof(int) );
    }
}

void Replay::SaveMonFrameToStream( RakNet::BitStream& stmReplay )
{
    stmReplay.Write( (char*)Num_Monster, sizeof(unsigned char)*GC_FPS_LIMIT );

    for( int i=0 ; i<GC_FPS_LIMIT ; ++i )
        stmReplay.Write( (char*)m_MonsterFrame[i], sizeof(Replay_MonsterFrameData)*Num_Monster[i] );
}

void Replay::SavePlayerFrameToStream( RakNet::BitStream& stmReplay )
{
    stmReplay.Write( (char*)Num_Player, sizeof(unsigned char)*GC_FPS_LIMIT );

    for(int i=0 ; i<GC_FPS_LIMIT ; ++i)
        stmReplay.Write( (char*)m_PlayerFrame[i], sizeof(Replay_PlayerFrameData)*Num_Player[i] );
}

bool Replay::Load_HeadData()
{
    m_eReplay_Speed = RS_NORMAL;

    // 임시파일 삭제 - 혹시라도 있다면.
    DeleteFile(L"Replay.tmp");
    // 압축을 Replay.tmp로 해제한다.
    //     if(!Replay_Uncompress())
    //         return false;

    //압축해제는 하지 않는다. 압축된 채로 메모리로 읽어서 압축해제 한다. milennium9
    BOOL bRet = ::CopyFileW( m_pFileName, L"Replay.tmp", FALSE );
    if( bRet == FALSE )
        return false;


    // 읽을 파일 열기
    m_hFile = CreateFile(L"Replay.tmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(m_hFile == INVALID_HANDLE_VALUE)
    {
        m_hFile = NULL;
        return false;
    }

    m_HeadData.Init();
    if( ReadHeaderDataFromCompressedFile( m_HeadData ) == false )
    {
        CloseHandle( m_hFile );
        m_hFile = NULL;
        return false;
    }

    // 버전이 다름
    if( m_HeadData.m_fVersion != REPLAY_VERSION  )
    {
        CloseHandle( m_hFile );
        m_hFile = NULL;
        return false;
    }

    m_iFeed = GC_FPS_LIMIT; //한 버퍼를 다 읽었다는 뜻.

    m_bEndOfFile = false;
    m_eReplay_Speed = RS_NORMAL; //리플레이 속도 정상으로.

    //게임 초반 정보 셋팅하기.
    SiKGCRoomManager()->UpdateShowGameModeCategory( true, SiKGCChannelManager()->GetCurrentChannelType() );
    SiKGCRoomManager()->SetGameModeCategory( (EGCGameModeCategory)m_HeadData.m_sGameModeCategory );
    SiKGCRoomManager()->SetGameMode( (EGCGameMode)m_HeadData.m_sGameMode );
    SiKGCRoomManager()->SetSubGameMode( m_HeadData.m_sSubGameMode );
    SiKGCRoomManager()->SetGameModeStage( (EGCGameStage)m_HeadData.m_ucStage );

    //이렇게 하면 맵과 몬스터가 초기화 된다...
    //g_MyD3D->UpdateStage( m_HeadData.m_ucStage );
    g_MyD3D->Story = m_HeadData.m_iStory;

    //게임에 나올 몬스터들 리소스를 미리 로딩해 둔다.
    for (int i = 0 ; i < MAX_NUM_MON_SLOT ; i++ )
    {
        if ( m_HeadData.m_sMonsterSlot[i] != MON_INVALID )
        {
            if ( ( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_MATCH || SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AUTOMATCH ) )
            {
				MONSTER* pMonster = g_kMonsterManager.GetMonster(0);
				if( pMonster != NULL) 
					pMonster->StartMonster((EGCMonster)m_HeadData.m_sMonsterSlot[i]);


            }
            else
            {
				MONSTER* pMonster = g_kMonsterManager.GetMonster(i);
				if( pMonster != NULL) 
					pMonster->StartMonster((EGCMonster)m_HeadData.m_sMonsterSlot[i]);
            }
        }
    }

    for( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
	{
        Get_PlayerData(i);
	}

    return true;
}

bool Replay::Load_FrameData()
{
    if(m_iCurrState != RP_VIEW)
        return false;

    if(m_bEndOfFile)
    {
        g_MyD3D->m_pStateMachine->OnExitGame();
        return true;
    }

    // 버퍼를 다 읽었으므로 새로 로드한다.
    if(m_iFeed == GC_FPS_LIMIT)
    {
        m_iFeed = 0;
        DWORD temp = 0;

        if(m_hFile == NULL)
        {
            m_iCurrState = RP_NONE;
            return false;
        }

        RakNet::BitStream stmFrameData;
        if( ReadFrameDataStreamFromCompressedFile( stmFrameData ) == false )
        {
            m_bEndOfFile = true;
            return false;
        }

        stmFrameData.Read( (char*)Num_Player, sizeof(unsigned char)*GC_FPS_LIMIT );

        for(int i=0;i<GC_FPS_LIMIT ;i++)
        {
            stmFrameData.Read( (char*)m_PlayerFrame[i], sizeof(Replay_PlayerFrameData) * Num_Player[i]);
        }

        stmFrameData.Read( (char*)&Num_Monster, sizeof(unsigned char)*GC_FPS_LIMIT);

        for ( int i = 0 ; i < GC_FPS_LIMIT ; i++ )
        {
            if( Num_Monster[i] > 0 )
                stmFrameData.Read( (char*) m_MonsterFrame[i], sizeof(Replay_MonsterFrameData) * Num_Monster[i]);
        }

        for ( int i = 0 ; i < GC_FPS_LIMIT ; i++ )
        {
            stmFrameData.Read( (char*) &m_ChatFrame[i].m_TimeFrame , sizeof(int) );

            if (m_ChatFrame[i].m_TimeFrame != -1)
            {
                stmFrameData.Read( (char*) m_ChatFrame[i].m_strChatData , sizeof(m_ChatFrame[i].m_strChatData) );
            }
            else
            {
                swprintf(m_ChatFrame[i].m_strChatData , 300, L"");
            }
        }

        for ( int i = 0 ; i < GC_FPS_LIMIT ; i++ )
        {
            stmFrameData.Read( (char*) &m_ItemFrame[i].m_TimeFrame , sizeof(int) );

            // [2/1/2008] breadceo. 읽으면 offset이 바뀐다 그래서 이렇게 읽어야 함.
            // 편하게 하려고 다음 멤버 변수주소로 한다
            if (m_ItemFrame[i].m_TimeFrame != -1)
            {
                stmFrameData.Read( (char*)&m_ItemFrame[i].m_cUser, sizeof(Replay_ItemFrameData) - sizeof(int) );
            }
        }

        for( int i = 0; i < GC_FPS_LIMIT; i++ )
        {
            stmFrameData.Read( (char*) &m_PetSKillFrame[i].m_TimeFrame, sizeof(int) );

            // [2/1/2008] breadceo. 읽으면 offset이 바뀐다 그래서 이렇게 읽어야 함.
            // 편하게 하려고 다음 멤버 변수주소로 한다
            if( m_PetSKillFrame[i].m_TimeFrame != -1 )
            {
                stmFrameData.Read( (char*)&m_PetSKillFrame[i].m_cPlayer, sizeof( Replay_PetSkillFrameData ) - sizeof(int) );
            }
        }

        for( int i = 0; i < GC_FPS_LIMIT; i++ )
        {
            stmFrameData.Read( (char*) &m_TagMatchFrame[i].m_TimeFrame, sizeof(int) );

            // [2/1/2008] breadceo. 읽으면 offset이 바뀐다 그래서 이렇게 읽어야 함.
            // 편하게 하려고 다음 멤버 변수주소로 한다
            if( m_TagMatchFrame[i].m_TimeFrame != -1 )
            {
                stmFrameData.Read( (char*) &m_TagMatchFrame[i].m_bArena, sizeof( Replay_TagMatchFrameData ) - sizeof(int) );
            }
        }

        for( int i = 0; i < GC_FPS_LIMIT; i++ )
        {
            stmFrameData.Read( (char*) &m_GameBoardFrame[i].m_TimeFrame, sizeof(int) );

            // [2/1/2008] breadceo. 읽으면 offset이 바뀐다 그래서 이렇게 읽어야 함.
            // 편하게 하려고 다음 멤버 변수주소로 한다
            if( m_GameBoardFrame[i].m_TimeFrame != -1 )
            {
                //stmFrameData.Read( (char*) &m_GameBoardFrame[i].aHow_Kill, sizeof( Replay_GameBoardData ) - sizeof(int) );
                stmFrameData.Read( (char*) &m_GameBoardFrame[i].aTeamScore, sizeof( Replay_GameBoardData ) - sizeof(int) );
            }
        }
    }

    for( int i = 0 ; i < MAX_PLAYER_NUM ; i++ )
    {
        g_MyD3D->MyPlayer[i]->m_kUserInfo.nUserState = GC_RUS_NONE;
        g_MyD3D->MyPlayer[i]->m_kUserInfo.bLive = false;

    }



    // 현재 플레이어 버퍼에서 값을 가져온다.
    for( int i = 0 ; i < Num_Player[m_iFeed] ; ++i )
    {
        Replay_PlayerFrameData PlayerFrame = m_PlayerFrame[m_iFeed][i];
        int Index = PlayerFrame.MyNumber;
        if( Index >= 0 && Index < MAX_PLAYER_NUM )
        {

            if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_INDIGO )
            {
                // 캐릭터가 바꼈음 (인디고)
                if( g_MyD3D->MyPlayer[Index]->m_kUserInfo.GetCurrentChar().iCharType != PlayerFrame.m_iSelectChar )
                {
                    g_MyD3D->MyPlayer[Index]->m_kUserInfo.SetCurrentChar( PlayerFrame.m_iSelectChar );
                    g_MyD3D->MyPlayer[Index]->AutoSetExtra();
                    g_MyD3D->MyPlayer[Index]->LoadPlayerTemplate();
                    //↓이 함수 잘못짜여져 있어 수정했음 아래꺼 쓸일 없을것임 wony
                    //g_MyD3D->MyPlayer[Index]->SetSkillType(0);
                    g_MyD3D->MyPlayer[Index]->CreatePlayer();
                }
            }

            g_MyD3D->MyPlayer[Index]->m_kUserInfo.bLive = true;
            g_MyD3D->MyPlayer[Index]->m_kUserInfo.nUserState = GC_RUS_PLAYING;

            g_MyD3D->MyPlayer[Index]->m_cLife = PlayerFrame.Life;
            g_MyD3D->MyPlayer[Index]->SetHP( PlayerFrame.HP );
			if( g_MyD3D->MyPlayer[Index]->uiMotion != PlayerFrame.Now_Motion )
				g_MyD3D->MyPlayer[Index]->SetPlayerState( PlayerFrame.Now_Motion );
            
            g_MyD3D->MyPlayer[Index]->cFrame = PlayerFrame.Frame;
            g_MyD3D->MyPlayer[Index]->Body_Angle = PlayerFrame.Body_Angle;
            g_MyD3D->MyPlayer[Index]->bIsRight = PlayerFrame.IsRight;
            g_MyD3D->MyPlayer[Index]->vPos.x = PlayerFrame.x;
            g_MyD3D->MyPlayer[Index]->vPos.y = PlayerFrame.y;
            g_MyD3D->MyPlayer[Index]->Super = PlayerFrame.Super;
            //g_MyD3D->MyPlayer[Index]->SetNotSpecialSuper( PlayerFrame.ucNotSpecialSuper );
            g_MyD3D->MyPlayer[Index]->Shield = PlayerFrame.Shield;
            g_MyD3D->MyPlayer[Index]->m_bRender = PlayerFrame.m_bRender;
            g_MyD3D->MyPlayer[Index]->RenderWeapon( PlayerFrame.m_bRenderWeapon );
            g_MyD3D->MyPlayer[Index]->RenderMergeWeapon( PlayerFrame.m_bRenderMergeWeapon );
            g_MyD3D->MyPlayer[Index]->DrawSlashLeft( PlayerFrame.IsSlashOnLeft( Replay_PlayerFrameData::SE_WEAPON ) );
            g_MyD3D->MyPlayer[Index]->DrawSlashRight( PlayerFrame.IsSlashOnRight( Replay_PlayerFrameData::SE_LEG ) );
            g_MyD3D->MyPlayer[Index]->DrawSlashLegLeft( PlayerFrame.IsSlashOnLeft( Replay_PlayerFrameData::SE_WEAPON ) );
            g_MyD3D->MyPlayer[Index]->DrawSlashLegRight( PlayerFrame.IsSlashOnRight( Replay_PlayerFrameData::SE_LEG ) );
            // 아레나 배틀용 데이타.
            g_pGameBoard->m_pStartingMember[Index].How_Kill = PlayerFrame.iKills;
            g_pGameBoard->m_pStartingMember[Index].How_Die = PlayerFrame.iDeaths;

            // 변신 ( 쪼아 이거야!! )
            if( g_MyD3D->MyPlayer[Index]->MetamorphosisForm != PlayerFrame.MetamorphosisForm && !g_MyD3D->MyPlayer[Index]->IsTagMotion() )
            {
                if( PlayerFrame.MetamorphosisForm == FORM_NORMAL )
                    g_MyD3D->MyPlayer[Index]->ProcessMetamorphosisNormalForm( true );
                else
                    g_MyD3D->MyPlayer[Index]->ProcessMetamorphosisFormChange( PlayerFrame.MetamorphosisForm );

                g_MyD3D->MyPlayer[Index]->MetamorphosisForm = PlayerFrame.MetamorphosisForm;
            }
        }
    }

	for ( std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.begin() ; mit != g_kMonsterManager.m_mapMonster.end() ; ++mit ) {
		mit->second->m_bLive = false;
	}

    for(int i=0;i<Num_Monster[m_iFeed];i++)
    {
        Replay_MonsterFrameData MonsterFrame = m_MonsterFrame[m_iFeed][i];
        int Index = MonsterFrame.MyNumber;
		std::map<int, MONSTER*>::iterator mit = g_kMonsterManager.m_mapMonster.find(Index);
		if( mit == g_kMonsterManager.m_mapMonster.end() ) continue;
		MONSTER *pMonster = mit->second;

		int RIndex = LATENCY+SHOWLATENCY-1;

		pMonster->m_bLive = true;
		pMonster->m_bRender = true; //렌더링을 허락한다.

		pMonster->m_fAttackPoint = MonsterFrame.m_fAtk_Point;
		//pMonster->m_fDef_Point = MonsterFrame.m_fDef_Point;
		pMonster->m_iMonsterType = MonsterFrame.Now_Monster;
		pMonster->m_fHP = MonsterFrame.HP;

		pMonster->m_abIsRight[RIndex] = MonsterFrame.IsRight;
		pMonster->m_aiMotion[RIndex] = MonsterFrame.Now_Motion;
		pMonster->m_aiFrame[RIndex] = MonsterFrame.Frame;
		pMonster->m_afX[RIndex] = MonsterFrame.x;
		pMonster->SetLatencyY( RIndex, MonsterFrame.y );

		pMonster->m_abIsRight[0] = MonsterFrame.Render_IsRight;
		pMonster->m_aiMotion[0] = MonsterFrame.Render_Now_Motion;
		pMonster->m_aiFrame[0] = MonsterFrame.Render_Frame;
		pMonster->SetLatencyX( 0, MonsterFrame.Render_x );
		pMonster->SetLatencyY( 0, MonsterFrame.Render_y );
    }

    // RP_SLOW 위함
    static int ox=0;
    ox++;

    if(ox>1)
        ox=0;

    for ( int i = 0 ; i < GC_FPS_LIMIT ; i++ )
    {
        if( m_eReplay_Speed == RS_SLOW && ox != 0 )
            break;

        int iSkip_Frame = 1; //리플레이 속도 기본 상태, frame은 1씩 증가한다.
        switch(m_eReplay_Speed)
        {
        case RS_FAST: iSkip_Frame = 2; break;
        case RS_TOP : iSkip_Frame = 4; break;
        }

        //채팅 메세지 출력
        if( m_ChatFrame[i].m_TimeFrame >= m_iFeed && m_ChatFrame[i].m_TimeFrame < m_iFeed + iSkip_Frame )
        {
            std::wstring strChatMsg = m_ChatFrame[i].m_strChatData;
            g_pkChatManager->AddChatMsg(strChatMsg);
        }

        //아이템 사용
        int iItemIdx = (int)m_ItemFrame[i].m_cUser;
        if( iItemIdx >= 0 && iItemIdx < MAX_PLAYER_NUM )
        {
            if( m_ItemFrame[i].m_TimeFrame >= m_iFeed && m_ItemFrame[i].m_TimeFrame < m_iFeed + iSkip_Frame )
            {
                char What = -1;
                for (int a = 0; a < MAX_PLAYER_NUM ; a++)
                {
                    g_MyD3D->MyPlayer[a]->Eff_Item( m_ItemFrame[i].Eff_Item[a] );
                    if( m_ItemFrame[i].Eff_Item[a].cWhat != -1 )
                        What = m_ItemFrame[i].Eff_Item[a].cWhat;
                }

                g_pkChatManager->AddSystemMsg( -1,
                    g_pkStrLoader->GetReplacedString(STR_ID_GAMEMSG_ITEM_USE1, "ss", 
                    g_MyD3D->MyPlayer[iItemIdx]->GetStrUserName(),
                    g_MyD3D->MyItms->GetItemDesc( What ) ) 
                    );
            }
        }

        int iPlayerIdx = (int)m_PetSKillFrame[i].m_cPlayer;
        if( iPlayerIdx >= 0 && iPlayerIdx < MAX_PLAYER_NUM )
        {
            if( m_PetSKillFrame[i].m_TimeFrame >= m_iFeed && m_PetSKillFrame[i].m_TimeFrame < m_iFeed + iSkip_Frame )
            {
                g_MyD3D->m_akPet[iPlayerIdx]->vPos.x = m_PetSKillFrame[i].m_fX;
                g_MyD3D->m_akPet[iPlayerIdx]->vPos.y = m_PetSKillFrame[i].m_fY;
                g_MyD3D->m_akPet[iPlayerIdx]->bIsRight = m_PetSKillFrame[i].m_bIsRight;

                g_MyD3D->m_akPet[iPlayerIdx]->usMotion = (int)m_PetSKillFrame[i].m_cMotion;//EGCPETMOTION::ATTACK;
                g_MyD3D->m_akPet[iPlayerIdx]->cFrame = 0;

                g_MyD3D->m_akPet[iPlayerIdx]->m_fCurrentMP = 0.0f;
            }
        }

        if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_TAG_MATCH )
        {
            if( m_TagMatchFrame[i].m_TimeFrame >= m_iFeed && m_TagMatchFrame[i].m_TimeFrame < m_iFeed + iSkip_Frame )
            {
                int iPlayer = (int)m_TagMatchFrame[i].m_cPlayer;
                if( iPlayer >= 0 && iPlayer < MAX_PLAYER_NUM )
                {
                    g_MyD3D->MyPlayer[iPlayer]->TagCharacter( m_TagMatchFrame[i].m_bArena );
                }
            }
        }
        else if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_DEATHMATCH ||
                 SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_FATAL_DEATHMATCH )
        {
            if( m_GameBoardFrame[i].m_TimeFrame >= m_iFeed && m_GameBoardFrame[i].m_TimeFrame < m_iFeed + iSkip_Frame )
            {
                //for ( int k = 0 ; k < MAX_PLAYER_NUM; k++ )
                //{
                //    g_pGameBoard->m_pStartingMember[k].How_Die = m_GameBoardFrame[i].aHow_Die[k];
                //    g_pGameBoard->m_pStartingMember[k].How_Kill = m_GameBoardFrame[i].aHow_Kill[k];
                //}

                for ( int k = 0; k < TEAM_NUM; k++ )
                {
                    g_pGameBoard->SetTeamScore(k, m_GameBoardFrame[i].aTeamScore[k]); 
                }

                g_pkGameOverlayUI->m_pkDeathMatchScoreBoard->UpdateGameData();
            }
        }
    }

    if(diks[DIK_RIGHT] && !Old_diks[DIK_RIGHT])
    {
        m_eReplay_Speed = (ERPSpeed)((int)m_eReplay_Speed + 1);
        if(m_eReplay_Speed > RS_TOP)
            m_eReplay_Speed = RS_TOP;
    }
    else if(diks[DIK_LEFT] && !Old_diks[DIK_LEFT])
    {
        m_eReplay_Speed = (ERPSpeed)((int)m_eReplay_Speed - 1);
        if(m_eReplay_Speed < RS_STOP)
            m_eReplay_Speed = RS_STOP;
    }

    switch(m_eReplay_Speed)
    {
    case RS_SLOW:
        if(!ox)
            m_iFeed++;
        break;
    case RS_NORMAL:
        m_iFeed++;
        break;
    case RS_FAST:
        m_iFeed += 2;
        if(m_iFeed>GC_FPS_LIMIT)
            m_iFeed=GC_FPS_LIMIT;
        break;
    case RS_TOP:
        m_iFeed += 4;
        if(m_iFeed>GC_FPS_LIMIT)
            m_iFeed=GC_FPS_LIMIT;
        break;
    }

    return true;
}

void Replay::Set_ItemData( KGC_PID_EFFECT_ITEM* pkPacket , char Who )
{
    //if( m_iCurrState != RP_RECORD )
    //    return;

    if( Who  < 0 && Who >= MAX_PLAYER_NUM )
        return;

    m_bItemDataEntered = true;
    for (int i = 0; i < MAX_PLAYER_NUM ; i++)
    {
        m_ItemFrameTemp.Eff_Item[i].cWhat = pkPacket->Eff_Item[i].cWhat;
        m_ItemFrameTemp.Eff_Item[i].sTime = pkPacket->Eff_Item[i].sTime;
    }

    m_ItemFrameTemp.m_cUser = Who;
}

void Replay::Set_UsePetSkill( KGC_PID_USEPETSKILL* pkPacket, char cWho )
{
    //if( m_iCurrState != RP_RECORD )
    //    return;

    if( cWho < 0 && cWho >= MAX_PLAYER_NUM )
        return;

    m_bUsePetSkill = true;

    m_PetSkillFrameTemp.m_cPlayer = cWho;
    m_PetSkillFrameTemp.m_fX = pkPacket->PetSkill.fX;
    m_PetSkillFrameTemp.m_fY = pkPacket->PetSkill.fY;
    m_PetSkillFrameTemp.m_bIsRight = pkPacket->PetSkill.bIsRight;
    m_PetSkillFrameTemp.m_cMotion = pkPacket->cPetMotion;
}

void Replay::Set_TagCharacter( KGC_PID_TAG_CHARACTER* pkPacket, char cWho )
{
    //if( m_iCurrState != RP_RECORD )
    //    return;

    if( cWho  < 0 && cWho >= MAX_PLAYER_NUM )
        return;

    m_bTagCharacter = true;

    m_TagMatchFrameTemp.m_bArena = pkPacket->bArena;
    m_TagMatchFrameTemp.m_cPlayer = cWho;
}

void Replay::Set_GameBoard( KGC_PID_GAME_BOARD_UPDATE* pkPacket )
{
    m_bUpdateGameboard = true;

    for( int i = 0 ; i < (int)m_GameBoardTemp.aTeamScore.size() ; ++i )
    {
        m_GameBoardTemp.aTeamScore[i] = pkPacket->aTeamScore[i];
    }
}

bool Replay::SaveReplayStart()
{
    //현재 날짜, 시간을 파일명으로 한다.
    tm *newtime;
    time_t aclock;

    time( &aclock );                 /* Get time in seconds */
    newtime = localtime( &aclock );  

    swprintf( m_pFileName, MAX_PATH, L"Replay\\%4d_%2d_%2d_%2d_%2d.first", 
        newtime->tm_year+1900, newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min );

    if(m_iCurrState != RP_NONE)
        return false;

    m_iFeed=0;
    m_iCurrState = RP_RECORD;
    return true;
}

void Replay::SaveReplayEnd()
{
    // 아지트면 생략
    if( SiKGCRoomManager()->GetGameModeCategory() == GC_GMC_AGIT )
        return;

    SiGCPairPlaySystem()->MakeDummyReplay();
    CloseHandle(m_hFile);
    m_hFile = NULL;

    //유저가 녹화를 하기 시작한 상태라면 Dummy Replay를 복사해서 파일 이름을 바꿔준다.
    if( m_iCurrState == RP_RECORD )
    {
        // [8/4/2007 breadceo] 이미 만들어 놓은 Replay를 사용
        BOOL bRet = ::CopyFile( DUMMY_REPLAY_FILE_NAME, m_pFileName, FALSE );
        g_pkChatManager->AddChatMsg( g_pkStrLoader->GetReplacedString( STR_ID_REPLAY_SAVED, "s", m_pFileName ) );
        g_pkChatManager->AddChatMsgTo( GS_ROOM, g_pkStrLoader->GetReplacedString( STR_ID_REPLAY_SAVED, "s", m_pFileName ), KGCChatManager::CHAT_TYPE_ALERT );
    }
    m_iCurrState = RP_NONE;
}

bool Replay::ReadHeaderDataFromCompressedFile( Replay_Head_Data& sHeader_ )
{
    DWORD temp;
    Replay_HeadData_Compress_Header sCompHeader;

    float fVersion;
    DWORD dwSrcByte;

    if( ReadFile( m_hFile, &fVersion, sizeof(float), &temp, NULL ) == FALSE )
        return false;

    if( ReadFile( m_hFile, &dwSrcByte, sizeof(DWORD), &temp, NULL ) == FALSE )
        return false;

    if ( fVersion < REPLAY_MINOR_VERSION && fVersion > REPLAY_VERSION )
        return false;

    IgnoreReplayAPIHeaderRegion();

    if( ReadFile(m_hFile, &sCompHeader, sizeof(Replay_HeadData_Compress_Header), &temp, NULL) == FALSE )
        return false;

    BYTE* apCompressedHeader = new BYTE[sCompHeader.ulCompressedSize];
    if( ReadFile(m_hFile, apCompressedHeader, sCompHeader.ulCompressedSize, &temp, NULL) == FALSE )
    {
        SAFE_DELETE_ARRAY( apCompressedHeader );
        return false;
    }

    int Result = Z_BUF_ERROR;
    DWORD dwUncompSize = sCompHeader.dwRealSize;
    BYTE* pDest = new BYTE[ sCompHeader.dwRealSize ];
    Result = uncompress( pDest, &dwUncompSize, apCompressedHeader, sCompHeader.ulCompressedSize );

    SAFE_DELETE_ARRAY( apCompressedHeader );
    if( Result != Z_OK ) 
    {
        SAFE_DELETE_ARRAY( pDest );
        return false;
    }

    if ( fVersion == REPLAY_VERSION ) 
    {
        if ( dwUncompSize != sizeof( Replay_Head_Data ) )
        {
            SAFE_DELETE_ARRAY( pDest );
            return false;
        }

        sHeader_ = *((Replay_Head_Data*)pDest);
        SAFE_DELETE_ARRAY( pDest );
    }
    else if ( fVersion == 9.0f )
    {

        if ( dwUncompSize != sizeof( Replay_Head_Data_Ver09 ) )
        {
            SAFE_DELETE_ARRAY( pDest );
            return false;
        }

        Replay_Head_Data_Ver09 sHeader09;
        sHeader09.Init();
        sHeader09 = *((Replay_Head_Data_Ver09*)pDest);
        sHeader_.Assign( sHeader09 );

        SAFE_DELETE_ARRAY( pDest );
    }
    else
    {
        SAFE_DELETE_ARRAY( pDest );
        return false;
    }

    return true;
}

bool Replay::ReadFrameDataStreamFromCompressedFile( RakNet::BitStream& stmFrameData )
{
    DWORD temp;
    Replay_FrameData_Compress_Header sCompHeader;

    if( ReadFile(m_hFile, &sCompHeader, sizeof(Replay_FrameData_Compress_Header), &temp, NULL) == FALSE )
        return false;

    BYTE* apCompressedHeader = new BYTE[sCompHeader.ulCompressedSize];
    if( ReadFile(m_hFile, apCompressedHeader, sCompHeader.ulCompressedSize, &temp, NULL) == FALSE )
        return false;

    int Result = Z_BUF_ERROR;
    DWORD dwUncompSize = sCompHeader.dwRealSize;
    BYTE* pFrameData = new BYTE[sCompHeader.dwRealSize];

    Result = uncompress( pFrameData, &dwUncompSize, apCompressedHeader, sCompHeader.ulCompressedSize);
    SAFE_DELETE_ARRAY( apCompressedHeader );
    if( Result != Z_OK )
    {
        return false;
    }

    stmFrameData.Write( (char*)pFrameData, dwUncompSize );
    SAFE_DELETE_ARRAY( pFrameData );
    return true;
}

ReplayAPI_Header Replay::GetReplayAPI_Header()
{
    ReplayAPI_Header header;

    header.m_iGameMode = SiKGCRoomManager()->GetGameMode();
    header.m_iGameModeCategory = SiKGCRoomManager()->GetGameModeCategory();
    header.m_iMapID = SiKGCRoomManager()->GetGameModeStage();
    header.m_iSubGameMode = SiKGCRoomManager()->GetSubGameMode();

    for( int i = 0 ; i < MAX_PLAYER_NUM; ++i )
    {
        GetReplayAPI_UserInfoHeader( header.m_sUser[i], i );
    }

    return header;
}

void Replay::GetReplayAPI_UserInfoHeader( ReplayAPI_UserInfo& userInfo, int iPlayerIndex )
{
    if( g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.bLive == false )
        return;

    userInfo.m_iCharType = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentChar().iCharType;
    userInfo.m_iPromotion = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentChar().iPromotionLevel;
    userInfo.m_iCharLevel = g_MyD3D->MyPlayer[iPlayerIndex]->m_kUserInfo.GetCurrentChar().iLevel;
    userInfo.m_iKill = g_pGameBoard->GetHowKill( iPlayerIndex );
    userInfo.m_iDeath = g_pGameBoard->GetHowDie( iPlayerIndex );
    userInfo.m_iRank = g_pGameBoard->GetRank(iPlayerIndex);

    swprintf( userInfo.m_strNick, 30, g_MyD3D->MyPlayer[iPlayerIndex]->GetStrUserName().c_str() );
}

void Replay::IgnoreReplayAPIHeaderRegion()
{
    DWORD temp;
    ReplayAPI_Header sAPI;
    ReadFile( m_hFile, &sAPI, sizeof(ReplayAPI_Header), &temp, NULL );
    return;
}
