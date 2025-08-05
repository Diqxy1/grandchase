#ifndef _KGCCHARLOADWND_H_
#define _KGCCHARLOADWND_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
class KD3DWnd;
class KD3DStatic;
class KGCLoadingStateBar;

class KGCCharLoadWnd : public KD3DWnd, public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCCharLoadWnd );
    /// Default constructor
    KGCCharLoadWnd( void );
    /// Default destructor
    virtual ~KGCCharLoadWnd( void );
    /// Copy constructor
    KGCCharLoadWnd( const KGCCharLoadWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharLoadWnd& operator=( const KGCCharLoadWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    void SetName( std::wstring& strName_, DWORD dwUID_, D3DCOLOR dwColor_ );
    void SetLoadingComplete( bool bComplete_ );
    void SetUID( DWORD dwUID_ ) { m_dwUID = dwUID_; }
	void SetNickNameAlign( DWORD dwAlign );
    DWORD GetUID(){ return m_dwUID; }
    void SetLoadingState(std::pair<DWORD, int> prLoadingState_ );
    void KGCCharLoadWnd::InitLoadingStateBar();
protected:
    virtual void OnCreate( void );
    void ActionPerformed( const KActionEvent& event );

protected:
    KD3DWnd*        m_pkPingRed;
    KD3DWnd*        m_pkPingGreen;
    KD3DStatic*     m_pkName;
    DWORD           m_dwUID;

    KGCLoadingStateBar* m_pkLoadingStateBar;
};

DEFINE_WND_FACTORY( KGCCharLoadWnd );
DECLARE_WND_FACTORY( KGCCharLoadWnd );
DECLARE_WND_FACTORY_NAME( KGCCharLoadWnd );

#endif//_KGCCHARLOADWND_H_