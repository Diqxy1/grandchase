// 2011. 7. 18, shmhlove
#include "GCExtraMesh.h"

GCExtraMesh::GCExtraMesh()
{
    m_vecMeshInfo.clear();
}

GCExtraMesh::~GCExtraMesh(){}

void GCExtraMesh::LoadExtraMesh( GCObject* pObj_, int iPlayerIdx )
{
    std::vector< SExtraMeshInfo >::iterator vecMeshIter = m_vecMeshInfo.begin();
    for(; vecMeshIter != m_vecMeshInfo.end(); ++vecMeshIter)
    {
        if( NULL == vecMeshIter->pObj )
        {
            KGCObjectAndMotion* pRet = (KGCObjectAndMotion*)g_pGCDeviceManager2->CreateLiveModel( vecMeshIter->strModelFile.c_str(), vecMeshIter->strTextureFile.c_str(), vecMeshIter->strBaseMotionFile.c_str(), vecMeshIter->iBoneIdx, pObj_ );
            if( NULL == pRet )
                continue;

            pRet->m_strName = vecMeshIter->strModelFile;
            vecMeshIter->pObj = pObj_;
            pRet->SetRender( false );

            g_MyD3D->MyPlayer[ iPlayerIdx ]->SetMeshToggleState( false );
            g_MyD3D->MyPlayer[ iPlayerIdx ]->ToggleExtraMesh(pRet->m_strName.c_str(), false);
        }
    }
}

void GCExtraMesh::ClearMeshInfo()
{
    m_vecMeshInfo.clear();
}

bool GCExtraMesh::SetExtraMeshInfo( SExtraMeshInfo &MeshInfo )
{
    m_vecMeshInfo.push_back( MeshInfo );
    return true;
}

void GCExtraMesh::FrameMove( int iPlayerIdx )
{
    if( g_MyD3D->IsPlayerIndex( iPlayerIdx ) && g_MyD3D->m_pStateMachine->GetState() == GS_GAME )
    {
        // 조건검사
        std::vector< SExtraMeshInfo >::iterator vecMeshIter = m_vecMeshInfo.begin();
        for(int iLoop=0; iLoop < int(m_vecMeshInfo.size()); ++iLoop)
        {
            // 플레이어 모션에 따른 오브젝트 상태변화
            CheckConditionPlayerMotion( iLoop, iPlayerIdx );
        }
    }
}

void GCExtraMesh::CheckConditionPlayerMotion( int iMeshInfoIdx, int iPlayerIdx )
{
    if( 0 > iMeshInfoIdx || iMeshInfoIdx >= int(m_vecMeshInfo.size()) )
        return;

    if( false == g_MyD3D->IsPlayerIndex( iPlayerIdx) )
        return;

    // 플레이어 모션에 따른 오브젝트 상태변화
    std::vector< SEM_PlayerMotionCondition >::iterator vecPMIter = m_vecMeshInfo[iMeshInfoIdx].vecCondition_PM.begin();
    for(; vecPMIter != m_vecMeshInfo[iMeshInfoIdx].vecCondition_PM.end(); ++vecPMIter)
    {
        if( CONDITION_PLAYERMOTION == vecPMIter->iConditionID )
        {
            if( vecPMIter->iPlayerMotionID == g_MyD3D->MyPlayer[iPlayerIdx]->uiMotion )
            {
                // 무기를 떼주자!!
                if( vecPMIter->bWeaponToggle )
                {
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMeshAll( false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleBaseMesh( true );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "WEAPON" , false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "WEAPON_0", false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "WEAPON_1", false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "MERGE_WEAPON" , false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "GRANDARK_2ND" , false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "GRANDARK_2ND_1" , false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "GRANDARK_2ND_2" , false );
                    g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( "DOG_GRANDARK" , false );
                }

                // 메시를 붙히고 모션을 주자.
                if( false == vecPMIter->bEnable )
                {
                    vecPMIter->bEnable = true;
                    g_MyD3D->MyPlayer[iPlayerIdx]->SetChildMeshMotion( const_cast<char*>(vecPMIter->strObjName.c_str()), const_cast<char*>(vecPMIter->strPC_MotionName.c_str()), true, true, false );
                }
                
                g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( vecPMIter->strObjName.c_str(), true );
                break;
            }
            else
            {
                // 메시를 떼주자.
                vecPMIter->bEnable = false;
                g_MyD3D->MyPlayer[iPlayerIdx]->ToggleExtraMesh( vecPMIter->strObjName.c_str(), false );
            }
        }
    }
}