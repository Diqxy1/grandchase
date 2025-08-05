#include "stdafx.h"
#include "GCUtil/GenericHTTPClient.h"
#include ".\gcglobalfunction.h"
#include "zlib.h"
#include "MyD3D.h"

#ifndef __PATH__
//#define NO_FILE_TEST
#endif
#include <future>

namespace GCFUNC
{
    bool CompressStream( void* pSrc_, void*& pDest_, int iByteNum_, unsigned long& ulBound_ )
    {
        ulBound_ = (DWORD)((float)(iByteNum_ + 12) * 1.1f);
        pDest_ = new BYTE[ulBound_];

        int Result = compress2( (BYTE*)pDest_, &ulBound_, (BYTE*)pSrc_, iByteNum_, 9 );

        if( Result != Z_OK )
        {
            SAFE_DELETE_ARRAY( pDest_ );
            return false ;
        }
        return true;
    }
    bool LoadLuaScript( KLuaManager& kLuaMgr_, std::string strFileName_, bool bAssert )
    {
        if( g_pGCDeviceManager2 == NULL )
        {
            if( bAssert )
			{
				ASSERT_MBOX( "DeviceManager2 is NULL " );
			}
			return false;
		}

		if( g_pGCDeviceManager2->GetMassFileManager() == NULL )
		{
			if( bAssert ) 
			{
				ASSERT_MBOX( "MassFileManager is NULL " );
			}
			return false;
		}

		

		GCMemoryFile memfile(g_pGCDeviceManager2->GetMassFileManager(), strFileName_ );
		if( !memfile.IsLoaded())
		{
			if( bAssert ) 
			{
				ASSERT_MBOX( "LuaScript file is NULL : "<<strFileName_ );
			}
			return false;
		}

		if( FAILED( kLuaMgr_.DoString( memfile.GetDataPointer(), memfile.GetFileSize() ) ) )
		{
#if !defined(__PATH__)
            MessageBoxA( NULL, kLuaMgr_.GetLastError().c_str(), "LuaFile Load Error", MB_OK );
#endif

			if( bAssert ) 
			{
				ASSERT_MBOX( "LuaScript Load Error : "<<strFileName_ );
			}		
			return false; // file not exist
		}
		
		return true;
	}

	bool LoadLuaScript( KGCLuabinder& kLuaBinder_, std::string strFileName_, bool bAssert )
	{
		if( g_pGCDeviceManager2 == NULL )
		{
			if( bAssert ) 
			{
				ASSERT_MBOX( "DeviceManager2 is NULL " );
			}
			return false;
		}

        if( g_pGCDeviceManager2->GetMassFileManager() == NULL )
        {
            if( bAssert ) 
			{
				ASSERT_MBOX( "MassFileManager is NULL " );
			}
            return false;
        }

        GCMemoryFile MemFile(g_pGCDeviceManager2->GetMassFileManager(), strFileName_ );

        if( !MemFile.IsLoaded())
        {
#ifdef NO_FILE_TEST
            START_LOG( clog2, L"FileIsNULL : " << strFileName_ );
            std::wstringstream stm;
            stm << g_pGCDeviceManager2->GetMassFileManager()->DumpMassFile();
            START_LOG( clog2, L"DumpMassFile" )
                << BUILD_LOG( stm.str() );
#endif
            if( bAssert ) 
			{
				ASSERT_MBOX( "LuaScript file is NULL : "<<strFileName_ );
			}
            return false;
        }

        if( FAILED( kLuaBinder_.DoString( MemFile.GetDataPointer(), MemFile.GetFileSize() ) ) )
        {
#ifdef NO_FILE_TEST
            START_LOG( clog2, L"FileDoStringError : " << strFileName_ );
#endif
#if !defined(__PATH__)
            MessageBoxA( NULL, kLuaBinder_.GetLastError().c_str(), "LuaFile Load Error", MB_OK );
#endif
            if( bAssert ) 
			{
                OKBOXA( kLuaBinder_.GetLastError(), strFileName_.c_str() );
			}         
            return false; // file not exist
        }        
        return true;
    }

    bool CallLuaFunction( KGCLuabinder& kLuaBinder_, std::string strFunc_ )
    {
        if( FAILED( kLuaBinder_.DoString( strFunc_.c_str(), strlen(strFunc_.c_str()) ) ) )
        {
            OKBOXA( kLuaBinder_.GetLastError(), strFunc_.c_str() );
            return false;
        }
        return true;
    }

