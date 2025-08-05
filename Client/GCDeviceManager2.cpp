#include "stdafx.h"
#include ".\gcdevicemanager2.h"

//
#include "GCObject.h"
#include "GCUI\GCCustomizarionDlg.h"

GCDeviceManagerWrapper* g_pGCDeviceManager2 = NULL;

GCDeviceManagerWrapper::GCDeviceManagerWrapper(LPDIRECT3DDEVICE9 d3ddevice) : GCDeviceManager(d3ddevice)
{
    g_pGCDeviceManager2 = this;
    m_pEdgeTexture = NULL;

    lua_tinker::class_add<GCDeviceManagerWrapper>( KGCLuabinder::getInstance().GetLuaState(), "GCDeviceMgr_Class" );
#if defined( _DEBUG )
    lua_tinker::class_def<GCDeviceManagerWrapper>( KGCLuabinder::getInstance().GetLuaState(), "Get2DObject",  &GCDeviceManagerWrapper::Get2DObject );
    lua_tinker::class_def<GCDeviceManagerWrapper>( KGCLuabinder::getInstance().GetLuaState(), "Get2DObjectSize",  &GCDeviceManagerWrapper::Get2DObjectSize  );
#endif
    lua_tinker::class_def<GCDeviceManagerWrapper>( KGCLuabinder::getInstance().GetLuaState(), "CheckFileExist",  &GCDeviceManagerWrapper::CheckFileExist );
	lua_tinker::decl( KGCLuabinder::getInstance().GetLuaState(), "DeviceMgr", this );
}

GCDeviceManagerWrapper::~GCDeviceManagerWrapper(void)
{
    SAFE_DELETE( m_pEdgeTexture );
	g_pGCDeviceManager2 = NULL;
}

GCDeviceTexture* GCDeviceManagerWrapper::CreateShopItemTexture( int iItemID )
{
	char strItemID[MAX_PATH];

	if( iItemID < 1000 )
		sprintf( strItemID, "Sbta%03d.dds", iItemID );
	else
		sprintf( strItemID, "Sbta%d.dds", iItemID );

    return CreateTexture( strItemID );
}

GCDeviceTexture* GCDeviceManagerWrapper::CreatePetItemTexture( int iPetGoodsID, int iEvolutionLevel /* = 0 */ )
{
    char strItemID[MAX_PATH];

    if( iEvolutionLevel != 0 )
    {	
        if( iPetGoodsID < 1000 )
            sprintf( strItemID, "Sbta%03d_%d.dds", iPetGoodsID, iEvolutionLevel );
        else
            sprintf( strItemID, "Sbta%d_%d.dds", iPetGoodsID, iEvolutionLevel );
    }
    else
    {
        if( iPetGoodsID < 1000 )
            sprintf( strItemID, "Sbta%03d.dds", iPetGoodsID );
        else
            sprintf( strItemID, "Sbta%d.dds", iPetGoodsID );
    }
    return CreateTexture( strItemID );
}

GCDeviceTexture* GCDeviceManagerWrapper::CreateShopSetItemTexture( int iSetID )
{
	iSetID = iSetID / 2;
	char strFileName[MAX_PATH];
	sprintf( strFileName, "Set%d.dds", iSetID );

	return CreateTexture( strFileName );
}

GCMeshObject* GCDeviceManagerWrapper::CreateAbtaModel(int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, bool build, char cIgBoneIdx)
{
	const char* strAbtaMeshName = GetAbtaName(iItemID_);
	std::string strMeshFileName = GetResourceFileName(strAbtaMeshName, iCharType_, iPromotion, ".p3m");

	int dwItemTextureID = g_pItemMgr->GetSameTextureIndex(iItemID_);
	const char* strAbtaTexName = GetAbtaName(dwItemTextureID);
	std::string strTexFileName = GetLupusResourceFileName(strAbtaTexName, iCharType_, iPromotion, ".dds");

	if ( "" == strTexFileName )
		strTexFileName = GetResourceFileName(strAbtaTexName, iCharType_, iPromotion, ".dds");

	int iTech = -1;
	if (build)
	{
		if (pObj_->AddStaticMesh(strMeshFileName, strTexFileName, cIgBoneIdx))
			return pObj_->GetBuildMeshObject();
	}

	return pObj_->AddMesh(strMeshFileName, strTexFileName, cIgBoneIdx, CARTOON_NORMAL, "", iTech, iItemID_);
}

