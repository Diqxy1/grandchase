#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include <boost/noncopyable.hpp>

#if defined( NATION_BRAZIL ) || defined( NATION_USA )
#define EMOTICON_ROW_MAX 9
#else
#define EMOTICON_ROW_MAX 7
#endif

class KGCEmoticonSlotRow;
class KGCEmoticon;

class GCEmoticonSelectDlg : public KD3DDialog,
                            public KActionListener,
                            public boost::noncopyable
{
public:
    DECLARE_CLASSNAME( GCEmoticonSelectDlg );
    GCEmoticonSelectDlg(void);
    virtual ~GCEmoticonSelectDlg(void);

    enum E_USE
    {
        INIT	= -1,
        NATEON	= 0,
		INPUT	= 1,
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
    void InitData();
    GCEmoticonSelectDlg::E_USE GetUse() const { return m_eUse; }
    void SetUse( GCEmoticonSelectDlg::E_USE eUse ) { m_eUse = eUse; }
    KGCEmoticon* GetData() const { return m_pData; }
    void SetEmoticon( DWORD dwID );

protected:
    virtual void OnCreate();
    virtual void OnDestroyComplete();
    virtual void OnChildCreateComplete();
    
    void OnClose();

private:
    KSafeArray<KGCEmoticonSlotRow*,EMOTICON_ROW_MAX> m_pkEmoticonRow;
    KD3DWnd* m_pkClose;
    int m_iRowIdx;
    int m_iColumnIdx;
    KGCEmoticon* m_pData;
    GCEmoticonSelectDlg::E_USE m_eUse;
    DWORD m_dwEmoticonID;
};

DEFINE_WND_FACTORY( GCEmoticonSelectDlg );
DECLARE_WND_FACTORY( GCEmoticonSelectDlg );
DECLARE_WND_FACTORY_NAME( GCEmoticonSelectDlg );