	std::wstring GetCharName( int iCharType )
	{
		std::wstring strName;
		switch( (int)( iCharType ) )
		{
			// 엘리시스
		case GC_CHAR_ELESIS:
			strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ELESIS );
			break;
			// 리르
		case GC_CHAR_LIRE:
			strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
			break;
			// 아르메
		case GC_CHAR_ARME:
			strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
			break;
			// 라스
		case GC_CHAR_LAS:
			strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
			break;
			// 라이언
		case GC_CHAR_RYAN:
			strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
			break;
			// 로난
		case GC_CHAR_RONAN:
			strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
			break;
		case GC_CHAR_AMY:
			strName = g_pkStrLoader->GetString( STR_ID_AMY);
			break;
		case GC_CHAR_JIN:
			strName = g_pkStrLoader->GetString( STR_ID_JIN );
            break;
        case GC_CHAR_SIEG:
            strName = g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
			break;
		case GC_CHAR_MARI:
			strName = g_pkStrLoader->GetString( STR_ID_MARI_NAME );
			break;
		case GC_CHAR_DIO:
			strName = g_pkStrLoader->GetString( STR_ID_DIO_NAME );
			break;
		case GC_CHAR_ZERO:
			strName = g_pkStrLoader->GetString( STR_ID_ZERO_NAME );
			break;
        case GC_CHAR_LEY:
            strName = g_pkStrLoader->GetString( STR_ID_LEY_NAME );
            break;
        case GC_CHAR_LUPUS:
            strName = g_pkStrLoader->GetString( STR_ID_LUPUS_NAME );
            break;
        case GC_CHAR_RIN:
            strName = g_pkStrLoader->GetString( STR_ID_RIN_NAME );
            break;
        case GC_CHAR_ASIN:
            strName = g_pkStrLoader->GetString( STR_ID_ASIN_NAME );
            break;
        case GC_CHAR_LIME:
            strName = g_pkStrLoader->GetString( STR_ID_LIME_NAME );
            break;
        case GC_CHAR_EDEL:
            strName = g_pkStrLoader->GetString( STR_ID_EDEL_NAME );
            break;
        case GC_CHAR_BEIGAS:
            strName = g_pkStrLoader->GetString( STR_ID_BEIGAS_NAME );
            break;
		case GC_CHAR_UNO:
			strName = g_pkStrLoader->GetString(STR_ID_UNO_NAME);
			break;
#ifdef PERYTON // Kawan>
		case GC_CHAR_PERYTON:
			strName = g_pkStrLoader->GetString( STR_ID_PERYTON_NAME );
			break;
#endif
#ifdef ALICE 
		case GC_CHAR_ALICE:
			strName = g_pkStrLoader->GetString( STR_ID_ALICE_NAME );
			break;
#endif
#ifdef DECANEE 
		case GC_CHAR_DECANEE:
			strName = g_pkStrLoader->GetString(STR_ID_DECANEE_NAME);
			break;
#endif
        case -1: //공용
            strName = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );
            break;
        default:
            strName = L"";
            break;

		}

		return strName;
	}

    std::wstring GetPromotionName( int iCharType, char cPromotion )
    {
        std::wstring strName;
        switch( (int)( iCharType ) )
        {
        case GC_CHAR_ELESIS:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_ELESIS_CLASS_NIGHT );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_ELESIS_CLASS_SPEARMAN );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_ELESIS_CLASS_SWORDMASTER );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_ELESIS_CLASS_SAVIOR );
            break;        
        case GC_CHAR_LIRE:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LIRE );
            break;
        case GC_CHAR_ARME:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_ARME );
            break;
        case GC_CHAR_LAS:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_LAS );
            break;
        case GC_CHAR_RYAN:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RYAN );
            break;
        case GC_CHAR_RONAN:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_CHAR_NAME_RONAN );
            break;
        case GC_CHAR_AMY:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_AMY );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_AMY );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_AMY );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_AMY );
            break;
        case GC_CHAR_JIN:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_JIN );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_JIN );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_JIN );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_JIN );
            break;
        case GC_CHAR_SIEG:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_SIEG_NAME );
            break;
        case GC_CHAR_MARI:
            if( cPromotion == 0 ) 
                strName = g_pkStrLoader->GetString( STR_ID_MARI_NAME );
            else if( cPromotion == 1 )
                strName = g_pkStrLoader->GetString( STR_ID_MARI_NAME );
            else if( cPromotion == 2 )
                strName = g_pkStrLoader->GetString( STR_ID_MARI_NAME );
            else if( cPromotion == 3 )
                strName = g_pkStrLoader->GetString( STR_ID_MARI_NAME );
            break;
        case -1: //공용
            strName = g_pkStrLoader->GetString( STR_ID_ITEM_CHAR_TYPE_COMMON );
            break;

        }

        return strName;
    }

    void ConvertPetInfo( OUT std::map< GCITEMUID, KPetInfo >& mapPetInfo, IN std::map< GCITEMUID, KPetInfo >& mapServerPetInfo )
    {
        // 내가 가진 모든 펫 정보를 저장한다
        mapPetInfo.clear();
        std::map< GCITEMUID, KPetInfo >::iterator mitPet;
        for( mitPet = mapServerPetInfo.begin(); mitPet != mapServerPetInfo.end(); ++mitPet )
        {
            KPetInfo kPet = mitPet->second;

            // 클라이언트에서 사용하는 값으로 바꿔준다
            GCITEMUID dwPetUID =  kPet.m_dwUID;
            GCITEMID dwPetID = kPet.m_dwID / 10;
            GCITEMID dwPetCostumeID = kPet.m_kCostume.m_dwID / 10;
            GCITEMUID dwPetCostumeUID =  kPet.m_kCostume.m_dwUID;

            // 바꾼 값으로 다시 세팅
            kPet.m_dwUID = dwPetUID;
            kPet.m_dwID = dwPetID;
            kPet.m_kCostume.m_dwID = dwPetCostumeID;
            kPet.m_kCostume.m_dwUID = dwPetCostumeUID;

            // 펫 장착 아이템에 관해서도 클라이언트에서 사용하는 값으로 바꿔준다
            for( int i = 0; i < (int)kPet.m_vecEquipItem.size(); i++ )
            {
                GCITEMUID dwPetItemUID = kPet.m_vecEquipItem[i].m_dwUID;
                GCITEMID dwPetItemID = kPet.m_vecEquipItem[i].m_dwID / 10;

                kPet.m_vecEquipItem[i].m_dwUID = dwPetItemUID;
                kPet.m_vecEquipItem[i].m_dwID = dwPetItemID;
            }

            mapPetInfo[ kPet.m_dwUID ] = kPet;
        }
    }

    void ConvertCharInfo( OUT SCharInfo& Info, IN const KCharacterInfo& kServerInfo )
    {
        Info.biExp = kServerInfo.m_biExp;
        Info.iLevel = Exp_2_Level( kServerInfo.m_biExp );
        Info.iCharType = kServerInfo.m_cCharType;
        Info.iPromotionLevel = kServerInfo.m_cPromotion;
        Info.setPromotion = kServerInfo.m_setPromotion;
        Info.dwSlotNum = kServerInfo.m_dwSlotNum;
        

        // 플레이어가 장착하고 있는 펫정보 세팅 ( 현재 캐릭터는 펫 테이블 참조, 다른 캐릭터는 서버값 의존 )
        GCITEMUID dwPetUID =  kServerInfo.m_kEquipPetInfo.m_dwUID;

        if( g_kGlobalValue.GetSavedCharType() == kServerInfo.m_cCharType )
        {
            if( dwPetUID != 0 )
            {
                Info.kPetInfo = g_MyD3D->m_mapPetInfo[ dwPetUID ];
            }
            else
            {
                Info.kPetInfo.Init();
            }
        }
        else
        {
            if( dwPetUID != 0 )
            {
                Info.kPetInfo = kServerInfo.m_kEquipPetInfo;
                Info.kPetInfo.m_dwID = Info.kPetInfo.m_dwID / 10;
                Info.kPetInfo.m_kCostume.m_dwID = Info.kPetInfo.m_kCostume.m_dwID / 10;

                for( int i = 0; i < (int)Info.kPetInfo.m_vecEquipItem.size(); i++ )
                {
                    GCITEMID dwPetItemID = Info.kPetInfo.m_vecEquipItem[i].m_dwID / 10;
                    Info.kPetInfo.m_vecEquipItem[i].m_dwID = dwPetItemID;
                }
            }
            else
            {
                Info.kPetInfo.Init();
            }
        }

		for (auto petGlyphData : kServerInfo.m_vecPetGlyphCharData)
		{
			KSimpleItem glyphItem;
			glyphItem.m_dwUID = petGlyphData.m_dwUID;
			glyphItem.m_dwID = petGlyphData.m_dwID;
			glyphItem.m_cItemType = petGlyphData.m_cType;

			Info.vecPetGlyphInfo.push_back(glyphItem);
		}

		Info.dwPetGlyphSize = Info.vecPetGlyphInfo.size();

        Info.iWin = kServerInfo.m_iWin;
        Info.iLose = kServerInfo.m_iLose;
        Info.m_mapEquipSkill.clear();
        Info.kSkillInfo = kServerInfo.m_kSkillInfo;
		Info.kELOUserData = kServerInfo.m_kELOUserData;
        Info.strCharName = kServerInfo.m_strCharName;

        Info.iGP = kServerInfo.m_nGamePoint;
        Info.kBonusPoint = kServerInfo.m_kBonusPoint;
        Info.m_nInvenCapacity = kServerInfo.m_nInvenCapacity;
        Info.m_nCoordiCapacity = kServerInfo.m_nLookInvenCapacity;
    }

	bool GetValueFromURL( const char* strURL, int& iValue, std::vector< std::pair<std::string, std::string> > vecPostArg )
	{
		GenericHTTPClient* kHttp = new GenericHTTPClient();

		for( int i = 0 ; i < (int)vecPostArg.size() ; ++i )
		{
			kHttp->AddPostArguments( vecPostArg[i].first.c_str(), vecPostArg[i].second.c_str() );
		}

		if( kHttp->Request( strURL, GenericHTTPClient::RequestPostMethod ) )
		{
			std::string str = kHttp->QueryHTTPResponse();

			//첫글자가 - 일 수도 있다. 음수일 수도 있잖소..
			if( !isdigit( str[0] ) && str.find( "-" ) != 0 )
			{
                SAFE_DELETE( kHttp );
				return false;
			}

			// 숫자가 아닌게 있으면 무조건 즐~
			for( int i = 1 ; i < (int)str.size() ; ++i )
			{
				if( !isdigit(str[i] ) )
				{
                    SAFE_DELETE( kHttp );
					return false;
				}
			}

			iValue = atoi( str.c_str() );
		}
		else
		{
            SAFE_DELETE( kHttp );
			return false;
		}

        SAFE_DELETE( kHttp );
		return true;
	}

    GCDeviceFont* GetFontManager( int iSize, bool bOutLine )
    {
        GCDeviceFont* pkReturn;

        if(SiKGCMultipleLanguages()->GetDefualtFontSize() == iSize) 
            pkReturn = bOutLine ? g_pkFontMgrOutline : g_pkFontMgr;
        else if(SiKGCMultipleLanguages()->GetMiddleFontSize() == iSize) 
            pkReturn = bOutLine ? g_pkFontMgrOutline14 : g_pkFontMgr14;
        else if(SiKGCMultipleLanguages()->GetLargeFontSize() == iSize) 
            pkReturn = bOutLine ? g_pkFontMgrOutline16 : g_pkFontMgr16;
        else if(SiKGCMultipleLanguages()->GetTitleFontSize() == iSize) 
            pkReturn = bOutLine ? g_pkFontMgrOutlineTitle : g_pkFontMgrTitle; 
        else 
            pkReturn = bOutLine ? g_pkFontMgrOutline : g_pkFontMgr;

        return pkReturn;
    }

	CParticleEventSeqPTR CreateSequenceToUI( KD3DWnd* pWnd, const std::string& name, float xoffset, float yoffset, int iLayer /*= GC_LAYER_UI*/ )
	{
		D3DXVECTOR2 vPos = pWnd->GetFixedWindowPos();
		CParticleEventSeqPTR pParticle = NULL;
		pParticle = g_ParticleManager->CreateSequence( name, 
			( vPos.x + xoffset ) / (400.0f*GC_SCREEN_DIV_WIDTH) - 1.0f, 
			0.75f - (( vPos.y + yoffset ) / (300.0f * GC_SCREEN_DIV_WIDTH)) * 0.75f, 0.5f );
		g_ParticleManager->SetLayer( pParticle, iLayer );
		return pParticle;
	}

	DWORD GetRealFileSize( std::string strPath_ )
	{
        HANDLE  hFile       = INVALID_HANDLE_VALUE;

		if ( INVALID_HANDLE_VALUE == ( hFile = ::CreateFileA( strPath_.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_READONLY,
			NULL ) ) )
		{
			return -1; // failed
		}
		
		DWORD dwFileSize = ::GetFileSize( hFile, NULL );
		//::CloseHandle( hFile );
		::CloseHandle( hFile );
        //return dwFileSize;
        return dwFileSize;

		//FILE* file = NULL;
		//DWORD dwFileSize = -1;
		//int result = 0;
		//file = fopen(strPath_.c_str(), "rt" );
		//if( file == NULL )
		//{
		//	//에러님 파일 못열 경우.			
		//}
		//else
		//{
		//	result = fseek(file,0,SEEK_END);
		//	dwFileSize = ftell(file);
		//	fclose(file);
		//}
		//return dwFileSize;
	}

    D3DXVECTOR2 UICoordinateConvertToParticleCoordinate( D3DXVECTOR2 vUICoordinate_ )
    {
        return D3DXVECTOR2( (vUICoordinate_.x / (400.0f*GC_SCREEN_DIV_WIDTH)) - 1.0f, 0.75f - ( vUICoordinate_.y / ((300.0f * GC_SCREEN_DIV_WIDTH)*0.75f) ) );
    }

	bool HadChar( const int& iCharType )
	{
		KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;
		
		KCharInfoVector::iterator vit = vecChar.begin();
		for (; vit != vecChar.end() ; ++vit)
		{
			if (vit->iCharType == iCharType)
				return true;
		}

		return false;

	}

	int GetStatusGradeByTotalAtk( IN const int iTotalAtk_ )
	{
		if( SiKGCChannelManager()->IsPVPBallanceServer() )
		{
			return 8;
		}

		if ( SiKGCRoomManager()->IsAngelEggModeAbility() ||
			SiKGCRoomManager()->IsDotaModeAbility() )
		{
			return 8;
		}

		int intGrade;


        if( iTotalAtk_ >= 70000 )     intGrade = 0;
		else if ( iTotalAtk_ >= 52600)      intGrade = 1;
		else if ( iTotalAtk_ >= 46700)      intGrade = 2;
		else if ( iTotalAtk_ >= 23700 )     intGrade = 3;
		else if ( iTotalAtk_ >= 20700 )     intGrade = 4;
		else if ( iTotalAtk_ >= 17800 )     intGrade = 5;
		else if ( iTotalAtk_ >= 16300 )     intGrade = 6;
		else if ( iTotalAtk_ >= 14800 )     intGrade = 7;
		else if ( iTotalAtk_ >= 13300 )     intGrade = 8;
		else if ( iTotalAtk_ >= 11900 )     intGrade = 9;
		else if ( iTotalAtk_ >= 10400 )     intGrade = 10;
		else if ( iTotalAtk_ >= 8900  )     intGrade = 11;
		else if ( iTotalAtk_ >= 7400 )      intGrade = 12;
		else if ( iTotalAtk_ >= 5900 )      intGrade = 13;
		else if ( iTotalAtk_ >= 4400 )      intGrade = 14;
		else if ( iTotalAtk_ >= 3000 )      intGrade = 15;
		else if ( iTotalAtk_ >= 1500 )      intGrade = 16;
		else                                intGrade = 17; 
							   
													   
		if ( SiKGCChannelManager()->IsPVPBallanceServer() )
		{
			intGrade= 7;
		}

        return intGrade;
	}


	std::wstring GetStatusGrade( IN int iGrade )
	{
		switch (iGrade) {
			case 0 : return  L"SS";
			case 1 : return	 L"S+";
			case 2 : return	 L"S";
			case 3 : return	 L"S-";
			case 4 : return	 L"A+";
			case 5 : return	 L"A";
			case 6 : return	 L"A-";
			case 7 : return	 L"B+";
			case 8 : return	 L"B";
			case 9 : return	 L"B-";
			case 10 : return L"C+";
			case 11 : return L"C";
			case 12 : return L"C-";
			case 13 : return L"D+";
			case 14: return	 L"D";
			case 15 : return L"D-";
			case 16 : return L"E";
			case 17 : return L"F"; 
		}

		return L"F";

	}

	void CharGainParticle( const int iCharType_ )
	{
		std::map<int , std::vector<std::string>>::iterator mit = g_kGlobalValue.m_mapCharGainParticleInfo.find(iCharType_);

		if (mit == g_kGlobalValue.m_mapCharGainParticleInfo.end())
			return;

		if (mit->second.empty())
			return;

		std::vector<std::string>::iterator vit = mit->second.begin();
		for (; vit != mit->second.end() ; ++vit)
		{
			g_ParticleManager->SetLayer( g_ParticleManager->CreateSequence(*vit,0.0f,0.0f,0.5f), vit == mit->second.begin() ? GC_LAYER_CUSTOM : GC_LAYER_UI);
		}

        g_kGlobalValue.RunLogProgram("13");

	}

	std::map<int,DWORD> GetCharListWithoutHave()
	{
		std::map<int,DWORD> mapOpenCharInfo = g_kGlobalValue.GetCharOpenType();
		KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;

		KCharInfoVector::iterator vIter = vecChar.begin();
		for(;vIter != vecChar.end(); ++vIter)
		{
			mapOpenCharInfo.erase(static_cast<int>(vIter->iCharType));
		}

		return mapOpenCharInfo;
	}


	bool CheckInEllips( int iStandardX_ , int iStandardY_ , int iRangeX_ , int iRangeY_ , POINT ptPoint_ )
	{
		return (std::pow( (ptPoint_.x - iStandardX_)*iRangeY_ , 2 ) + std::pow( (ptPoint_.y - iStandardY_)*iRangeX_ , 2 )) < std::pow(iRangeX_*iRangeY_,2);
	}

	void DrawDotEllips( DWORD* dwTexAccess_, D3DSURFACE_DESC TexDesc_ , int iStartCol_ ,int iEndCol_ , int iStartRow_ ,int iEndRow_, POINT ptCenter_ ,D3DCOLOR col_, float fPercent_ /*= 1.0f */ )
	{
		for (int i=iStartCol_ ; i< iEndCol_ ; i++)
		{
			if ( i < 0 || i >= static_cast<int>(TexDesc_.Width) )
				continue;
			for (int j=iStartRow_ ; j<iEndRow_ ; j++ )
			{
				if ( j < 0 || j >= static_cast<int>(TexDesc_.Height) )
					continue;

				if ( randf() > fPercent_)
					continue;

				POINT ptPoint;
				ptPoint.x = i;
				ptPoint.y = j;
				int iIndex = i + j*(TexDesc_.Width);
				if (iIndex < 0 || iIndex >= static_cast<int>(TexDesc_.Width * TexDesc_.Height))
					continue;
				if ( CheckInEllips(ptCenter_.x,ptCenter_.y,abs(iEndCol_ - ptCenter_.x),abs(iEndRow_ - ptCenter_.y) ,ptPoint ) )
				{
					dwTexAccess_[iIndex] = col_;
				}
			}
		}

	}

	bool CheckColInRect( DWORD* dwTexAccess_, D3DSURFACE_DESC TexDesc_ , int iStartCol_ ,int iEndCol_ , int iStartRow_ ,int iEndRow_,D3DCOLOR col_, float fPercent_ /*= 1.0f*/ )
	{
		DWORD dwTotalDot = (iEndCol_ - iStartCol_) * (iEndRow_ - iStartRow_);
		DWORD dwCnt = 0;

		for (int i=iStartCol_ ; i< iEndCol_ ; i++)
		{
			if ( i < 0 || i > static_cast<int>(TexDesc_.Width) )
				continue;
			for (int j=iStartRow_ ; j<iEndRow_ ; j++ )
			{
				if (dwTexAccess_[i + j*(TexDesc_.Width)] == col_)
					dwCnt++;
			}
		}

		if ( (dwTotalDot * fPercent_) <= dwCnt )
			return true;
		else
			return false;
	}

	void FillTheCol( DWORD* dwTexAccess_, D3DSURFACE_DESC TexDesc_ , D3DCOLOR col_ )
	{
		for (int i=0 ; i< static_cast<int>(TexDesc_.Width) ; i++)
		{
			for (int j=0 ; j<static_cast<int>(TexDesc_.Height) ; j++ )
			{
				dwTexAccess_[i + j*(TexDesc_.Width)] = col_;
			}
		}
	}

	bool Is4SpSkill( int iCharType_ , EGCSkillTree eSkillID_ )
	{
		switch(iCharType_)
		{
		case GC_CHAR_ELESIS:
			switch(eSkillID_)
			{
			case SID_ELESIS1_HIDDEN_MP4:					
			case SID_ELESIS1_SPECIAL_4:						
			case SID_ELESIS2_HIDDEN_MP4:					
			case SID_ELESIS2_SPECIAL_4:						
			case SID_ELESIS3_HIDDEN_MP4:					
			case SID_ELESIS3_SPECIAL_4:						
			case SID_ELESIS4_HIDDEN_MP4:					
			case SID_ELESIS4_SPECIAL_4:						
				return true;
			default:
				return false;
			} 
		case GC_CHAR_LIRE:
			switch(eSkillID_)
			{
			case SID_LIRE1_HIDDEN_MP4:					
			case SID_LIRE1_SPECIAL_4:						
			case SID_LIRE2_HIDDEN_MP4:					
			case SID_LIRE2_SPECIAL_4:						
			case SID_LIRE3_HIDDEN_MP4:					
			case SID_LIRE3_SPECIAL_4:						
			case SID_LIRE4_HIDDEN_MP4:					
			case SID_LIRE4_SPECIAL_4:						
				return true;
			default:
				return false;
			} 
		case GC_CHAR_ARME:
			switch(eSkillID_)
			{
			case SID_ARME1_HIDDEN_MP4:					
			case SID_ARME1_SPECIAL_4:						
			case SID_ARME2_HIDDEN_MP4:					
			case SID_ARME2_SPECIAL_4:						
			case SID_ARME3_HIDDEN_MP4:					
			case SID_ARME3_SPECIAL_4:						
			case SID_ARME4_HIDDEN_MP4:					
			case SID_ARME4_SPECIAL_4:						
				return true;
			default:
				return false;
			} 
		case GC_CHAR_LAS:
			switch(eSkillID_)
			{
			case SID_LAS1_HIDDEN_MP4:					
			case SID_LAS1_SPECIAL_4:						
			case SID_LAS2_HIDDEN_MP4:					
			case SID_LAS2_SPECIAL_4:						
			case SID_LAS3_HIDDEN_MP4:					
			case SID_LAS3_SPECIAL_4:						
			case SID_LAS4_HIDDEN_MP4:					
			case SID_LAS4_SPECIAL_4:						
				return true;
			default:
				return false;
			} 
		case GC_CHAR_RYAN:
			switch(eSkillID_)
			{
			case SID_LYAN1_HIDDEN_MP4:
			case SID_LYAN1_SPECIAL_4:
			case SID_LYAN2_HIDDEN_MP4:
			case SID_LYAN2_SPECIAL_4:
			case SID_LYAN3_HIDDEN_MP4:
			case SID_LYAN3_SPECIAL_4:
			case SID_LYAN4_HIDDEN_MP4:
			case SID_LYAN4_SPECIAL_4:
				return true;
			default:
				return false;
			} 
		case GC_CHAR_RONAN:
			switch(eSkillID_)
			{
			case SID_RONAN1_HIDDEN_MP4:					
			case SID_RONAN1_SPECIAL_4:						
			case SID_RONAN2_HIDDEN_MP4:					
			case SID_RONAN2_SPECIAL_4:						
			case SID_RONAN3_HIDDEN_MP4:					
			case SID_RONAN3_SPECIAL_4:						
			case SID_RONAN4_HIDDEN_MP4:					
			case SID_RONAN4_SPECIAL_4:						
				return true;
			default:
				return false;
			}
		case GC_CHAR_AMY:
			switch(eSkillID_)
			{
			case SID_AMY1_HIDDEN_MP4:					
			case SID_AMY1_SPECIAL_4:						
			case SID_AMY2_HIDDEN_MP4:					
			case SID_AMY2_SPECIAL_4:						
			case SID_AMY3_HIDDEN_MP4:					
			case SID_AMY3_SPECIAL_4:						
			case SID_AMY4_HIDDEN_MP4:					
			case SID_AMY4_SPECIAL_4:						
				return true;
			default:
				return false;
			} 
		case GC_CHAR_JIN:
			switch(eSkillID_)
			{
			case SID_JIN1_HIDDEN_MP4:					
			case SID_JIN1_SPECIAL_4:						
			case SID_JIN2_HIDDEN_MP4:					
			case SID_JIN2_SPECIAL_4:						
			case SID_JIN3_HIDDEN_MP4:					
			case SID_JIN3_SPECIAL_4:						
			case SID_JIN4_HIDDEN_MP4:					
			case SID_JIN4_SPECIAL_4:						
				return true;
			default:
				return false;
			} 
		case GC_CHAR_SIEG:
			switch(eSkillID_)
			{
			case SID_SIEG1_HIDDEN_MP4:					
			case SID_SIEG1_SPECIAL_4:						
			case SID_SIEG2_HIDDEN_MP4:					
			case SID_SIEG2_SPECIAL_4:						
			case SID_SIEG3_HIDDEN_MP4:					
			case SID_SIEG3_SPECIAL_4:						
			case SID_SIEG4_HIDDEN_MP4:					
			case SID_SIEG4_SPECIAL_4:						
				return true;
			default:
				return false;
			}
		case GC_CHAR_MARI:
			switch(eSkillID_)
			{
			case SID_MARI1_HIDDEN_MP4:
			case SID_MARI1_SPECIAL_4:
			case SID_MARI2_HIDDEN_MP4:
			case SID_MARI2_SPECIAL_4:
			case SID_MARI3_HIDDEN_MP4:
			case SID_MARI3_SPECIAL_4:
			case SID_MARI4_HIDDEN_MP4:
			case SID_MARI4_SPECIAL_4:
				return true;
			default:
				return false;
			}
		case GC_CHAR_DIO:
			switch(eSkillID_)
			{
			case SID_DIO_HIDDEN_MP4:					
			case SID_DIO_SPECIAL_4:												
				return true;
			default:
				return false;
			} 
		case GC_CHAR_ZERO:
			switch(eSkillID_)
			{
			case SID_ZERO_HIDDEN_MP4:					
			case SID_ZERO_SPECIAL_4:											
				return true;
			default:
				return false;
			} 
		case GC_CHAR_LEY:
			switch(eSkillID_)
			{
			case SID_LEY_HIDDEN_MP4:					
			case SID_LEY_SPECIAL_4:										
				return true;
			default:
				return false;
			} 
		case GC_CHAR_LUPUS:
			switch(eSkillID_)
			{
			case SID_LUPUS_HIDDEN_MP4:					
			case SID_LUPUS_SPECIAL_4:										
				return true;
			default:
				return false;
			} 
		case GC_CHAR_RIN:
			switch(eSkillID_)
			{
			case SID_RIN_HIDDEN_MP4:					
			case SID_RIN_SPECIAL_4:											
				return true;
			default:
				return false;
			}
        case GC_CHAR_EDEL:
            switch(eSkillID_)
            {
            case SID_EDEL1_HIDDEN_MP4:
            case SID_EDEL1_HIDDEN_SPECIAL4:
                return true;
            default:
                return false;
            }
		case GC_CHAR_BEIGAS:
			switch (eSkillID_)
			{
			case SID_BEIGAS_HIDDEN_MP4:
			case SID_BEIGAS_SPECIAL_4:
				return true;
			default:
				return false;
			}
		case GC_CHAR_UNO:
			switch (eSkillID_)
			{
			case SID_UNO1_HIDDEN_MP4:
			case SID_UNO1_MP_SPECIAL4:
			case SID_UNO1_HP_SPECIAL4:
				return true;
			default:
				return false;
			}
#ifdef PERYTON // Kawan>
		case GC_CHAR_PERYTON:
			switch (eSkillID_)
			{
			case SID_PERYTON1_HIDDEN_MP4:
			case SID_PERYTON1_SPECIAL4:
				return true;
			default:
				return false;
			}
#endif
#ifdef ALICE
		case GC_CHAR_ALICE:
			switch (eSkillID_)
			{
			case SID_ALICE1_HIDDEN_MP4:
			case SID_ALICE1_SPECIAL_04:
				return true;
			default:
				return false;
			}
#endif
#ifdef DECANEE
		case GC_CHAR_DECANEE:
			switch (eSkillID_)
			{
			case SID_DECANEE1_HIDDEN_MP4:
			case SID_DECANEE1_SPECIAL_04:
				return true;
			default:
				return false;
			}
#endif
		case GC_CHAR_ASIN:
			return false;
		case GC_CHAR_LIME:
			return false;
		default:
			return false;
		}

	}


    void CreateCharViewMotion( int iExtraCharNum, GCDeviceMotionFRM** pWaitMotion, GCDeviceMotionFRM** pActionMotion )
    {
        int nCharNum = iExtraCharNum % GC_CHAR_NUM;
        int nJobLevel = iExtraCharNum / GC_CHAR_NUM;

        char strTemp[MAX_PATH];
        std::string strShopMotion;
        sprintf( strTemp, "shop_%d_%d", nCharNum, nJobLevel );
        strShopMotion = strTemp;

        (*pWaitMotion) = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_0.frm" );
        if( (*pWaitMotion)->IsDisable() )
        {
            //assert(0);
            SAFE_RELEASE((*pWaitMotion));
        }

        (*pActionMotion) = g_pGCDeviceManager2->CreateMotionFRM( strShopMotion + "_1.frm" );
        if( (*pActionMotion)->IsDisable() )
        {
            //assert(0);
            SAFE_RELEASE((*pActionMotion));
        }
    }

    void GetRankGradeText(IN int iRankGrade, OUT std::wstring& wstrRankGrade )
    {
        switch(iRankGrade)
        {
        case 0:
            wstrRankGrade = L"F";
            break;
        case 1:
            wstrRankGrade = L"E";
            break;
        case 2:
            wstrRankGrade = L"D";
            break;
        case 3:
            wstrRankGrade = L"C";
            break;
        case 4:
            wstrRankGrade = L"B";
            break;
        case 5:
            wstrRankGrade = L"A";
            break;
        case 6:
            wstrRankGrade = L"S";
            break;
        case 7:
            wstrRankGrade = L"SS";
            break;
        default:
            wstrRankGrade = L"F";
            break;
        }
    }

	void CreateCharViewMotionForMenu(int iExtraCharNum, GCDeviceMotionFRM** pWaitMotion, GCDeviceMotionFRM** pActionMotion)
	{
		const char* Male_Motions[8] = { "Menu_UI_Oh01_M.frm", "Menu_UI_Oh02_M.frm" ,"Menu_UI_Hi01_M.frm" ,"Menu_UI_Hi02_M.frm" ,"Menu_UI_Haha_M.frm" ,"Menu_UI_Dance_M.frm" ,"Menu_UI_Cly_M.frm", "Menu_UI_Angry_M.frm" };
		const char* Female_Motions[8] = { "Menu_UI_Oh02_W.frm","Menu_UI_Hi01_W.frm","Menu_UI_Hi02_W.frm","Menu_UI_Haha_W.frm","Menu_UI_Dance_W.frm","Menu_UI_Cly_W.frm","Menu_UI_Angry_W.frm", "Menu_UI_Oh_W.frm" };

		int iCharSex = g_kGlobalValue.IsSexCharacter(g_MyD3D->m_TempPlayer.GetCurrentChar().iCharType);

		const char* GetCorretAnimation = 0;

		if (iCharSex == SEX_GRIL)
		{
			GetCorretAnimation = Female_Motions[rand() % 8];
			(*pWaitMotion) = g_pGCDeviceManager2->CreateMotionFRM(GetCorretAnimation);
		}
		else
		{
			GetCorretAnimation = Male_Motions[rand() % 8];
			(*pWaitMotion) = g_pGCDeviceManager2->CreateMotionFRM(GetCorretAnimation);
		}

		if ((*pWaitMotion)->IsDisable())
		{
			//assert(0);
			SAFE_RELEASE((*pWaitMotion));
		}
	}

	std::string Base64Encode(std::string in) {
		std::string out;
		int val = 0, valb = -6;
		for (u_char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		return out;
	}

	std::string Base64Decode(std::string in) {
		while (in.size() % 4) {
			in.push_back('=');
		}
		std::string out;
		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) {
			T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
		}
		int val = 0, valb = -8;
		for (u_char c : in) {
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}
		return out;
	}

	bool __forceinline IsHookAPI( BYTE* pbFunction )
	{
		if (pbFunction[0] == 0xE9)
			return true;
		if (pbFunction[0] == 0xFF && pbFunction[1] == 0x25)
			return true;
		return false;
	}
}


