#pragma once
//#include "uifx/D3DWnd.h"
//#include "uifx/ActionListener.h"
//#include "uifx/D3DEdit.h"
//#include "uifx/D3DStatic.h"
//#include "uifx/D3DComboBox.h"

class KD3DEdit;
class KD3DStatic;
class KD3DComboBox;

class KGCReportBox : public KD3DDialog,
                     public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCReportBox );

    KGCReportBox(void);
    ~KGCReportBox(void);
    /// Copy constructor
    KGCReportBox( const KGCReportBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCReportBox& operator=( const KGCReportBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }


private:
    KD3DStatic*     m_pkStatic_Alert;
    KD3DStatic*     m_pkStatic_ReportWho;
    KD3DStatic*     m_pkStatic_ReportWhy;
    KD3DStatic*     m_pkStatic_Comment;
    KD3DEdit*       m_pkEdit_Comment;
    KD3DComboBox*   m_pkCombo_ReportWhy;
    KD3DWnd*        m_pkBtn_Ok;
    KD3DWnd*        m_pkBtn_Close;
    KD3DWnd*        m_pkBtn_Exit;

    std::wstring    m_strReporter;
    std::wstring    m_strBadGuy;

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
    virtual void OnCreate( void );	
    virtual void FrameMoveInEnabledState( void );
    virtual void FrameMoveInDisabledState( void );
    virtual void PostChildDraw( void );

    bool SetReporterAndBadGuy( std::wstring strReporter_ /* 신고자 */, std::wstring strBadGuy_ /* 신고 당한자 */);

private:
    void YouAreUnderArrest();
};


DEFINE_WND_FACTORY( KGCReportBox );
DECLARE_WND_FACTORY( KGCReportBox );
DECLARE_WND_FACTORY_NAME( KGCReportBox );