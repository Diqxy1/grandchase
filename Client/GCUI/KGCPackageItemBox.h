#ifndef _KGCPACKAGEITEMBOX_H_
#define _KGCPACKAGEITEMBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCPackageItemBox : public KD3DDialog,
					      public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCPackageItemBox );

	KGCPackageItemBox(void);
	virtual ~KGCPackageItemBox(void);
	/// Copy constructor
	KGCPackageItemBox( const KGCPackageItemBox& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCPackageItemBox& operator=( const KGCPackageItemBox& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState( void );

private:
    KD3DSizingBox         *m_pkBack;
    KGCItemImgWnd         *m_pkImg;
    KD3DStatic            *m_pkPeriod;
    KD3DSizingBox         *m_pkGradeFrame[ (KItem::GRADE_NUM-1) ];

public:
    bool SetItemInfoBox();
    void SetBoxSize(int iWidth, int iHeight);
    void SetItem( KDropItemInfo *paItemInfo = NULL );
};

DEFINE_WND_FACTORY( KGCPackageItemBox );
DECLARE_WND_FACTORY( KGCPackageItemBox );
DECLARE_WND_FACTORY_NAME( KGCPackageItemBox );

#endif	//_KGCPACKAGEITEMBOX_H_