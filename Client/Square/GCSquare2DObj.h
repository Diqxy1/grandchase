#pragma once
////#include <vector>
#include "igcsquareobj.h"

class GCDeviceTexture;
class GCDeviceVertexBuffer;

class KGCSquare2DObj :
	public IGCSquareObj
{
public:
	KGCSquare2DObj(void);
	~KGCSquare2DObj(void);

private:
	//그려질 텍스쳐들
	std::vector< GCDeviceTexture* >		m_vecTex;
	int m_iCurrTexIndex;

	float m_fWidth, m_fHeight;
	int m_iPicked;	// 0 이면 아무것도 아님, 0이 아니면 클릭됨.
	BYTE m_cAlpha;
	bool m_bMoveWithCamera;	//카메라의 움직임과 함께 따라다님 UI와 동일함.

	std::string strObjType;
	float m_fYrotation, m_fXrotation, m_fZrotation;
	float m_fXaxisOffset, m_fYaxisOffset, m_fZaxisOffset;
	float m_fXscale, m_fYscale;
	float m_fXtrans, m_fYtrans;
    DWORD m_dwGapRandTime;
    DWORD m_dwBeforeTime, m_dwNowTime;

protected:
	int AddTexture( const char* strID );
	void SetCurrTexIndex( int iIndex);
	int GetCurrTexIndex();
	D3DXVECTOR3*	GetPosition();	
	void SetPosition( float x, float y );
	void SetAlpha( int iAlpha );
	void CameraMatrixOff()	{ m_bMoveWithCamera = false; };
	void CameraMatrixOn()	{ m_bMoveWithCamera = true;  };
    void SelectRandomTexture();

public:
	static void BindLua();
	virtual void Render();
	virtual void FrameMove();
	virtual bool Load();
	virtual bool TimeShareLoading();
	virtual bool Picking( const D3DXVECTOR2& vMousePos, const int iMouseBtn );

    void SetType( SQUARE_OBJ_TYPE eType );
	void SetSize( float fWidth, float fHeight );
	void SetScale( float x, float y );
	void SetRotation( float x, float y, float z );
	void SetTrans( float x, float y );
    void SetRandomTime(int iTime);

	void LoadObject( std::string strObjName );
};
