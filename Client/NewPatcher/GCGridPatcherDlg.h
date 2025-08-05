/*▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤

★ 설계자 ☞ 이상호
★ 설계일 ☞ 2010년 11월 10일
★ E-Mail ☞ shmhlove@naver.com
★ 클래스 ☞ Grid다운로드 패쳐 다이얼로그

▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤*/

#if !defined(AFX_GCGRIDPATCHERDLG_H__DEDF685C_128B_468B_BED3_DAA8D1DDCBDF__INCLUDED_)
#define AFX_GCGRIDPATCHERDLG_H__DEDF685C_128B_468B_BED3_DAA8D1DDCBDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include "GCHtmlView.h"
#include "KProgressCtrl.h"
#include "ColorStatic.h"
#include "NBitmapButton.h"
#include "kgcstringloader.h"
#include "afxwin.h"
#include "resource.h"
#include "patch.h"
#include "GCRgnDlg.h"

#include "GridInclude/GridLoad_Define.h"
#define MAX_GRID_DOWN_FILE_LIST_SIZE 100


class CGCGridPatcherDlg : public CRGNDlg
{

public:
	CGCGridPatcherDlg(bool mannual,boost::shared_ptr<KGCStringLoader> strloader,boost::shared_ptr<Komfile> imagefile,CWnd* pParent = NULL);	// standard constructor
    ~CGCGridPatcherDlg();

	enum { IDD = IDD_GRID_GCPATCHER_DIALOG };
    enum { ERRORTYPE_NONE, ERRORTYPE_MAIN, ERRORTYPE_ETC };
    CColorStatic                    m_kStaticDownLoadTotalRate;     // 업데이트 정보
    CColorStatic                    m_StaticAggriment;              // 그리드 동의 안내문
    CColorStatic                    m_StaticHTML;                   // 웹
    CButton				            m_kGridDownCheckBox;            // 그리드 동의 체크박스
    CNBitmapButton		            m_kStartDownloadBtn;            // 업데이트 버튼
    CBitmap*                        m_pkBitmapBack;                 // 백그라운드
    CBitmap*                        m_pkBitmapBottom;               // 백그라운드 바닥
    CBitmap*                        m_pkBitmapError;                // 다운로드 오류 이미지
    CBitmap*                        m_pkBitmapNormality;            // 다운로드 중 이미지

protected:
    HINSTANCE                       m_hInstance;                    // 다이얼로그 인스턴스
	CGCHtmlView                     *m_pHtmlPR;                     // 웹 브라우져
	HICON                           m_hIcon;                        // 프로그램 아이콘

protected:
	BYTE				            m_bError;                       // 에러확인
    char                            m_strTotalDownLoadInfo[1024];   // 업데이트 상태정보
    FLOAT                           m_fTotalRate;                   // 현재 다운로드 용량
    std::string                     m_strCapacityRead;              // 받은용량
    std::string                     m_strCapacityTotal;             // 전체용량

protected:
	CFont                           m_kFont;                        // 14포인트 폰트
	KProgressCtrl                   m_kProgressTotal;               // 다운로드 프로그래스바
	boost::shared_ptr<KGCStringLoader>	m_pStrLoader;               // String변환객체
	boost::shared_ptr<Komfile>			m_pMassFile;                // 리소스 메스파일
	bool				            m_bMannual;                     // 메뉴얼 패치확인(현재 사용안함)

private:
    CGridDownLib			        *m_pGridDownloadSDK;            // 그리드 라이브러리
    std::vector<HGRIDJOB>	        m_vecGridDownloadJob;           // Job리스트

private:
    int						        m_iGridFailCnt;                 // 그리드 실패 카운트
    int						        m_iNumDownloadFiles;            // 다운로드 받은 파일겟수
    int						        m_iNumGridJob;                  // Job 갯수
    DWORD                           m_dwGridDownSize;               // 그리드로 받은 용량

private:
    boost::shared_ptr<KPatchLib>    *m_pPatchlib;                   // 업데이트 라이브러리
    boost::thread                   *m_pT;                          // 업데이트 라이브러리 스레드

protected:// Generated message map functions
    virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();    
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedEnd();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnExit();    
    DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void DoDataExchange(CDataExchange* pDX);

public:	
    void OnStart();
    void ShowWeb( std::string url );
    void OnProgress(SProgressInfo & info);
    void OnError(std::string filename,int errorcode);
    void OnComplete(bool success, bool bMsgBox );
    void OnInvalidate();
    BOOL ModalDestroyWindow();

public:// 그리드 처리 함수
    void SendLog();
    void GridDownloadStart();
    BOOL AddGridDownLoadFile(SGridDownFileInfo &a_fileinfo);
    void SetPatchLib( boost::shared_ptr<KPatchLib> *pArg=NULL)
    {   m_pPatchlib = pArg;         }
    void BoostJoin()
    {   if( m_pT ) m_pT->join();    }

private:// Job처리 함수
	void OnGridLoadSuccess(const char* a_szFullPath, void* a_pUserData);
	void OnGridLoadFail(void* a_pUserData);
	void OnDownLoadStart();
    void OnGridJobAllDestroy();
    void DestroyGridSDK( HGRIDJOB a_hJob );


