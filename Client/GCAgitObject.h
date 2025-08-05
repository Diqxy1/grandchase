#pragma once

#define TO_AGIT_COORDI(p)  static_cast<float>(p * FOOTHOLDSIZE)

class KGC2DObject;
class KGCAgitObjectInfoList;
struct KAgitObjectPos;
struct KAgitCharacterPos;
struct KAgitObjectInfo;
class KGCAgitObjectContainer;
class KGCAgitObjectAction;

class KGCAgitObject
{
public:
    KGCAgitObject(void);
    ~KGCAgitObject(void);

    enum {
        E_FLOWER_STATE_NONE = 0,
        E_FLOWER_STATE_BUD,
        E_FLOWER_STATE_BLOOM,
    };
private:
    GCITEMID            m_ItemID;
    GCITEMUID           m_ItemUID;
    int                 m_iPosX;
    int                 m_iPosY;
    KGC2DObject*        m_p2DObject;
    KAgitObjectInfo*    m_pObjectInfo;
    KFlowerPot*         m_pkFlowerPot;
    KGCAgitObjectContainer* m_pkContainer;
    bool                m_bFootHoldExist;
    bool                m_bNeedUpdate;
    bool                m_bPosUpdated;
    int                 m_iFlowerState;
    int                 m_iStackedLayer;
    D3DXVECTOR2 m_dxvFootHoldPos;
    D3DXVECTOR3 m_dxvParticleTrace;
    std::vector< KAgitFootHold > m_vecBottomFoothold;

    CParticleEventSeqPTR m_pFlowerParticle;
    std::vector< CParticleEventSeqPTR > m_vecObjectParticle;

    KTrainingObj*   m_pkTraingObj;
    std::vector<KGC2DObject*> m_vecSubObject;


    KGCAgitObjectAction* m_pObjectAction;

public:
    void SetObjectAction( KGCAgitObjectAction* pObjectAction );
    void DoAction ( const std::vector<DWORD>& vecParam );
    DWORD GetUserUID();

private:
    void CreateFlowerParticle();
    void RemoveFlowerParticle();
    void RemoveAllParticle( void );
    void UpdateFlowerState();
    void SetFlowerState( int iState );
    void EventChangeFlowerState();

public:
    void SetItemID( GCITEMID itemID );
    void SetItemUID( GCITEMUID itemUID );
    GCITEMID GetItemID() const { return m_ItemID; }
    GCITEMUID GetItemUID() const { return m_ItemUID; }
    float GetWidth();
    float GetHeight();
    int GetLayer();
    void CreateObjectParticle( void );
    void RemoveObjectParticle( void );

    int GetStackedLayer() const { return m_iStackedLayer; }
    void SetStackedLayer(int val) { m_iStackedLayer = val; }

    void SetPosX( float fPosX_ );
    void SetPosY( float fPosY_ );
    void SetPos( float fPosX_, float fPosY_ );
    float GetPosX();
    float GetPosY();
    D3DXVECTOR2 GetPos() { return D3DXVECTOR2(GetPosX(), GetPosY()); }

    void SetIndexPosX( int iPosX_ );
    void SetIndexPosY( int iPosY_ );
    int GetIndexPosX() const { return m_iPosX; }
    int GetIndexPosY() const { return m_iPosY; }
    void SetIndexPos( int iPosX_, int iPosY_ );

    void SetContainer( KGCAgitObjectContainer* pkContainer_ );
    void SetObjectInfo( const KAgitObjectInfo* kInfo_ );
    bool Create2DObject();
    bool Create2DObject( int iPosX_, int iPosY_ );
    bool CheckAvailablePlace();
    bool IsHide();
    void SetHide();
    bool CheckBounce( const D3DXVECTOR2& dxvMousePos );
    bool CheckBounce( const D3DXVECTOR2& dxvMousePos, const D3DXVECTOR4& dxvWeight );
    bool CheckBounceEnableSitDownd( const D3DXVECTOR2& dxvMousePos );
    bool CheckBounceEnableLie( const D3DXVECTOR2& dxvMousePos );
    KGC2DObject* Get2DObject() { return m_p2DObject; }

    bool IsNeedUpdate();
    void SetNeedUpdate(bool val) { m_bNeedUpdate = val; }
    bool IsPosUpdated();
    void SetPosUpdated(bool val) { m_bPosUpdated = val; }

    void RemoveFootHold();
    void SetFootHold( bool bForce_ = false );

    void FrameMove();
    void Render();
    bool CrashCheckWithFootHold();

    bool IsFlowerPot();
    bool IsPlantableFlowerPot();
    bool IsHarvestableFlowerPot();
    void SetFlowerPotInfo( const KFlowerPot& kFlowerPot_, bool bOnFlowerInfoUpdate_ = false );
    const KFlowerPot* GetFlowerPotInfo() { return m_pkFlowerPot; }
    int  GetFlowerState( void ) { return m_iFlowerState; }
    int  GetRemainBloomMinute( void );

    bool IsMyFootHold( int iPosX_, int iPosY_ );

    void SetTrainingObjInfo( const KTrainingObj& info_, bool ImgUpdate_ );
    int  GetTrainingObjTile( void );
    void UpdateTrainingTile( bool bUptate_ );
    const KTrainingObj* GetTrainingObjInfo( void ) { return m_pkTraingObj; }
    D3DXVECTOR2 GetTrainingObjPos( void );

    void ClearSubObject( void );

    bool IsEnableSitDown( void );
    bool IsEnableLie( void );
};
