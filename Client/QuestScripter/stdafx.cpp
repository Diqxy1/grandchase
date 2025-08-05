// stdafx.cpp : 표준 포함 파일을 포함하는 소스 파일입니다.
// QuestScripter.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj는 미리 컴파일된 형식 정보를 포함합니다.

#include "stdafx.h"

char                    g_Path[MAX_PATH];

void SetWindowTextToEdit(CEdit *pEdit, int value)
{
	CString strValue;
	strValue.Format(L"%d", value);
	pEdit->SetWindowText(strValue);
}

void SetWindowTextToEdit(CEdit *pEdit, float value)
{
	CString strValue;
	strValue.Format(L"%f", value);
	pEdit->SetWindowText(strValue);
}

void GetWindowTextToEdit(CEdit *pEdit, int *value)
{
	CString strValue;
	pEdit->GetWindowText(strValue);
	*value = _wtoi(strValue);
}

void GetWindowTextToEdit(CEdit *pEdit, float *value)
{
	CString strValue;
	pEdit->GetWindowText(strValue);
	*value = (float)_wtof(strValue);
}

void HeapChk(CString strText, int iIndex)
{
	int heapstatus = _heapchk();
	CString strCheck = strText;
	strCheck.Format(L"%s(%d)", strText, iIndex);
	switch (heapstatus)
	{
	case _HEAPOK:
		strCheck += L" OK - heap is fine\n";
		MessageBox(0, strCheck, L"", MB_OK);
		break;
	case _HEAPEMPTY:
		strCheck += L" OK - heap is empty\n";
		MessageBox(0, strCheck, L"", MB_OK);
		break;
	case _HEAPBADBEGIN:
		strCheck += L"ERROR - bad start of heap\n";
		MessageBox(0, strCheck, L"", MB_OK);
		break;
	case _HEAPBADNODE:
		strCheck += L"ERROR - bad node in heap\n";
		MessageBox(0, strCheck, L"", MB_OK);
		break;
	}
}

void strncpy_lastNull(char *pDest, char *pSource, int iSize)
{
	if (!iSize) return;

	strncpy(pDest, pSource, iSize - 1);
	pDest[iSize - 1] = '\0';
}

POINT Convert_3DTo2D_Point(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 v)
{
	POINT pt;

	if (!pd3dDevice)
	{
		pt.x = -1;
		pt.y = -1;

		return pt;
	}

	D3DVIEWPORT9 vp;
	D3DXMATRIX matProj;
	D3DXMATRIX matView;
	pd3dDevice->GetViewport(&vp);
	pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	pd3dDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matCombine = matView * matProj;
	D3DXVec3TransformCoord(&v, &v, &matCombine);

	v.x += 1.0f;
	v.y += 1.0f;

	v.x = (vp.Width * (v.x)) / 2.0f + vp.X;
	v.y = (vp.Height * (2.0f - v.y)) / 2.0f + vp.Y;

	pt.x = (long)v.x;
	pt.y = (long)v.y;

	return pt;
}