#include <iostream>
#include <fstream>
#include <sstream>
#include "BlockedAttackIP.h"
#include <dbg/dbg.hpp >
#include <kncutil.h>
#include "Log4.h"

ImplementSingleton( KBlockedAttackIP );
ImplOstmOperatorW2A( KBlockedAttackIP );
NiImplementRootRTTI( KBlockedAttackIP );

KBlockedAttackIP::KBlockedAttackIP( void )
{
    m_IPBanSet.clear();
}

KBlockedAttackIP::~KBlockedAttackIP( void )
{
}

ImplToStringW( KBlockedAttackIP )
{
    KLocker lock( m_csKBlockedAttackIP );
    return START_TOSTRINGW << TOSTRINGW( m_IPBanSet.size() );
}

bool KBlockedAttackIP::LoadScript()
{
    std::string filename = "AttackersList.txt";
    struct stat buffer;
    if ( stat( filename.c_str(), &buffer ) == 0 )
    {
        try
        {
            std::ifstream inputFile( filename );
            std::string line;
            while ( std::getline( inputFile, line ) )
            {
                unsigned int value;
                std::istringstream iss( line );
                if ( iss >> value )
                    m_IPBanSet.insert( value );
            }
            return true;
        }
        catch ( const std::ifstream::failure& e )
        {
            START_LOG( cerr, L"Failed to open or read from attackers ip list file!" ) << END_LOG;
        }
    }
    return true;
}

void KBlockedAttackIP::InsertBanIP( unsigned int ipaddr )
{
    if( m_IPBanSet.find( ipaddr ) != m_IPBanSet.end() )
        return;
    m_IPBanSet.insert( ipaddr );
    InsertIPToFile( ipaddr );
}

void KBlockedAttackIP::InsertIPToFile( unsigned int ipaddr )
{
    std::string filename = "AttackersList.txt";
    std::string newLineToAdd = std::to_string( ipaddr );
    std::fstream outFile;
    outFile.open( filename, std::fstream::in | std::fstream::out | std::fstream::app );
    if ( !outFile )
    {
        START_LOG( clog, L"Attackers list file doesn't exist. Creating it..." ) << END_LOG;
        outFile.open( filename, std::fstream::in | std::fstream::out | std::fstream::trunc );
        if ( !outFile )
        {
            START_LOG( cerr, L"Failed to insert into attackers list file. IP: " ) << BUILD_LOG( newLineToAdd ) << END_LOG;
            return;
        }
        outFile << newLineToAdd << "\n";
        outFile.close();
        START_LOG(clog, L"Inserted new IP into attackers list. IP: ") << BUILD_LOG(newLineToAdd) << END_LOG;
    }
    else
    {
        outFile << newLineToAdd << "\n";
        outFile.close();
        START_LOG( clog, L"Inserted new IP into attackers list. IP: " ) << BUILD_LOG( newLineToAdd ) << END_LOG;
    }
}