    BEGIN_GRIDLOAD_MSG_DISPATCHMAP(OnGridLoadProc)
        DISPATCH_GRIDLOAD_SUCCESS(OnGridLoadSuccess)
        DISPATCH_GRIDLOAD_FAIL(OnGridLoadFail)
    END_GRIDLOAD_MSG_DISPATCHMAP()

private: // Grid Download 이벤트 처리 함수
	void OnGridJobInitialize(HGRIDJOB a_hJob, PST_GDX_FILE_INITIALIZE a_pstInit, void* a_pUserData);
	void OnGridJobProgress(HGRIDJOB a_hJob, PST_GDX_FILE_PROGRESS a_pstProgress, void* a_pUserData);
	void OnGridJobFileComplete(HGRIDJOB a_hJob, PST_GDX_FILE_COMPLETE a_pstFileComp, void* a_pUserData);
	void OnGridJobComplete(HGRIDJOB a_hJob, PST_GDX_DOWNLOAD_COMPLETE a_pstJobComp, void* a_pUserData);
	void OnGridJobError(HGRIDJOB a_hJob, PST_GDX_ERROR a_pstError, void* a_pUserData);

private: // Grid Engine 이벤트 처리 함수
	void OnGridEngineUpdateInitialize(PST_GDX_ENGINE_UPDATE_INITIALIZE a_pstInit, void* a_pUserData);
	void OnGridEngineUpdateProgress(PST_GDX_ENGINE_UPDATE_PROGRESS a_pstProgress, void* a_pUserData);
	void OnGridEngineUpdateComplete(void* a_pUserData);
	void OnGridEngineUpdateFail(void* a_pUserData);

	BEGIN_GRID_MSG_DYNAMIC_DISPATCHMAP(OnGridDownProc, m_pGridDownloadSDK)
		DISPATCH_GRID_DYNAMIC_JOBINITIALIZE(OnGridJobInitialize)
		DISPATCH_GRID_DYNAMIC_JOBPROGRESS(OnGridJobProgress)
		DISPATCH_GRID_DYNAMIC_JOBFILECOMPLETE(OnGridJobFileComplete)
		DISPATCH_GRID_DYNAMIC_JOBCOMPLETE(OnGridJobComplete)
		DISPATCH_GRID_DYNAMIC_JOBERROR(OnGridJobError)
		DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_INIT(OnGridEngineUpdateInitialize)
		DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_PROGRESS(OnGridEngineUpdateProgress)
		DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_COMPLETE(OnGridEngineUpdateComplete)
		DISPATCH_GRID_DYNAMIC_ENGINE_UPDATE_FAIL(OnGridEngineUpdateFail)
	END_GRID_MSG_DYNAMIC_DISPATCHMAP()

private:// 기타 함수
    void ChangeStringFromNumber(IN int iNumber, OUT std::string* szpStr);
};

#endif // !defined(AFX_GCGRIDPATCHERDLG_H__DEDF685C_128B_468B_BED3_DAA8D1DDCBDF__INCLUDED_)