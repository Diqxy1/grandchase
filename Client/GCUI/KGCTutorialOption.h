// #ifndef _GCTUTORIALOPTION_H_
// #define _GCTUTORIALOPTION_H_
// 
// #if _MSC_VER > 1000
// #pragma once
// #endif // _MSC_VER > 1000
// 
// //#include "define.h"
// //#include <set>
// 
// // 2007/2/5. iridology. 이제 USE Define은 케릭터가 있든 없든
// // 튜토리얼에 사용할 수 있는지를 결정합니다.
// // 라이언 이후로는 자동으로 해당 케릭터를 가지거나 전직을 하면
// // 자동으로 활성화 됩니다.
// //캐릭터 사용 여부를 여기에서 결정한다.
// #if defined( NATION_JAPAN )
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_LAS1            // 도적
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #elif defined( NATION_TAIWAN )
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_LIRE4           // 리르3차
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_LAS1            // 도적
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RYAN4           // 제노사이더
// #define USE_RONAN           // 로난
// #define USE_RONAN2			// 용기사
// #define USE_RONAN3          // 로난2차
// #define USE_RONAN4          // 로난3차
// #define USE_AMY1            // 에이미
// #define USE_AMY2            // 뮤즈
// #define USE_AMY3            // 에이미2차
// #define USE_AMY4            // 에이미2차
// #define USE_JIN1			// 진 - 파이터
// #define USE_JIN2			// 진 1차
// #define USE_JIN3			// 진 2차
// #define USE_JIN4			// 진 2차
// #define USE_SIEG1			// 지크하트 - 검투사
// #define USE_SIEG2			// 지크하트 1차
// #define USE_SIEG3			// 지크하트 1차
// //#define USE_SIEG4			// 지크하트 1차
// #define USE_MARI1
// //#define USE_MARI2
// #elif defined( NATION_HONGKONG )
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_LIRE4           // 리르3차
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_LAS1            // 도적
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RONAN           // 로난
// #define USE_AMY1            // 에이미
// #define USE_RONAN2			// 용기사
// #define USE_RONAN3          // 로난2차
// #define USE_RYAN4           // 제노사이더
// #define USE_AMY2            // 뮤즈
// #define USE_RONAN4          // 로난3차
// #define USE_AMY3            // 에이미2차
// #define USE_JIN1			// 진 - 파이터
// #define USE_JIN2			// 진 1차
// #define USE_JIN3			// 진 2차(아수라)
// #define USE_SIEG1           // 지그하트
// #elif defined( NATION_CHINA )
// #define USE_ELESIS2         // 창병
// #define USE_LIRE2           // 석궁
// #define USE_ARME2           // 연금
// #elif defined( NATION_BRAZIL )
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_LIRE4           // 리르3차
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_LAS1            // 라스
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RYAN4           // 제노사이더
// #define USE_RONAN           // 로난
// #define USE_RONAN2          // 로난
// #define USE_AMY1            // 에이미
// #define USE_RONAN3          // 로난2차
// #define USE_AMY2            // 뮤즈
// #define USE_RONAN4          // 로난3차
// #define USE_AMY3            // 에이미2차
// #define USE_AMY4            // 에이미2차
// #define USE_JIN1			// 진 - 파이터
// #define USE_JIN2			// 진 1차
// #define USE_JIN3			// 진 2차
// #define USE_JIN4			// 진 2차
// #define USE_SIEG1			
// #define USE_SIEG2			
// #define USE_SIEG3		
// #define USE_SIEG4		
// #define USE_MARI1	
// #define USE_MARI2	
// #define USE_MARI3	
// 
// #elif defined( NATION_USA )
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 석궁
// #define USE_LIRE4           // 노바
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RYAN4           // 제노사이더
// #define USE_LAS1            // 도적
// #define USE_LAS2			// 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RONAN           // 로난
// #define USE_RONAN2          // 로난
// #define USE_RONAN3          // 로난2차
// #define USE_RONAN4          // 로난3차
// #define USE_AMY1            // 에이미
// #define USE_AMY2            // 에이미
// #define USE_AMY3            // 에이미
// #define USE_AMY4            // 시스티나
// #define USE_JIN1			// 진
// #define USE_JIN2			// 태사자
// #define USE_JIN3			// 아수라
// #define USE_JIN4			// 진
// #define USE_SIEG1
// #define USE_SIEG2
// #define USE_SIEG3
// #define USE_MARI1
// #define USE_DIO1	
// 
// #elif defined( NATION_PHILIPPINE )
// #define USE_ELESIS2         // 창병
// #define USE_LIRE2           // 석궁
// #define USE_ARME2           // 연금
// #define USE_RONAN           // 로난
// #define USE_RONAN2          // 로난(용기사)
// #define USE_LAS1            // 라스
// #define USE_ELESIS3         // 소드마스터
// #define USE_LIRE3           // 아크레인져
// #define USE_ARME3           // 워록
// #define USE_LAS2            // 자객
// #define USE_RONAN3			// 로난(이지스나이트)
// #define USE_RYAN1			// 농인
// #define USE_LAS3            // 닭세
// #define USE_RYAN2			// 닭농인
// #define USE_RYAN3			// 개닭농인
// #define USE_ELESIS4			// 세이버
// #define USE_LIRE4			// 노바
// #define USE_AMY1			// 에이미
// #define USE_ARME4			// 배틀메이지
// #define USE_LAS4			// 스트라이퍼
// #define USE_RONAN4          // 로난3차
// #define USE_JIN1			//	진
// #define USE_AMY2			//	에이미 1차전직 뮤즈
// #define USE_RYAN4           // 제노사이더
// #elif defined( NATION_THAILAND ) //태국은 다풀어 놔도 알아서 서버에서 받아서 처리함
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_LIRE4           // 리르3차
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_LAS1            // 라스
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RYAN4           // 제노사이더
// #define USE_RONAN           // 로난
// #define USE_RONAN2          // 로난
// #define USE_RONAN3          // 로난2차
// #define USE_RONAN4          // 로난3차
// #define USE_AMY1            // 에이미
// #define USE_AMY2            // 뮤즈
// #define USE_AMY3            // 에이미2차
// #define USE_AMY4            // 에이미2차
// #define USE_JIN1			// 진 - 파이터
// #define USE_JIN2			// 진 1차
// #define USE_JIN3			// 진 2차
// #define USE_SIEG1			
// #define USE_SIEG2	
// #elif defined( NATION_IDN )
// #define USE_MARI1
// #define USE_ELESIS4
// // #define USE_ELESIS2         // 창병
// // #define USE_ELESIS3         // 소드마스터
// // #define USE_ELESIS4         // 세이버
// // #define USE_LIRE2           // 석궁
// // #define USE_LIRE3           // 아크레인져
// // #define USE_LIRE4           // 리르3차
// // #define USE_ARME2           // 연금
// // #define USE_ARME3           // 워록
// // #define USE_ARME4           // 배틀메이지
// // #define USE_LAS1            // 라스
// // #define USE_LAS2            // 자객
// // #define USE_LAS3            // 다크어쎄씬
// // #define USE_LAS4            // 스트라이퍼
// // #define USE_RYAN1           // 드루이드
// // #define USE_RYAN2           // 센티넬
// // #define USE_RYAN3           // 바이켄
// //#define USE_RYAN4           // 제노사이더
// // #define USE_RONAN           // 로난
// // #define USE_RONAN2          // 로난
// // #define USE_AMY1            // 에이미
// // #define USE_RONAN3          // 로난2차
// // #define USE_AMY2            // 뮤즈
// // #define USE_RONAN4          // 로난3차
// // #define USE_AMY3            // 에이미2차
// //#define USE_AMY4            // 에이미2차
// // #define USE_JIN1			// 진 - 파이터
// // #define USE_JIN2			// 진 1차
// // #define USE_JIN3			// 진 2차
// // #define USE_JIN4			// 진 2차
// //#define USE_SIEG1			
// //#define USE_SIEG2			
// //#define USE_SIEG3		
// //#define USE_MARI1	
// //#define USE_MARI2	
// //#define USE_MARI3	
// //#define USE_MARI4
// #else // 국내
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_LIRE4           // 리르3차
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_LAS1            // 라스
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RYAN4           // 제노사이더
// #define USE_RONAN           // 로난
// #define USE_RONAN2          // 로난
// #define USE_AMY1            // 에이미
// #define USE_RONAN3          // 로난2차
// #define USE_AMY2            // 뮤즈
// #define USE_RONAN4          // 로난3차
// #define USE_AMY3            // 에이미2차
// #define USE_AMY4            // 에이미2차
// #define USE_JIN1			// 진 - 파이터
// #define USE_JIN2			// 진 1차
// #define USE_JIN3			// 진 2차
// #define USE_JIN4			// 진 2차
// #define USE_SIEG1			
// #define USE_SIEG2			
// #define USE_SIEG3		
// #define USE_MARI1	
// #define USE_MARI2	
// #define USE_MARI3	
// #define USE_MARI4
// #else // 국내
// #define USE_ELESIS2         // 창병
// #define USE_ELESIS3         // 소드마스터
// #define USE_ELESIS4         // 세이버
// #define USE_LIRE2           // 석궁
// #define USE_LIRE3           // 아크레인져
// #define USE_LIRE4           // 리르3차
// #define USE_ARME2           // 연금
// #define USE_ARME3           // 워록
// #define USE_ARME4           // 배틀메이지
// #define USE_LAS1            // 라스
// #define USE_LAS2            // 자객
// #define USE_LAS3            // 다크어쎄씬
// #define USE_LAS4            // 스트라이퍼
// #define USE_RYAN1           // 드루이드
// #define USE_RYAN2           // 센티넬
// #define USE_RYAN3           // 바이켄
// #define USE_RYAN4           // 제노사이더
// #define USE_RONAN           // 로난
// #define USE_RONAN2          // 로난
// #define USE_AMY1            // 에이미
// #define USE_RONAN3          // 로난2차
// #define USE_AMY2            // 뮤즈
// #define USE_RONAN4          // 로난3차
// #define USE_AMY3            // 에이미2차
// #define USE_AMY4            // 에이미2차
// #define USE_JIN1			// 진 - 파이터
// #define USE_JIN2			// 진 1차
// #define USE_JIN3			// 진 2차
// #define USE_JIN4			// 진 2차
// #define USE_SIEG1			
// #define USE_SIEG2			
// #define USE_SIEG3		
// #define USE_SIEG4		
// #define USE_MARI1	
// #define USE_MARI2	
// #define USE_MARI3	
// 
// #define USE_DIO1	
// 
// #define USE_MARI4
// #endif
// 
// #include "../UIFX/D3DFrameWnd.h"
// #include "../uifx/ActionListener.h"
// #include "../uifx/D3DSizingBox.h"
// #include "../UIFX/D3DCheckBox.h"
// #include "../uifx/D3DStatic.h"
// #include "../uifx/D3DEdit.h"
// #include "../UIFX/D3DScrollbar.h"
// #include "../gcui/GCChangeBox.h"
// #include "../uifx/D3DComboBox.h"
// #include "../gcui/KGCCharicterSelBox.h"
// 
// #if defined( NATION_BRAZIL )
// #	define NUM_TUTORIAL_MAP         (14) // 튜토리얼 맵의 갯수.
// #elif defined (NATION_THAILAND)
// #	define NUM_TUTORIAL_MAP         (13) // 튜토리얼 맵의 갯수. - 태국은 수상시장맵 더 있음.
// #elif defined (NATION_PHILIPPINE)
// #	define NUM_TUTORIAL_MAP         (13) // 튜토리얼 맵의 갯수. - 필리핀도 더 있음
// #else
// #	define NUM_TUTORIAL_MAP         (14) // 튜토리얼 맵의 갯수.
// #	define NUM_TUTORIAL_MAP         (16) // 튜토리얼 맵의 갯수. 14 + 아마존 + 수상시장 
// #endif
// #define SHOWWND_CHAR_JOB         (3)
// 
// class KGCTutorialMonsterSetting;
// 
// class KGCTutorialOption : public KD3DDialog, // extends
//     public KActionListener    // implements
// {
// public:
//     DECLARE_CLASSNAME( KGCTutorialOption );
//     /// Default constructors
//     KGCTutorialOption( void );
//     /// Default destructor
//     virtual ~KGCTutorialOption( void );
//     /// Copy constructor
//     KGCTutorialOption( const KGCTutorialOption& )
//     { ASSERT( false && "Do not use copy constructor!" ); }
//     /// Assignment operator
//     const KGCTutorialOption& operator=( const KGCTutorialOption& )
//     { ASSERT( false && "Do not use assignment operator!" ); return *this; }
// 
//     enum ETutorialOptionWndResult
//     {
//         EXIT,
//         DESTROYED,
//         STARTTUTORIAL,
//     };
// 
//     void ActionPerformed( const KActionEvent& event );
//     virtual bool InitDialog( IN const KDialogInfo& kInfo_ );
//     void SetVisible( bool bVisible_ ){ m_bVisible = bVisible_; }
//     void UpdateTutorialCharBtn();
// 
// protected:
//     virtual void OnCreate( void );
//     virtual void FrameMoveInEnabledState( void );
//     virtual void OnDestroyComplete( void );
// 
// protected:
//     int m_iCurMapIndex;                                             // 현재 맵의 인덱스.
//     KGCCharicterSelBox      *m_pkMySelBox;
//     KGCCharicterSelBox      *m_pkTutorialSelBox;
// 
//     KD3DSizingBox*  m_pkBackGround;         // 메인창
//     KD3DSizingBox*  m_pkCharBackGround;     // 캐릭터 백그라운드
//     KD3DSizingBox*  m_pkMySelBG_Sel;
//     KD3DSizingBox*  m_pkMySelBG_Def;
//     KD3DSizingBox*  m_pkTutorialSelBG_Sel;
//     KD3DSizingBox*  m_pkTutorialSelBG_Def;
//     KD3DSizingBox*  m_pkMapSelBG;
//     KD3DSizingBox*  m_pkTipBG;
// 
//     KD3DStatic*     m_pkStaticTip;
//     KD3DWnd*        m_pkBtnOk;
//     KD3DWnd*        m_pkCloseBtn;
//     KD3DWnd*        m_pktabMyChar;
//     KD3DWnd*        m_pktabTutorialChar;
//     KD3DWnd*        m_pkExit;
// 	KD3DWnd*		m_pkOkCancelBtnBackGround;
// 
//     bool m_bMyCharSelMode;
//     void SelectCharTab(bool bMyChar);
//     void OnClickOk(void);                                           // 확인을 눌렀을 경우.
//     void OnClickClose(void);                                        // 닫기를 눌렀을경우.
// 
//     
//     KSafeArray<int, NUM_TUTORIAL_MAP > m_iMapRealIndex;            // 맵 인덱스.
//     KSafeArray<int, NUM_TUTORIAL_MAP > m_iMapStringIndex;          // 맵이름.
//     KSafeArray<KD3DWnd*, NUM_TUTORIAL_MAP > m_pkMap;                    // 맵의 종류.
//     KGCChangeBox*   m_pkChangeBox;                                  // 맵 종류 변경 컨트롤.
// 
//     KSafeArray<KD3DWnd*, GC_CHAR_NUM * NUM_JOB_LEVEL > m_pkChar;        // 내가 고를 수 있는 캐릭터.
//     void            SetCharHover(int index);                        // index Char를 Hover상태로 만들어준다.
//     bool            m_bVisible;
// 	bool			m_bFirstTutorial;
// 
//     KD3DScrollbar*  m_pkScrollbar;
//     int             m_iTopIndex;
// 
//     // 한 창에 표시되어야 할 위치를 가지고 있다.
//     KSafeArray<D3DXVECTOR2, GC_CHAR_NUM * NUM_JOB_LEVEL > m_vecOriginalPos;
//     KSafeArray<D3DXVECTOR2, GC_CHAR_NUM * NUM_JOB_LEVEL > m_vecPos;
//     void            UpdateData();
// 	void			UpdateDialogSize();	// 밝혀진 케릭터 수만큼으로 사이즈를 조절합시다.
// 
// 	// 가리거나 비활성화될 케릭터, 저장되는 값은 케릭터의 UI 인덱스 입니다.
// 	std::set< int > m_setIfExistThenEnable;	// 획득해야만 활성화 되는 케릭터;
// 	std::set< int > m_setNotPatched;	// 아직 패치되지 않은 케릭터; 비활성화 창마저 가리자..
// 	void AddExistEnableChar( int iCharType );
// 	void AddExistEnableExtraChar( int iExtraCharType );
// 	void AddNotPatchedChar( int iCharType );
// 	void AddNotPatchedExtraChar( int iExtraCharType );
// 
// 
//     void SetEnableChar();
// 	void ResetWndSize();
// 
// public:
//     int GetMapIndex( void )                 { return m_iMapRealIndex[m_iCurMapIndex]; }
//     int GetMyCharIndex( void )              { return m_pkMySelBox->GetCurCharIndex(); }
//     int GetTutorialCharIndex( void )        { return m_pkTutorialSelBox->GetCurCharIndex(); }
//     void SetMyCharIndex( int iIndex )       { m_pkMySelBox->SelectCharicter(iIndex); }
//     void SetTutorialCharIndex(int iIndex)   { m_pkTutorialSelBox->SelectCharicter(iIndex); }
//     void SelectMap( int index ); // 맵 선택함수.
//     void InitTutorialChar( void );
//     void LoadTutorial( void );
// 	void SetTutorialCharList();
// 
// private:
//     // 디버그 모드나 사내테섭일때만.. 연습모드에서 몬스터를 등장시킬 수 있다!
//     //KGCTutorialMonsterSetting*  m_pkMonsterSetting;
// 
// 	D3DXVECTOR2 vSingleBoxScale;
// 	float fScaleFactor;
// };
// 
// DEFINE_WND_FACTORY( KGCTutorialOption );
// DECLARE_WND_FACTORY( KGCTutorialOption );
// DECLARE_WND_FACTORY_NAME( KGCTutorialOption );
// 
// #endif  // _GCTUTORIALOPTION_H_
