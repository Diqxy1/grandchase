#pragma once

class KGCAgitObject;
struct LTP_AGIT_OBJECT_POS;


class KGCAgitObjectContainer
{
public:
    KGCAgitObjectContainer(void);
    ~KGCAgitObjectContainer(void);

private:
    KGCAgitObject*              m_pPickedObject;
    std::vector<KGCAgitObject*> m_vecObject;
    std::map< int, std::vector< KGC2DObject* > > m_map2DObj;            // 화면에 그릴 때 사용 < 레이어, 해당 레이어에 그려질 오브젝트 >
    std::vector< std::map< GCITEMUID, std::pair<int,int> > >   m_vecSavedStep;

public:
    void InitLocalInventoryObject( const KGCAgitObjectInfoList* pkObjInfoList_, int iFlowerPotLimit = 5 );
    void SetObjectPosList( const std::vector<KAgitObjectPos>& vecObjPos_, const KGCAgitObjectInfoList* pkObjInfoList_ );
    std::map< GCITEMID, int > GetHideObjectCount();
    GCITEMUID GetHideObjecctUID( GCITEMID itemID_ );

    void ClearObjectList();
    void Create2DObjectALL();
    bool Create2DObject( GCITEMUID itemUID_ );

    void FrameMove();
    void Render( EGCAgitObjectLayer eAgitLayer_ );
    void Render_PickedObj();

    bool GetObjectInMousePos( IN const D3DXVECTOR2& dxvMousePos, OUT GCITEMUID& itemUID_ );
   
    bool GetFlowerPotInMousePos( IN const D3DXVECTOR2& dxvMousePos, OUT GCITEMUID& itemUID_ );
    GCITEMUID GetPickedObjUID();
    bool IsObjectPick() const { return m_pPickedObject != NULL; }
    bool PickUpObject( GCITEMUID itemUID_, D3DXVECTOR2* pOffset = NULL );
    void PutDownObject();
    void SetPickedObjAlpha( BYTE alpha_ );
    void SetPickedObjRGB( BYTE R_, BYTE G_, BYTE B_ );
    void UpdatePickedObjectPos( const D3DXVECTOR2& dxvMousePos, D3DXVECTOR2* pOffset = NULL  );

    bool IsHideObject( GCITEMUID itemUID_ );
    void HideObject( GCITEMUID itemUID_ );
    void HideObjectAll();
    void ShowObject( GCITEMUID itemUID_, int iPosX_, int iPosY_ );

    void SaveCurrentPosition();
    void GetCurrentPosition( std::map< GCITEMUID, std::pair<int,int> > &mapPos );

    void RollbackPosition();

    bool IsNeedUpdate();
    bool IsPosUpdated();
    void SetPosUpdated();

    std::vector<KAgitObjectPos> GetUpdatedPosList();

    void SetFlowerPotInfo( KFlowerPot& info_ );
    bool IsHarvestableFlower( GCITEMUID itemUID_ );
    bool IsPlantableFlower( GCITEMUID itemUID_ );

    int GetStackedObjectLayer( const GCITEMUID& itemUID_, int iFootholdX_, int iFootholdY_ );

    bool AddDummyObject( GCITEMID itemID_, const KGCAgitObjectInfoList* pkObjInfoList_, int iPosX = -1, int iPosY = -1 );
    void RemoveDummyObject();
    void BuyDummyObject();
    void UpdateLatencyInfo( const LTP_AGIT_OBJECT_POS& kPacket );

    int GetItemCount( GCITEMID itemID_ );
    D3DXVECTOR2 GetObjectPos( GCITEMUID itemUID_ );

    std::map< GCITEMUID, std::pair<int,int> > GetCurSavedPos( void );

    int GetFlowerPotState( GCITEMUID itemUID_ );
    int GetRemainBloomMinute( GCITEMUID itemUID_ );

    bool CheckBoundToMousePos( const D3DXVECTOR2& dxvMousePos, GCITEMID itemID, OUT GCITEMUID& itemUID, D3DXVECTOR4 dxvWeight = D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f) );
    void SetTrainingObjInfo( KTrainingObj& info_, bool ImgUpdate_ );
    int  GetBrokenTrainingTile( GCITEMUID itemUID );
    void ClearTrainingTileImg( GCITEMUID itemUID );
    D3DXVECTOR2 GetTrainigObjPos( GCITEMUID itemUID );

    bool CheckEnableSitDownObject( D3DXVECTOR2& dxvPos_ );
    bool CheckEnableLieObject( D3DXVECTOR2& dxvPos_) ;

    void DoAction( GCITEMUID _itemUID, const std::vector<DWORD>& vecParam );
    std::vector<KGCAgitObject*> GetObjectList( GCITEMID _itemID );

private:
    KGCAgitObject* GetObject( GCITEMUID itemUID_ );
    bool Create2DObject( KGCAgitObject* pObj );
    void SortObjectWithY();
    void AddObject( std::vector<KGCAgitObject*>& vecList, GCITEMUID itemUID, GCITEMID itemID, 
                    KGCAgitObjectInfoList::element_type* kInfo, int iPosX = -1, int iPosY = -1 );
    
    void PackLatencyInfo( KGCAgitObject* pObj );
};
