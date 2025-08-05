#pragma once
//#include <string>
////#include <vector>
//#include <map>
using namespace std;

#define MAX_DIRECTION   15
#define DIRECTION_NONE   0
#define DIRECTION_RIGHT  1
#define DIRECTION_DOWN   2
#define DIRECTION_LEFT   4
#define DIRECTION_UP     8
struct TileMap
{
    TileMap()
    {
        strName     = "";
        iStage      = -1;
        iD_Path     = DIRECTION_NONE;
        iFlag       = 0;
        iRow        = -1;
        iCol        = -1;
    }

    std::string     strName;
    int             iStage;
    int             iD_Path;
    int             iFlag;
    int             iRow;
    int             iCol;
};

//************************************************************************
// 퀘스트 스크립트의 기본구조
//************************************************************************
struct struct_base
{
    struct_base()
    {
    }

    virtual ~struct_base()
    {
    }

    // 얕은 복사로써... 하위구조까지 복사하지 않는다. 오직 자신의 값만 복사.
    virtual struct_base &ShallowCopy(const struct_base &rhs)
    {
        return *this;
    }

    virtual void DeleteItem(void)
    {

    }

    struct_base & operator=(const struct_base &rhs)
    {

        return *this;
    }
};

//************************************************************************
// 스테이지 구조
//************************************************************************
struct Stage : public struct_base
{
    Stage()
    {
        strStageMap = "";
    }

    Stage &ShallowCopy(const Stage &rhs)
    {
        this->strStageMap		    = rhs.strStageMap;

        return *this;
    }

    void DeleteItem(void)
    {
        Stage();
    }

    Stage & operator=(const Stage &rhs)
    {   
        this->strStageMap		    = rhs.strStageMap;
        return *this;
    }
    
    std::string strStageMap;
};

//************************************************************************
// 퀘스트 구조
//************************************************************************
struct Quest : public struct_base
{
    Quest()
    {
        iTileX = 0;
        iTileY = 0;
    }

    Quest &ShallowCopy(Quest &rhs)
    {
        CopyTileMapFromOther(rhs);

        return *this;
    }

    void DeleteItem(void)
    {
        Quest();

        for(int i=0;i<(int)vtStage.size();i++)
            vtStage[i].DeleteItem();
        vtStage.clear();

        ClearTileMap();
    }

    Quest & operator=(Quest &rhs)
    {
        this->vtStage.clear();
        for(int i=0;i<(int)rhs.vtStage.size();i++)
            this->vtStage.push_back(rhs.vtStage[i]);

        CopyTileMapFromOther(rhs);

        return *this;
    }

    void CopyTileMapFromOther(Quest &rhs)
    {
        ClearTileMap();
        map<std::string, TileMap*>::iterator itTileMap = rhs.mapTileMap.begin();
        for(;rhs.mapTileMap.end() != itTileMap;itTileMap++)
        {
            CopyTileMap(itTileMap->first, *itTileMap->second);            
        }
    }

    TileMap *GetTileMap(int iRow, int iCol)
    {
        char szTemp[1024];
        sprintf(szTemp, "%d_%d", iRow, iCol);
        if( mapTileMap.count(szTemp) == 0 )
            return NULL;

        return mapTileMap[szTemp];
    }

    TileMap *GetTileMap(std::string strRowCol)
    {
        if( mapTileMap.count(strRowCol) == 0 )
            return NULL;

        return mapTileMap[strRowCol];
    }

    void SetTileMap(TileMap &tilemap)
    {
        char szTemp[1024];
        sprintf(szTemp, "%d_%d", tilemap.iRow, tilemap.iCol);

        DelTileMap(szTemp);

        mapTileMap[szTemp] = new TileMap;
        TileMap* pTile = mapTileMap[szTemp];
        *pTile = tilemap;
    }

    void CopyTileMap(std::string strRowCol, TileMap &tilemap)
    {
        DelTileMap(strRowCol);

        TileMap *pTileMap = new TileMap;
        *pTileMap = tilemap;
        mapTileMap[strRowCol] = pTileMap;
    }

    TileMap FindStage(int iStage)
    {
        map<std::string, TileMap*>::iterator itorTM = mapTileMap.begin();
        for(;itorTM != mapTileMap.end();itorTM++)
        {
            if( itorTM->second->iStage == iStage )
                return *itorTM->second;
        }

        TileMap blank;
        return blank;
    }

	TileMap FindBossStage( void )
	{
		map<std::string, TileMap*>::iterator itorTM = mapTileMap.begin();
		for(;itorTM != mapTileMap.end();itorTM++)
		{
			// 2가 보스임 ㅋㄷㅋㄷ
			if( itorTM->second->iFlag == 2 )
				return *itorTM->second;
		}

		TileMap blank;
		return blank;
	}

    void DelTileMap(int iRow, int iCol)
    {
        char szTemp[1024];
        sprintf(szTemp, "%d_%d", iRow, iCol);
        DelTileMap(szTemp);
    }

    void DelTileMap(std::string strRowCol)
    {
        map<std::string, TileMap*>::iterator itTileMap = mapTileMap.find(strRowCol);
        if( mapTileMap.end() != itTileMap )
        {
            SAFE_DELETE(itTileMap->second);
            mapTileMap.erase(itTileMap);
        }
    }

    void ClearTileMap(void)
    {
        map<std::string, TileMap*>::iterator itTileMap = mapTileMap.begin();
        for( ;mapTileMap.end() != itTileMap; ++itTileMap )
            SAFE_DELETE(itTileMap->second);
        mapTileMap.clear();
    }

    int                         iTileX;
    int                         iTileY;
    vector<Stage>               vtStage;
    map<std::string, TileMap*>  mapTileMap;
};

