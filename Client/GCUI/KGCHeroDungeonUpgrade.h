#pragma once

#include "GCHeroDungeonManager.h"

class KGCHeroItemUpgradeBox;
class KGCHeroItemUpgradeMaterialSelect;

class KGCHeroDungeonUpgrade :	public KD3DDialog,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCHeroDungeonUpgrade );
    /// Default constructor
    KGCHeroDungeonUpgrade( void );
    /// Default destructor
    virtual ~KGCHeroDungeonUpgrade( void );
    /// Copy constructor
    KGCHeroDungeonUpgrade( const KGCHeroDungeonUpgrade& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCHeroDungeonUpgrade& operator=( const KGCHeroDungeonUpgrade& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroy( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

private:
    enum { NUM_UPGRADE_BOX = 8 };

    // 캐릭터 선택 관련
private:
    KSafeArray<KD3DWnd*, GC_CHAR_NUM>  m_apkCharacterName;
    EGCCharType m_eCurrentCharacter;

    // 전체 페이지 관리 관련
private:
    KD3DWnd*    m_pkBtnPrevPage;
    KD3DWnd*    m_pkBtnNextPage;
    KD3DStatic* m_pkStaticPage;
    KD3DStatic* m_pkStaticHeroCoin;
    KD3DStatic* m_pkStaticHeroCoinPiece;

    int     m_iCurrentPage;
    int     m_iTotalPage;

    void OnPrevPage( void );
    void OnNextPage( void );
    void ResetPage( int iItemCount_ );

    // 영웅 주화, 주화 조각
private:
    KD3DWnd* m_pkHeroCoin;
    KD3DWnd* m_pkHeroCoinPiece;

    // 아이템 정보 저장
private:
    std::map< EGCCharType, std::vector< KHeroItemUpgradeInfo > > m_mapUpgrageInfo;  // < 캐릭터 타입, 제작 목록 >
    KSafeArray< KGCHeroItemUpgradeBox*, NUM_UPGRADE_BOX >        m_apkUpgradeBox;
    KGCHeroItemUpgradeMaterialSelect*                            m_pkMaterialSelectBox;

public:
    void SendItemUpgradeInfoReq( void );
    void SetItemUpgradeInfo( void );
    void UpdateCharacterInfo( void );
    
    int GetItemCount( EGCCharType eCharType_ );
    void SetUpgradeBoxInfo( void );
    void SetMyHeroCoin( void );

    void ShowMateriaBox( D3DXVECTOR2 dxvPos_, int iBoxNumber_, int iSlotNumber_, GCITEMID itemID_, int iGrade_ );
    void SetSelectMaterial( int iBoxNumber_, int iSlotNumber_, GCITEMUID itemUID_ );
    bool IsRenderOnMaterial( void );
    void CloseMaterialBox( void );

    void UptateUpgradeMaterial( void );
    bool GetUpgradeMaterialUID( IN const std::pair< GCITEMID, int > prKey_, OUT std::vector< GCITEMUID >& vecUIDList_ );
};

DEFINE_WND_FACTORY( KGCHeroDungeonUpgrade );
DECLARE_WND_FACTORY( KGCHeroDungeonUpgrade );
DECLARE_WND_FACTORY_NAME( KGCHeroDungeonUpgrade );