#pragma once
//#include <map>
////#include <vector>
//#include <set>
//#include <windows.h>
#include <dxutil.h>
#include "GCDeviceLib/GCDeviceVertexBuffer.h"
#include "GCDeviceLib/GCDeviceTexture.h"
#include "KNCSingleton.h"
#include <cmath>
#include "ParticleLib/KTDGParticleSystem.h"
#include "GCUtil/GCRandomObj.h"
#include "GCDropItemBox.h"
#include "GCUtil/KGCRandomInList.h"

class KGCDropItem;

typedef std::map<DWORD,KGCDropItem* > DropItemList;
typedef DropItemList::iterator DIL_itor;

struct SGotItem
{
    int m_iPlayerIndex;
    DWORD m_dwPlayerUID;
    DWORD m_dwDropUID;
    DWORD m_dwGoodsID;

    SGotItem()
    {
        m_iPlayerIndex = -1;
        m_dwPlayerUID = 0;
        m_dwDropUID = 0;
        m_dwGoodsID = 0;
    }

    SGotItem( int iPlayerIndex, DWORD dwPlayerUID, DWORD dwDropUID, DWORD dwGoodsID )
        :m_iPlayerIndex( iPlayerIndex )
        ,m_dwPlayerUID( dwPlayerUID )
        ,m_dwDropUID( dwDropUID )
        ,m_dwGoodsID( dwGoodsID )
    {

    }
};

struct SGPStyle
{
    std::string strMesh;
    std::string strTex;
    std::string strMotion;

    D3DXVECTOR3 vScale;
    D3DXVECTOR2 vTrans;
    D3DXVECTOR2 vSpeed;

    int iStartFrame;

    SGPStyle() : iStartFrame(2)
    {
        strMesh = "Gp.p3m";
        strTex = "Gp_silver.dds";
        strMotion = "Gp_s.frm";

        vTrans = D3DXVECTOR2(0.0f, 0.52f);
        vScale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
        vSpeed = D3DXVECTOR2(4000.0f, 240000.0f);
    }
};

class KGCDropItemManager
{
    DeclareSingleton(KGCDropItemManager);
public:
    KGCDropItemManager(void);
    ~KGCDropItemManager(void);

	void Init();
    void FrameMove();
    void Alpha_Render();

    bool DropItemListPreSet( std::vector< KGameDropElement > vecDropList, DWORD dwRandomSeed );
	bool MissionItemListPreSet( std::vector< KGameDropElement > vecMissionItem, DWORD dwRandomSeed);
    bool GPListPreSet( std::vector< KGameDropElement > vecGpList_, DWORD dwRandomSeed );

    bool AddDropItemListPreSet( std::vector< KGameDropElement > vecDropList );
    bool AddMissionItemListPreSet( std::vector< KGameDropElement > vecMissionItem );
    bool AddGPListPreSet( std::vector< KGameDropElement > vecGpList );
    
    
    bool SendDropInfo( int iTriggerID, D3DXVECTOR2 Pos );
    bool DropItem( std::vector<DWORD> vecDropUID , D3DXVECTOR2 Pos );
	bool DropItemFromList( std::map<DWORD,KGCDropItem* >& mapList, std::vector<DWORD>& vecDropUID , D3DXVECTOR2& Pos);
    void PushGotItem( DWORD dwUserUID_, DWORD dwDropUID_, char cDropType_ );
    void SetObserverToMonster( int iTriggerID, int iMonsterIndex, int iMonsterID );
	void ProcessGoBoxReadyItem();
    void GetStageDropItem( int iCharacterIdx );
    void SetItemUIDToVector( int iTriggerID, std::vector<DWORD>& vecOutList );
    void AddItemUIDToVector( std::map<DWORD,KGCDropItem* >& mapList, int iTriggerID, std::vector<DWORD>& vecOutList );

    bool IsMissionItemTrigger( int iTriggerID );
    DWORD GetGoodsIDFromDropUID( DWORD dwUID );
    std::vector< SGotItem > GetPlayersItemLIst();
    std::set< DWORD > GetObtainGpList() { return m_setGetGP; }
    std::map< DWORD, DWORD > GetObtainItemList() { return m_mapGetItem; }

    bool Clear();
    bool ClearNowDropedItem();
    bool ClearNowDropedGP();
	
    std::set<KGCDropItem* > GetAllDropItem();

private:
    void SetItemVertex();    
    bool PlayerGetItem( DWORD dwUserUID_, int dwDropUID_ );
	bool PlayerGetItemFromList( std::map<DWORD,KGCDropItem* >& vecList, DWORD dwUserUID_, DWORD dwDropUID_ );

public:
    // 한방에 깨기 테스트
    void ClearDungeonGetItem();
    
private:
    //동전을 먹을 때는 패킷을 바로 보내지 않고 GC_PID_MONSTER_LATENCY_FRAME_INFO 보낼 때 같이 보내기위한 멤버
    std::vector<KGC_PID_PLAYERGETITEM> m_vecGPListForSend; 

public:
    void SetGPListForSend(KGC_PID_PLAYERGETITEM kPack ) { m_vecGPListForSend.push_back(kPack); }
    bool IsGPListForSendEmpty() { return m_vecGPListForSend.empty(); }
    bool IsExistGetItem(KGC_PID_PLAYERGETITEM& kGetItem_);

private:
    std::map< std::pair<int,int>, std::vector<D3DXVECTOR4> >    m_mapItemDropRect;  // Key : 던전ID, 스테이지ID, Value : 영역 리스트

public:
    void GetItemDropRect(int iGameMode, int iStage, std::vector<D3DXVECTOR4> &vecItemDropRect);
    void LoadDropRect();
    void LoadScriptGPStyle();
    SGPStyle* GetGpStyle(int iGpType);


private:
    std::map<DWORD,KGCDropItem* > m_mapDropList;
    std::map<DWORD,KGCDropItem* > m_mapMissionItem;
    std::map<DWORD,KGCDropItem* > m_mapGpList;

    std::vector< std::pair< DWORD, DWORD > > m_vecGotItemList; // 플레이어가 먹어서 상태를 바꿔야 하는 아이템, 클리어된다.
    std::vector< KGCDropItem* > m_vecGoBoxItemList;	// 상자에 들어가야 하는 아이템 리스트
    std::vector< SGotItem > m_vecSomeonesItem;   //플레이어가 먹은 아이템
    std::set< DWORD > m_setNoMoreDrop;    //더이상 드랍되지 않을 아이템들
    std::set< DWORD > m_setNoMoreGet;     //이미 누군가 먹은 아이템

    KGCDropItemBox	m_kDropItemBox;		// 주사위 굴려야 되는 아이템을 담는 박스 UI 실제로 데이터를 담지는 않음.

    std::set< DWORD> m_setGetGP;
    std::map< DWORD, DWORD > m_mapGetItem;

    std::map< GCITEMID, SGPStyle > m_mapGPStyle; // < GPType, Gp스타일속성 >

private:
    GCUTIL::GCRand_Int m_kDropRandom;
    GCUTIL::GCRand_Int m_kMissionRandom;
    GCUTIL::GCRand_Int m_kGPRandom;

};
DefSingletonInline(KGCDropItemManager);
