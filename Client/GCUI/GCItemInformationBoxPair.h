#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCItemInformationBox;
class KGCItemInformationBreakupBox;
class KGCCoordiItemInformationBox;

class KGCItemInformationBoxPair : public KD3DWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME(KGCItemInformationBoxPair);
    /// Default constructor
    KGCItemInformationBoxPair(void);
    /// Default destructor
    virtual ~KGCItemInformationBoxPair(void);
    /// Copy constructor
    KGCItemInformationBoxPair(const KGCItemInformationBoxPair&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCItemInformationBoxPair& operator=(const KGCItemInformationBoxPair&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

public:
    virtual void FrameMoveInEnabledState(void);
    virtual void ActionPerformed(const KActionEvent& event);

protected:
    virtual void OnCreate(void);
    virtual void OncreateComplete(void);
    virtual void PostDraw();
    virtual void PostChildDraw();
    virtual void BindToLua();

private:
    KGCItemInformationBox* m_pkPointedItemInformationBox;
    KGCItemInformationBox* m_pkEquippedItemInformationBox;
    KGCItemInformationBreakupBox* m_pkBreakupItemInformationBox;
    KGCCoordiItemInformationBox* m_pkCoordiItemInformationBox;

public:
    void PostDrawEquip();
    void PostDrawBreakUP();
    void PostDrawPointItem();
    void PostDrawCoordiItem();


    void SetOriginPosition(D3DXVECTOR2 vPos_);
    void SetPointedItemInformation(GCItem* pItem_, char cPetPromotion_ = -1, KItem* pInven_ = NULL, SUserItem* pUserItem_ = NULL, KPetInfo* pPetInfo_ = NULL, char cItemGrade_ = -1, int iLevel_ = -1);
    void SetEquippedItemInformation();
    void SetDisplayState(bool bAlone_, bool bCoordi = false);
    int MakeCompareStandardValue(GCItem* pItem_, int iCharacterLevel_, KItem* pInventory_, SUserItem* pUserItem_, KPetInfo* pPetInfo_);
    virtual DWORD GetWidth() const;
    virtual DWORD GetHeight() const;
    DWORD GetEquippedItemInformationWidth();
    DWORD GetEquippedItemInformationHeight();
    void SetShowItemInfoBox();
    void SetHeroItemMode(bool bHero) { m_bHeroItemCheck = bHero; }

    bool IsCoordiItem(GCItem* pItem_, KItem* pInven_ = NULL);

private:
    enum
    {
        SHOW_BOX_GAP = 300,
        SHOW_BOX_ALPHA = 16,
    };
    bool    m_bEnableEquippedItemInformationBox;
    int     m_iPointedItemAbility;
    int     m_iEquippedItemAbility;;
    D3DXVECTOR2     m_vOriginPos;
    bool    m_bShowExtraInfo;

    int     m_iAlphaValue;
    DWORD   m_dwStartGapCount, m_dwEndGapCount;
    GCITEMUID   m_dwCheckUID;

    bool    m_bHeroItemCheck;
    bool    m_bCoordiItem;      //  ÄÚµð ¾ÆÀÌÅÛÀÌ´Ù.

public:
    //·ç¾Æ·Î ¹ÙÀÎµùÇÏ±â À§ÇØ ¸¸µç ÇÔ¼ö
    void SetPointedItemInfoByItemUID(KD3DWnd* pWnd_, GCITEMUID ItemUID_);
    void SetPointedItemInfoByItemID(KD3DWnd* pWnd_, GCITEMID ItemID_);
    void SetPointedItemInfoByItemIDAndLevel(KD3DWnd* pWnd_, GCITEMID ItemID_, int iLevel = -1);
    D3DXVECTOR2 GetNewPosition(KD3DWnd* pWnd_);
    void DrawItemInfoBox(DWORD dwGoodsID, D3DXVECTOR2 vPos);
    void DrawItemInfoBoxByScale(DWORD dwGoodsID, D3DXVECTOR2 vPos);
};

DEFINE_WND_FACTORY(KGCItemInformationBoxPair);
DECLARE_WND_FACTORY(KGCItemInformationBoxPair);
DECLARE_WND_FACTORY_NAME(KGCItemInformationBoxPair);
