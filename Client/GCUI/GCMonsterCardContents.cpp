

#include "stdafx.h"
#include "GCMonsterCardContents.h"

IMPLEMENT_CLASSNAME( KGCMonsterCardContents );
IMPLEMENT_WND_FACTORY( KGCMonsterCardContents );
IMPLEMENT_WND_FACTORY_NAME( KGCMonsterCardContents, "gc_monstercardcontents" );


KGCMonsterCardContents::KGCMonsterCardContents( void )
: m_pkMonsterCardImg( NULL )
, m_pkMonsterCardPlus( NULL )
, m_pkMonsterCardAttributeShort( NULL )
, m_pkMonsterCardAttributeLong( NULL )
, m_pkMonsterCardPercent( NULL )
, m_pkMonsterCardLV( NULL )
, m_pkMonsterCardNum( NULL )
, m_pkMonsterCardLVNum(NULL)
, m_pkMonsterCardComma(NULL)
, m_pkMonsterCardMinus(NULL)
, m_pkMonsterCardDurationNum(0)
, m_dItemMonsterCardID(0)
, m_iOrderComma(1)
, m_bDecimal(false)
, m_bShortAttribute(true)
, m_bPlusValue(true)
, m_fSignDecimalOffset(5.f)
, m_fSignOffset(13.f)
{
    
    LINK_CONTROL( "monstercard_img", m_pkMonsterCardImg );
    LINK_CONTROL( "monstercard_plus", m_pkMonsterCardPlus );
    LINK_CONTROL( "monstercard_minus", m_pkMonsterCardMinus );
    LINK_CONTROL( "monstercard_attribute_short", m_pkMonsterCardAttributeShort );
    LINK_CONTROL( "monstercard_attribute_long", m_pkMonsterCardAttributeLong );
    LINK_CONTROL( "monstercard_percent", m_pkMonsterCardPercent );
    LINK_CONTROL( "monstercard_comma", m_pkMonsterCardComma );

    LINK_CONTROL( "monstercard_lv", m_pkMonsterCardLV );
    LINK_CONTROL( "monstercard_num", m_pkMonsterCardNum );
    LINK_CONTROL( "monstercard_lv_num", m_pkMonsterCardLVNum );
    LINK_CONTROL( "monstercard_duration", m_pkMonsterCardDurationNum );
    

    for( int i = 0 ; i < PRIME_NUM; ++i )
    { 
        char szTemp[MAX_PATH] = "";
        m_pkMonsterCardPrimeNum[i] = NULL;
        sprintf_s( szTemp, MAX_PATH, "monstercard_primenum%d", i );
        LINK_CONTROL( szTemp, m_pkMonsterCardPrimeNum[i] );
    }

    

    
}

KGCMonsterCardContents::~KGCMonsterCardContents( void )
{
}


