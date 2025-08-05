#ifndef _KGCUIHELPER_H_
#define _KGCUIHELPER_H_

//#include "../uifx/D3DWnd.h"
//#include "../GCEnum.h"
#include "GCUserBar.h"

class KGCUIHelper
{
private:
	std::map<int, int> m_iUILockTime;
public:
    KGCUIHelper(void);
    ~KGCUIHelper(void);
    void SetCharacterGrade( KSafeArray<KD3DWnd*,KGCUserBar::NUM_USER_GRADE>& apkWnd, int iGrade_ );
	void FrameMove();
	void CheckPressureState();

    /////////////////////////////////////////////////////////////////////
	// Date :  06-12-14 
	// milennium9@kogstudios.com 권상구
	//*******************************************************************
	// UI 버튼들의 동작에서 버튼을 누른후 일정 시간 버튼을 잠그고 싶을 때
	// 사용됩니다.
	/////////////////////////////////////////////////////////////////////
	void SetUILockTime( int LockID_,  int time_ );
	int GetUILockTime( int LockID_ );
};
//====================================================================================
// 2006.11.15 : Asirion
// 언듯 보면 UIManager에서 해도 될듯 보이지만 UIManager는 D3DWnd관련이고
// 이 클래스는 GCUI 시리즈 관련이다.
extern KGCUIHelper* g_pkUIHelper;
#endif//_KGCUIHELPER_H_