#ifndef _KGCTreasureItemBox_H_
#define _KGCTreasureItemBox_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DStatic.h"

// 흐음 여기가 적당할랑가 모르겠어요....
// 혹시 위치 변경 하고 싶으시면 변경 하셔도 될듯?
// 이 enum의 목적은....... Req 서버에 보내기 위한......종류?? 현재는 여기서만 사용해서
// 혹시나 다른곳에서도 사용한다면야........ 전체가 참조하는 해더 파일에다가... 정의 하심이..
enum ReqMsgType{ 
	REQ_MSG_NONE = 0 /*없음*/, 
	REQ_MSG_MISSION_DATE_CHANGE_REQ,
};

class KGCTreasureItemBox : public KD3DDialog,          // extends
	public KActionListener   // implements
{
public:
	DECLARE_CLASSNAME( KGCTreasureItemBox );
	KGCTreasureItemBox(void);
	~KGCTreasureItemBox(void);	

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();
    virtual void OnDestroyComplete( void );

	void SetTitle( std::wstring strTitle_ );
	void SetContent( std::wstring strMsg_ );
	void SetContentAutoMultiLine( std::wstring strMsg_ );
    void SetItem(int iItemID, int nEnchantLevel = 0 );
	int  GetItemID() const { return m_iItemID; };

    void PostChildDraw( void );
	void OnConfirm();
    void OnDestroy();
	void SetCount( std::wstring strTitle_ );
	void ShowCount(bool bRender);
	void OnReq(int r_);	
protected:
	bool						  m_bDoReq;
	int							  m_Req;
	KD3DStatic*             m_pkTitle;
	KD3DStatic*             m_pkContent;
	KD3DWnd*	            m_pkBtnOk;
	KD3DWnd*	            m_pkPresent;	


    int                     m_iItemID;
    GCDeviceTexture*        m_pTexItem;
	KD3DStatic*				m_pkCount;
};


DEFINE_WND_FACTORY( KGCTreasureItemBox );
DECLARE_WND_FACTORY( KGCTreasureItemBox );
DECLARE_WND_FACTORY_NAME( KGCTreasureItemBox );

#endif // _KGCTreasureItemBox_H_