void KGCMonsterCardContents::OnCreate( void )
{
    //일반 
    m_pkMonsterCardImg->InitState( true );
    m_pkMonsterCardImg->SetLikeBtn( true );
    m_pkMonsterCardPlus->InitState( false, false, this );
    m_pkMonsterCardMinus->InitState( false, false, this );
    m_pkMonsterCardAttributeShort->InitState( false, false, this );
    m_pkMonsterCardAttributeLong->InitState( false, false, this );
    m_pkMonsterCardPercent->InitState( false, false, this );
    m_pkMonsterCardLV->InitState( false, false, this );
    m_pkMonsterCardComma->InitState( false, false, this );

    //숫자 
    m_pkMonsterCardNum->InitState( false );
    m_pkMonsterCardNum->SetStringType( "monstercard_value_num" );
    m_pkMonsterCardNum->SetInterval(15.f);
    m_pkMonsterCardNum->SetShadow( false );

    m_pkMonsterCardLVNum->InitState( false );
    m_pkMonsterCardLVNum->SetStringType( "monstercard_lv_black_num");
    m_pkMonsterCardLVNum->SetInterval(15.f);
    m_pkMonsterCardLVNum->SetShadow( false );

    m_pkMonsterCardDurationNum->InitState( false );
    m_pkMonsterCardDurationNum->SetStringType( "blue_gradation_num" );
    m_pkMonsterCardDurationNum->SetShadow( false );
    m_pkMonsterCardDurationNum->SetWriteAlign( ALIGN_RIGHT_SIDE);

    for( int i = 0 ; i < PRIME_NUM; ++i )
    {
        m_pkMonsterCardPrimeNum[i]->InitState( false );
        m_pkMonsterCardPrimeNum[i]->SetStringType( "monstercard_value_num");
        m_pkMonsterCardPrimeNum[i]->SetScale( 0.3f );
        m_pkMonsterCardPrimeNum[i]->SetInterval(15.f);
        m_pkMonsterCardPrimeNum[i]->SetShadow( false );
        m_pkMonsterCardPrimeNum[i]->SetWriteAlign( ALIGN_LEFT_SIDE);
    }
    
    
}
std::vector<D3DXVECTOR2> KGCMonsterCardContents::InitMonsterCardSignPos()
{
    std::vector< D3DXVECTOR2 > vecMonsterCardPos;
    
    vecMonsterCardPos.push_back(m_pkMonsterCardPercent->GetFixedWindowLocalPos());
    vecMonsterCardPos.push_back(m_pkMonsterCardComma->GetFixedWindowLocalPos());
    vecMonsterCardPos.push_back(m_pkMonsterCardPlus->GetFixedWindowLocalPos());
    vecMonsterCardPos.push_back(m_pkMonsterCardNum->GetFixedWindowLocalPos());
    vecMonsterCardPos.push_back(m_pkMonsterCardMinus->GetFixedWindowLocalPos());

    return vecMonsterCardPos;
}

void KGCMonsterCardContents::SetMonsterCardSignPos(std::vector<D3DXVECTOR2> vecpos)
{
    m_vecPercentPos = vecpos.at(0);
    m_vecCommaPos= vecpos.at(1);
    m_vecPlusPos= vecpos.at(2);
    m_vecNumPos= vecpos.at(3);
    m_vecMinusPos = vecpos.at(4);
}

void KGCMonsterCardContents::AllOff()
{
    //일반 
    m_pkMonsterCardImg->ToggleRender( false );
    m_pkMonsterCardPlus->ToggleRender( false );
    m_pkMonsterCardMinus->ToggleRender( false );
    m_pkMonsterCardAttributeShort->ToggleRender( false );
    m_pkMonsterCardAttributeLong->ToggleRender( false );
    m_pkMonsterCardPercent->ToggleRender( false );
    m_pkMonsterCardLV->ToggleRender( false );
    m_pkMonsterCardComma->ToggleRender( false );

    //숫자 
    m_pkMonsterCardNum->ToggleRender( false );
    m_pkMonsterCardLVNum->ToggleRender( false );
    m_pkMonsterCardDurationNum->ToggleRender( false );
    
    m_pkMonsterCardNum->SetNumber( 0 );
    m_pkMonsterCardLVNum->SetNumber( 0 );
    m_pkMonsterCardDurationNum->SetNumber( 0 );
    for( int i = 0 ; i < PRIME_NUM; ++i )
    {
        m_pkMonsterCardPrimeNum[i]->ToggleRender( false );
        m_pkMonsterCardPrimeNum[i]->SetNumber( 0 );
    }

    m_dItemMonsterCardID = 0;
    m_iDuration = 0;
    m_iOrderComma = 1;
    m_bDecimal = false;
}
void KGCMonsterCardContents::SetMonsterCardAllNumShadow(bool bShadow)
{
    m_pkMonsterCardNum->SetShadow( bShadow );
    m_pkMonsterCardLVNum->SetShadow( bShadow );
    m_pkMonsterCardDurationNum->SetShadow( bShadow );

    for( int i = 0 ; i < PRIME_NUM; ++i )
        m_pkMonsterCardPrimeNum[i]->SetShadow( bShadow );
}
void KGCMonsterCardContents::ShowDuration( bool bshow )
{
    m_pkMonsterCardDurationNum->ToggleRender( bshow );
}

