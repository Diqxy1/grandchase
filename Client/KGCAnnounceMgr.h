#pragma once

#include <KNCSingleton.h>
//#include <string>
#include <d3dx9math.h>
//#include <vector>
#include <WTypes.h>
//#include "UserPacket.h"
#include "KNeonSign.h"
//#include "./uifx/KD3DEmoticonString.h"

class KGCEmoticon;
class GCDeviceTexture;

class SimpleAnnounce
{
public:
    SimpleAnnounce();
    virtual ~SimpleAnnounce();

    // functions
    void Init();
    static void InitAnnounceData( KSignBoardData& kData );
    void IncreaseTime()
    {
        m_dwAnnounceTime++;
        m_kNeonSignData.IncreaseTime();
    }
    void RenderFrame(GCDeviceTexture * tex); // 판때기 그리기
    //void RenderFrameType0(); // 판때기 그리기
    //void RenderFrameType1(); // 판때기 그리기
    void RenderFrameType2(GCDeviceTexture * tex); // 판때기 그리기
    void RenderFrameAdmin(GCDeviceTexture * tex); // 판때기 그리기
    void ResetTime();

    // getter/setter
    void SetAnnounceData( const KSignBoardData& kData );
    const KSignBoardData& GetAnnounceData() { return m_kAnnounceData; }
    void SetMoveTerm( float fWidth ) { m_fMoveTerm = fWidth; }
    float GetMoveTerm() const { return m_fMoveTerm; }
    void SetTextRect( const D3DXVECTOR2& vRect ) { m_vTextRect = vRect; }
    const D3DXVECTOR2& GetTextRect() const { return m_vTextRect; }
    void SetTextPos( const D3DXVECTOR2& vPos ) { m_vTextPos = vPos; }
    const D3DXVECTOR2& GetTextPos() const { return m_vTextPos; }
    DWORD GetAnnounceTime() const { return m_dwAnnounceTime; }
    void GetMessage( std::wstring& strMsg ) const;
    const KNeonSignItem& GetNeonsignData() const { return m_kNeonSignData; }
    void SetNeonsignData( const KNeonSignItem& kData )
    {
        m_kNeonSignData = kData;
        m_kAnnounceData.m_eType = m_kNeonSignData.GetType();
    }
    void SetTextTerm( D3DXVECTOR2 vTextTerm ) { m_vTextTerm = vTextTerm; }
    D3DXVECTOR2 GetTextTerm() const { return m_vTextTerm; }
    DWORD GetColor() const { return m_kAnnounceData.m_dwColor; }
    void SetColor( DWORD dwColor ) { m_kAnnounceData.m_dwColor = dwColor; }
    void SetNeonsignRect( const D3DXVECTOR2& vRect ) { m_vNeonsignRect = vRect; }
    const D3DXVECTOR2& GetNeonsignRect() const { return m_vNeonsignRect; }
    int GetTopLoopCnt() const { return m_nTopLoopCnt; }
    void SetTopLoopCnt(int val) { m_nTopLoopCnt = val; }
    int GetBottomLoopCnt() const { return m_nBottomLoopCnt; }
    void SetBottomLoopCnt(int val) { m_nBottomLoopCnt = val; }
    int GetFontSize() const { return m_iFontSize; }
    bool IsAdminAnnounce() const;
    const std::vector< KD3DEmoticonString::EmotStrPair >& GetText() const { return m_vecText; }
    bool IsHasEmoticon() const;
    float GetTextWidth();

private:
    DWORD m_dwAnnounceTime;
    float m_fMoveTerm;
    D3DXVECTOR2 m_vTextPos; // 글자위치
    D3DXVECTOR2 m_vTextRect; // 글자영역
    D3DXVECTOR2 m_vTextTerm; // 글자 텀
    D3DXVECTOR2 m_vNeonsignRect; // 네온사인영역
    KSignBoardData m_kAnnounceData;
    KNeonSignItem m_kNeonSignData;
    int     m_nTopLoopCnt;
    int     m_nBottomLoopCnt;
    int     m_iFontSize;
    std::vector< KD3DEmoticonString::EmotStrPair > m_vecText;
};

class KGCAnnounceMgr
{
    DeclareSingleton( KGCAnnounceMgr );

public:
    KGCAnnounceMgr(void);
    virtual ~KGCAnnounceMgr(void);

    // functions
    void FrameMove();
    void Render();
    void ReserveAnnounce( const KSignBoardData& kData );
    void ProcessAnnounce( SimpleAnnounce& kAnnounce );
    void RefreshAnnounceList();
    void RenderMessage( const SimpleAnnounce& kAnnounce );
    bool IsAnnounceRenderState();
    void Preprocess( OUT SimpleAnnounce& kReserve, float fAnnouncePosY );
    void PreviewFrameMove();
    void PreviewRender();
    bool TimeOver( const SimpleAnnounce& kAnnounce );
    bool IsAnnounceListFull() const { return m_pairAnnounceList.first <= m_pairAnnounceList.second; }
    void PreprocessForAdmin( OUT SimpleAnnounce& kReserve, float fAnnouncePosY );

    // getter/setter
    void GetAnnounceInfo( SimpleAnnounce& kInfo );
    void SetAnnounceList( const std::pair< DWORD, DWORD >& kList );
    void SetWaitTime( int nWait );
    std::wstring GetWaitTime();
    void SetPreviewData( const SimpleAnnounce& kPreview );
    bool GetPreview() const { return m_bPreview; }
    void SetPreview(bool val) { m_bPreview = val; }

    // for debug
    void ReserveforDebug( const KSignBoardData& kData );

	GCDeviceTexture* GetTexture(){return m_pTexture;}

private:
    void DrawContent_( const std::wstring& strContent, D3DXVECTOR2& vPos, bool bHasEmoticon, DWORD dwColor, int iFontSize );
    void DrawEmoticon_( KGCEmoticon* pEmoticon, D3DXVECTOR2& vPos );

    bool m_bAnnounce;
    std::vector< SimpleAnnounce > m_vecAnnounce;
    SimpleAnnounce m_kCurrentAnnounce;
    std::pair< DWORD, DWORD > m_pairAnnounceList;
    int m_nWaitTime;
    GCDeviceTexture* m_pTexture;
    SimpleAnnounce  m_kPreview;
    GCDeviceTexture* m_pPreviewTex;
    bool m_bPreview;

	// 레인보우 이벤트용
	GCDeviceTexture* m_pRainbowEvent;
	float m_fX1Pos;
	bool m_bIsRight;
	void SetRainbowPos();

    //보름달 이벤트용
    CParticleEventSeqPTR m_pFullMoonPtr[2];
    void CreateFullMoonPtr();
    void ToggleFullMoon( bool bRender_ );
    void ClearFullMoonPtr();
    void RenderFullMoon();
};

DefSingletonInline( KGCAnnounceMgr );
