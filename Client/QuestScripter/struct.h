#pragma once
#include "stdafx.h"
//#include <vector>
//#include <map>
//#include <string>
#include "FunctionToScriptFile.h"
#include "Category.h"

enum NamesOfLevel { QUESTX = 0, STAGEX, NOTHING };


// TileMap Info
#define MAX_DIRECTION   32
#define DIRECTION_NONE   0
#define DIRECTION_RIGHT  1
#define DIRECTION_DOWN   2
#define DIRECTION_LEFT   4
#define DIRECTION_UP     8
#define DIRECTION_USE_SELECT     16

struct TileMap
{
    TileMap()
    {
        strName     = L"";
        iStage      = -1;
        iD_Path     = DIRECTION_NONE;
        iFlag       = 0;
        iRow        = 0;
        iCol        = 0;
    }

    std::wstring     strName;
    int             iStage;
    int             iD_Path;
    int             iFlag;
    int             iRow;
    int             iCol;
};

struct NOLInfo
{
    NOLInfo::NOLInfo()
    {
        NOL     = QUESTX;
        index   = 0;
    }

    int             index;
    NamesOfLevel    NOL;
};

struct struct_base
{
    struct_base()
    {
        MatchKey = 0;
        NOL = QUESTX;
    }

    virtual void SaveToFile(HANDLE hWrite, int iTab, int cnt, int &iTG_ID)
    {

    }

    virtual struct_base* FindStructFromItem(HTREEITEM hItem)
    {
        return NULL;
    }

    struct_base* FindStructFromItem(std::vector<NOLInfo> *vtNOLInfo, int index, HTREEITEM hItem)
    {
        return NULL;
    }

	virtual void DeleteItem(void)
	{

	}

	virtual void DeleteItem(HTREEITEM hItem)
	{

	}

    virtual void AddBlankChild(HTREEITEM hItem, NamesOfLevel NOL)
    {

    }

    // 얕은 복사로써... 하위구조까지 복사하지 않는다. 오직 표시되는 값만 표시
    virtual struct_base &ShallowCopy(const struct_base &rhs)
    {
        return *this;
    }

    virtual bool Verify(void)
    {
        return true;
    }

    struct_base & operator=(const struct_base &rhs)
    {
        this->MatchKey		        = rhs.MatchKey;
        this->NOL			        = rhs.NOL;

        return *this;
    }

    HTREEITEM MatchKey;
    NamesOfLevel NOL;
};

struct Stage : public struct_base
{
    Stage()
    {
        m_iStage    = -1;
        strStageMap = "";
        strStageBGM = "";
        NOL = STAGEX;
    }
    ~Stage()
    {
        ClearCategory();
    }

    void SaveToFile(HANDLE hWrite, int iTab, int cnt, int &iTG_ID)
    {
        if( !hWrite ) return;
        try
        {
            CStringA str;
            str.Format("stage%d", cnt);
            WriteOpenBrace(hWrite, str, iTab);
            WriteToFileScript(hWrite, "StageMap", strStageMap, iTab+1);
            WriteToFileScript(hWrite, "StageBGM", strStageBGM, iTab+1);
            for(int i=0;i<(int)vtCategory.size();i++)
                vtCategory[i]->SaveToFile(hWrite, iTab+1, i, iTG_ID);
            WriteCloseBrace(hWrite, iTab, cnt);
        }
        catch (...)
        {
            MessageBox(0, L"파일을 저장하던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요.", L"알림", MB_OK);
        }
    }

    struct_base* FindStructFromItem(HTREEITEM hItem)
    {
        if( NULL == hItem ) return NULL;

        if( hItem != MatchKey )
        {
            return NULL;
        }

        return this;
    }

    struct_base* FindStructFromItem(std::vector<NOLInfo> *vtNOLInfo, int index, HTREEITEM hItem)
    {
        if( NULL == hItem ) return NULL;

        if( hItem != MatchKey )
        {
            return NULL;
        }

        NOLInfo temp;
        temp.index = index;
        temp.NOL = STAGEX;
        vtNOLInfo->push_back(temp);
        return this;
    }

	void DeleteItem(void)
	{
        Stage();
        ClearCategory();
	}

	void DeleteItem(HTREEITEM hItem)
	{
        if( this->MatchKey == hItem )
        {
            DeleteItem();
            return;
        }
	}

    void AddBlankChild(HTREEITEM hItem, NamesOfLevel NOL)
    {
    }

