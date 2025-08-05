#ifndef _GCITEMINFOBOX_H_
#define _GCITEMINFOBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KD3DStatic;
class KGCItemImgWnd;
class KGCMonsterCard : public KD3DFrameWnd,      // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCMonsterCard );
    /// Default constructor
    KGCMonsterCard( void );
    /// Default destructor
    virtual ~KGCMonsterCard( void );
    /// Copy constructor
    KGCMonsterCard( const KGCMonsterCard& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMonsterCard& operator=( const KGCMonsterCard& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void FrameMoveInEnabledState( void );
    virtual void ActionPerformed( const KActionEvent& even );
    virtual void OnCreate( void );

private:
    KD3DStatic*     m_pkItemName;
    KD3DStatic*     m_pkItemAttribute;
    KD3DStatic*     m_pkItemDecs;
    KGCItemImgWnd*  m_pkItemImg;
    KD3DWnd*        m_pkEdge[ KItem::GRADE_NUM ];

public:
    void SetMonsterCard( GCITEMID ItemID_ );
};

DEFINE_WND_FACTORY( KGCMonsterCard );
DECLARE_WND_FACTORY( KGCMonsterCard );
DECLARE_WND_FACTORY_NAME( KGCMonsterCard );

#endif // _GCITEMINFOBOX_H_
