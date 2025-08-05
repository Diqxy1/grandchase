#pragma once

class KGCRoomUserPotionS6 : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRoomUserPotionS6 );
    /// Default constructor
    KGCRoomUserPotionS6( void );
    /// Default destructor
    virtual ~KGCRoomUserPotionS6( void );
    /// Copy constructor
    KGCRoomUserPotionS6( const KGCRoomUserPotionS6& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRoomUserPotionS6& operator=( const KGCRoomUserPotionS6& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

public:
    void SetSlotStatus( bool bStat );
    void SetSlotItem();
    void SetSlotImage( int m_iIndex );
    void SlotRelease();

public:
    KD3DWnd*                m_pkPotionGroup;

    //Luma 가 수행한 클래스 최적화
    KSafeArray<GCDeviceTexture*,5> m_apTextureItem;
    KSafeArray<KD3DWnd*, ITEM_SLOT_SIZE> m_apkItemFrame;
    KSafeArray<KD3DWnd*, ITEM_SLOT_SIZE> m_apkItemFrameImage;

};

DEFINE_WND_FACTORY( KGCRoomUserPotionS6 );
DECLARE_WND_FACTORY( KGCRoomUserPotionS6 );
DECLARE_WND_FACTORY_NAME( KGCRoomUserPotionS6 );