GCDeviceTexture* GCDeviceManagerWrapper::CreateItemTextureCustom(int iItemID, int iCharType /*= -1*/, int iCharPromotion /*= -1 */)
{
	std::stringstream stmFileName;
	stmFileName.str("");
	std::stringstream stmExtension;
	std::stringstream stmFile;
	if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 1 ) )
	{
		stmExtension.str("");
		stmFileName << "Cbta" << std::setw(3) << std::setfill('0') << iItemID;
		stmExtension << ".dds";
		stmFile.str("");
		stmFile << stmFileName.str() << stmExtension.str();
	}
	else if ( g_pkUIScene->m_pkCustomizationDlg->IsSubWindowActive( 2 ) )
	{
		stmExtension.str("");
		stmFileName << g_pCutInSkinMgr->GetCutInSkinTextureID(iItemID);
		stmFile.str("");
		stmFile << stmFileName.str();
	}

	return CreateTexture(stmFile.str());
}

GCMeshObject* GCDeviceManagerWrapper::CreateRoomAnimationModel(std::string RoomAnimationModel, GCObject* pObj_, bool build, char cIgBoneIdx)
{
	std::stringstream roomModel, roomTexture;
	roomModel.str("");
	roomTexture.str("");
	roomModel << RoomAnimationModel << ".p3m";
	roomTexture << RoomAnimationModel << ".dds";

	std::string strMeshFileName = roomModel.str();
	std::string strTexFileName = roomTexture.str();

	int iTech = -1;
	if (build)
	{
		if (pObj_->AddStaticMesh(strMeshFileName, strTexFileName, cIgBoneIdx))
			return pObj_->GetBuildMeshObject();
	}

	return pObj_->AddMesh(strMeshFileName, strTexFileName, cIgBoneIdx, CARTOON_NORMAL, "", iTech, -1);
}

GCMeshObject* GCDeviceManagerWrapper::CreateLas4MergeAbtaModel( int iItemID_, int iCharType_, GCObject* pObj_ ,bool build)
{
	const char* strAbtaName = GetAbtaName(iItemID_);
	
	std::string strMeshFileName = GetResourceFileName( strAbtaName, iCharType_, 0, "_m.p3m" );
	std::string strTexFileName = GetResourceFileName( strAbtaName, iCharType_, 0, ".dds" );

	int iTech = EGCShaderTechnique::TECHNIQUE_NUM;
	if(build)
	{
		if ( pObj_->AddStaticMesh( strMeshFileName ,strTexFileName ) )
			return NULL;

		iTech = -1;
	}
	
    return pObj_->AddMesh( strMeshFileName ,strTexFileName, -1, CARTOON_NORMAL, "", iTech );
}

void GCDeviceManagerWrapper::Draw_ShopImage( D3DXVECTOR2 vPos, float fWidth, float fHeight, GCDeviceTexture* tex, bool bSetItem, int iSetItemNumber /*= -1*/, D3DCOLOR col/*=0xffffffff*/ )
{
	if( tex == NULL )
	{
		ASSERT( !"Null Texture" );
		return;
	}

	if( bSetItem == true && iSetItemNumber == -1 )
		return;

	PushState();
	SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	if( bSetItem == false )
	{
		DrawInScreen(tex,
			vPos.x, vPos.y, vPos.x + fWidth, vPos.y + fHeight,
			0.0f, 0.0f, 1.0f, 1.0f, false, 0.1f,col );	
	}
	else
	{

		D3DXVECTOR4 vTexCoords;

		if ( 0 == iSetItemNumber % 2 )
		{
			vTexCoords.x = 0.0f;
			vTexCoords.y = 0.0f;
			vTexCoords.z = 0.5f;
			vTexCoords.w = 1.0f;
		}
		else
		{
			vTexCoords.x = 0.5f;
			vTexCoords.y = 0.0f;
			vTexCoords.z = 1.0f;
			vTexCoords.w = 1.0f;
		}

		DrawInScreen(tex,
			vPos.x + fWidth * 0.1f , vPos.y - fHeight * 0.2f, vPos.x + fWidth * 0.8f, vPos.y + fHeight,
			vTexCoords.x, vTexCoords.y, vTexCoords.z, vTexCoords.w, false, 0.1f, col );		
	}	
	PopState();
}


