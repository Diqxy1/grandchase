// GCMatrix.h: interface for the GCMatrix class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _GCMatrix_H_
#define _GCMatrix_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using std::stack;

class GCMatrix
{
private:
	struct MatrixSet
	{
		D3DXMATRIX	m_FinalMatrix;
		D3DXMATRIX	m_TranslationMatrix;
		D3DXMATRIX	m_RotationMatrix;
		D3DXMATRIX	m_ScalingMatrix;

		D3DXVECTOR3	m_Pos;
		D3DXVECTOR3	m_Rotate;
		D3DXVECTOR3	m_Scale;
	};

public:
	GCMatrix();
	virtual ~GCMatrix();

	void UpdateWorldMatrix();
	void UpdateWorldMatrixBillboard();

	void Move(float XPos, float YPos, float ZPos);
	void MoveRel(float XAdd, float YAdd, float ZAdd);

	void Rotate(float XRot, float YRot, float ZRot);
	void RotateRel(float XAdd, float YAdd, float ZAdd);

	void Scale(float XScale, float YScale, float ZScale);
	void ScaleRel(float XAdd, float YAdd, float ZAdd);

	void PushMatrix();
	int	 PopMatrix();

	const D3DXMATRIX&	GetMatrix( bool bCompute_ = true );
	const D3DXMATRIX&	GetBillBoardMatrix();
	const D3DXMATRIX&	GetTransMatrix()	{ return m_TranslationMatrix; }
	const D3DXMATRIX&	GetRotMatrix()		{ return m_RotationMatrix; }
	const D3DXMATRIX&	GetScaleMatrix()	{ return m_ScalingMatrix; }

	const D3DXVECTOR3& GetPos()	{ return m_Pos; }
	const D3DXVECTOR3& GetRotate()	{ return m_Rotate; }
	const D3DXVECTOR3& GetScale()	{ return m_Scale; }

	float GetXPos()			{ return m_Pos.x; }
	float GetYPos()			{ return m_Pos.y; }
	float GetZPos()			{ return m_Pos.z; }
	float GetXRotate()		{ return m_Rotate.x; }
	float GetYRotate()		{ return m_Rotate.y; }
	float GetZRotate()		{ return m_Rotate.z; }
	float GetXScale()		{ return m_Scale.x; }
	float GetYScale()		{ return m_Scale.y; }
	float GetZScale()		{ return m_Scale.z; }
	static void SetDevice( LPDIRECT3DDEVICE9 pd3dDevice_ ){ ms_pd3dDevice = pd3dDevice_; }
protected:

	static LPDIRECT3DDEVICE9 ms_pd3dDevice;

	D3DXMATRIX	m_FinalMatrix;
	D3DXMATRIX	m_TranslationMatrix;
	D3DXMATRIX	m_RotationMatrix;
	D3DXMATRIX	m_ScalingMatrix;

	D3DXVECTOR3	m_Pos;
	D3DXVECTOR3	m_Rotate;
	D3DXVECTOR3	m_Scale;

	MatrixSet	m_StackBuffer;

	stack<MatrixSet>	m_StackMatrix;
};

#endif // _GCMatrix_H_
