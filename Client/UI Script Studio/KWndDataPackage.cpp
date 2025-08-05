#include "stdafx.h"

#include"KWndDataPackage.h"
#include ".\kWndManager.h"

void KWndDataPackage::WndData::Init(std::string strName , std::string* strValue)
{
    m_strWndName = strName;
	m_pRealData = strValue;
	m_eDataType = IDE_STRING;
}

void KWndDataPackage::WndData::Init(std::string strName, int* iValue)
{
	m_strWndName = strName;
	m_pRealData = iValue;
	m_eDataType = IDE_INT;
}

void KWndDataPackage::WndData::Init(std::string strName, float* fValue)
{
	m_strWndName = strName;
	m_pRealData = fValue;
	m_eDataType = IDE_FLOAT;
}

CString KWndDataPackage::WndData::GetName()
{
	return m_strWndName.c_str();
}
CString KWndDataPackage::WndData::GetValue()
{

	CString strResult;
	
	
	switch(m_eDataType)
	{
	case IDE_INT:        
		strResult.Format( _T("%d"),*(int*)m_pRealData);
		break;
	case IDE_FLOAT:
		strResult.Format( _T("%f"),*(float*)m_pRealData);
		break;
	case IDE_STRING:
		if ( ((std::string*)m_pRealData)->empty() )
			return _T("");
		strResult.Format( _T("%s"), ((std::string*)m_pRealData)->c_str() );
		break;        
	}
	return strResult;
}

std::string KWndDataPackage::WndData::ToString( UINT uiTab )
{
	char strTemp[1024] = {0};
	std::string strTab;
	strTab.clear();
	
	for ( int i = 0 ; i < uiTab ; i++ )
		strTab += "\t";

	switch(m_eDataType)
	{
	case IDE_INT:
		{
			//sprintf( strTemp, "%s = %d,", m_strWndName.c_str(), *(int*)m_pRealData ); unsafe
            if(GetName().Right(3) == "idx")
            {
                sprintf_s(strTemp, sizeof(strTemp), "%s = %s,",m_strWndName.c_str(), m_strTextureFileName.c_str());
            }
            else
            {
                sprintf_s( strTemp, sizeof(strTemp), "%s = %d,", m_strWndName.c_str(), *(int*)m_pRealData );
            }
			break;
		}
	case IDE_FLOAT:
		{
			//sprintf( strTemp, "%s = %f,", m_strWndName.c_str(), *(float*)m_pRealData );
			sprintf_s( strTemp, sizeof(strTemp), "%s = %f,", m_strWndName.c_str(), *(float*)m_pRealData );
			break;
		}
		
	case IDE_STRING:
		{
			if ( ((std::string*)m_pRealData)->empty() )
				return "";
			//sprintf(strTemp,"%s = \"%s\",",m_strWndName.c_str(), ((std::string*)m_pRealData)->c_str() );
			sprintf_s(strTemp, sizeof(strTemp), "%s = \"%s\",",m_strWndName.c_str(), ((std::string*)m_pRealData)->c_str() );
			break;        
		}
		
	}
	
	return strTab + strTemp;
}

void KWndDataPackage::WndData::SetValue(char* value) 
{
	switch( m_eDataType )
	{
	case IDE_INT:        
		*((int*)m_pRealData) = atoi(value);
		break;
	case IDE_FLOAT:
		*((float*)m_pRealData) = (float)atof(value);
		break;
	case IDE_STRING:
		*((std::string*)m_pRealData) = value;
		break;
		
	default:
		break;
	}
}

void KWndDataPackage::WndData::SetTexFileString( std::vector<std::string>& pVecString )
{
    if(m_eDataType != IDE_INT) return;
    if(GetName().Right(3) != "idx") return;

    int iTextureIndex = *(int*)m_pRealData;
    if(iTextureIndex < pVecString.size()){
        std::string strTextureFileName = "UI_TEXTURE_";
        strTextureFileName += pVecString[iTextureIndex];
        strTextureFileName = strTextureFileName.substr(0,strTextureFileName.find('.'));
        m_strTextureFileName.clear();
        m_strTextureFileName.resize(strTextureFileName.size());
        transform( strTextureFileName.begin(), strTextureFileName.end(), m_strTextureFileName.begin(), (int(*)(int))toupper );
    }

}