    void ClearCategory(void)
    {
        for(int i=0;i<(int)vtCategory.size();i++)
            SAFE_DELETE(vtCategory[i]);
        vtCategory.clear();
    }

    Stage &ShallowCopy(const Stage &rhs)
    {
        this->strStageMap		    = rhs.strStageMap;
        this->strStageBGM		    = rhs.strStageBGM;

        this->ClearCategory();
        for(int i=0;i<(int)rhs.vtCategory.size();i++)
        {
            CCategory *pCategory = new CCategory;
            *pCategory = *rhs.vtCategory[i];
            this->vtCategory.push_back(pCategory);
        }

        return *this;
    }

	Stage & operator=(const Stage &rhs)
	{   
        this->m_iStage              = rhs.m_iStage;
		this->MatchKey		        = rhs.MatchKey;
		this->NOL			        = rhs.NOL;
		this->strStageMap		    = rhs.strStageMap;
		this->strStageBGM		    = rhs.strStageBGM;
        this->ClearCategory();
        for(int i=0;i<(int)rhs.vtCategory.size();i++)
        {
            CCategory *pCategory = new CCategory;
            *pCategory = *rhs.vtCategory[i];
            this->vtCategory.push_back(pCategory);
        }
		return *this;
	}

    virtual bool Verify(void)
    {
        if( strStageMap.GetLength() <= 0 )
        {
            if( IDNO == MessageBox(0, L"스테이지 맵을 지정하지 않았습니다. 저장하시겠습니까?\
                                      YES를 누르면 저장을 NO를 누르면 저장을 취소하고 해당위치로 이동합니다.", L"알림", MB_YESNO) )
            {
                AfxGetMainWnd()->SendMessage(WM_CHAGNE_TREE_FOCUS, (WPARAM)this, 0);
                return false;
            }
        }

        return true;
    }

    bool IsDuplicateTGName(std::wstring strTGName)
    {
        for(int i=0;i<(int)vtCategory.size();++i)
        {
            for(int j=0;j<(int)vtCategory[i]->m_vtBaseTrigger.size();j++)
            {
                if( strTGName == vtCategory[i]->m_vtBaseTrigger[j]->m_strTGName )
                    return true;
            }
        }
        return false;
    }

    void GetSimilarTGName(std::wstring strTGName, std::vector<std::wstring> *vtSimilar)
    {
        for(int i=0;i<(int)vtCategory.size();++i)
        {
            for(int j=0;j<(int)vtCategory[i]->m_vtBaseTrigger.size();j++)
            {
                if( wcsstr(vtCategory[i]->m_vtBaseTrigger[j]->m_strTGName.c_str(), strTGName.c_str()) )
                    vtSimilar->push_back(vtCategory[i]->m_vtBaseTrigger[j]->m_strTGName);
            }
        }
    }

    CBaseTrigger *GetEqualNameTrigger(std::wstring strTGName)
    {
        for(int i=0;i<(int)vtCategory.size();++i)
        {
            for(int j=0;j<(int)vtCategory[i]->m_vtBaseTrigger.size();j++)
            {
                if( strTGName == vtCategory[i]->m_vtBaseTrigger[j]->m_strTGName )
                    return vtCategory[i]->m_vtBaseTrigger[j];
            }
        }
        return NULL;
    }

    void GetEqualTypeOfEAction(const int iType, std::vector<CBaseEventAction*> &vtEAction)
    {
        for(int i=0;i<(int)vtCategory.size();++i)
        {
            for(int j=0;j<(int)vtCategory[i]->m_vtBaseTrigger.size();++j)
            {
                CBaseTrigger *pBaseTrigger = vtCategory[i]->m_vtBaseTrigger[j];
                for(int k=0;k<(int)pBaseTrigger->m_vtEvent.size();++k)
                {
                    if( iType == (int)pBaseTrigger->m_vtEvent[k]->m_enType )
                        vtEAction.push_back(pBaseTrigger->m_vtEvent[k]);
                }

                for(int k=0;k<(int)pBaseTrigger->m_vtAction.size();++k)
                {
                    if( iType == (int)pBaseTrigger->m_vtAction[k]->m_enType )
                        vtEAction.push_back(pBaseTrigger->m_vtAction[k]);
                }
            }
        }
    }

