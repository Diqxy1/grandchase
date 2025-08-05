#ifndef _KGCMACROCOMMUNITYSLOT_H_
#define _KGCMACROCOMMUNITYSLOT_H_

#define DECIMAL_SIZE			10

class KGCMacroCommunitySlot : public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCMacroCommunitySlot );

	KGCMacroCommunitySlot();
	virtual ~KGCMacroCommunitySlot(void);
	/// Copy constructor
	KGCMacroCommunitySlot( const KGCMacroCommunitySlot& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCMacroCommunitySlot& operator=( const KGCMacroCommunitySlot& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

protected:
	virtual void OnCreate(void);
	virtual void OnDestroy(void);
	virtual void PostChildDraw(void);
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState();

public:
    void InitData();
    void SetIndex( int iIndex )     {   m_MyIndex = iIndex; };
    bool SendCommunityMsg();
    void SetCoolTime( int iCoolTime );
    void CoolTimeFrameMove();

private:
    KD3DWnd* m_pkFrame;

private:
    int m_MyIndex;
    int m_iItemAlpha;
    int m_iRealCoolTime;
    int m_iMaxCoolTime;
    GCDeviceTexture* m_pkImage;
    GCDeviceTexture* m_pkCooltime_Pannel;
    KSafeArray<KD3DUIVertex,4> m_pImageVertex;

public:
    SMacroCommunity m_sMacroCommunityInfo;

};

DEFINE_WND_FACTORY( KGCMacroCommunitySlot );
DECLARE_WND_FACTORY( KGCMacroCommunitySlot );
DECLARE_WND_FACTORY_NAME( KGCMacroCommunitySlot );

#endif
