#pragma once

class KD3DWnd;
class KD3DDialog : public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KD3DDialog );
    /// Default constructor
    KD3DDialog( void );
    /// Default destructor
    virtual ~KD3DDialog( void );
    /// Copy constructor
    KD3DDialog( const KD3DDialog& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DDialog& operator=( const KD3DDialog& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    struct KDialogInfo
    {
        int             m_nMsgBoxType;
        int             m_nMsgBoxUse;
        std::wstring    m_strText1;
        std::wstring    m_strText2;
        KEVETPARM           m_lParam;
        KEVETPARM           m_lParam2;

        KDialogInfo( void )
        {
            m_nMsgBoxType = 0;
            m_nMsgBoxUse = 0;
            m_strText1.erase();
            m_strText2.erase();
            m_lParam = 0;
            m_lParam2 = 0;
        }
        KDialogInfo( const KDialogInfo& src )
        {
            m_nMsgBoxType = src.m_nMsgBoxType;
            m_nMsgBoxUse  = src.m_nMsgBoxUse;
            m_strText1    = src.m_strText1;
            m_strText2    = src.m_strText2;
            m_lParam	  = src.m_lParam;
            m_lParam2	  = src.m_lParam2;
        }
        const KDialogInfo& operator=( const KDialogInfo& src )
        {
            m_nMsgBoxType = src.m_nMsgBoxType;
            m_nMsgBoxUse  = src.m_nMsgBoxUse;
            m_strText1    = src.m_strText1;
            m_strText2    = src.m_strText2;
            m_lParam	  = src.m_lParam;
            m_lParam2	  = src.m_lParam2;

            return *this;
        }
    };
public:
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ ) { 
        return true;
    };
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ ) {
        return true;
    }

    void CloseDlg();
    virtual void BindToLua();
};

DEFINE_WND_FACTORY( KD3DDialog );
DECLARE_WND_FACTORY( KD3DDialog );
DECLARE_WND_FACTORY_NAME( KD3DDialog );
