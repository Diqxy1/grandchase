#pragma once
//#include "knc\UserPacket.h"
#include "ParticleLib/KTDGParticleSystem.h"
#include "GCUtil/GCRandomObj.h"
//#include "GCItem.h"

class GCDeviceTexture;

class KGCDropItem
{
public:
    KGCDropItem(void);
    virtual ~KGCDropItem(void);

    enum{
        IS_NONE     = -1,
        IS_DROP     = 0,
        IS_GETTING  = 1,
        IS_GOT      = 2,
		IS_GOBOX_READY = 3,
		IS_GOBOX	= 4,
        IS_END,
    };
public:
    void FrameMove();

    virtual void Render();
    virtual bool DropItem( D3DXVECTOR2 Pos ) { return true; };
    virtual GCDeviceTexture* GetTexture();
    void SetRandomSeed( DWORD dwSeed )  { m_kRandom.SetSeed( dwSeed ); }
    char     GetDropType() { return m_cDropType; }
    DWORD   GetDropUID()  { return m_dwDropUID; }
    DWORD   GetGoodsID()  { return m_dwGoodsID; }
    int     GetState()    { return m_iState;    }
	int		GetTriggerID()	{ return m_iTriggerID; }
    void    SetState(int iState)    { m_iState = iState; }
    virtual bool    PlayerGetItem( DWORD dwUserUID_ );
    bool    CheckTriggerID( int iTriggerID ) { return m_iTriggerID == iTriggerID; }
    int     RemoveParticleFromItem();
	virtual void    SetPacketData( const KGameDropElement& src_ );
	bool IsRareItem();

	static void InitItemKindColor();
	static void ClearItemKindColor();
	static std::string& GetItemKindColor( GCITEM_KIND eKind );
    
    void SetItemDropRect( std::vector<GCCollisionRect<float>> &vecRect) { m_vecDropRect = vecRect; }

    int PlayerGetDropedItem( int iPlayerIndex_ );
    void    SetPosition( D3DXVECTOR2 arg )
    {
        m_fx = arg.x;
        m_fy = arg.y;
    }
    bool operator==( const D3DXVECTOR3 arg )
    {
        if( abs( m_fx - arg.x ) < 0.2f && abs( m_fy + 0.056f - arg.y ) < 0.2f )
            return true;
        return false;
    }
protected:
    virtual void Drop_FrameMove()       {}
    virtual void Getting_FrameMove()    {}
    virtual void Got_FrameMove()        {}
	virtual void GoBox_FrameMove()		{}    
    int CheckBoundWithStage();
    int CheckBoundWithPlayer();
	int CheckBoundWithMaxXRange();
    void CheckBoundWithSettingRect();
    bool LineCollision(D3DXVECTOR2 &vPos1, D3DXVECTOR2 &vPos2, D3DXVECTOR2 &vPos3, D3DXVECTOR2 &vPos4, D3DXVECTOR2 &vCrossPos);
    int SetParticleToItem( char* strParticleName_ );

	int GetPlayerIndexFromUID( DWORD dwUID );
protected:
    int                 m_iTriggerID;
    DWORD               m_dwDropUID;
    DWORD               m_dwUserUID;
    int                 m_iOwnerIndex;
    char                m_cDropType;
    DWORD               m_dwGoodsID;
    int                 m_iDuration;
    int                 m_iMonID;
	int					m_iStageCountType;	//현재까지 클리어한 스테이지 수에 따른 드랍조건

    //아이템 상태
    int                 m_iState;
    int			        m_iTime;

    //렌더링&물리 관련 변수  
	bool				m_bAddMatrixCamera;
    float			    m_fx,m_fy;
	float				m_fMaxLeftX, m_fMaxRightX;	//아이템이 최대로 이동 가능한 X좌표, 맵은 크기보다 작거나 같으면 됨
    float			    m_fXspeed,m_fYspeed;	//아이템 움직임에 물리 적용하기.
    float               m_fXrotation, m_fYrotation, m_fZrotation;
    float               m_fXscale, m_fYscale, m_fZScale;
    float               m_fXtrans, m_fYtrans;
	BYTE				m_cAlpha;
    GCDeviceTexture*    m_pTexture;
    CParticleEventSeqPTR m_pParticle;
    std::wstring        m_strName;
	DWORD               m_dwFontColor;
	static std::map< GCITEM_KIND, std::string > m_mapItemKindColor;

    // 이 객체에서 사용할 랜덤 객체, 모든 유저가 랜덤을 돌리면 같은 값이 나와야 함.
    GCUTIL::GCRand_Int m_kRandom;

    GCDeviceMotionFRM*  m_pBoxMotion;
    GCObject*           m_pBoxObject;
    DWORD               m_dwCurrentFrame;

    float               m_fGravity;
    float               m_fReverseSpeed;
    float               m_fGettingUpSpeed;
    DWORD               m_dwPlusFrame;
    float               m_fGettingUpValocity;
    float               m_fGettingUpGravity;
    D3DXVECTOR3         m_vParticleTracePos;
    std::vector<GCCollisionRect<float>>   m_vecDropRect;
};

class KGCDropItem_ITEM : public KGCDropItem
{
public:
    KGCDropItem_ITEM(void);
    ~KGCDropItem_ITEM(void){};

	virtual void Render();
	virtual bool DropItem( D3DXVECTOR2 Pos );
	virtual bool PlayerGetItem( DWORD dwUserUID_ );
    virtual GCDeviceTexture* GetTexture();
    virtual void SetPacketData( const KGameDropElement& src_ );
protected:
    virtual void Drop_FrameMove();
    virtual void Getting_FrameMove();
    virtual void Got_FrameMove();
	virtual void GoBox_FrameMove();
};

class KGCDropItem_GP : public KGCDropItem_ITEM
{
public:
    KGCDropItem_GP(void){}
    ~KGCDropItem_GP(void) {}

    virtual bool DropItem( D3DXVECTOR2 Pos );
    virtual GCDeviceTexture* GetTexture();
    virtual void SetPacketData( const KGameDropElement& src_ );
    bool PlayerGetItem( DWORD dwUserUID_ );
protected:
    virtual void Getting_FrameMove();
};

class KGCDropItem_EXP : public KGCDropItem
{
public:
    KGCDropItem_EXP() {}
    ~KGCDropItem_EXP() {}
    virtual bool DropItem( D3DXVECTOR2 Pos );
    virtual void Getting_FrameMove();
    virtual void SetPacketData( const KGameDropElement& src_ );
};

class KGCDropItem_MISSION : public KGCDropItem_ITEM
{
public:
	KGCDropItem_MISSION() {};
	 ~KGCDropItem_MISSION() {};
	virtual void Render();
	virtual bool DropItem( D3DXVECTOR2 Pos );
	virtual bool PlayerGetItem( DWORD dwUserUID_ );
    virtual void SetPacketData( const KGameDropElement& src_ );
protected:
	virtual void Drop_FrameMove();
	virtual void Getting_FrameMove();
};
