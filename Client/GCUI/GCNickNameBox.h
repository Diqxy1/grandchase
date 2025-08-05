#ifndef _GCNICKNAMEBOX_H_
#define _GCNICKNAMEBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../UIFX/D3DFrameWnd.h"
//#include "../uifx/ActionListener.h"
//#include "../uifx/D3DSizingBox.h"
//#include "../UIFX/D3DCheckBox.h"
#define PET_NICKNAME_SIZE 8
class KD3DStatic;
class KD3DEdit;

class KGCNickNameBox : public KD3DDialog,      // extends
                       public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KGCNickNameBox );
    /// Default constructor
    KGCNickNameBox( void );
    /// Default destructor
    virtual ~KGCNickNameBox( void );
    /// Copy constructor
    KGCNickNameBox( const KGCNickNameBox& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCNickNameBox& operator=( const KGCNickNameBox& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum ENickNameBoxResult
    {
        NBR_OK,
		NBR_CANCEL,
        NBR_CREATED,
        NBR_DESTROYED,
    };

public:
    const std::wstring& GetNickName( void ) const;
    const std::wstring& GetRecommendNickName( void ) const;

    bool IsValidNickName();
    bool IsValidRecommendNickName();

    bool GetNickNameRegisterComplete(){ return m_bNickRegComplete; }
    bool GetRecommendRegisterComplete(){ return m_bRecommendRegComplete; }
    void NickNameRegNameReset(){m_bNickRegComplete = false; m_bRecommendRegComplete = true; }
    void SetNickNameRegisterComplete( bool bTrue ){ m_bNickRegComplete = bTrue; }
    void SetRecommendRegisterComplete( bool bTrue){ m_bRecommendRegComplete = bTrue; }
    void SetChangeNickName( bool bTrue ){ m_bUseChangeNickName = bTrue; }

    void    SetUse( DWORD dwUse ) { m_dwUse = dwUse; }
    DWORD   GetUse(){ return m_dwUse; }
    void    SetlParam1( GCITEMID dwItemID_ ){ m_iItemID = dwItemID_; }
    void    SetlParam2( GCITEMUID dwItemUID_ ){ m_dwItemUID = dwItemUID_; }
    GCITEMID   GetlParam1() { return m_iItemID; }
    GCITEMUID   GetlParam2() { return m_dwItemUID; }

    bool    CheckNickNameWidthMsgBox();

    virtual void ActionPerformed( const KActionEvent& event );
    virtual bool CheckCondition(  IN const KDialogInfo& kInfo_ );

protected:
    virtual void OnCreate( void );
    virtual void OnCreateComplete( void );
    virtual void OnDestroyComplete( void );
    virtual void FrameMoveInEnabledState( void );

    void OnCheckNot();
    void OnCheckLater();
    void OnCheck(bool bLater);
    void OnCancel();
    void OnConfirmNickName();

    void OnClickEditNick();
    void OnClickEditRecommend();

protected:
    KSafeArray<KD3DSizingBox*,2> m_pkBackGround;      // 0 : 닉네임 등록창, 1 : 추천인 등록창
    KSafeArray<KD3DSizingBox*,2> m_pkBackFront;           // 청회색같은 창
    KSafeArray<KD3DSizingBox*,2> m_pkBackNickname;    // 닉넴 배경
    KSafeArray<KD3DEdit*,2> m_pkEditNick;
    KSafeArray<KD3DWnd*,2> m_pkWndNickName;     // 닉네임 이라고 적힌 ui글자 
    KSafeArray<KD3DWnd*,2> m_pkWndWelcome;      // 0 : welcome to grandchase!!, 1: 친구와 함께
    KSafeArray<KD3DWnd*,2> m_pkBtnOk;           // 0: 닉넴등록창의 확인버튼 1: 추천인등록창 확인버튼

    // 추천인 등록창에만 있는 ui들
    KD3DSizingBox*  m_pkBackDesc;
    KD3DSizingBox*  m_pkBackBottom;
    KD3DListBox*     m_pkStaticDesc;         // 이벤트 설명 static
    KD3DStatic*     m_pkStaticBottom;       // 나중에 광장에서 할 수 있다는 static
    KSafeArray<KD3DStatic*,2> m_pkStaticCheckbox;  // 0 : 안할래요 1: 나중에 할래요
    KSafeArray<KD3DCheckBox*,2> m_pkCheckBox;
    KD3DScrollbar*  m_pkScroll;
    


    KSafeArray<std::wstring,2> m_strNickName;
    DWORD           m_dwUse;
    GCITEMID             m_iItemID;
    GCITEMUID           m_dwItemUID;
    bool            m_bNickRegComplete;
    bool            m_bRecommendRegComplete;

    bool            m_bUseChangeNickName;
    bool            m_bChange;  // Destroy 한 후 다시 켤때를 위해서 ㅠ
    bool            m_bIsNickName;

public:
	void SetNickRegComplete(bool bNickRegComplete_) { m_bNickRegComplete = bNickRegComplete_; }
    bool SetMsgBox();
    void OnConfirmRecommend();
    void SetRecommendBox(bool bTrue);
    void SetNickBox(bool bTrue);
    void SetLock( int i );
    void SetRecommendRegComplete(bool bRecommendRegComplete) { m_bRecommendRegComplete = bRecommendRegComplete; }
	void KeydownEditNick();
};

DEFINE_WND_FACTORY( KGCNickNameBox );
DECLARE_WND_FACTORY( KGCNickNameBox );
DECLARE_WND_FACTORY_NAME( KGCNickNameBox );

#endif // _GCNICKNAMEBOX_H_
