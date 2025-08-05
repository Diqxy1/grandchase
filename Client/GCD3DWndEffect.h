#pragma once
#include "GCUtil/GCInterpolate.h"
//#include "uifx/D3DWnd.h"
//#include <boost/shared_ptr.hpp>

class KGCD3DWndEffect
{
public:
    KGCD3DWndEffect(void)
    {
        m_iOriginalWidth            = 0;
        m_iOriginalHeight           = 0;
        m_dwRemainShowStep          = 0;
        m_dwRemainStepTime          = 0;
        m_dwOriginalRemainStepTime  = 0;
    }
    ~KGCD3DWndEffect(void) { }

    // 이펙트 윈도우의 최기값 설정
    void InitValue(int iOriXPos, int iOriYPos, int iOriWidth, int iOriHeight)
    {
		InitValue( D3DXVECTOR2( (float)iOriXPos, (float)iOriYPos), iOriWidth, iOriHeight );
    }

	void InitValue( D3DXVECTOR2 vPos, int iOriWidth, int iOriHeight )
	{
		m_vOriginalPos = vPos;
		m_iOriginalWidth  = iOriWidth;
		m_iOriginalHeight = iOriHeight;
	}

    // 스케일 변화량 설정 (스케일변경 프레임, 이펙트 표시 프레임, 시작시 스케일 크기(%), 종료시 스케일 크기(%))
    void SetFramePerAlterationScale(int iScaleFrame, int iShowFrame, int iStartScalePercent, int iDestScalePercent)
    {
		SetFramePerAlterationScale( iScaleFrame, iShowFrame, (iStartScalePercent/100.0f),(iDestScalePercent/100.0f) );
    }

	void SetFramePerAlterationScale(int iScaleFrame, int iShowFrame, float fFStartScalePercent, float fDestScalePercent)
	{
		const float fStartWidth   = (fFStartScalePercent)*m_iOriginalWidth;
		const float fStartHeight  = (fFStartScalePercent)*m_iOriginalHeight;
		const float fDeWidth    = (fDestScalePercent)*m_iOriginalWidth;
		const float fDeHeight   = (fDestScalePercent)*m_iOriginalHeight;

		m_Width.SetInterpolate(iScaleFrame, fStartWidth, fDeWidth);
		m_Height.SetInterpolate(iScaleFrame, fStartHeight, fDeHeight);
		m_dwRemainShowStep = iShowFrame;
        m_dwOriginalRemainStepTime = iShowFrame; 
	}

    // 위치 변화량 설정 (위치변경 프레임, 이펙트 표시 프레임, 시작 위치, 종료 위치)
    void SetFramePerAlterationPos(int iTPosFrame, int iTShowFrame, D3DXVECTOR2 vStartPos, D3DXVECTOR2 vEndPos)
    {
        m_PosX.SetInterpolate(iTPosFrame, vStartPos.x, vEndPos.x);
        m_PosY.SetInterpolate(iTPosFrame, vStartPos.y, vEndPos.y);
        m_dwRemainShowStep = iTShowFrame;
        m_dwOriginalRemainStepTime = iTShowFrame; 
    }

    // 블랜드 변화량 설정 (블랜드변경 프레임, 이펙트 표시 프레임, 시작값, 종료값)
    void SetFramePerAlterationBlend(int iTBlendFrame, int iTShowFrame, int iStartBlend, int iEndBlend)
    {
        m_Blend.SetInterpolate(iTBlendFrame, (float)iStartBlend, (float)iEndBlend);
        m_dwRemainShowStep = iTShowFrame;
        m_dwOriginalRemainStepTime = iTShowFrame;
    }

    // 프레임당 스케일 변화
    void ScaleStep(void)
    {
        m_Width.Step();
        m_Height.Step();
    }

    // 프레임당 위치 변화
    void PosStep(void)
    {
        m_PosX.Step();
        m_PosY.Step();
    }

    // 프레임당 블랜드 변화
    void BlendStep(void)
    {
        m_Blend.Step();
    }

    // 프레임당 이펙트 표시 변화량
    void ShowStep(void) { if( m_dwRemainShowStep > 0 ) --m_dwRemainShowStep; }

	void AllStepWithWnd()
	{
		ScaleStep();
		PosStep();
		BlendStep();

		if( m_pWnd == NULL )
			return;

		m_pWnd->SetWindowPosDirect( GetCurrentPos() );
		m_pWnd->SetWidthDirect( GetCurrentWidth() );
		m_pWnd->SetHeightDirect( GetCurrentHeight() );
		m_pWnd->SetWindowColor(D3DCOLOR_ARGB( (int)GetCurrentBlend(), 0xff, 0xff, 0xff));
	}

    // 각각의 변화량이 종료 되었나 확인하는 코드
    bool IsEndOfBlend(void) { return m_Blend.IsComplete(); }
    bool IsEndOfPos(void) { return m_PosY.IsComplete(); }
    bool IsEndOfScale(void) { return m_Width.IsComplete(); }
    bool IsEndOfShow(void) { return (0 >= m_dwRemainShowStep); }

	D3DXVECTOR2 GetOriginalPos()	{ return m_vOriginalPos; }
	int GetOriginalWidth()			{ return m_iOriginalWidth; }
	int GetOriginalHeight()			{ return m_iOriginalHeight; }

	D3DXVECTOR2 GetCurrentPos()		{ return D3DXVECTOR2( m_PosX.GetCurValue(), m_PosY.GetCurValue() ); }
	int	GetCurrentWidth()			{ return (int)m_Width.GetCurValue(); }
	int	GetCurrentHeight()			{ return (int)m_Height.GetCurValue(); }
	float GetCurrentBlend()			{ return m_Blend.GetCurValue(); }

	bool IsComplete()				{ return m_PosX.IsComplete() && m_PosY.IsComplete() && m_Width.IsComplete() && m_Height.IsComplete(), m_Blend.IsComplete(); }
	void SetComplete()				{ m_dwRemainShowStep = 0; }

	void WndRegisteration( KD3DWnd* pWnd )	
	{
		m_pWnd = pWnd;
	}

	void WndUnregistration()
	{
		m_pWnd = NULL;
	}

    void Reset()
    {
        m_Width.Init();
        m_Height.Init();
        m_PosX.Init();
        m_PosY.Init();
        m_Blend.Init();

        m_dwRemainShowStep = m_dwOriginalRemainStepTime;
    }

	void SetEnd()
	{
		Reset();
		m_dwRemainShowStep = 0;
	}
private:
	D3DXVECTOR2					m_vOriginalPos;
    int                         m_iOriginalWidth;
    int                         m_iOriginalHeight;
    DWORD                       m_dwOriginalRemainStepTime;
    DWORD                       m_dwRemainStepTime;
    DWORD                       m_dwRemainShowStep;

    KGCLinearInterpolate        m_PosX;
    KGCLinearInterpolate        m_PosY;
    KGCLinearInterpolate        m_Width;
    KGCLinearInterpolate        m_Height;
    KGCLinearInterpolate        m_Blend;

	KD3DWnd*	m_pWnd;
};