    void GetEqualTypeOfCondition(const int iType, std::vector<CBaseCondition*> &vtCondition)
    {
        for(int i=0;i<(int)vtCategory.size();++i)
        {
            for(int j=0;j<(int)vtCategory[i]->m_vtBaseTrigger.size();++j)
            {
                CBaseTrigger *pBaseTrigger = vtCategory[i]->m_vtBaseTrigger[j];
                for(int k=0;k<(int)pBaseTrigger->m_vtCondition.size();++k)
                {
                    if( iType == (int)pBaseTrigger->m_vtCondition[k]->m_enType )
                        vtCondition.push_back(pBaseTrigger->m_vtCondition[k]);
                }
            }
        }
    }

    int                     m_iStage;
    CString                 strStageMap;
    CString                 strStageBGM;
    std::vector<CCategory*>      vtCategory;
};


struct Quest : public struct_base
{
    Quest()
    {
        Init();
    }

    ~Quest()
    {
        ClearStage();
        ClearTileMap();
    }

    void Init(void)
    {
        NOL             = QUESTX;
        iTileX          = 0;
        iTileY          = 0;
    }

    void ClearStage(void)
    {
        for(int i=0;i<(int)vtStage.size();i++)
            SAFE_DELETE(vtStage[i]);
        vtStage.clear();
    }

    void SaveToFile(HANDLE hWrite, int iTab, int cnt, int &iTG_ID)
    {
        if( !hWrite ) return;

        try
        {
            WriteOpenBrace(hWrite, "quest", iTab);
            WriteOpenBrace(hWrite, "questtilemap", iTab+1);
                WriteToFileScript(hWrite, "X_Size", iTileX, iTab+2);
                WriteToFileScript(hWrite, "Y_Size", iTileY, iTab+2);
                for(int i=0;i<iTileY;i++)
                {
                    CStringA strRow;
                    strRow.Format("row%d", i);
                    WriteOpenBrace(hWrite, strRow, iTab+2);
                    for(int j=0;j<iTileX;j++)
                    {
                        CStringA strCol;
                        strCol.Format("col%d", j);
                        TileMap *pTileMap = GetTileMap(i, j);
                        TileMap tilemap;
                        tilemap.iRow = i;
                        tilemap.iCol = j;
                        if( !pTileMap ) pTileMap = &tilemap;
                        WriteOpenBrace(hWrite, strCol, iTab+3);
                        WriteToFileScript(hWrite, "Name", pTileMap->strName, iTab+4);
                        WriteToFileScript(hWrite, "StageNum", pTileMap->iStage, iTab+4);
                        WriteToFileScript(hWrite, "D_Path", pTileMap->iD_Path, iTab+4);
                        WriteToFileScript(hWrite, "Flag", pTileMap->iFlag, iTab+4);
                        WriteCloseBrace(hWrite, iTab+3, cnt);
                    }
                    WriteCloseBrace(hWrite, iTab+2, cnt);
                }
            WriteCloseBrace(hWrite, iTab+1, cnt);
            for(int i=0;i<(int)vtStage.size();i++)
                vtStage[i]->SaveToFile(hWrite, iTab+1, i, iTG_ID);
            WriteCloseBrace(hWrite, iTab, cnt);
        }
        catch (...)
        {
            MessageBox(0, L"파일을 저장하던중 예외가 발생하였습니다, 스크립터가 정상적인 동작을 하지 않으면 다시 실행해 주세요.", L"알림", MB_OK);
        }
    }

    struct_base* FindStructFromItem(HTREEITEM hItem)
    {
        if( NULL == hItem ) return NULL;

        if( hItem != MatchKey )
        {
            struct_base *base;
            for(int i=0;i<(int)vtStage.size();i++)
            {
                base = vtStage[i]->FindStructFromItem(hItem);
                if( base ) return base;                
            }

            return NULL;
        }

        return this;
    }

    struct_base* FindStructFromItem(std::vector<NOLInfo> *vtNOLInfo, int index, HTREEITEM hItem)
    {
        if( NULL == hItem ) return NULL;

        if( hItem != MatchKey )
        {
            struct_base *base;
            for(int i=0;i<(int)vtStage.size();i++)
            {
                base = vtStage[i]->FindStructFromItem(vtNOLInfo, i, hItem);
                if( base )
                {
                    NOLInfo temp;
                    temp.index = index;
                    temp.NOL = QUESTX;
                    vtNOLInfo->push_back(temp);
                    return base;
                }
            }

            return NULL;
        }

        NOLInfo temp;
        temp.index = index;
        temp.NOL = QUESTX;
        vtNOLInfo->push_back(temp);
        return this;
    }

