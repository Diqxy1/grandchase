/*�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�

	�� ������ �� �̻�ȣ
	�� ������ �� 2013�� 05�� 21��
	�� E-Mail �� shmhlove@kog.co.kr
	�� Ŭ���� �� �� Ŭ����( �������� ������ ���� �������� ���� �� )

�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�*/
#pragma once
#include "../KGCObj.h"
#define PUSH_GAP 0.0005f

struct KWallParticle
{
    std::string strSquence;
    D3DXVECTOR2 vOffset;
    KWallParticle()
    {
        strSquence.clear();
        vOffset = D3DXVECTOR2(0.0f, 0.0f);
    }
};

class KGCWallTemplate
{
public:
    KGCWallTemplate()     {   Init(); };
    ~KGCWallTemplate()    {           };

public:
    void Init();

public:
    DWORD       m_dwID;
    int         m_iDirection;
    int         m_iInBoundSize;

public:
    std::vector<KWallParticle> m_vecCreateParticleList;
    std::vector<KWallParticle> m_vecBrokenParticleList;
};


enum
{
    WALL_LEFT = 0,      // ���ʿ��� ������� ���ϴ� ��
    WALL_RIGHT = 1,     // �����ʿ��� ������� ���ϴ� ��
};

class KGCWall
{
public:
    KGCWall( const KGCWallTemplate *pTemplate, float fPosX );
    ~KGCWall();

public:
    void ReleaseResource();

public:
    void AddParticle( const char *szSequence, float fOffsetX, float fOffsetY );

public:
    static void RegisterLuaBind();

public:
    void FrameMove();
    void CheckUnitPushAndInBound();
    void CheckInWallBound( float &fPosX );

public:
    void TextRender();
    void RenderBoundingBox();

public:
    DWORD m_dwUID;
    float m_fPosX;
    bool m_bDestroyWall;
    bool m_bCheckInBound;

public:
    const KGCWallTemplate* m_pWallTemplate;

private:
    std::vector<CParticleEventSeqPTR>   m_vecParticleList;
};