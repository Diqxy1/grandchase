#pragma once

class KGCCharacterName : public KD3DWnd    // extends
{
public:
    DECLARE_CLASSNAME( KGCCharacterName );
    /// Default constructor
    KGCCharacterName( void );
    /// Default destructor
    virtual ~KGCCharacterName( void );
    /// Copy constructor
    KGCCharacterName( const KGCCharacterName& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharacterName& operator=( const KGCCharacterName& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    void SetCharacterInfo( int iCharType, int iPromotion );
    virtual void PostChildDraw();
    void SetTexPos();
    bool GetCharacterTextureInfo( OUT int& iIndexNum , OUT int& iFileNum , IN int iCharType, IN int iPromotion );
private:
    GCDeviceTexture*    m_pNameTexture;
    //텍스쳐의 기본 시작 좌표
    D3DXVECTOR2 m_vBaseTexStartAxis;
    //텍스쳐 두번째 이미지 시작 좌표
    D3DXVECTOR2 m_vSecondTexStartAxis;
    //텍스쳐 한 이미지의 사이즈
    D3DXVECTOR2 m_vTexSize;
    //그릴 텍스쳐의 좌표
    D3DXVECTOR2 m_vImageTexStartAxis;
    D3DXVECTOR2 m_vImageTexEndtAxis;
    //그릴 UI위치
    D3DXVECTOR2 m_vImagePos;
    //텍스쳐 크기
    D3DXVECTOR2 m_vTextureFullSize;

};

DEFINE_WND_FACTORY( KGCCharacterName );
DECLARE_WND_FACTORY( KGCCharacterName );
DECLARE_WND_FACTORY_NAME( KGCCharacterName );
