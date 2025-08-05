/*�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�

	�� ������ �� �̻�ȣ
	�� ������ �� 2013�� 05�� 21��
	�� E-Mail �� shmhlove@kog.co.kr
	�� Ŭ���� �� �� Ŭ����( �������� ������ ���� �������� ���� �� )

�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�*/
#pragma once
#include "GCWall.h"

struct KWallReloadList
{
    DWORD dwSlot;
    DWORD dwType;
    float fPosX;
    KWallReloadList() : dwSlot(0), dwType(0), fPosX(0.0f){}
};

class KGCWallManager
{
    DeclareSingleton( KGCWallManager );
public:
    KGCWallManager(void);
    ~KGCWallManager(void);

#if !defined( __PATH__ )
    void CheckFileMonitor();
    void RegisterFileMoniter( const char *szFileName );
#endif

public:
    void RegisterLuaBind();
    void LoadScript();
    void LoadWallTemplate( KLuaManager &luaMgr );

public:
    KGCWall* CreateWall( DWORD dwType, DWORD dwSlot, float fPosX );

public:
    void DestoryAllInstance();
    void ClearInstance( DWORD dwSlot );

public:
    void FrameMove();
    void CheckInWallBound( float &fPos );

public:
    void TextRender();

public:
    const KGCWallTemplate* GetWallTemplate( DWORD dwType );
    KGCWall* GetWallInstance( DWORD dwSlot );

private:
    std::map<DWORD, KGCWall*>           m_mapWallInstance;
    std::map<DWORD, KGCWallTemplate>    m_mapWallTemplate;
};

DefSingletonInline( KGCWallManager );