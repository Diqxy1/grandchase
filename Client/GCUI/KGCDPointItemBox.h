#ifndef _KGCDPOINTITEMBOX_H_
#define _KGCDPOINTITEMBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCDPointItemBox : public KD3DDialog,
					      public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCDPointItemBox );

	KGCDPointItemBox(void);
	virtual ~KGCDPointItemBox(void);
	/// Copy constructor
	KGCDPointItemBox( const KGCDPointItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCDPointItemBox& operator=( const KGCDPointItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    int                   m_iItemID;
    KD3DSizingBox         *m_pkBackPrice;
    KD3DSizingBox         *m_pkSelecter;
    KGCItemImgWnd         *m_pkImg;
    KD3DStatic            *m_pkPrice;
    KD3DWnd               *m_pkMarkCheck;

public:
    void SetRollOverBox( bool bSelect );
    void SetSelectBox( bool bSelect );
    void SetItemTexture( GCDeviceTexture *pTex ) { m_pkImg->SetItemTexture( pTex ); };
    void SetItemID( int iItemID ) { m_iItemID = iItemID; };
    void SetPriceText( WCHAR *wszPrice ) { m_pkPrice->SetText( wszPrice ); };
    void SetPriceTextColor( D3DCOLOR dwColor ) { m_pkPrice->SetFontColor( dwColor ); };
    int GetItemID() { return m_iItemID; };

};

DEFINE_WND_FACTORY( KGCDPointItemBox );
DECLARE_WND_FACTORY( KGCDPointItemBox );
DECLARE_WND_FACTORY_NAME( KGCDPointItemBox );

#endif	//_KGCDPOINTITEMBOX_H_