#ifndef _GCKGCMISSIONCHARSELECTOR_S4_H_
#define _GCKGCMISSIONCHARSELECTOR_S4_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class KGCMissionCharSelector_S4 : public KD3DDialog, public KActionListener
{    
public:
    DECLARE_CLASSNAME( KGCMissionCharSelector_S4 );
    /// Default constructor
    KGCMissionCharSelector_S4( void );
    /// Default destructor
    virtual ~KGCMissionCharSelector_S4( void );
    /// Copy constructor
    KGCMissionCharSelector_S4( const KGCMissionCharSelector_S4& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCMissionCharSelector_S4& operator=( const KGCMissionCharSelector_S4& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreateComplete( void );

private:
    enum
    {
        CHAR_UI_NAME,
        CHAR_UI_FACE,
        NUM_CHAR_UI,
    };

private:
    KD3DWnd             *m_pkBtnCharSel;
    KD3DWnd             *m_pkChar[ GC_CHAR_NUM ][ NUM_CHAR_UI ];

private:
    int                 m_iSelectIdx;
    boost::function2<void, int, bool> m_FuncCallBack;
    boost::function1<void, bool> m_FuncCallBackParm1;

private:
    void OnEnterCursor();

public:
    void SetCallBack( boost::function2<void, int, bool> Func ) 
    {  m_FuncCallBack = Func;   }

    void SetCallBackParm1( boost::function1<void, bool> Func ) 
    {  m_FuncCallBackParm1 = Func;   }

public:
    int GetCharIdx()   {   return m_iSelectIdx;    }

};

DEFINE_WND_FACTORY( KGCMissionCharSelector_S4 );
DECLARE_WND_FACTORY( KGCMissionCharSelector_S4 );
DECLARE_WND_FACTORY_NAME( KGCMissionCharSelector_S4 );

#endif  // _GCKGCMISSIONCHARSELECTOR_S4_H_