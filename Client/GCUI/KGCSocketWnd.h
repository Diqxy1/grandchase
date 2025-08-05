#ifndef _GCUSOCKETWND_H_
#define _GCUSOCKETWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"

class KD3DStatic;
class GCDeviceTexture;
struct KInventoryItem;

class KGCSocketWnd : public KD3DWnd, public KActionListener
{
public:
	enum SocketIcon
	{
		WEAPON	= 0,
		HELMET	= 1,
		ARMOR	= 2,
		MAX_NUM,
	};
public:
	DECLARE_CLASSNAME( KGCSocketWnd );
	/// Default constructor
	KGCSocketWnd( void );
	/// Default destructor
	virtual ~KGCSocketWnd( void );
	/// Copy constructor
	KGCSocketWnd( const KGCSocketWnd& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSocketWnd& operator=( const KGCSocketWnd& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	KItem* GetItem( void ){ return m_pInvenItem; }
	void SetItem( KItem* pInvenItem_ = NULL );
	void SetIcon( KGCSocketWnd::SocketIcon eIcon_ );
protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
	virtual void PostDraw( void );
	virtual void ActionPerformed( const KActionEvent& event );

protected:
	KItem*     m_pInvenItem;
	GCDeviceTexture*    m_pTexture;
	KSafeArray<KD3DWnd*,MAX_NUM> m_pIcon;
};

DEFINE_WND_FACTORY( KGCSocketWnd );
DECLARE_WND_FACTORY( KGCSocketWnd );
DECLARE_WND_FACTORY_NAME( KGCSocketWnd );

#endif//_GCUSOCKETWND_H_