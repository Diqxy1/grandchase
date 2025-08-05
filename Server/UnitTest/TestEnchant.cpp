#include <boost/test/unit_test.hpp>

#include <Windows.h>
#include "GenericLogfile.h"
#include "Lottery.h"
#include <map>

void InitCash2( std::map<int,KLottery>& mapLottery )
{
    mapLottery[0].AddCase( 0,100);
    mapLottery[0].AddCase( 1,0);
    mapLottery[0].AddCase( 2,0);

    mapLottery[1].AddCase( 0,100);
    mapLottery[1].AddCase( 1,0);
    mapLottery[1].AddCase( 2,0);

    mapLottery[2].AddCase( 0,80);
    mapLottery[2].AddCase( 1,0);
    mapLottery[2].AddCase( 2,0);

    mapLottery[3].AddCase( 0,60);
    mapLottery[3].AddCase( 1,0);
    mapLottery[3].AddCase( 2,0);

    mapLottery[4].AddCase( 0,60);
    mapLottery[4].AddCase( 1,20);
    mapLottery[4].AddCase( 2,0);

    mapLottery[5].AddCase( 0,60);
    mapLottery[5].AddCase( 1,20);
    mapLottery[5].AddCase( 2,0);

    mapLottery[6].AddCase( 0,26);
    mapLottery[6].AddCase( 1,0);
    mapLottery[6].AddCase( 2,0);

    mapLottery[7].AddCase( 0,14);
    mapLottery[7].AddCase( 1,30);
    mapLottery[7].AddCase( 2,0);

    mapLottery[8].AddCase( 0,4);
    mapLottery[8].AddCase( 1,30);
    mapLottery[8].AddCase( 2,0);

}

void InitCash( std::map<int,KLottery>& mapLottery )
{
    mapLottery[0].AddCase( 0,100);
    mapLottery[0].AddCase( 1,0);
    mapLottery[0].AddCase( 2,0);

    mapLottery[1].AddCase( 0,50);
    mapLottery[1].AddCase( 1,0);
    mapLottery[1].AddCase( 2,0);

    mapLottery[2].AddCase( 0,40);
    mapLottery[2].AddCase( 1,0);
    mapLottery[2].AddCase( 2,0);

    mapLottery[3].AddCase( 0,30);
    mapLottery[3].AddCase( 1,0);
    mapLottery[3].AddCase( 2,0);

    mapLottery[4].AddCase( 0,30);
    mapLottery[4].AddCase( 1,20);
    mapLottery[4].AddCase( 2,0);

    mapLottery[5].AddCase( 0,30);
    mapLottery[5].AddCase( 1,20);
    mapLottery[5].AddCase( 2,0);

    mapLottery[6].AddCase( 0,13);
    mapLottery[6].AddCase( 1,0);
    mapLottery[6].AddCase( 2,0);

    mapLottery[7].AddCase( 0,7);
    mapLottery[7].AddCase( 1,30);
    mapLottery[7].AddCase( 2,0);

    mapLottery[8].AddCase( 0,2);
    mapLottery[8].AddCase( 1,30);
    mapLottery[8].AddCase( 2,0);

}

void InitGP( std::map<int,KLottery>& mapLottery )
{
    mapLottery[0].AddCase( 0,100);
    mapLottery[0].AddCase( 1,0);
    mapLottery[0].AddCase( 2,0);

    mapLottery[1].AddCase( 0,50);
    mapLottery[1].AddCase( 1,0);
    mapLottery[1].AddCase( 2,0);

    mapLottery[2].AddCase( 0,40);
    mapLottery[2].AddCase( 1,0);
    mapLottery[2].AddCase( 2,0);

    mapLottery[3].AddCase( 0,30);
    mapLottery[3].AddCase( 1,0);
    mapLottery[3].AddCase( 2,0);

    mapLottery[4].AddCase( 0,30);
    mapLottery[4].AddCase( 1,30);
    mapLottery[4].AddCase( 2,0);

    mapLottery[5].AddCase( 0,30);
    mapLottery[5].AddCase( 1,30);
    mapLottery[5].AddCase( 2,0);

    mapLottery[6].AddCase( 0,20);
    mapLottery[6].AddCase( 1,0);
    mapLottery[6].AddCase( 2,15);

    mapLottery[7].AddCase( 0,10);
    mapLottery[7].AddCase( 1,0);
    mapLottery[7].AddCase( 2,15);

    mapLottery[8].AddCase( 0,10);
    mapLottery[8].AddCase( 1,0);
    mapLottery[8].AddCase( 2,20);

}

void TestEnchant()
{

    enum eType
    {
        EA_SUCCESS      = 0, // 성공
        EA_WEAKNESS     = 1, // 하락
        EA_BREAK        = 2, // 깨짐
        EA_NONE         = 3, // 제자리
        EA_MAX,
    };

    KGenericLogfile log;
    log.OpenFile( std::wstring(L"EnChantLog"));

    int naNeedGem[] = {1, 2, 2, 2, 2, 3, 3, 3, 3};
    std::map<int,KLottery> mapLottery;
    //InitCash( mapLottery );
    //InitGP( mapLottery );
    InitCash2( mapLottery );

    for( int i = 0 ; i < 100000 ; ++i )
    {
        DWORD dwUseGemCount = 0;
        int nLv = 0;
        int naTypeCount[EA_MAX];
        ::memset( naTypeCount, 0, sizeof( naTypeCount ) );
        int nLoopCount = 0;
        int nDecision  = 0;
        while( true )
        {
            nLoopCount++;
            dwUseGemCount += naNeedGem[nLv];
            nDecision = mapLottery[nLv].Decision();
            switch( nDecision )
            {
            case EA_SUCCESS:
                naTypeCount[EA_SUCCESS] += 1;
                ++nLv;
                break;
            case EA_WEAKNESS:
                naTypeCount[EA_WEAKNESS] += 1;
                --nLv;
                break;
            case EA_BREAK:
                naTypeCount[EA_BREAK] += 1;
                break;
            case -1:
                naTypeCount[EA_NONE] += 1;
                break;
            }
            if( nLv == 9 || EA_BREAK == nDecision )
                break;
        }
        if( EA_BREAK == nDecision )
            nLv -= 5;

        log << L" Lv : " << nLv 
            << L", UseGem : " << dwUseGemCount
            << L", Count : " << nLoopCount 
            << L", SUCCESS : " << naTypeCount[EA_SUCCESS]
            << L", WEAKNESS : " << naTypeCount[EA_WEAKNESS]
            << L", BREAK : " << naTypeCount[EA_BREAK]
            << L", NONE : " << naTypeCount[EA_NONE]
            << GLOG::endl;
    }

    
    log.CloseFile();
}