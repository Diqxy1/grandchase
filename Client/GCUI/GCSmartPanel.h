#pragma once

#define MAX_SHOW_LINE       7   //최대 보이는 줄
#define TEXT_LINE_HEIGHT    19  //한줄에 적힐 텍스트의 높이
#define PHONENUMBER_SIZE    11  //폰번호 사이즈

class KGCSmartPanel : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSmartPanel );
    /// Default constructor
    KGCSmartPanel( void );
    /// Default destructor
    virtual ~KGCSmartPanel( void );
    /// Copy constructor
    KGCSmartPanel( const KGCSmartPanel& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSmartPanel& operator=( const KGCSmartPanel& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void OnInitialize();
    virtual void FrameMoveInEnabledState( void );

    void SetEditID( const std::wstring& strEditID );
    void PanelRender( bool render_ );
    void SetOnlyIDString( const std::wstring& editString_ );
    void SearchMailList( const std::wstring& seachKeyward_, bool bPhoneNumber_ = false );
    void PopupListAllClear();
    void SetPopupHeight( int height_ );
    bool IsPhoneNumber( const std::wstring& strID_ );
    std::wstring GetSelectedID();
    void SetSelectPanelPos( int selectIndex_, bool render_ );
    void UpdateSelectPanel();
    bool IsRenderPopup();

private:

    std::wstring    m_strEditID;
    std::wstring    m_strID;
    std::wstring    m_strSelectedID;
    KD3DSizingBox*  m_pkSmartPanel_Popoup;
    KD3DSizingBox*  m_pkSmartPanel_Select;

    std::vector< std::wstring > m_vecAllMailList;
    std::vector< std::wstring > m_vecShowMailList;

    KD3DStatic* m_pkPopupTextList[MAX_SHOW_LINE];
    int         m_iShowLineIndex;
    int         m_iSelectLineIndex;
};

DEFINE_WND_FACTORY( KGCSmartPanel );
DECLARE_WND_FACTORY( KGCSmartPanel );
DECLARE_WND_FACTORY_NAME( KGCSmartPanel );
