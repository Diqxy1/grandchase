// KTDGMatrix.cpp: implementation of the KTDGMatrix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCMatrix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPDIRECT3DDEVICE9 GCMatrix::ms_pd3dDevice = NULL;

GCMatrix::GCMatrix()
{
	ZeroMemory( &m_TranslationMatrix,	sizeof(D3DXMATRIX) );
	ZeroMemory( &m_RotationMatrix,		sizeof(D3DXMATRIX) );
	ZeroMemory( &m_ScalingMatrix,		sizeof(D3DXMATRIX) );

	ZeroMemory( &m_Pos,					sizeof(D3DXVECTOR3) );
	ZeroMemory( &m_Rotate,				sizeof(D3DXVECTOR3) );
	ZeroMemory( &m_Scale,				sizeof(D3DXVECTOR3) );

	ZeroMemory( &m_StackBuffer,			sizeof(MatrixSet) );

	D3DXMatrixIdentity(&m_TranslationMatrix);
	D3DXMatrixIdentity(&m_RotationMatrix);
	D3DXMatrixIdentity(&m_ScalingMatrix);
}

GCMatrix::~GCMatrix()
{
}

void GCMatrix::UpdateWorldMatrix()
{
	D3DXMatrixIdentity( &m_FinalMatrix );
	D3DXMatrixMultiply( &m_FinalMatrix, &m_FinalMatrix, &m_ScalingMatrix );
	D3DXMatrixMultiply( &m_FinalMatrix, &m_FinalMatrix, &m_RotationMatrix );
	D3DXMatrixMultiply( &m_FinalMatrix, &m_FinalMatrix, &m_TranslationMatrix );

	ms_pd3dDevice->SetTransform(D3DTS_WORLD, &m_FinalMatrix);
}

void GCMatrix::UpdateWorldMatrixBillboard()
{
	D3DXMATRIX matView, matTranspose;
	D3DXMATRIX matbill, matFinal;

	ms_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixTranspose(&matTranspose, &matView);
	D3DXMatrixIdentity(&matbill);

	matbill._11 = matTranspose._11;
	matbill._12 = matTranspose._12;
	matbill._13 = matTranspose._13;
	matbill._21 = matTranspose._21;
	matbill._22 = matTranspose._22;
	matbill._23 = matTranspose._23;
	matbill._31 = matTranspose._31;
	matbill._32 = matTranspose._32;
	matbill._33 = matTranspose._33;

	m_FinalMatrix = m_ScalingMatrix * m_RotationMatrix * matbill * m_TranslationMatrix;

	ms_pd3dDevice->SetTransform(D3DTS_WORLD, &m_FinalMatrix);
}

void GCMatrix::Move(float XPos, float YPos, float ZPos)
{
	m_Pos.x = XPos;
	m_Pos.y = YPos;
	m_Pos.z = ZPos;

	D3DXMatrixTranslation(&m_TranslationMatrix, m_Pos.x, m_Pos.y, m_Pos.z);
}

void GCMatrix::MoveRel(float XAdd, float YAdd, float ZAdd)
{
	Move(m_Pos.x + XAdd, m_Pos.y + YAdd, m_Pos.z + ZAdd);
}

void GCMatrix::Rotate(float XRot, float YRot, float ZRot)
{
	m_Rotate.x = XRot;
	m_Rotate.y = YRot;
	m_Rotate.z = ZRot;

	D3DXMatrixRotationYawPitchRoll(&m_RotationMatrix,
		m_Rotate.y, m_Rotate.x, m_Rotate.z);
}

void GCMatrix::RotateRel(float XAdd, float YAdd, float ZAdd)
{
	Rotate(m_Rotate.x + XAdd, m_Rotate.y + YAdd, m_Rotate.z + ZAdd);
}

void GCMatrix::Scale(float XScale, float YScale, float ZScale)
{
	m_Scale.x = XScale;
	m_Scale.y = YScale;
	m_Scale.z = ZScale;

	D3DXMatrixScaling(&m_ScalingMatrix, m_Scale.x, m_Scale.y, m_Scale.z);
}

void GCMatrix::ScaleRel(float XAdd, float YAdd, float ZAdd)
{
	Scale(m_Scale.x + XAdd, m_Scale.y + YAdd, m_Scale.z + ZAdd);
}

void GCMatrix::PushMatrix()
{
	m_StackBuffer.m_FinalMatrix			= m_FinalMatrix;
	m_StackBuffer.m_TranslationMatrix	= m_TranslationMatrix;
	m_StackBuffer.m_RotationMatrix		= m_RotationMatrix;
	m_StackBuffer.m_ScalingMatrix		= m_ScalingMatrix;

	m_StackBuffer.m_Pos					= m_Pos;
	m_StackBuffer.m_Rotate				= m_Rotate;
	m_StackBuffer.m_Scale				= m_Scale;

	m_StackMatrix.push( m_StackBuffer );
}

int GCMatrix::PopMatrix()
{
	if( m_StackMatrix.empty() != true )
	{
		m_StackBuffer = m_StackMatrix.top();

		m_FinalMatrix		= m_StackBuffer.m_FinalMatrix;
		m_TranslationMatrix	= m_StackBuffer.m_TranslationMatrix;
		m_RotationMatrix	= m_StackBuffer.m_RotationMatrix;
		m_ScalingMatrix		= m_StackBuffer.m_ScalingMatrix;

		m_Pos				= m_StackBuffer.m_Pos;
		m_Rotate			= m_StackBuffer.m_Rotate;
		m_Scale				= m_StackBuffer.m_Scale;

		m_StackMatrix.pop();
	}
	return static_cast<int>(m_StackMatrix.size());
}

const D3DXMATRIX& GCMatrix::GetMatrix( bool bCompute_ )
{
	if ( bCompute_ == true )
	{
		D3DXMatrixIdentity( &m_FinalMatrix );
		D3DXMatrixMultiply( &m_FinalMatrix, &m_FinalMatrix, &m_ScalingMatrix );
		D3DXMatrixMultiply( &m_FinalMatrix, &m_FinalMatrix, &m_RotationMatrix );
		D3DXMatrixMultiply( &m_FinalMatrix, &m_FinalMatrix, &m_TranslationMatrix );
	}
	return m_FinalMatrix;
}

const D3DXMATRIX& GCMatrix::GetBillBoardMatrix()
{
	D3DXMATRIX matView, matTranspose;
	D3DXMATRIX matbill, matFinal;

	ms_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixTranspose(&matTranspose, &matView);
	D3DXMatrixIdentity(&matbill);

	matbill._11 = matTranspose._11;
	matbill._12 = matTranspose._12;
	matbill._13 = matTranspose._13;
	matbill._21 = matTranspose._21;
	matbill._22 = matTranspose._22;
	matbill._23 = matTranspose._23;
	matbill._31 = matTranspose._31;
	matbill._32 = matTranspose._32;
	matbill._33 = matTranspose._33;

	m_FinalMatrix = m_ScalingMatrix * m_RotationMatrix * matbill * m_TranslationMatrix;

	return m_FinalMatrix;
}
