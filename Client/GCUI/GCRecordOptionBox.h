#pragma once

#define MAX_LIST 6

class KGCRecordOptionBox : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCRecordOptionBox );
    /// Default constructor
    KGCRecordOptionBox( void );
    /// Default destructor
    virtual ~KGCRecordOptionBox( void );
    /// Copy constructor
    KGCRecordOptionBox( const KGCRecordOptionBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRecordOptionBox& operator=( const KGCRecordOptionBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );

    void SetNowValue( void );
    void SetList( std::vector<std::wstring> &vecList );
    void SetIndex( int iIndex_ ) { m_iIndex = iIndex_; }
    int GetIndex( void ) { return m_iIndex; }
    std::wstring GetIndexText( void ) { return m_apkStaticInfo[m_iIndex]->GetText(); }

    enum RECORD_OPTION_END_TYPE { FET_OK = 20004, FET_CANCEL = 20005, };    
    KSafeArray<KD3DStatic*, MAX_LIST>  m_apkStaticInfo;
    KD3DScrollbar*              m_pkScrollBar;
    int m_iIndex;
};

DEFINE_WND_FACTORY( KGCRecordOptionBox );
DECLARE_WND_FACTORY( KGCRecordOptionBox );
DECLARE_WND_FACTORY_NAME( KGCRecordOptionBox );
