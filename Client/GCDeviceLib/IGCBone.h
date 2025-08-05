#pragma once
#include "stdafx.h"

#define INVALID_BONE_INDEX 255
struct KAngleBoneFromFile
{
	D3DXVECTOR3 vPos;
	float fScale;
	KSafeArray<unsigned char,10> acChildIndex; // (최고 10개의 자식을 가질 수 있다)
};

struct KAngleBoneOnMemory
{
	D3DXMATRIX matLocal;
	KSafeArray<unsigned char,10> acChildIndex; // (최고 10개의 자식을 가질 수 있다)
};

struct KPositionBone
{
	D3DXVECTOR3 vPos;
	KSafeArray<unsigned char,10> acChildIndex; // (최고 10개의 자식을 가질 수 있다)
};

class IGCBone
{
public:
	IGCBone(void);
	virtual ~IGCBone(void);
	std::vector<KAngleBoneOnMemory*>& GetAngleBone(){ return m_vecAngleBone; }
	std::vector<KPositionBone*>&      GetPositionBone(){ return m_vecPositionBone; }
	DWORD GetNumAngleBone()             { return m_dwNumAngleBone; }
	DWORD GetNumPositionBone()          { return m_dwNumPositionBone; }
	inline void ClearAngleBon(){
		if(m_vecAngleBone.empty()) return;
		if(*m_vecAngleBone.begin() == NULL ) return;
		delete [](*m_vecAngleBone.begin());
		m_vecAngleBone.clear();
	}
	inline void ClearPositionBone(){
		if(m_vecPositionBone.empty()) return;
		if(*m_vecPositionBone.begin() == NULL ) return;
		delete [](*m_vecPositionBone.begin());
		m_vecPositionBone.clear();
	}
	inline void SetAngleBon(KAngleBoneOnMemory* pAngleBon, int iMaxIndex){
		for(int i = 0; i < iMaxIndex ; i++)
		{
			m_vecAngleBone.push_back(&(pAngleBon[i]));
		}
	}
	inline void SetPositionBone(KPositionBone* pPositionBone, int iMaxIndex){
		for(int i = 0; i < iMaxIndex ; i++)
		{
			m_vecPositionBone.push_back(&(pPositionBone[i]));
		}
	}
	inline void CopyAngleBon(std::vector<KAngleBoneOnMemory*> vecAngleBon){
		std::vector<KAngleBoneOnMemory*>::iterator vit = vecAngleBon.begin();
		std::vector<KAngleBoneOnMemory*>::iterator m_vit = m_vecAngleBone.begin();
		for(; vit != vecAngleBon.end() ;vit++, m_vit++)
		{
			if (m_vit == m_vecAngleBone.end())
				break;
			*(*m_vit) = *(*vit);
		}
	}
	inline void CopyPositionBone(std::vector<KPositionBone*> vecPositionBone){
		std::vector<KPositionBone*>::iterator vit = vecPositionBone.begin();
		std::vector<KPositionBone*>::iterator m_vit = m_vecPositionBone.begin();
		for(; vit != vecPositionBone.end() ;vit++, m_vit++)
		{
			if (m_vit == m_vecPositionBone.end())
				break;
			*(*m_vit) = *(*vit);
		}
	}

protected:
	std::vector<KAngleBoneOnMemory*>    m_vecAngleBone;
	std::vector<KPositionBone*>         m_vecPositionBone;
	DWORD						m_dwNumPositionBone;		//파일에는 BYTE형으로 저장되어 있음
	DWORD						m_dwNumAngleBone;			//파일에는 BYTE형으로 저장되어 있음
};