	void DeleteItem(void)
	{
        Init();

		ClearStage();
        ClearTileMap();
	}

	void DeleteItem(HTREEITEM hItem)
	{
        if( this->MatchKey == hItem )
        {
            DeleteItem();
            return;
        }

		for(int i=0;i<(int)vtStage.size();i++)
		{
			if( hItem == vtStage[i]->MatchKey )
			{
				vtStage[i]->DeleteItem();
                SAFE_DELETE(vtStage[i]);
				vtStage.erase(vtStage.begin()+i);
				return;
			}
			vtStage[i]->DeleteItem(hItem);
		}
	}

    void AddBlankChild(HTREEITEM hItem, NamesOfLevel NOL)
    {
        switch(NOL)
        {
            case STAGEX:
                {
                    Stage *pStage = new Stage;
                    pStage->MatchKey   = hItem;
                    pStage->NOL        = STAGEX;
                    vtStage.push_back(pStage);
                }
                break;
        }
    }

    Quest &ShallowCopy(Quest &rhs)
    {
        iTileX = rhs.iTileX;
        iTileY = rhs.iTileY;
        CopyTileMapFromOther(rhs);

        return *this;
    }

	Quest & operator=(Quest &rhs)
	{
		this->MatchKey		= rhs.MatchKey;
		this->NOL			= rhs.NOL;
        this->ClearStage();
        for(int i=0;i<(int)rhs.vtStage.size();i++)
        {
            Stage *pStage = new Stage;
            *pStage = *rhs.vtStage[i];
            this->vtStage.push_back(pStage);
        }

        CopyTileMapFromOther(rhs);

		return *this;
	}

    virtual bool Verify(void)
    {
        for(int i=0;i<(int)vtStage.size();i++)
        {
            if( !vtStage[i]->Verify() ) return false;
        }

        return true;
    }

    void CopyTileMapFromOther(Quest &rhs)
    {
        ClearTileMap();
        std::map<std::string, TileMap*>::iterator itTileMap = rhs.mapTileMap.begin();
        for(;rhs.mapTileMap.end() != itTileMap;)
        {
            CopyTileMap(itTileMap->first, *itTileMap->second);
            itTileMap++;
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

        if( mapTileMap.count( szTemp ) != 0 )
            SAFE_DELETE( mapTileMap[szTemp] );

        mapTileMap[szTemp] = new TileMap;
        *mapTileMap[szTemp] = tilemap;
    }

    void CopyTileMap(std::string strRowCol, TileMap &tilemap)
    {
        TileMap *pOriTileMap = GetTileMap(strRowCol);
        SAFE_DELETE(pOriTileMap);

        TileMap *pTileMap = new TileMap;
        *pTileMap = tilemap;
        mapTileMap[strRowCol] = pTileMap;
    }

    void DelTileMap(int iRow, int iCol)
    {
        char szTemp[1024];
        sprintf(szTemp, "%d_%d", iRow, iCol);
        SAFE_DELETE(mapTileMap[szTemp]);
    }

    void DelTileMap(std::string strRowCol)
    {
        SAFE_DELETE(mapTileMap[strRowCol]);
    }

    void ClearTileMap(void)
    {
        std::map<std::string, TileMap*>::iterator itTileMap = mapTileMap.begin();
        for( ;mapTileMap.end() != itTileMap; ++itTileMap )
            SAFE_DELETE(itTileMap->second);
        mapTileMap.clear();
    }

    void GetEqualTypeOfEAction(const int iType, std::vector<CBaseEventAction*> &vtEAction)
    {
        for(int i=0;i<(int)vtStage.size();++i)
            vtStage[i]->GetEqualTypeOfEAction(iType, vtEAction);
    }

    void GetEqualTypeOfCondition(const int iType, std::vector<CBaseCondition*> &vtCondition)
    {
        for(int i=0;i<(int)vtStage.size();++i)
            vtStage[i]->GetEqualTypeOfCondition(iType, vtCondition);
    }

    bool IsExistFindStageInTileMap(int iStage)
    {
        std::map<std::string, TileMap*>::iterator itorTM = mapTileMap.begin();
        for(;itorTM != mapTileMap.end();itorTM++)
        {
            if( itorTM->second->iStage == iStage )
                return true;
        }

        return false;
    }

    int                                 iTileX;
    int                                 iTileY;
    std::vector<Stage*>                      vtStage;
    std::map<std::string, TileMap*>          mapTileMap;
};