void GCDeviceManagerWrapper::Draw_Text( float f_x, float f_y, const WCHAR* Text, D3DCOLOR Color, int iSize, bool bShadow /*= false*/ ,bool bAutoScale /* = true */)
{	
    if(iSize == -1) iSize = SiKGCMultipleLanguages()->GetLargeFontSize();

	GCDeviceFont* font;
    if(SiKGCMultipleLanguages()->GetDefualtFontSize() == iSize) font = g_pkFontMgr;
    else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == iSize) font = g_pkFontMgr14;
    else if(SiKGCMultipleLanguages()->GetLargeFontSize() == iSize) font = g_pkFontMgr16;
    else if(SiKGCMultipleLanguages()->GetTitleFontSize() == iSize) font = g_pkFontMgrTitle;
    else font = g_pkFontMgr;

	int left,top;
	if (bAutoScale==true)
	{
		left = static_cast<int>( ( f_x + 1.0f ) * GC_SCREEN_FLOAT_WIDTH * GetWindowScaleX() * 0.5f);
		top = static_cast<int>( GC_SCREEN_FLOAT_HEIGHT * GetWindowScaleY() - (int)( ( f_y + 0.75f ) / 1.5f * GC_SCREEN_FLOAT_HEIGHT) * GetWindowScaleY());
	}
	else
	{
		left = static_cast<int>( ( f_x + 1.0f ) * GC_SCREEN_FLOAT_WIDTH * 0.5f);
		top = static_cast<int>( GC_SCREEN_FLOAT_HEIGHT - (int)( ( f_y + 0.75f ) / 1.5f * GC_SCREEN_FLOAT_HEIGHT) );
	}

	

	if ( bShadow )
	{
		RECT rt;
		int strlen = (int)::wcslen( Text );
		rt.left = left + 1;
		rt.top = top + 1;
		rt.right = left + iSize * strlen;
		rt.bottom = top + iSize;

		font->OutTextXY( rt.left, rt.top, Text, D3DCOLOR_ARGB(255,0,0,0), &rt, DT_LEFT | DT_TOP | DT_NO_COLOR );
	}

	font->OutTextXY(left, top, Text, Color );
}

void GCDeviceManagerWrapper::Draw_Text( D3DXVECTOR2 vPos, const WCHAR* Text, D3DCOLOR Color /*= 0xffffffff*/, int iSize /*= SiKGCMultipleLanguages()->GetLargeFontSize()*/, bool bShadow /*= false */,bool bAutoScale /*= true*/ )
{
    if(iSize == -1) iSize = SiKGCMultipleLanguages()->GetLargeFontSize();
	return Draw_Text( vPos.x, vPos.y, Text, Color, iSize, bShadow, bAutoScale );
}

void GCDeviceManagerWrapper::CreateTmpModel( int iItemID_, int iCharType_, int iPromotion, char cIgBoneIdx/* = -1*/ )
{
	const char* strAbtaName = GetAbtaName(iItemID_);

	std::string strMeshFileName = GetResourceFileName( strAbtaName, iCharType_, 0, ".p3m" );
	std::string strTexFileName = GetResourceFileName( strAbtaName, iCharType_, iPromotion, ".dds" );

	CreateTmpTexture(strTexFileName);
	CreateTmpMeshP3M(strMeshFileName, cIgBoneIdx);
}

