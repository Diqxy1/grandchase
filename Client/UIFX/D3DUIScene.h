#ifndef _D3DUISCENE_H_
#define _D3DUISCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

//#include "D3DUIObject.h"
//#include "D3DWnd.h"
////#include <vector>

class KD3DUIScene : public KD3DUIObject // extends
{
public:
    DECLARE_CLASSNAME( KD3DUIScene );
    /// Constructor
    explicit KD3DUIScene( const std::string& strSceneName = "" );
    /// Destructor
    virtual ~KD3DUIScene( void );
    /// Copy constructor
    KD3DUIScene( const KD3DUIScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DUIScene& operator=( const KD3DUIScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return * this; }

public:
    enum ED3DUISceneState
    {
        D3DUIS_DISABLED,
        D3DUIS_ENTER,
        D3DUIS_ENABLED,
        D3DUIS_LEAVE,
    };

public:
    inline ED3DUISceneState GetUISceneState( void ) const { return m_eState; }

    HRESULT FrameMove( void );
    HRESULT Render( void );

    void AddWindow( KD3DWnd* pWnd );
	void ResetResolution();
    void RemoveAllWindow( void );
    void EnterUIScene( void );
    void LeaveUIScene( void );
    void SetActiveWindow( KD3DWnd* pWnd );
    void CheckMouseClick( void );
    void Stop( bool bStop );
    bool IsStop( void ) const;
    bool GetReverse() const { return m_bReverse; }
    void SetReverse(bool val) { m_bReverse = val; }

    void SetChildOrder( IN KD3DWnd* pScrWnd_, IN KD3DWnd* pTargetWnd_, bool bNext );
    void SetChildOrderTop( IN KD3DWnd* pScrWnd_ );
    void SetChildOrderBottom( IN KD3DWnd* pScrWnd_ );
    int GetMemorySize();

    inline void SetActivationManage( bool bActivationManage )
    {
        m_bActivationManage = bActivationManage;
    }
    inline bool IsActivationManaging( void ) const
    {
        return m_bActivationManage;
    }
    inline void LinkWindow( std::string strWndName, void** ppvLink )
    {
        m_mapLink[strWndName] = ppvLink;
    }

    #define LINK_WINDOW( name, link ) LinkWindow( ( name ), reinterpret_cast<void**>( &( link ) ) )

protected:
    virtual void OnEnter( void )                    {}
    virtual void OnEnterComplete( void )            {}
    virtual void OnLeave( void )                    {}
    virtual void OnLeaveComplete( void )            {}
    virtual void OnClear( void )                    {}
    virtual void FrameMoveInDisabledState( void )   {}
    virtual void FrameMoveInEnterState( void )      {}
    virtual void FrameMoveInEnabledState( void )    {}
    virtual void FrameMoveInLeaveState( void )      {}

protected:
    std::string                     m_strSceneName;

    ED3DUISceneState                m_eState;
    std::map<std::string, void**>   m_mapLink;
    bool                            m_bActivationManage;
    bool                            m_bStop;
    std::vector<KD3DWnd*>           m_vecWnd;
    bool                            m_bReverse;
};

class KD3DUISceneFactory
{
public:
    virtual KD3DUIScene* Create( const std::string& strSceneName ) const
    {
        return new KD3DUIScene( strSceneName );
    }
};

#define DEFINE_SCENE_FACTORY(x) \
class x##Factory : public KD3DUISceneFactory \
{ \
public: \
    virtual KD3DUIScene* Create( const std::string& strSceneName ) const \
    { \
        return new x##( strSceneName ); \
    } \
}

#endif // _D3DUISCENE_H_