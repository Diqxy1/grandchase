
#include "stdafx.h"
#include ".\gcsquarepet.h"


GCSquarePet::GCSquarePet(void)
: m_pPetMaster(NULL)
{
	// Jaeho 2008.09.26
	// std::wstring 이 있는데다가 memset이나 zeromemory 걸면 문제 생깁니다.
	// 주의 하세요.
	//ZeroMemory( &m_kPetInfo, sizeof(m_kPetInfo) );

    m_eType = SOT_PET;
}

GCSquarePet::~GCSquarePet(void)
{
    m_kPet.EndPet();
}

void GCSquarePet::SetMaster( GCSquarePeople* pPeople )
{
    if( m_pPetMaster == NULL )
    {
        m_pPetMaster = pPeople;
        SetSimplePetInfo( m_pPetMaster->GetPeopleInfo().m_kPetInfo );
    }
}

void GCSquarePet::FrameMove()
{
    if( m_pPetMaster != NULL )
    {
        SetRender( m_pPetMaster->GetRender() );
    }

    m_kPet.FrameMove();
}

void GCSquarePet::ShadowRender()
{
    if( GetRender() == false || GetInit() == false )
        return;

    m_kPet.RenderShadow();
}

void GCSquarePet::RenderPetName()
{
    if( GetRender() == false || GetInit() == false )
        return;

    m_kPet.RenderPetName();
}

bool GCSquarePet::Load()
{
    if( true == GetInit() )
        return true;

    m_kPet.EndPet();
    m_kPet.SetBaseInformation( g_kGlobalValue.GetPetIDFromItemID( m_kPetInfo.m_dwPetID / 10 ), (int)m_kPetInfo.m_cPetLV, (int)m_kPetInfo.m_cPetPromotion, m_kPetInfo.m_strPetName );
    m_kPet.CreateObject( true );
    m_kPet.SetOwnerInfo( m_pPetMaster->GetUID() );
    m_kPet.LoadPetModel( EGCPETLOCATION::GAME, m_kPetInfo.m_kCostume.m_dwID / 10 );
    int iPetID = g_kGlobalValue.GetPetIDFromItemID( m_kPetInfo.m_dwPetID/10 );
    m_kPet.LoadPetMotion( EGCPETLOCATION::GAME, iPetID , m_kPetInfo.m_cPetPromotion );
    m_kPet.LoadPetEffect();
    //m_kPet.LoadPetSound( m_kPetInfo.m_dwPetID, (int)m_kPetInfo.m_cPetPromotion );

    OnLoadingComplete();
    return true;
}

void GCSquarePet::OnLoadingComplete()
{	
    SetRender( true );
}

bool GCSquarePet::TimeShareLoading()
{
    if( true == GetInit() )
        return true;

    if( m_kPet.m_pObject == NULL )
    {
        m_kPet.EndPet();
        m_kPet.SetBaseInformation( g_kGlobalValue.GetPetIDFromItemID( m_kPetInfo.m_dwPetID / 10 ), (int)m_kPetInfo.m_cPetLV, (int)m_kPetInfo.m_cPetPromotion, m_kPetInfo.m_strPetName );
        m_kPet.CreateObject( true );
        m_kPet.SetOwnerInfo( m_pPetMaster->GetUID() );
        return false;
    }

    m_kLoadedState.RefreshTime();
    if( false == m_kLoadedState.m_bMotionLoad )
    {
        int iPetID = g_kGlobalValue.GetPetIDFromItemID( m_kPetInfo.m_dwPetID/10 );
        m_kPet.LoadPetMotion( EGCPETLOCATION::GAME, iPetID, m_kPetInfo.m_cPetPromotion );
        m_kLoadedState.m_bMotionLoad = true;
        return false;
    }

    m_kLoadedState.RefreshTime();
    if( false == m_kLoadedState.m_bModelLoad )
    {
        m_kPet.LoadPetModel( EGCPETLOCATION::GAME, m_kPetInfo.m_kCostume.m_dwID / 10 );
        m_kLoadedState.m_bModelLoad = true;
        return false;
    }

    m_kLoadedState.m_bEffectLoad = true;
    m_kLoadedState.m_bEffectLoad2 = true;
    m_kLoadedState.m_bCreateEffect = true;

    if( m_kLoadedState.IsComplete() )
    {
        OnLoadingComplete();
        m_kLoadedState.Init();
        return true;
    }

    return false;
}