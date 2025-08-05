#pragma once

const int RANK_HEIGHT = 83;
const float RANK_CORRECTION_NUM = 0.003f;

class KGCPlayerRank : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCPlayerRank );
    /// Default constructor
    KGCPlayerRank( void );
    /// Default destructor
    virtual ~KGCPlayerRank( void );
    /// Copy constructor
    KGCPlayerRank( const KGCPlayerRank& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCPlayerRank& operator=( const KGCPlayerRank& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

    void SetPlayerRank( float fRankPoint );
    void PercentForDamage( IN float fRankPoint, OUT float& fPercent, OUT int& iIndex );
    void SetBackgroundImage( int iIndex );
    void ShowEffect( int iIndex );
    void ShowExpEffect(int iIndex);
private:
    int m_iPreRank;
    KD3DWnd*    m_pRankBack;
    KD3DWnd*    m_pRankShow;
    KD3DWnd*    m_pkRankImg;

    //텍스쳐의 기본 시작 좌표
    D3DXVECTOR2 m_vBaseTexStartAxis;

    //텍스쳐 두번째 이미지 시작 좌표
    D3DXVECTOR2 m_vSecondTexStartAxis;

    //텍스쳐 한 이미지의 사이즈
    D3DXVECTOR2 m_vTexSize;

    //그릴 텍스쳐의 좌표
    D3DXVECTOR2 m_vImageTexStartAxis;
    D3DXVECTOR2 m_vImageTexEndtAxis;

    //그릴 텍스쳐의 좌표
    D3DXVECTOR2 m_vBackImgTexStartAxis;
    D3DXVECTOR2 m_vBackImgTexEndtAxis;

    //그릴 UI위치
    D3DXVECTOR2 m_vImagePos;

    //텍스쳐 크기
    D3DXVECTOR2 m_vTextureFullSize;

protected:
    D3DXVECTOR4 m_vRankTexCoords;
    D3DXVECTOR2 m_vRankWndPos;
};

DEFINE_WND_FACTORY( KGCPlayerRank );
DECLARE_WND_FACTORY( KGCPlayerRank );
DECLARE_WND_FACTORY_NAME( KGCPlayerRank );
