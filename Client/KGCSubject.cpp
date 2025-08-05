 /*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

     ★ 설계자 ☞ 이상호
     ★ 설계일 ☞ 2013년 05월 23일
     ★ E-Mail ☞ shmhlove@kog.co.kr
     ★ 클래스 ☞ 달성과제 클래스

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#include "KGCSubject.h"


/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★달성과제 템플릿★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
void KGCSubjectTemplate::Init()
{
    m_dwTypeID = 0;
    m_dwSimpleDescStringID = 0;
    m_strActionScript.clear();
    m_mapCallBackList.clear();
    m_kRewardInfo.Init();
}

std::string KGCSubjectTemplate::GetCallBackKeyName( int iIndex )
{
    switch( iIndex )
    {
    case CALLBACK_INIT:
        return "Init";
    case CALLBACK_HIT_MONSTER_DMG:
        return "HitMonDamage";
    case CALLBACK_END:
        return "End";
    }
    return "";
}

std::string KGCSubjectTemplate::GetCallBackFuncName( int iIndex ) const
{
    std::map<int, std::string>::const_iterator mapIter = m_mapCallBackList.find( iIndex );
    if( mapIter != m_mapCallBackList.end() )
        return mapIter->second;
    return "";
}



/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤
★★★★★★ 달성과제 클래스 ★★★★★★★★
▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
KGCSubject::KGCSubject( DWORD dwUID, KGCSubjectTemplate* pTemplate )
: m_pStrLoader( NULL )
{
    if( NULL == pTemplate )
        return;

    m_dwUID = dwUID;
    m_pSubjectTemplate = pTemplate;
    m_strNowAction.clear();

    Init();
}

KGCSubject::~KGCSubject()
{
    CallLuaFunc_Callback( KGCSubjectTemplate::CALLBACK_END );
    Release();
}

void KGCSubject::Release()
{

}

void KGCSubject::Init()
{
    if( NULL == m_pSubjectTemplate )
        return;

    std::vector< std::pair<std::string, KGCSubjectAction> >::const_iterator vecActionIter = m_pSubjectTemplate->m_vecActionList.begin();
    if( vecActionIter != m_pSubjectTemplate->m_vecActionList.end() )
        m_strNowAction = vecActionIter->first;

    m_bDeleteSubject = false;
    m_iTG_ID = -1;
    m_iStageID = -1;
    m_iSuccess = -1;

    m_iBackAttackCnt = 0;
    m_iSpecialCnt = 0;
    m_iDamageHittedCnt = 0;
    m_fDamageVolumeCnt = 0.0f;
    m_iDieCnt = 0;
    m_bComboBreak = false;

    m_iProgressNow = -1;
    m_iProgressMax = -1;
}

void KGCSubject::TextRender()
{
    if( NULL == m_pSubjectTemplate )
        return;
    
}

void KGCSubject::FrameMove()
{
    CallLuaFunc_Action();
    UnLockAddChecker();
}

void KGCSubject::CallLuaFunc_Action()
{
    if( g_kGlobalValue.m_bFrameLock && o3 != 0 )
        return;

    const KGCSubjectAction* pAction = GetActionFromState( m_strNowAction.c_str() );
    if( pAction )
        CallLuaFunction( pAction->strFunctionName.c_str() );
}

void KGCSubject::CallLuaFunc_Callback( int iCallbackIndex )
{
    if( NULL == m_pSubjectTemplate )
        return;

    std::string strFunc = m_pSubjectTemplate->GetCallBackFuncName( iCallbackIndex );
    if( 0 < static_cast<int>(strlen(strFunc.c_str())) )
    {
            CallLuaFunction( strFunc.c_str() );
    }
}

void KGCSubject::CallLuaFunc_Callback_To_HitMonDamage( int iAttackerID, int iHitterID, int iDamageID )
{
    if( NULL == m_pSubjectTemplate )
        return;

    std::string strFunc = m_pSubjectTemplate->GetCallBackFuncName( KGCSubjectTemplate::CALLBACK_HIT_MONSTER_DMG );
    if( 0 < static_cast<int>(strlen(strFunc.c_str())) )
    {
        CallLuaFunction_HitMonDamageCallback( strFunc.c_str(), iAttackerID, iHitterID, iDamageID );
    }
}


void KGCSubject::CallLuaFunction( const char *szFunction )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();

    lua_checkstack( L, 10 );
    lua_pushstring( L, szFunction );
    lua_rawget( L, LUA_GLOBALSINDEX );
    if( lua_isfunction( L, -1 ) )
    {
        lua_pushinteger( L, m_dwUID );
        if( 0 != lua_pcall( L, 1, 0, 0 ) )
        {
#if !defined(__PATH__)
            printf( "%s\n", lua_tostring( L, -1 ) );
#endif
            lua_pop( L, 1 );
        }
    }
    else
    {
        lua_pop( L, 1 );
    }
}

void KGCSubject::CallLuaFunction_HitMonDamageCallback( const char *szFunction, int iAttackerID, int iHitterID, int iDamageID )
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();

    lua_checkstack( L, 10 );
    lua_pushstring( L, szFunction );
    lua_rawget( L, LUA_GLOBALSINDEX );
    if( lua_isfunction( L, -1 ) )
    {
        lua_pushinteger( L, m_dwUID );
        lua_pushinteger( L, iAttackerID );
        lua_pushinteger( L, iHitterID );
        lua_pushinteger( L, iDamageID );
        if( 0 != lua_pcall( L, 4, 0, 0 ) )
        {
#if !defined(__PATH__)
            printf( "%s\n", lua_tostring( L, -1 ) );
#endif
            lua_pop( L, 1 );
        }
    }
    else
    {
        lua_pop( L, 1 );
    }
}

const KGCSubjectAction* KGCSubject::GetActionFromIndex( int iIndex )
{
    if( NULL == m_pSubjectTemplate )
        return NULL;

    std::vector< std::pair<std::string, KGCSubjectAction> >::const_iterator vecActionIter = m_pSubjectTemplate->m_vecActionList.begin();
    for(; vecActionIter != m_pSubjectTemplate->m_vecActionList.end(); ++vecActionIter)
    {
        if( iIndex == vecActionIter->second.m_iStateIndex )
            break;
    }

    if( vecActionIter == m_pSubjectTemplate->m_vecActionList.end() )
        return NULL;

    return &vecActionIter->second;
}

const KGCSubjectAction* KGCSubject::GetActionFromState( const char *szState )
{
    if( NULL == m_pSubjectTemplate )
        return NULL;

    std::vector< std::pair<std::string, KGCSubjectAction> >::const_iterator vecActionIter = m_pSubjectTemplate->m_vecActionList.begin();
    for(; vecActionIter != m_pSubjectTemplate->m_vecActionList.end(); ++vecActionIter)
    {
        if( vecActionIter->first == szState )
            break;
    }

    if( vecActionIter == m_pSubjectTemplate->m_vecActionList.end() )
        return NULL;

    return &vecActionIter->second;
}

DWORD KGCSubject::GetSubjectType()
{
    if( NULL == m_pSubjectTemplate )
        return 0;

    return m_pSubjectTemplate->m_dwTypeID;
}

DWORD KGCSubject::GetSubjectSimpleStringID()
{
    if( NULL == m_pSubjectTemplate )
        return 0;

    return m_pSubjectTemplate->m_dwSimpleDescStringID;
}

void KGCSubject::AddUIParticle( char *szSequence, float fPosX, float fPosY )
{
    CParticleEventSeqPTR pParticle = NULL;
    pParticle = g_ParticleManager->CreateSequence( szSequence, 
        fPosX / (400.0f*GC_SCREEN_DIV_WIDTH) - 1.0f, 
        0.75f - (( fPosY ) / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f, 0.5f );
    g_ParticleManager->SetLayer( pParticle, GC_LAYER_UI );
}

void KGCSubject::SetState( const char *szState )
{
    if( NULL == m_pSubjectTemplate )
        return;

    const KGCSubjectAction* pAction = GetActionFromState( szState );
    if( NULL == pAction )
        return;

    m_strNowAction = pAction->strFunctionName;
}

const char* KGCSubject::GetState()
{
    return m_strNowAction.c_str();
}

bool KGCSubject::IsState( char *szState )
{
    if( m_strNowAction == szState )
        return true;

    return false;
}

void KGCSubject::SetTempValue( char *strKey, float fValue )
{
    m_mapTempValue[strKey] = fValue;
}

float KGCSubject::GetTempValue( char *strKey )
{
    std::map<std::string, float>::iterator mapIter = m_mapTempValue.find( strKey );
    if( mapIter != m_mapTempValue.end() )
        return mapIter->second;
    return 0.0f;
}

void KGCSubject::GetRewardInfo( KInGameMessageAttribute *pMsgAttribute )
{
    if( NULL == m_pSubjectTemplate )
        return;

    if( NULL == pMsgAttribute )
        return;

    std::wstringstream strStream;
    strStream.clear();
    m_strReward.clear();

    if( -1< pMsgAttribute->iStringID )
    {
        if( m_pStrLoader )
        {
            if( INT_MAX != pMsgAttribute->iValue1 )
                strStream << m_pStrLoader->GetReplacedString(pMsgAttribute->iStringID, "i", pMsgAttribute->iValue1);
            else if( pMsgAttribute->strValue1 && pMsgAttribute->strValue2 )
                strStream << m_pStrLoader->GetReplacedString(pMsgAttribute->iStringID, "ss", pMsgAttribute->strValue1, pMsgAttribute->strValue2);
            else if( pMsgAttribute->strValue1 )
                strStream << m_pStrLoader->GetReplacedString(pMsgAttribute->iStringID, "s", pMsgAttribute->strValue1);
            else
                strStream << m_pStrLoader->GetString(pMsgAttribute->iStringID);

            strStream << L"\n";
        }
    }

    // Exp 보상정보
    if( 0 != m_pSubjectTemplate->m_kRewardInfo.m_iExp )
    {
        strStream << L"EXP + " << m_pSubjectTemplate->m_kRewardInfo.m_iExp;
    }

    // Gp 보상정보
    if( 0 != m_pSubjectTemplate->m_kRewardInfo.m_iGp )
    {
        if( 0 != m_pSubjectTemplate->m_kRewardInfo.m_iExp )
            strStream << L" / GP + " << m_pSubjectTemplate->m_kRewardInfo.m_iGp;
        else
            strStream << L"GP + " << m_pSubjectTemplate->m_kRewardInfo.m_iGp;
    }
    strStream << L"\n";

    // 버프 보상정보
    std::vector<KGCSubjectReward_Buff>::const_iterator vecBuffIter = m_pSubjectTemplate->m_kRewardInfo.m_vecBuffList.begin();
    for(; vecBuffIter != m_pSubjectTemplate->m_kRewardInfo.m_vecBuffList.end(); ++vecBuffIter)
    {
        std::vector<std::wstring>& vecMsg = g_pMagicEffect->GetHudString( vecBuffIter->iBuffID );
        std::vector<std::wstring>::iterator vecStrIter = vecMsg.begin();
        for(int iLoop=0; vecStrIter != vecMsg.end(); ++iLoop, ++vecStrIter)
        {
            if( 0 < iLoop )
                strStream << L" / ";
            strStream << vecStrIter->c_str();
        }
    }
    m_strReward = strStream.str().c_str();
    if( 1 < static_cast<int>(m_strReward.size()) )
        pMsgAttribute->strDirectString = const_cast<WCHAR*>(m_strReward.c_str());

    // 아이템 보상정보
    std::vector<KGCSubjectReward_Item>::const_iterator vecItemIter = m_pSubjectTemplate->m_kRewardInfo.m_vecItemList.begin();
    for(; vecItemIter != m_pSubjectTemplate->m_kRewardInfo.m_vecItemList.end(); ++vecItemIter)
    {
        KGCInGameMessageItem kInGameMessageItem;
        kInGameMessageItem.dwItemID = vecItemIter->dwItemID;
        kInGameMessageItem.iGrade = vecItemIter->iGrade;
        kInGameMessageItem.iPeriod = vecItemIter->iPeriod;
        kInGameMessageItem.iCount = vecItemIter->iCount;
        pMsgAttribute->m_vecItemList.push_back( kInGameMessageItem );
    }
}

int KGCSubject::GetCombo()
{
    return g_MyD3D->MyHead->Combo_Count;
}

int KGCSubject::GetAerialCombo()
{
    return g_MyD3D->MyHead->Combo_Count_Aerial;
}

void KGCSubject::ResetCombo()
{
    g_MyD3D->MyHead->Combo_Count = 0;
    g_MyD3D->MyHead->Combo_Count_Aerial = 0;
    g_MyD3D->MyHead->ShowCombo = false;
}

void KGCSubject::UnLockAddChecker()
{
    memset( m_bAddChecker, 0, sizeof(bool)*MAX_SUBJECT_ADDCHECKER);
}

void KGCSubject::AddBackAttack()
{
    if( m_bAddChecker[SUBJECT_ADDCHECKER_BACKATTACK] )
        return;

    m_bAddChecker[SUBJECT_ADDCHECKER_BACKATTACK] = true;
    ++m_iBackAttackCnt;
}

void KGCSubject::AddSpecial()
{
    if( m_bAddChecker[SUBJECT_ADDCHECKER_SPECIAL] )
        return;

    m_bAddChecker[SUBJECT_ADDCHECKER_SPECIAL] = true;
    ++m_iSpecialCnt;
}

void KGCSubject::AddDamageHitCnt()
{
    if( m_bAddChecker[SUBJECT_ADDCHECKER_DAMAGE_CNT] )
        return;

    m_bAddChecker[SUBJECT_ADDCHECKER_DAMAGE_CNT] = true;
    ++m_iDamageHittedCnt;
}

void KGCSubject::AddDamageVolume( float fDamage )
{
    if( m_bAddChecker[SUBJECT_ADDCHECKER_DAMAGE_VOLUME] )
        return;

    m_bAddChecker[SUBJECT_ADDCHECKER_DAMAGE_VOLUME] = true;
    m_fDamageVolumeCnt += fDamage;
}

void KGCSubject::AddDieCnt()
{
    if( m_bAddChecker[SUBJECT_ADDCHECKER_DIE_CNT] )
        return;

    m_bAddChecker[SUBJECT_ADDCHECKER_DIE_CNT] = true;
    ++m_iDieCnt;
}

void KGCSubject::SetComboBreak()
{
    if( m_bAddChecker[SUBJECT_ADDCHECKER_COMBO_BREAK] )
        return;

    m_bAddChecker[SUBJECT_ADDCHECKER_COMBO_BREAK] = true;
    m_bComboBreak = true;
}

void KGCSubject::EndSubject( bool bSuccess )
{
    m_bDeleteSubject = true;

    if( NULL == m_pSubjectTemplate )
        return;

    if( bSuccess )
    {
        int iMyPlayer = g_MyD3D->Get_MyPlayer();
        if( false == g_MyD3D->IsPlayerIndex(iMyPlayer) )
            return;

        // 버프보상처리
        PLAYER *pPlayer = g_MyD3D->MyPlayer[iMyPlayer];
        std::vector<KGCSubjectReward_Buff>::const_iterator vecIter = m_pSubjectTemplate->m_kRewardInfo.m_vecBuffList.begin();
        for(; vecIter != m_pSubjectTemplate->m_kRewardInfo.m_vecBuffList.end(); ++vecIter)
        {
            pPlayer->SetMagicEffect( vecIter->iBuffID, vecIter->fBuffTime, vecIter->iBuffLevel, vecIter->bIsLocalBuff );            
        }

        // 민감한 보상이 있다면 서버에 보상요청( Exp, Gp, Item )
        if( (0 != m_pSubjectTemplate->m_kRewardInfo.m_iExp) ||
            (0 != m_pSubjectTemplate->m_kRewardInfo.m_iGp) ||
            (false == m_pSubjectTemplate->m_kRewardInfo.m_vecItemList.empty()) )
        {
            KEVENT_DUNGEON_SUBJECT_REWARD_REQ kPacket;
            kPacket.m_dwTriggerID = m_iTG_ID;
            kPacket.m_dwSubjectID = m_pSubjectTemplate->m_dwTypeID;
            KP2P::GetInstance()->Send_SubjectRewardReq(kPacket);
        }
    }

    m_iSuccess = (bSuccess?1:0);
}

void KGCSubject::RegisterLuaBind()
{
    lua_State* L = KGCLuabinder::getInstance().GetLuaState();
    CHECK_STACK(L)
    lua_tinker::class_add<KGCSubject>( L, "KGCSubject" );

    lua_tinker::class_mem<KGCSubject>( L, "m_iBackAttackCnt",           &KGCSubject::m_iBackAttackCnt );
    lua_tinker::class_mem<KGCSubject>( L, "m_iSpecialCnt",              &KGCSubject::m_iSpecialCnt );
    lua_tinker::class_mem<KGCSubject>( L, "m_iDamageHittedCnt",         &KGCSubject::m_iDamageHittedCnt );
    lua_tinker::class_mem<KGCSubject>( L, "m_fDamageVolumeCnt",         &KGCSubject::m_fDamageVolumeCnt );
    lua_tinker::class_mem<KGCSubject>( L, "m_iDieCnt",                  &KGCSubject::m_iDieCnt );
    lua_tinker::class_mem<KGCSubject>( L, "m_bComboBreak",              &KGCSubject::m_bComboBreak );

    lua_tinker::class_def<KGCSubject>( L, "SetState",                   &KGCSubject::SetState );
    lua_tinker::class_def<KGCSubject>( L, "GetState",                   &KGCSubject::GetState );
    lua_tinker::class_def<KGCSubject>( L, "IsState",                    &KGCSubject::IsState );
    lua_tinker::class_def<KGCSubject>( L, "SetTempValue",               &KGCSubject::SetTempValue );
    lua_tinker::class_def<KGCSubject>( L, "GetTempValue",               &KGCSubject::GetTempValue );
    lua_tinker::class_def<KGCSubject>( L, "GetRewardInfo",              &KGCSubject::GetRewardInfo );
    lua_tinker::class_def<KGCSubject>( L, "EndSubject",                 &KGCSubject::EndSubject );
    lua_tinker::class_def<KGCSubject>( L, "AddUIParticle",              &KGCSubject::AddUIParticle );    
    lua_tinker::class_def<KGCSubject>( L, "GetCombo",                   &KGCSubject::GetCombo );
    lua_tinker::class_def<KGCSubject>( L, "GetAerialCombo",             &KGCSubject::GetAerialCombo );
    lua_tinker::class_def<KGCSubject>( L, "ResetCombo",                 &KGCSubject::ResetCombo );

    lua_tinker::class_def<KGCSubject>( L, "GetProgressNow",             &KGCSubject::GetProgressNow );
    lua_tinker::class_def<KGCSubject>( L, "GetProgressMax",             &KGCSubject::GetProgressMax );
    lua_tinker::class_def<KGCSubject>( L, "SetProgressNow",             &KGCSubject::SetProgressNow );
    lua_tinker::class_def<KGCSubject>( L, "SetProgressMax",             &KGCSubject::SetProgressMax );

}