void KGCMonsterCardContents::SetMonsterCardAttribute()
{
    D3DXVECTOR4 vMonsterAttributeTexCoord = GetMonsterCardAttributeTexCoord();   
    if( D3DXVECTOR4(0,0,0,0) != vMonsterAttributeTexCoord )
    {
        if( m_bShortAttribute )
        {
            m_pkMonsterCardAttributeShort->SetWndMode(D3DWM_DEFAULT);
            m_pkMonsterCardAttributeShort->SetWndTex(D3DWM_DEFAULT, 350);
            m_pkMonsterCardAttributeShort->SetWndTexCoords(D3DWM_DEFAULT,vMonsterAttributeTexCoord );
            m_pkMonsterCardAttributeShort->ToggleRender( true );
            m_pkMonsterCardAttributeLong->ToggleRender( false );
        }
        else 
        {
            m_pkMonsterCardAttributeLong->SetWndMode(D3DWM_DEFAULT);
            m_pkMonsterCardAttributeLong->SetWndTex(D3DWM_DEFAULT, 350);
            m_pkMonsterCardAttributeLong->SetWndTexCoords(D3DWM_DEFAULT,vMonsterAttributeTexCoord );
            m_pkMonsterCardAttributeLong->ToggleRender( true );
            m_pkMonsterCardAttributeShort->ToggleRender( false );
        }

        
    }
}
 
void KGCMonsterCardContents::SetMonsterCardLv()
{
    m_pkMonsterCardLVNum->SetWriteAlign( ALIGN_LEFT_SIDE );
    m_pkMonsterCardLVNum->SetScale(0.3f);
    m_pkMonsterCardLVNum->SetShadow( false );

    int iMonsterCardGrade = GetMonsterCardGrade( GetMonsterCardID() );
    if( iMonsterCardGrade == 0)
    {
        m_pkMonsterCardLVNum->SetStringType( "monstercard_lv_black_num");
        SetMonsterCardLvTexCoord( 0 );
    }
    else if( iMonsterCardGrade == 1)
    {
        m_pkMonsterCardLVNum->SetStringType( "monstercard_lv_puple_num");
        SetMonsterCardLvTexCoord( 1 );
    }
    else if( iMonsterCardGrade == 2)
    {
        m_pkMonsterCardLVNum->SetStringType( "monstercard_lv_orange_num");
        SetMonsterCardLvTexCoord( 2 );
        
    }
    else if( iMonsterCardGrade == 3)
    {
        m_pkMonsterCardLVNum->SetStringType( "monstercard_lv_gold_num");
        SetMonsterCardLvTexCoord( 3 );
    }

    
    m_pkMonsterCardLVNum->SetNumber(g_pItemMgr->GetEquipLevelFromMonsterCard(GetMonsterCardID()));
    m_pkMonsterCardLVNum->ToggleRender( true );
    m_pkMonsterCardLV->ToggleRender( true );
}
void KGCMonsterCardContents::SetMonsterCardDecimalSignOffSet(float ioffset)
{
    m_fSignDecimalOffset = ioffset;
}
void KGCMonsterCardContents::SetMonsterCardSignOffSet(float ioffset)
{
    m_fSignOffset = ioffset;
}
void KGCMonsterCardContents::SetMonsterCardGeneralSign()
{
    if( m_bShortAttribute )
    {
        m_pkMonsterCardPlus->SetWindowPosDirectFloat(m_vecPlusPos.x , m_vecPlusPos.y);
        m_pkMonsterCardMinus->SetWindowPosDirectFloat(m_vecMinusPos.x , m_vecMinusPos.y);
        m_pkMonsterCardNum->SetWindowPosDirectFloat(m_vecNumPos.x , m_vecNumPos.y);
    }
    else
    {
        m_pkMonsterCardPlus->SetWindowPosDirectFloat(m_vecPlusPos.x + m_fSignOffset, m_vecPlusPos.y);
        m_pkMonsterCardMinus->SetWindowPosDirectFloat(m_vecMinusPos.x + m_fSignOffset, m_vecMinusPos.y);
        m_pkMonsterCardNum->SetWindowPosDirectFloat(m_vecNumPos.x + m_fSignOffset, m_vecNumPos.y);
    }
}
void KGCMonsterCardContents::SetMonsterCardDecimalSign()
{
    if( m_bDecimal )
    {
        if( m_iOrderComma == 1 )
        {
            m_pkMonsterCardPercent->SetWindowPosDirectFloat(m_vecPercentPos.x , m_vecPercentPos.y);
            m_pkMonsterCardComma->SetWindowPosDirectFloat(m_vecCommaPos.x , m_vecCommaPos.y);
        }
        else if(m_iOrderComma == 2 )
        {
            m_pkMonsterCardPercent->SetWindowPosDirectFloat(m_vecPercentPos.x + m_fSignDecimalOffset, m_vecPercentPos.y);
            m_pkMonsterCardComma->SetWindowPosDirectFloat(m_vecCommaPos.x + m_fSignDecimalOffset, m_vecCommaPos.y);
        }
        else if(m_iOrderComma == 3 )
        {
            m_pkMonsterCardPercent->SetWindowPosDirectFloat(m_vecPercentPos.x + (m_fSignDecimalOffset * 2.f), m_vecPercentPos.y);
            m_pkMonsterCardComma->SetWindowPosDirectFloat(m_vecCommaPos.x + (m_fSignDecimalOffset * 2.f), m_vecCommaPos.y);
        }

        m_pkMonsterCardPercent->ToggleRender( true );
        m_pkMonsterCardComma->ToggleRender( true );
    }
    else
    {
        m_pkMonsterCardComma->ToggleRender( false );
        m_pkMonsterCardPercent->ToggleRender( false );
    }
}
void KGCMonsterCardContents::SetMonsterCardContents()
{
    //AllOff();
    
    SetMonsterCardImg();

    KGCAttributeTable attributetable = GetAttributeTable();
    if( !attributetable.m_vecAttributeList.empty() ) {
        SetMonsterCardLv();
        SetMonsterCardAttribute();
        SetMonsterCardNum();
        SetMonsterCardDecimalSign();
        SetMonsterCardGeneralSign();

        if( m_bPlusValue )
        {
            m_pkMonsterCardPlus->ToggleRender( true );
            m_pkMonsterCardMinus->ToggleRender( false );
        }
        
        else
        {
            m_pkMonsterCardPlus->ToggleRender( false );
            m_pkMonsterCardMinus->ToggleRender( true );
        }
    }
}