void GCDeviceManagerWrapper::DrawUI( GCBaseTexture * tex, KD3DUIVertex* vert,D3DXVECTOR4 * uv1, D3DXVECTOR4 * uv2 )
{
	FlushWorld();
	if(m_pCurTex!=tex || m_iScreenVBSize+1 > SCREEN_VB_SIZE)
		FlushScreen();

    for( int i = 0 ; i < 4 ; ++i )
    {
        m_pScreenVertex[4*m_iScreenVBSize+i] = vert[i];
    }

    m_pScreenVertex[4*m_iScreenVBSize+0].m_vTex.x += uv1->z - uv2->x;	m_pScreenVertex[4*m_iScreenVBSize+0].m_vTex.y += uv1->w - uv2->y;
    m_pScreenVertex[4*m_iScreenVBSize+1].m_vTex.x += uv1->z - uv2->x;	m_pScreenVertex[4*m_iScreenVBSize+1].m_vTex.y += uv1->w - uv2->y;
    m_pScreenVertex[4*m_iScreenVBSize+2].m_vTex.x += uv1->z - uv2->x;	m_pScreenVertex[4*m_iScreenVBSize+2].m_vTex.y += uv1->w - uv2->y;
    m_pScreenVertex[4*m_iScreenVBSize+3].m_vTex.x += uv1->z - uv2->x;	m_pScreenVertex[4*m_iScreenVBSize+3].m_vTex.y += uv1->w - uv2->y;	

    m_pCurTex = tex;
    
	m_iScreenVBSize+=1;
}

GCDeviceTexture* GCDeviceManagerWrapper::CreateItemTexture( int iItemID, int iCharType /*= -1*/, int iCharPromotion /*= -1 */ )
{
	std::stringstream stmFileName;
	stmFileName.str("");
	std::stringstream stmExtension;
	stmExtension.str("");
	stmFileName<<"Sbta"<<std::setw(3)<<std::setfill('0')<<iItemID;
    stmExtension<<".dds";
	std::stringstream stmFile;
	stmFile<<stmFileName.str()<<stmExtension.str();
	if( g_pGCDeviceManager->GetMassFileManager()->IsIn( stmFile.str() ) == false )
	{
		stmFile.str("");
		stmFileName<<std::setw(0)<<"_"<<iCharType;
		stmFile<<stmFileName.str()<<stmExtension.str();
		if( g_pGCDeviceManager->GetMassFileManager()->IsIn( stmFile.str() ) == false )
		{
			stmFile.str("");
			stmFileName<<"_"<<iCharPromotion;
			stmFile<<stmFileName.str()<<stmExtension.str();
		}
	}
	return CreateTexture( stmFile.str() );
	
}

std::vector<GCObject*> GCDeviceManagerWrapper::CreateLiveModelList( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, D3DXVECTOR3 vOptionalPos, bool bOnlyTraceParentPos, char *szMotionFileName, int iBoneIdx, bool bUseBoneIDFromArg )
{
    /*
    int iItemID_ : 로드할 아이템ID
    int iCharType_ : 캐릭터 ID
    int iPromotion : 전직 ID
    GCObject* pObj_ : 오브젝트 포인터
    D3DXVECTOR3 vOptionalPos : 붙힐 위치
    bool bPosOnly : 붙은곳의 위치만 따라가게 할것인지( false면 회전도 같이 먹힘 )
    char *szMotionFileName : 메시에 붙힐 모션의 파일명
    int iBoneIdx : 붙힐 본ID
    bool bUseBoneIDFromArg = false : 파라미터로 전달받은 BoneID를 사용한다.
    */

    GCObject* pRet;
    std::vector<GCObject*> vecRet;
    if( -1 != iBoneIdx )
    {
        pRet = CreateLiveModel( iItemID_, iCharType_, iPromotion, pObj_, vOptionalPos, bOnlyTraceParentPos, szMotionFileName, iBoneIdx, bUseBoneIDFromArg );
        if( pRet )
            vecRet.push_back( pRet );
    }
    else
    {
        for( int iLoop = 0; iLoop < 50; iLoop++ )
        {
            pRet = CreateLiveModel( iItemID_, iCharType_, iPromotion, pObj_, vOptionalPos, bOnlyTraceParentPos, szMotionFileName, iLoop );
            if( pRet )
                vecRet.push_back( pRet );
        }
    }
    return vecRet;
}

