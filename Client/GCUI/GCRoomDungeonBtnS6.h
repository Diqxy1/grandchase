#pragma once

class KGCRoomDungeonBtnS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomDungeonBtnS6 );
    /// Default constructor
    KGCRoomDungeonBtnS6( void );
    /// Default destructor
    virtual ~KGCRoomDungeonBtnS6( void );
    /// Copy constructor
    KGCRoomDungeonBtnS6( const KGCRoomDungeonBtnS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomDungeonBtnS6& operator=( const KGCRoomDungeonBtnS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	void SetDungeonID( int iID );
	void SetBossImage( );
	void PostDraw();
	void SetTexPos(bool bInMouse );
	void FrameMoveInEnabledState( void );
	void OnClick( );
private:
	int m_iDungeonID;

	int m_iIndex;
	bool m_bChangeState;


	KD3DWnd *m_pkHoverBox;

	GCDeviceTexture*    m_pBossTexture;

	//텍스쳐의 기본 시작 좌표
	D3DXVECTOR2 m_vBaseTexStartAxis;

	//텍스쳐 두번째 이미지 시작 좌표
	D3DXVECTOR2 m_vSecondTexStartAxis;

	//텍스쳐 세번째 이미지 시작 좌표
	D3DXVECTOR2 m_vThirdTexStartAxis;

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

DEFINE_WND_FACTORY( KGCRoomDungeonBtnS6 );
DECLARE_WND_FACTORY( KGCRoomDungeonBtnS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomDungeonBtnS6 );
