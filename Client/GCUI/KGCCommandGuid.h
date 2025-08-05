/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

    ★ 설계자 ☞ 이상호
    ★ 설계일 ☞ 2013년 06월 30일
    ★ E-Mail ☞ shmhlove@kog.co.kr
    ★ 클래스 ☞ 커맨드 가이드

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/
#pragma once

class KD3DSizingBox;
class KGCCommandGuid : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCCommandGuid );
    /// Default constructor
    KGCCommandGuid( void );
    /// Default destructor
    virtual ~KGCCommandGuid( void );
    /// Copy constructor
    KGCCommandGuid( const KGCCommandGuid& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCommandGuid& operator=( const KGCCommandGuid& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreateComplete( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostChildDraw( void );

public:
    void RegisterLuaBind();
    void SetHardKeyUV();

private:
    int GetConvertToTexKeyID( int iKeyID );

public:
    void ClearCommand( bool bAllClear );
    void AddCommand(int iKeyID );

public:
    void SetCheckCommand( bool bCheck );

public:
    bool IsKeyON( int iKeyUID );
    void SetON( int iKeyUID );
    void SetOFF( int iKeyUID );
    void SetAllON();
    void SetAllOFF();

public:
    KD3DSizingBox         *m_pkBack;
    GCDeviceTexture       *m_pKeyTexture;

public:
    enum
    {
        KEY_TEX_UP = 0,
        KEY_TEX_DOWN,
        KEY_TEX_LEFT,
        KEY_TEX_RIGHT,
        KEY_TEX_Z,
        KEY_TEX_X,
        MAX_KEY_TEX,
    };

    // 하드하드한 키 텍스쳐 UV정보
    D3DXVECTOR4           m_vUV_KeyIn[MAX_KEY_TEX];
    D3DXVECTOR4           m_vUV_KeyOut[MAX_KEY_TEX];

public:
    bool                   m_bShow;
    std::vector<DWORD>     m_vecCommandList;
    std::map<DWORD, bool>  m_mapInputChecker;
    std::map<DWORD, int>   m_mapInputParticleChecker;
};

DEFINE_WND_FACTORY( KGCCommandGuid );
DECLARE_WND_FACTORY( KGCCommandGuid );
DECLARE_WND_FACTORY_NAME( KGCCommandGuid );