void KGCMonsterCardContents::SetMonsterCardNum()
{
    
    float fValue = GetValuebyMonsterCardID();

    int iStoreCriticalRate[3] = {0,};

    if( fValue > 0.f)
        m_bPlusValue =  true;
    else 
        m_bPlusValue =  false;

    if( m_bDecimal )
    {
        if( fValue  < 1.f)
        {
            fValue *= 1000.f;
            int temp = (int)fValue;

            //0.001일 경우
            if( fValue>= 1.f && fValue < 10.f )
            {
                m_pkMonsterCardPrimeNum[0]->SetNumber(0);
                m_pkMonsterCardPrimeNum[1]->SetNumber(0);
                m_pkMonsterCardPrimeNum[2]->SetNumber(0);
                m_pkMonsterCardPrimeNum[3]->SetNumber(temp);

            }
            if( fValue>= 10.f && fValue< 100.f )
            {
                m_pkMonsterCardPrimeNum[0]->SetNumber(0);
                m_pkMonsterCardPrimeNum[1]->SetNumber(0);
                m_pkMonsterCardPrimeNum[2]->SetNumber( temp / 10);
                m_pkMonsterCardPrimeNum[3]->SetNumber( temp % 10);

            } 
            if( fValue>= 100.f && fValue< 1000.f )
            {
                m_pkMonsterCardPrimeNum[0]->SetNumber(0);
                m_pkMonsterCardPrimeNum[1]->SetNumber( temp / 100);
                m_pkMonsterCardPrimeNum[2]->SetNumber( (temp % 100)/10);
                m_pkMonsterCardPrimeNum[3]->SetNumber( ( temp % 100)%10);

            }
            
            for(int i = 0; i < PRIME_NUM -1 ; i ++)
                m_pkMonsterCardPrimeNum[i]->ToggleRender( true );
        }
        else
        {
            if(fValue * 100.f > 100.f && fValue * 100.f < 1000.f)
            {
                m_iOrderComma = 1;
            }
            else if(fValue * 100.f > 1000.f && fValue * 100.f < 10000.f)
            {
                m_iOrderComma = 2;
            }
            else if(fValue * 100.f > 10000.f)
            {
                m_iOrderComma = 3;
            }
            m_pkMonsterCardNum->SetNumber( (int)(fValue * 100) );   

            for(int i = 0; i < PRIME_NUM -1 ; i ++)
                m_pkMonsterCardPrimeNum[i]->ToggleRender( false );
        }
    }
    else
    {
        m_pkMonsterCardNum->SetNumber( (int)fValue );   
        for(int i = 0; i < PRIME_NUM -1 ; i ++)
            m_pkMonsterCardPrimeNum[i]->ToggleRender( false );
    }
    
    m_pkMonsterCardNum->SetWriteAlign( ALIGN_LEFT_SIDE );
    m_pkMonsterCardNum->SetScale( 0.3f );
    m_pkMonsterCardNum->SetShadow( false );

    if( m_pkMonsterCardPrimeNum[0]->IsRenderOn() )
        m_pkMonsterCardNum->ToggleRender( false );
    else
    {
        m_pkMonsterCardNum->ToggleRender( true );
    }
}

