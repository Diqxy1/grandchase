#include ".\manufacture.h"
#include <algorithm>
#include <dbg/dbg.hpp>
#include <boost/bind.hpp>
#include "Log4.h"
//FILE_NAME_FOR_LOG

ImplementSingleton( KManufacture );
ImplOstmOperatorW2A( KManufacture );
ImplToStringW( KManufacture )
{
    return stm_;
}

KManufacture::KManufacture(void)
{
    m_mapRecipe.clear();
}

KManufacture::~KManufacture(void)
{
}

bool KManufacture::LoadScript()
{
    return _LoadScript(std::string());
}

bool KManufacture::_LoadScript( OUT std::string& strScript_ )
{
    std::map< DWORD, sRecipe > mapRecipe;
    mapRecipe.clear();

    KLuaManager kLuaMng;
    strScript_ = "InitManufacture.lua";
    _JIF( kLuaMng.DoFile( "InitManufacture.lua" ) == S_OK, return false );
    if( kLuaMng.BeginTable( "Manufacture" ) == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng.BeginTable( i ) == S_OK )
            {
                sRecipe recipeInfo;
                _JIF( LoadRecipe( kLuaMng, recipeInfo ), return false );

                _JIF( kLuaMng.EndTable() == S_OK, return false );
                std::pair< std::map<DWORD,sRecipe>::iterator, bool> prRet;
                prRet = mapRecipe.insert( std::make_pair( recipeInfo.m_dwRecipeID, recipeInfo ) );
                if( !prRet.second )
                {
                    START_LOG( cerr, L"중복된 Recipe가 있습니다.. ID : " << recipeInfo.m_dwRecipeID ) << END_LOG;
                    return false;
                }
            }
            else
            {
                START_LOG( cerr, L"테이블 읽기 종료.., Index : " << i ) << END_LOG;
                break;
            }
        }
        _JIF( kLuaMng.EndTable() == S_OK, return false );
    }
    else
    {
        START_LOG( cerr, L"InitManufacture.lua 읽기 실패." ) << END_LOG;
        return false;
    }

    START_LOG( cerr, L" 제작 스크립트 읽기 완료, size : " << mapRecipe.size() ) << END_LOG;
    KLocker lock( m_csRecipe );
    m_mapRecipe.swap( mapRecipe );
    return true;
}


bool KManufacture::LoadRecipe( IN OUT KLuaManager& kLuaMng_, IN sRecipe& recipeInfo_ )
{
    _JIF( 0 == kLuaMng_.GetValue( "RecipeID", recipeInfo_.m_dwRecipeID ) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "ItemID", recipeInfo_.m_dwDestItem ) , return false );
    _JIF( 0 == kLuaMng_.GetValue( "factor", recipeInfo_.m_nFactor ) , return false );

    if( kLuaMng_.BeginTable( "Material" ) == S_OK )
    {
        for( int i = 1 ; ; ++i )
        {
            if( kLuaMng_.BeginTable( i ) == S_OK )
            {
                std::pair< DWORD, int > prMaterial;
                _JIF( 0 == kLuaMng_.GetValue( 1, prMaterial.first ) , return false );
                _JIF( 0 == kLuaMng_.GetValue( 2, prMaterial.second ) , return false );
                _JIF( kLuaMng_.EndTable() == S_OK, return false );

                if( std::find_if( recipeInfo_.m_vecMaterial.begin(), recipeInfo_.m_vecMaterial.end(),
                    boost::bind( &std::pair< DWORD, int >::first, _1 ) == prMaterial.first ) != recipeInfo_.m_vecMaterial.end() )
                {
                    START_LOG( cerr, L"중복된 재료가 있습니다." )
                        << BUILD_LOG( recipeInfo_.m_dwRecipeID )
                        << BUILD_LOG( prMaterial.first ) << END_LOG;
                    return false;
                }
                recipeInfo_.m_vecMaterial.push_back( prMaterial );
            }
            else
                break;
        }
        _JIF( kLuaMng_.EndTable() == S_OK, return false );
    }

    return true;
}

bool KManufacture::GetRecipe( IN DWORD dwRecipeID_, OUT sRecipe& kRecipe_ )
{
    KLocker lock( m_csRecipe );
    std::map< DWORD, sRecipe >::iterator mit;
    mit = m_mapRecipe.find( dwRecipeID_ );
    if( mit == m_mapRecipe.end() )
        return false;

    kRecipe_ = mit->second;
    return true;
}