KWndDataPackage::KWndDataPackage()
{
	m_mapData.clear();
	m_vecData.clear();
}

KWndDataPackage::~KWndDataPackage()
{
	m_vecData.clear();
	m_mapData.clear();
}

void KWndDataPackage::PushWnd(KD3DWndClass* Wnd)
{
	m_mapData.clear();
	m_vecData.clear();

	Wnd->vCreatePos.x = (int)Wnd->vCreatePos.x;
	Wnd->vCreatePos.y = (int)Wnd->vCreatePos.y;
	Wnd->vWindowPos.x = (int)Wnd->vWindowPos.x;
	Wnd->vWindowPos.y = (int)Wnd->vWindowPos.y;
	Wnd->vDestroyPos.x = (int)Wnd->vDestroyPos.x;
	Wnd->vDestroyPos.y = (int)Wnd->vDestroyPos.y;
	
	Wnd->vCreatePos.x = (int)Wnd->vCreatePos.x;
	Wnd->vCreatePos.y = (int)Wnd->vCreatePos.y;
	Wnd->vWindowPos.x = (int)Wnd->vWindowPos.x;
	Wnd->vWindowPos.y = (int)Wnd->vWindowPos.y;
	Wnd->vDestroyPos.x = (int)Wnd->vDestroyPos.x;
	Wnd->vDestroyPos.y = (int)Wnd->vDestroyPos.y;

	PushData("name"			,&Wnd->strWndName);
//  PushData("sort_index"   ,&Wnd->nSortIndex);
	PushData("visible"		,&Wnd->bVisible);
	PushData("type"			,&Wnd->strType);
	PushData("width"		,&Wnd->dwWidth);
	PushData("height"		,&Wnd->dwHeight);

	PushData("create_x"		,&Wnd->vCreatePos.x);
	PushData("create_y"		,&Wnd->vCreatePos.y);
	PushData("window_x"		,&Wnd->vWindowPos.x);
	PushData("window_y"		,&Wnd->vWindowPos.y);
	PushData("destroy_x"	,&Wnd->vDestroyPos.x);
	PushData("destroy_y"	,&Wnd->vDestroyPos.y);
	
	PushData("def_tex_idx"	,&Wnd->iDefTextureIDx);
	PushData("def_tu1"		,&Wnd->vDefTexCoords.x);
	PushData("def_tv1"		,&Wnd->vDefTexCoords.y);
	PushData("def_tu2"		,&Wnd->vDefTexCoords.z);
	PushData("def_tv2"		,&Wnd->vDefTexCoords.w);

	PushData("hov_tex_idx"	,&Wnd->iHovTextureIDx);
	PushData("hov_tu1"		,&Wnd->vHovTexCoords.x);
	PushData("hov_tv1"		,&Wnd->vHovTexCoords.y);
	PushData("hov_tu2"		,&Wnd->vHovTexCoords.z);
	PushData("hov_tv2"		,&Wnd->vHovTexCoords.w);

	PushData("sel_tex_idx"	,&Wnd->iSelTextureIDx);
	PushData("sel_tu1"		,&Wnd->vSelTexCoords.x);
	PushData("sel_tv1"		,&Wnd->vSelTexCoords.y);
	PushData("sel_tu2"		,&Wnd->vSelTexCoords.z);
	PushData("sel_tv2"		,&Wnd->vSelTexCoords.w);

	PushData("act_tex_idx"	,&Wnd->iActTextureIDx);
	PushData("act_tu1"		,&Wnd->vActTexCoords.x);
	PushData("act_tv1"		,&Wnd->vActTexCoords.y);
	PushData("act_tu2"		,&Wnd->vActTexCoords.z);
	PushData("act_tv2"		,&Wnd->vActTexCoords.w);

	PushData("loc_tex_idx"	,&Wnd->iLocTextureIDx);
	PushData("loc_tu1"		,&Wnd->vLocTexCoords.x);
	PushData("loc_tv1"		,&Wnd->vLocTexCoords.y);
	PushData("loc_tu2"		,&Wnd->vLocTexCoords.z);
	PushData("loc_tv2"		,&Wnd->vLocTexCoords.w);

	PushData("move_gap"		,&Wnd->fMoveGap);
	PushData("pixelvalue"	,&Wnd->bPixelValue);
	PushData("tool_tip"		,&Wnd->strToolTip);

	PushData("motion"		,&Wnd->strMotion);
	PushData("alpha"		,&Wnd->strAlpha);
	PushData("predraw"		,&Wnd->bPredraw);
	PushData("auto_create"  ,&Wnd->bAutoCreate);
}

