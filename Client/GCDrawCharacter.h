#pragma once


#include "Player.h"

class GCObject;

class KGCDrawCharacter
{
public:
	KGCDrawCharacter(void);
	KGCDrawCharacter( IDirect3DDevice9* pDevice );
	~KGCDrawCharacter(void);
	enum{
		DC_ONCE,
		DC_LOOP_ONE,
		DC_LOOP_ALL,
		DC_LOOP_RANDOM,
	};

	void Draw();

	void AddMotion( std::string DeviceID, int iMotionID );

	void SetD3DDevice( IDirect3DDevice9*	pDevice );
	void SetPlayerInfo( SUserInfo sUserInfo );
	void SetBackgroundMesh( std::string MeshID );

	void SetViewPort( D3DXVECTOR2 vPosition, DWORD dwWidth, DWORD dwHeight );
	void SetViewPort( DWORD dwPosX, DWORD dwPosY, DWORD dwWidth, DWORD dwHeight );

	void ChangeMotion();

	void Clear();
	void ClearMotionList();
    PLAYER & GetPlayer () { return m_Player; }

protected:
	void CreatePlayer();

private:
	IDirect3DDevice9*   m_pd3dDevice;
	D3DVIEWPORT9        m_viewport;
	GCObject*			m_pBackground;
	GCDeviceMotionFRM*  m_pCurrentMotion;
	std::vector< GCDeviceMotionFRM*>	m_vecMotions;
    std::vector< int >	m_vecMotionID;
	D3DXMATRIX          m_matProject;

	PLAYER				m_Player;

	float               m_fAngleMove;
	int					m_iMotionCount;
	float				m_fZoom;
	float				m_fCharX;
	float				m_fYow, m_fPitch, m_fRoll;
	float				m_fPosX, m_fPosY, m_fPosz;

	int				m_iMotionLoop;
};
