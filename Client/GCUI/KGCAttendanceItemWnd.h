#ifndef _GCATTENDANCEITEMWND_H_
#define _GCATTENDANCEITEMWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
#include "CommonPacket.h"
//#include "../GCItem.h"

class GCDeviceTexture;
class KD3DStatic;

class KGCAttendanceItemWnd : public KD3DWnd,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCAttendanceItemWnd );
    /// Default constructor
    KGCAttendanceItemWnd( void );
    /// Default destructor
    virtual ~KGCAttendanceItemWnd( void );
    /// Copy constructor
    KGCAttendanceItemWnd( const KGCAttendanceItemWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCAttendanceItemWnd& operator=( const KGCAttendanceItemWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        ENCHANT_BOX_OFFSET_X    = 197,
        ENCHANT_BOX_OFFSET_Y    = -59,
    };

protected:
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    virtual void PostDraw( void );
    virtual void ActionPerformed( const KActionEvent& event );

    GCDeviceTexture*    m_pTextureItem;
    int                 m_iItemID;
    int                 m_iDuration;
    bool                m_bShow;
    KItem               m_kItemUnit;

    KD3DStatic*			m_pkItemName;
    //KD3DWnd*            m_pkGetItemButton;
	KGCItemImgWnd*	m_pkItemImg;
    KD3DWnd*            m_pkActiveFrame;
    //KD3DStatic*			m_pkItemLimit;
    KD3DStatic*			m_pkDuration;

public:
    void SetItem( bool bShow, const KItem kItem );
    void ShowItem( bool bShow );
    bool GetHoveredItem( GCItem*& pItem, D3DXVECTOR2 &vecPos );

    bool                m_bIsGetButtonClick;
    GCItem*             m_ItemInfo;
};

DEFINE_WND_FACTORY( KGCAttendanceItemWnd );
DECLARE_WND_FACTORY( KGCAttendanceItemWnd );
DECLARE_WND_FACTORY_NAME( KGCAttendanceItemWnd );

#endif // _GCATTENDANCEITEMWND_H_
