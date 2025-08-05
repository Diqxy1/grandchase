#ifndef _GCRECOMMENDLIST_H_
#define _GCRECOMMENDLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DStatic.h"
//#include "../uifx/ActionListener.h"
//#include "UserPacket.h"

#define MAX_LIST_NUM 7
class KGCCheckuser_ListElement;
class KD3DScrollbar;
class KGCRecommendList : public KD3DDialog,          // extends
    public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRecommendList );
    /// Default constructor
    KGCRecommendList( void );
    /// Default destructor
    virtual ~KGCRecommendList( void );
    /// Copy constructor
    KGCRecommendList( const KGCRecommendList& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRecommendList& operator=( const KGCRecommendList& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum EReplayResult
    {
        IRR_LOADING,
        IRR_CLOSE,
        IRR_CREATED,
        IRR_DESTROYED,
    };


public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
    void RequestList( void );
    bool IsNew( void ) { return m_bNew; }
    void SetNew( bool bTrue) { m_bNew = bTrue; }

protected:
    void UpdateList( void );
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );

protected:
    int                 m_iTopIndex;
    KD3DScrollbar*      m_pkScrollbar;
    KD3DWnd*			m_pkExit;
    KSafeArray<KGCCheckuser_ListElement*,MAX_LIST_NUM> m_pkUserList;
    KD3DWnd*            m_pkBtnAccept;
    KD3DWnd*            m_pkBtnReject;

    KD3DStatic*         m_pkStaticTitle;

    bool                m_bNew;


    std::set<DWORD>       m_setCheckedID;
};

DEFINE_WND_FACTORY( KGCRecommendList );
DECLARE_WND_FACTORY( KGCRecommendList );
DECLARE_WND_FACTORY_NAME( KGCRecommendList );

#endif // _GCRECOMMENDLIST_H_