GCObject* GCDeviceManagerWrapper::CreateLiveModel( int iItemID_, int iCharType_, int iPromotion, GCObject* pObj_, D3DXVECTOR3 vOptionalPos, bool bOnlyTraceParentPos, char *szMotionFileName, int iBoneIdx, bool bUseBoneIDFromArg )
{
    
    int iBoneIndex;

    if( bUseBoneIDFromArg )
        iBoneIndex = -1;
    else
        iBoneIndex = iBoneIdx;

    // 모델 파일
	std::string strMeshFileName = GetLiveAbtaName( iItemID_, iCharType_, iPromotion, ".p3m", &iBoneIndex );
	if ( strMeshFileName.empty() )
		return NULL;

    // 모션 파일
	std::string strMotionFileName = szMotionFileName;
    if( "" == strMotionFileName )
        strMotionFileName = GetLiveAbtaName( iItemID_, iCharType_, iPromotion, ".frm", &iBoneIndex );
    else
    {
        // 2011.10.10 : shmhlove( Lupus 작업 중... )
        // 하나의 GoodsID로 두개이상의 라이브모델을 로드하기위해서 모션Name을 모델별로 여러개 전달받아야하는데
        // 그걸 std::map<modelId, std::string>과 같은 문자열 배열로 받기보다는 파일이름 형식을 통일해서 
        // 아래와 같이 .frm단어가 없으면 BoneID를 덧붙혀 .frm을 붙혀주도록 처리하겠습니다.
        if( NULL == strstr(szMotionFileName, ".frm") )
        {
            char szTemp[1024];
            sprintf(szTemp, "%s_%d.frm", strMotionFileName.c_str(), iBoneIdx);
            strMotionFileName = szTemp;
        }
    }

    if( strMotionFileName.empty() )
        return NULL;

	GCITEMID gcLiveTextureID = g_pItemMgr->GetSameTextureIndex( iItemID_ );
	std::string strTexFileName = GetLiveAbtaName( gcLiveTextureID, iCharType_, iPromotion, ".dds", &iBoneIndex );
	
    if( bUseBoneIDFromArg )
        iBoneIndex = iBoneIdx;

    // 라이브 모델 생성
    GCObject* pRet = CreateLiveModel( strMeshFileName, strTexFileName, strMotionFileName, iBoneIndex, pObj_, D3DXVECTOR3(0.0f,0.0f,0.0f), false, bOnlyTraceParentPos );
	if ( pRet )
	{
		((KGCObjectAndMotion*)pRet)->dwGoodsID = iItemID_;
	}

	return pRet;
}

GCObject* GCDeviceManagerWrapper::CreateLiveModel( std::string modelName, std::string textureName, std::string motionName, int BoneIndex, GCObject* pObj_, D3DXVECTOR3 vOptionalPos /*= D3DXVECTOR3(0.0f,0.0f,0.0f) */, bool bOneTimeMotion /*= false*/, bool bOnlyTraceParentPos /*= false */ )
{

	KGCObjectAndMotion* test = new KGCObjectAndMotion();    	

	test->AddMesh( modelName, textureName );
	test->AddMotion( motionName, bOneTimeMotion );
	test->FrameMove();    
	if ( pObj_)
	{
		pObj_->AddChildObject( test, BoneIndex, bOnlyTraceParentPos );
	}

	return test;
	
}

#if defined( _DEBUG )
void GCDeviceManagerWrapper::Add2DObject( KGC2DObject* pObj )
{
	std::vector<KGC2DObject*>::iterator vit = std::find( m_vec2DObject.begin(), m_vec2DObject.end(), pObj );

	if( vit != m_vec2DObject.end() )
		return;

	m_vec2DObject.push_back( pObj );
}

void GCDeviceManagerWrapper::Remove2DObject( KGC2DObject* pObj )
{
	std::vector<KGC2DObject*>::iterator vit = std::find( m_vec2DObject.begin(), m_vec2DObject.end(), pObj );

	if( vit == m_vec2DObject.end() )
		return;

	m_vec2DObject.erase( vit );
}