//  디폴트값이랑 같으면 export하지 않는다.
void KWndDataPackage::PushWndSimple(KD3DWndClass* Wnd)
{
    m_mapData.clear();
    m_vecData.clear();

    Wnd->vCreatePos.x = (int)Wnd->vCreatePos.x;
    Wnd->vCreatePos.y = (int)Wnd->vCreatePos.y;
    Wnd->vWindowPos.x = (int)Wnd->vWindowPos.x;
    Wnd->vWindowPos.y = (int)Wnd->vWindowPos.y;
    Wnd->vDestroyPos.x = (int)Wnd->vDestroyPos.x;
    Wnd->vDestroyPos.y = (int)Wnd->vDestroyPos.y;

    PushData("name"			,&Wnd->strWndName);
//  PushData("sort_index"   ,&Wnd->nSortIndex);
    PushData("type"			,&Wnd->strType);
    
    if ( Wnd->bVisible != TRUE )   
        PushData("visible", &Wnd->bVisible);

    if ( Wnd->dwWidth != 0 ) 
        PushData("width", &Wnd->dwWidth);

    if ( Wnd->dwHeight != 0 )
        PushData("height", &Wnd->dwHeight);

    if ( Wnd->vCreatePos.x != 0.0f )
        PushData("create_x", &Wnd->vCreatePos.x);

    if ( Wnd->vCreatePos.y != 0.0f )
        PushData("create_y", &Wnd->vCreatePos.y);

    if ( Wnd->vWindowPos.x != 0.0f )
        PushData("window_x", &Wnd->vWindowPos.x);

    if ( Wnd->vWindowPos.y != 0.0f )
        PushData("window_y", &Wnd->vWindowPos.y);

    if ( Wnd->vDestroyPos.x != 0.0f )
        PushData("destroy_x", &Wnd->vDestroyPos.x);

    if ( Wnd->vDestroyPos.y != 0.0f )
        PushData("destroy_y", &Wnd->vDestroyPos.y);

    
    // 디폴트 Texture와 같은값이라면 기본값으로 바꾼다.
    CheckTexData( Wnd );

    if ( Wnd->iDefTextureIDx != -1 ) 
    {
        PushData("def_tex_idx"	,&Wnd->iDefTextureIDx);
        PushData("def_tu1"		,&Wnd->vDefTexCoords.x);
        PushData("def_tv1"		,&Wnd->vDefTexCoords.y);
        PushData("def_tu2"		,&Wnd->vDefTexCoords.z);
        PushData("def_tv2"		,&Wnd->vDefTexCoords.w);
    }

    if ( Wnd->iHovTextureIDx != -1 ) 
    {
        PushData("hov_tex_idx"	,&Wnd->iHovTextureIDx);
        PushData("hov_tu1"		,&Wnd->vHovTexCoords.x);
        PushData("hov_tv1"		,&Wnd->vHovTexCoords.y);
        PushData("hov_tu2"		,&Wnd->vHovTexCoords.z);
        PushData("hov_tv2"		,&Wnd->vHovTexCoords.w);
    }

    if ( Wnd->iSelTextureIDx != -1 ) 
    {
        PushData("sel_tex_idx"	,&Wnd->iSelTextureIDx);
        PushData("sel_tu1"		,&Wnd->vSelTexCoords.x);
        PushData("sel_tv1"		,&Wnd->vSelTexCoords.y);
        PushData("sel_tu2"		,&Wnd->vSelTexCoords.z);
        PushData("sel_tv2"		,&Wnd->vSelTexCoords.w);
    }

    if ( Wnd->iActTextureIDx != -1 ) 
    {
        PushData("act_tex_idx"	,&Wnd->iActTextureIDx);
        PushData("act_tu1"		,&Wnd->vActTexCoords.x);
        PushData("act_tv1"		,&Wnd->vActTexCoords.y);
        PushData("act_tu2"		,&Wnd->vActTexCoords.z);
        PushData("act_tv2"		,&Wnd->vActTexCoords.w);
    }

    if ( Wnd->iLocTextureIDx != -1 ) 
    {
        PushData("loc_tex_idx"	,&Wnd->iLocTextureIDx);
        PushData("loc_tu1"		,&Wnd->vLocTexCoords.x);
        PushData("loc_tv1"		,&Wnd->vLocTexCoords.y);
        PushData("loc_tu2"		,&Wnd->vLocTexCoords.z);
        PushData("loc_tv2"		,&Wnd->vLocTexCoords.w);
    }


    if ( Wnd->fMoveGap != 0.0f )
        PushData("move_gap", &Wnd->fMoveGap);
    
    if ( Wnd->bPixelValue != 0 )
        PushData("pixelvalue", &Wnd->bPixelValue);
    
    if ( Wnd->strToolTip != "" )
        PushData("tool_tip", &Wnd->strToolTip);

//     if ( Wnd->strMotion != "linear" )
//         PushData("motion", &Wnd->strMotion);
// 
//     if ( Wnd->strAlpha != "linear" )
//         PushData("alpha", &Wnd->strAlpha);
    
    if ( Wnd->bPredraw != FALSE )
        PushData("predraw", &Wnd->bPredraw);
    
    if ( Wnd->bAutoCreate != TRUE )
        PushData("auto_create", &Wnd->bAutoCreate);
}