void KGCMonsterCardContents::ActionPerformed( const KActionEvent& event )
{
}

void KGCMonsterCardContents::PostChildDraw( void )
{
    
}

KGCAttributeTable KGCMonsterCardContents::GetAttributeTable()
{
    //몬스터 카드 아이디를 넣어서 어트리뷰트 아이디를 얻어온다.
    KGCAttributeTable attributetable;
    GCItem* pItem = g_pItemMgr->GetItemData( GetMonsterCardID() );
    if ( !pItem )
        return attributetable;

    g_pItemMgr->GetAttributeTable(g_pItemMgr->GetAttributeTableID( GetMonsterCardID()),pItem->cItemGrade,attributetable);
    return attributetable;
}

float  KGCMonsterCardContents::GetValuebyMonsterCardID()
{
    KGCAttributeTable attributetable= GetAttributeTable();    
    std::vector<KGCAttributeTable::SAttribute>::iterator itr = attributetable.m_vecAttributeList.begin();

    if( itr != attributetable.m_vecAttributeList.end() )
        return itr->m_fValue;
    else
        return 0.0f;
}

void  KGCMonsterCardContents::SetMonsterCardLvTexCoord(int iGrade)
{
    D3DXVECTOR4 vMonsterAbilityTexCoord;

    if( 0 == iGrade )
    {
         vMonsterAbilityTexCoord = D3DXVECTOR4(2.f,323.f,46.f,350.f)/512.f;
    }
    else if( 1 == iGrade )
    {
        vMonsterAbilityTexCoord = D3DXVECTOR4(2.f,291.f,46.f,318.f)/512.f;
    }
    else if( 2 == iGrade )
    {
        vMonsterAbilityTexCoord = D3DXVECTOR4(2.f,259.f,46.f,286.f)/512.f;
    }
    else if( 3 == iGrade )
    {
        vMonsterAbilityTexCoord = D3DXVECTOR4(2.f,227.f,46.f,254.f)/512.f;
    }
    else
    {
        return;
    }
    
    m_pkMonsterCardLV->SetWndMode(D3DWM_DEFAULT);
    m_pkMonsterCardLV->SetWndTex(D3DWM_DEFAULT, 350);
    m_pkMonsterCardLV->SetWndTexCoords(D3DWM_DEFAULT,vMonsterAbilityTexCoord );
}
D3DXVECTOR4  KGCMonsterCardContents::GetMonsterCardAttributeTexCoord()
{
    KGCAttributeTable attributetable = GetAttributeTable();    
    if( attributetable.GetIndex()== -1)
          return D3DXVECTOR4(0,0,0,0);
    std::vector<KGCAttributeTable::SAttribute>::iterator itr = attributetable.m_vecAttributeList.begin();

    m_bDecimal = false;
     m_iOrderComma = 1;
    switch(itr->m_iType)
    {
    case ATTRTYPE_ATK_POINT:
        m_bShortAttribute = true;
        return D3DXVECTOR4(2.f, 2.f, 107.f, 29.f) / 512.f;

    case ATTRTYPE_DEF_POINT:
        m_bShortAttribute = true;
        return D3DXVECTOR4(2.f, 34.f, 107.f, 61.f) / 512.f;

    case ATTRTYPE_HP_POINT:
        m_bShortAttribute = true;
        return D3DXVECTOR4(2.f, 66.f, 107.f, 93.f) / 512.f;

    case ATTRTYPE_ALL_STAT_POINT:
        m_bShortAttribute = true;
        return D3DXVECTOR4(212.f, 2.f, 317.f, 29.f) / 512.f;

    case ATTRTYPE_SPECIAL_ATK_POINT:
        m_bShortAttribute = false;
        return D3DXVECTOR4(2.f, 130.f, 162.f, 157.f) / 512.f;

    case ATTRTYPE_SPECIAL_DEF_POINT:
        m_bShortAttribute = false;
        return D3DXVECTOR4(212.f, 33.f, 372.f, 62.f) / 512.f;

    case ATTRTYPE_MP_RECOVERY_RATE:
        m_bShortAttribute = false;
        m_bDecimal = true;
        return D3DXVECTOR4(2.f, 98.f, 162.f, 125.f) / 512.f;

    case ATTRTYPE_CRITICAL_ATTACK_RATE:
        m_bShortAttribute = false;
        m_bDecimal = true;
        return D3DXVECTOR4(2.f, 162.f, 162.f, 189.f) / 512.f;

    case ATTRTYPE_CRITICAL_DAMAGE_UP:
        m_bShortAttribute = false;
        m_bDecimal = true;
        return D3DXVECTOR4(2.f, 194.f, 162.f, 221.f) / 512.f;

    case ATTRTYPE_MP_DRAIN_RATE:
        m_bShortAttribute = true;
        m_bDecimal = false;
        return D3DXVECTOR4(212.f, 34.f, 345.f, 61.f) / 512.f;

    case ATTRTYPE_HP_RECOVERY_RATE:
        m_bShortAttribute = true;
        m_bDecimal = false;
        return D3DXVECTOR4(211.f, 65.f, 372.f, 93.f) / 512.f;

    case ATTRTYPE_ELYOS_RESIST:
        m_bShortAttribute = false;
        return D3DXVECTOR4(210.f, 96.f, 373.f, 126.f) / 512.f;

    case ATTRTYPE_DARK_DAMAGE_ATK_POINT:
        m_bShortAttribute = true;
        m_bDecimal = false;
        return D3DXVECTOR4(211.f, 129.f, 372.f, 157.f) / 512.f;

    }
    return D3DXVECTOR4(0,0,0,0);


}
void KGCMonsterCardContents::SetMonsterCardItemID(DWORD dmoncardID)
{
    m_dItemMonsterCardID = dmoncardID;
}
DWORD KGCMonsterCardContents::GetMonsterCardID()
{
    return m_dItemMonsterCardID;
}

