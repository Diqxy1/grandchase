#pragma once

class KGCUINumber;

class KGCMonsterCardContents: public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMonsterCardContents );
    /// Default constructor
    KGCMonsterCardContents( void );
    /// Default destructor
    virtual ~KGCMonsterCardContents( void );
    /// Copy constructor
    KGCMonsterCardContents( const KGCMonsterCardContents& )
    { ASSERT( false && "Do not use copy constructor !" ); }
    /// Assignment operator
    const KGCMonsterCardContents& operator=( const KGCMonsterCardContents& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

    enum {
        COMMA_NUM = 3,//콤마의 개수
        PRIME_NUM = 4,//소수점 3자리 수까지 나타내기 위해 만든 숫자컨트롤의 개수
    };
private:
    
    KGCItemImgWnd*              m_pkMonsterCardImg;

    KD3DWnd*                    m_pkMonsterCardPlus;
    KD3DWnd*                    m_pkMonsterCardMinus;
    KD3DWnd*                    m_pkMonsterCardAttributeShort;
    KD3DWnd*                    m_pkMonsterCardAttributeLong;
    KD3DWnd*                    m_pkMonsterCardPercent;
    KD3DWnd*                    m_pkMonsterCardLV;
    KD3DWnd*                    m_pkMonsterCardComma;

    KGCUINumber*                m_pkMonsterCardNum;
    KGCUINumber*                m_pkMonsterCardLVNum;
    KGCUINumber*		        m_pkMonsterCardDurationNum;
    
    KGCUINumber*                m_pkMonsterCardPrimeNum[PRIME_NUM];
    


    DWORD                       m_dItemMonsterCardID;
    int                         m_iDuration;
    int                         m_iOrderComma;
    float                       m_fSignDecimalOffset;   
    float                       m_fSignOffset;   
    bool                        m_bDecimal;
    bool                        m_bShortAttribute;
    bool                        m_bPlusValue;

    D3DXVECTOR2                 m_vecPercentPos;
    D3DXVECTOR2                 m_vecCommaPos;
    D3DXVECTOR2                 m_vecPlusPos;
    D3DXVECTOR2                 m_vecMinusPos;
    D3DXVECTOR2                 m_vecNumPos;
    
   

public:
    std::vector<D3DXVECTOR2> InitMonsterCardSignPos();
    void SetMonsterCardSignPos(std::vector<D3DXVECTOR2> vecpos);
    void AllOff();
    
    void SetMonsterCardImg();
    void SetMonsterCardAttribute();
    void SetMonsterCardContents();
    void SetMonsterCardNum();
    void SetMonsterCardLv();
    void SetMonsterCardLvTexCoord(int iGrade);
    
    void SetMonsterCardNumScale( float fscale );
    void SetMonsterCardLVNumScale( float fscale );
    void SetMonsterCardDurationNumScale( float fscale );
    void SetMonsterCardPrimeNumScale( float fscale );
    void SetMonsterCardAllNumShadow(bool bShadow);
    void SetMonsterCardGeneralSign();
    void SetMonsterCardSignOffSet(float ioffset);
    
    DWORD GetMonsterCardID();
    int GetCommaOrder();

    void SetDuration(int iduration);
    int GetDuration();
    void SetMonsterCardItemID(DWORD dmoncardID);

    void ShowDuration( bool bshow );
    KGCAttributeTable GetAttributeTable();
    float  GetValuebyMonsterCardID();
    D3DXVECTOR4  GetMonsterCardAttributeTexCoord();

    bool GetMonsterCardImgRenderOn();
    void SetMonsterCardDecimalSign();
    void SetMonsterCardDecimalSignOffSet(float ioffset);

    int GetMonsterCardGrade(DWORD dwItemID);
    void SetMonsterCardLVPos(float x, float y);
};

DEFINE_WND_FACTORY( KGCMonsterCardContents );
DECLARE_WND_FACTORY( KGCMonsterCardContents );
DECLARE_WND_FACTORY_NAME( KGCMonsterCardContents );