void KWndDataPackage::CheckTexData(KD3DWndClass* Wnd)
{
    //  텍스쳐 각 항목들이 동일하다면 기본값으로 변경한다.
    //  소스코드에 해당 항목에 대한 처리가 되어 있다.
    if ( Wnd->iDefTextureIDx == Wnd->iHovTextureIDx && 
         Wnd->vDefTexCoords.x == Wnd->vHovTexCoords.x &&
         Wnd->vDefTexCoords.y == Wnd->vHovTexCoords.y &&
         Wnd->vDefTexCoords.z == Wnd->vHovTexCoords.z &&
         Wnd->vDefTexCoords.w == Wnd->vHovTexCoords.w )
    {
        Wnd->iHovTextureIDx = -1;
        Wnd->vHovTexCoords.x = 0.0f;
        Wnd->vHovTexCoords.y = 0.0f;
        Wnd->vHovTexCoords.z = 1.0f;
        Wnd->vHovTexCoords.x = 1.0f;
    }

    if ( Wnd->iDefTextureIDx == Wnd->iSelTextureIDx && 
        Wnd->vDefTexCoords.x == Wnd->vSelTexCoords.x &&
        Wnd->vDefTexCoords.y == Wnd->vSelTexCoords.y &&
        Wnd->vDefTexCoords.z == Wnd->vSelTexCoords.z &&
        Wnd->vDefTexCoords.w == Wnd->vSelTexCoords.w )
    {
        Wnd->iSelTextureIDx = -1;
        Wnd->vSelTexCoords.x = 0.0f;
        Wnd->vSelTexCoords.y = 0.0f;
        Wnd->vSelTexCoords.z = 1.0f;
        Wnd->vSelTexCoords.x = 1.0f;
    }

    if ( Wnd->iDefTextureIDx == Wnd->iActTextureIDx && 
        Wnd->vDefTexCoords.x == Wnd->vActTexCoords.x &&
        Wnd->vDefTexCoords.y == Wnd->vActTexCoords.y &&
        Wnd->vDefTexCoords.z == Wnd->vActTexCoords.z &&
        Wnd->vDefTexCoords.w == Wnd->vActTexCoords.w )
    {
        Wnd->iActTextureIDx = -1;
        Wnd->vActTexCoords.x = 0.0f;
        Wnd->vActTexCoords.y = 0.0f;
        Wnd->vActTexCoords.z = 1.0f;
        Wnd->vActTexCoords.x = 1.0f;
    }

    if ( Wnd->iDefTextureIDx == Wnd->iLocTextureIDx && 
        Wnd->vDefTexCoords.x == Wnd->vLocTexCoords.x &&
        Wnd->vDefTexCoords.y == Wnd->vLocTexCoords.y &&
        Wnd->vDefTexCoords.z == Wnd->vLocTexCoords.z &&
        Wnd->vDefTexCoords.w == Wnd->vLocTexCoords.w )
    {
        Wnd->iLocTextureIDx = -1;
        Wnd->vLocTexCoords.x = 0.0f;
        Wnd->vLocTexCoords.y = 0.0f;
        Wnd->vLocTexCoords.z = 1.0f;
        Wnd->vLocTexCoords.x = 1.0f;
    }
}