void KGCMonsterCardContents::SetMonsterCardImg()
{
    m_pkMonsterCardImg->SetItemInfo( GetMonsterCardID() );
    m_pkMonsterCardImg->ToggleRender( true );
}
bool KGCMonsterCardContents::GetMonsterCardImgRenderOn()
{
    if( true == m_pkMonsterCardImg->IsRenderOn() )
        return true;
    else
        return false;
}

void KGCMonsterCardContents::SetDuration(int iduration)
{
    m_iDuration = iduration;
    m_pkMonsterCardDurationNum->SetNumber( iduration );
    m_pkMonsterCardDurationNum->ToggleRender( true );
}
int KGCMonsterCardContents::GetDuration()
{
    return m_iDuration;
}

int KGCMonsterCardContents::GetMonsterCardGrade(DWORD dwItemID)
{
    GCItem *pkItemInfo = g_MyD3D->m_kItemMgr.GetItemData( dwItemID );

    return pkItemInfo->cItemGrade;
}


void KGCMonsterCardContents::SetMonsterCardLVNumScale( float fscale )
{
       m_pkMonsterCardLVNum->SetScale( fscale );
}
void KGCMonsterCardContents::SetMonsterCardDurationNumScale( float fscale )
{
        m_pkMonsterCardDurationNum->SetScale( fscale );
}
void KGCMonsterCardContents::SetMonsterCardNumScale( float fscale )
{
    m_pkMonsterCardNum->SetScale( fscale );
}
void KGCMonsterCardContents::SetMonsterCardPrimeNumScale( float fscale )
{
    for( int i = 0 ; i < PRIME_NUM; ++i )
        m_pkMonsterCardPrimeNum[i]->SetScale( fscale );
}
