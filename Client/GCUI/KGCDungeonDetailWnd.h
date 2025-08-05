#pragma once

class KGCDungeonDetailWnd : public KD3DWnd    // extends
    ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCDungeonDetailWnd );
    /// Default constructor
    KGCDungeonDetailWnd( void );
    /// Default destructor
    virtual ~KGCDungeonDetailWnd( void );
    /// Copy constructor
    KGCDungeonDetailWnd( const KGCDungeonDetailWnd& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCDungeonDetailWnd& operator=( const KGCDungeonDetailWnd& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState();

public:
    void InitializeListBox();
    void SetListAutoDevideStringCntDesc1( int &index, int &AutoDevideStringCnt);
    void SetListAutoDevideStringCntDesc2( int &index, int &AutoDevideStringCnt);
    void SetListAutoDevideStringValue1( int index);
    void SetListAutoDevideStringValue2( int index);
    void SetDescString();
    void SetValueString();

protected:
    KD3DListBox* m_pkDesc1;
    KD3DListBox* m_pkDesc2;
    KD3DListBox* m_pkValue1;
    KD3DListBox* m_pkValue2;
    
    std::map<int, int> m_mapListIndexAutoDivideStringDesc1;
    std::map<int, int> m_mapListIndexAutoDivideStringDesc2;
};

DEFINE_WND_FACTORY( KGCDungeonDetailWnd );
DECLARE_WND_FACTORY( KGCDungeonDetailWnd );
DECLARE_WND_FACTORY_NAME( KGCDungeonDetailWnd );