void KWndDataPackage::PushData(std::string strName, std::string* pValue)
{
	WndData data;//= new WndData();
	data.Init(strName , pValue);
	m_mapData[strName] = data;
	m_vecData.push_back(data);
}

void KWndDataPackage::PushData(std::string strName, int* pValue)
{
	WndData data;// = new WndData();
	data.Init(strName , pValue);
	m_mapData[strName] = data;
	m_vecData.push_back(data);
}

void KWndDataPackage::PushData(std::string strName, float* pValue)
{
	WndData data;// = new WndData();
	data.Init(strName , pValue);
	m_mapData[strName] = data;
	m_vecData.push_back(data);
}

void KWndDataPackage::SetValue(std::string strMsg, char* Value)
{
	m_mapData[strMsg.c_str()].SetValue(Value);
}

std::string KWndDataPackage::ToString( UINT uiTab)
{
	std::string result = "";
	//std::map<std::string, WndData*>::iterator it;

	for(int i = 0 ; i < m_vecData.size(); i++)
	{
		// 텍스쳐 인덱스가 -1인건 뛰어넘어라
		if( m_vecData[i].GetName().Right(3) == "idx" )
		{
			if( atoi( ToCharStr( m_vecData[i].GetValue().GetBuffer() )) < 0 )
			{			
				i += 4;
				continue;
			}
            m_vecData[i].SetTexFileString(m_vecTextureFileName);
		}
		result += m_vecData[i].ToString( uiTab );
		result += "\n";
	}
	return result;
}
bool KWndDataPackage::GetData(int index, std::string* pName, std::string* pValue)
{
	if( index >= m_vecData.size() )
		return false;

	( *pName ) = ToCharStr( (m_vecData[index].GetName().GetBuffer()) );
	( *pValue) = ToCharStr( (m_vecData[index].GetValue().GetBuffer()) );
	/*strcpy(pName, m_vecData[index].GetName() );
	strcpy(pValue , m_vecData[index].GetValue() );*/
	return true;
    	
}

void KWndDataPackage::Clear()
{
    m_vecData.clear();
	m_mapData.clear();
	m_vecChildList.clear();
}

void KWndDataPackage::SetTexFileString( std::vector<std::string>& pVecString )
{
    m_vecTextureFileName = pVecString;
}

