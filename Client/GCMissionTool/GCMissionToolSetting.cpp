#include "StdAfx.h"
#include ".\gcmissiontoolsetting.h"

ImplementSingleton(KGCMissionToolSetting);

KGCMissionToolSetting::KGCMissionToolSetting(void)
{
}

KGCMissionToolSetting::~KGCMissionToolSetting(void)
{
}

void KGCMissionToolSetting::GetMissionKind( std::vector< std::string >& vecMissionKind )
{
    CIniReader cIniReader( "MissionKind" );
    for( int i = 0 ; ; ++i )
    {
        char strKind[50];
        sprintf( strKind, "Kind%d", i );
        std::string strKindName( cIniReader.ReadString( "MissionKind", strKind, NULL ) );
        if( strKindName.empty() == true )
            break;
        vecMissionKind.push_back( strKindName );
    }
}

void KGCMissionToolSetting::GetCharacters( std::vector< std::string >& vecCharacters )
{

}