KGC2DObject* GCDeviceManagerWrapper::Get2DObject( int iIndex )
{
	if( iIndex < 0 || iIndex >= (int)m_vec2DObject.size() )
		return NULL;

	return m_vec2DObject[iIndex];
}
#endif

bool GCDeviceManagerWrapper::CheckFileExist( const char* strFileName )
{
    return GetMassFileManager()->IsIn( strFileName );
}


const char* GCDeviceManagerWrapper::GetAbtaName( int iItemID_, const char* strNose )
{
	static char strAbtaName[MAX_PATH];
	if( iItemID_ < 1000 )
	{	
		sprintf( strAbtaName, "%s%03d", strNose,iItemID_ );
	}
	else
	{
		sprintf( strAbtaName, "%s%d", strNose,iItemID_ );
	}

	return strAbtaName;
}

const char* GCDeviceManagerWrapper::GetLupusResourceFileName( const char* strNose, int iCharType, int iPromotion, const char* strTail )
{
    if( iCharType == GC_CHAR_LUPUS && iPromotion == 3 )
    {
        static char strLupusResourceFileName[MAX_PATH];
        sprintf( strLupusResourceFileName, "%s_%d_%d_yellow%s", strNose, iCharType, iPromotion, strTail );
        if ( CheckFileExist( strLupusResourceFileName ) == false)
        {
            sprintf( strLupusResourceFileName, "%s_%d_yellow%s", strNose, iCharType, strTail );
            if ( CheckFileExist( strLupusResourceFileName ) == false)
            {
                sprintf( strLupusResourceFileName, "%s_yellow%s", strNose, strTail );
                if ( CheckFileExist( strLupusResourceFileName ) == false)
                {
                    return "";
                }
            }
        }
        return strLupusResourceFileName;
    }

    return "";
}

const char* GCDeviceManagerWrapper::GetResourceFileName( const char* strNose, int iCharType, int iPromotion, const char* strTail )
{
	static char strResourceFileName[MAX_PATH];
	
	sprintf( strResourceFileName, "%s_%d_%d%s", strNose, iCharType, iPromotion, strTail );
	if ( CheckFileExist( strResourceFileName ) == false)
	{
		sprintf( strResourceFileName, "%s_%d%s", strNose, iCharType, strTail );
		if ( CheckFileExist( strResourceFileName ) == false)
		{
			sprintf( strResourceFileName, "%s%s", strNose, strTail );
			if ( CheckFileExist( strResourceFileName ) == false)
			{
				return "";
			}
		}
	}
	return strResourceFileName;
}

const char* GCDeviceManagerWrapper::GetLiveAbtaName( int iItemID_, int iCharType, int iPromotion, const char* strTail, int* piBoneID )
{
	static char strAbtaName[MAX_PATH];
	static std::string strRet;
	memset( strAbtaName,0, MAX_PATH );
	strRet.clear();
	
    // 본ID가 지정되있으면 해당 본ID만 찾아본다.
    if( piBoneID && (-1 != (*piBoneID)) )
    {
        // 제로는 17번에 붙힐 15번짜리 모델을 로드해야한다.
        // BoneID변경없이 15번을 로드 시켜야한다.
        sprintf( strAbtaName, "LiveAbta%d_%d", iItemID_, (*piBoneID) );
        strRet = GetResourceFileName( strAbtaName, iCharType, iPromotion, strTail );
    }
    else
    {
        int iBoneIndex;
        for( iBoneIndex = 0; iBoneIndex < 50; iBoneIndex++ )
        {
            sprintf( strAbtaName, "LiveAbta%d_%d", iItemID_ , iBoneIndex );

            strRet = GetResourceFileName( strAbtaName, iCharType, iPromotion, strTail );

            if ( !strRet.empty() )
            {			
                if ( piBoneID )
                {
                    *piBoneID = iBoneIndex;
                }
                break;
            }
        }
    }

	return strRet.c_str();
}