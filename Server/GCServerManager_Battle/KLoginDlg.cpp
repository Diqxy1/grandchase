// KLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gcservermanager.h"
#include "KLoginDlg.h"
#include "MD5Checksum.h"
#include <string>
#include <KncUtil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KLoginDlg dialog


KLoginDlg::KLoginDlg(CWnd* pParent /*=NULL*/)
    : CDialog(KLoginDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(KLoginDlg)
    m_cstrLoginID = _T("");
    m_cstrLoginPW = _T("");

    m_vecKoreaLoginIDList.clear();
    //}}AFX_DATA_INIT
    //m_cstrLoginID = _T("khj0658");
    //m_cstrLoginPW = _T("ehdgus983704");
}


void KLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(KLoginDlg)
    DDX_Text(pDX, IDC_EDIT_LOGIN_ID, m_cstrLoginID);
    DDX_Text(pDX, IDC_EDIT_LOGIN_PW, m_cstrLoginPW);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KLoginDlg, CDialog)
    //{{AFX_MSG_MAP(KLoginDlg)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

namespace {

    struct KLoginInfo {
        std::string     strID;
        std::string     strPW;
        std::string     strCode;
        int             m_nLevel;

        KLoginInfo() {}
        KLoginInfo(const KLoginInfo& t) { *this = t; }
        KLoginInfo& operator=(const KLoginInfo& t)
        {
            strID       = t.strID;
            strPW       = t.strPW;
            strCode     = t.strCode;
            m_nLevel    = t.m_nLevel;

            return *this;
        }

        typedef std::map< std::string, KLoginInfo > MapType;
    };

    void Put( KLoginInfo::MapType& mapTarget_, LPCSTR szID_, LPCSTR szPW_, LPCSTR szCode_, int nLevel_ = -1 )
    {
        KLoginInfo kInfo;
        kInfo.strID     = szID_;
        kInfo.strPW     = szPW_;
        kInfo.strCode   = szCode_;
        kInfo.m_nLevel  = nLevel_;

        mapTarget_.insert( std::make_pair( kInfo.strID, kInfo ) );
    }
}

/////////////////////////////////////////////////////////////////////////////
// KLoginDlg message handlers
#include <iostream>
#include ".\klogindlg.h"

void KLoginDlg::OnOK( void )
{
    //CStringA cstrLoginPW;
    //cstrLoginPW = "ppgm_01"; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = "ppgm_02"; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = "ppgm_03"; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = "ppgm_04"; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = "ppgm_05"; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = "ppgm_06"; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = ""; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;
    //cstrLoginPW = ""; std::cout << CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)cstrLoginPW), cstrLoginPW.GetLength() ) << std::endl;



    KLoginInfo::MapType mapInfo;

	Put(mapInfo, "raymonf", "3fde6bb0541387e4ebdadf7c2ff31123", "3fde6bb0541387e4ebdadf7c2ff31123");

    // 국내
    Put( mapInfo, "koggc1",         "b56810abff50660c4eefcab8ce98e6aa", "NM0061796996" );   // wnsgh01
    Put( mapInfo, "koggc2",         "0682b307766ad1f47b2c8ae3991fbfba", "NM0061797166" );   // wnsgh02
    Put( mapInfo, "koggc3",         "e87df4ea97f0b08f08f8eed955957ded", "NM0061797299" );   // wnsgh03
    Put( mapInfo, "koggc4",         "fdd2be2806dbdf9bbb75758a4e92999d", "NM0061797452" );   // wnsgh04
    Put( mapInfo, "koggc5",         "dfe566dd9e0d7fae9f56e97c2977dcde", "NM0061796994" );   // wnsgh05
    Put( mapInfo, "koggc6",         "14b8bff87e56772a2a089648490fd12d", "NM0061797769" );   // wnsgh06
    Put( mapInfo, "koggc7",         "c4adb1fc0a6a59215a036b3028309333", "NM0061797113" );   // dnsdud07
    Put( mapInfo, "koggc8",         "9356170ce08446e60f96a3ab7fff20a3", "NM0061797872" );   // dnsdud08

    CString name;
    // 국내 운영자 리스트를 만들고 이것을 이용해 툴을 한글로 출력할 것인지 판별
    for ( int i=0; i<8; i++ ) {
        name.Format(_T("koggc%d"), i+1);
        m_vecKoreaLoginIDList.push_back(name);
    }

    // 대만
    Put( mapInfo, "gccbe00960",     "0056896c0527c11483f0658e5254dc59", "NM1381325399" );   // cchigjcf
    Put( mapInfo, "gccbe00888",     "5026e33469baa069355757c4d9c06798", "NM1515936599" );   // cidedcdg
    Put( mapInfo, "gccbe00815",     "db34a292895b4a0ac886ae70fc42fd5f", "NM1465015127" );   // gfccijeb
    Put( mapInfo, "gccbe00100",     "b56810abff50660c4eefcab8ce98e6aa", "NM1465015127" );   // wnsgh01
    Put( mapInfo, "gccbe00200",     "0682b307766ad1f47b2c8ae3991fbfba", "NM1465015127" );   // wnsgh02
    Put( mapInfo, "gccbe00300",     "e87df4ea97f0b08f08f8eed955957ded", "NM1465015127" );   // wnsgh03

    // 브라질
    Put( mapInfo, "skoal",          "b2c69ce84e22b896b75e12cf3a8c2c65", "8231FC01B4248FAED3FFC7FBC4B39D7569A4F1EF" );   // m555d
    //Put( mapInfo, "gmtoki",         "feb853d709839d3d8c4397cd56938bf9", "feb853d709839d3d8c4397cd56938bf9" );   // jaberwocky
    //Put( mapInfo, "gmnaza",         "0318c3213d99581f74ee1c43fba9aa8a", "0318c3213d99581f74ee1c43fba9aa8a" );   // f3ixfna11
    //Put( mapInfo, "gmrodrigo",      "89a20470adc73e385367e65fdb3bfe28", "89a20470adc73e385367e65fdb3bfe28" );   // omgwtfory
    Put( mapInfo, "gcmanager",      "fc46c3bb15779877b8855548b3b59b14", "96D6A160E4CA9D2BD3C442B6240F2CFB5CE949D4" );   // moicano123
    //Put( mapInfo, "gmbasques",      "e99a18c428cb38d5f260853678922e03", "e99a18c428cb38d5f260853678922e03" );   // abc123
    //Put( mapInfo, "fabiogm",        "64277133d98cdb08fa5f017cfe1b7703", "64277133d98cdb08fa5f017cfe1b7703" );   // dfspkdf
    //Put( mapInfo, "gmalfredo",      "36f17c3939ac3e7b2fc9396fa8e953ea", "36f17c3939ac3e7b2fc9396fa8e953ea" );   // qweasd
    //Put( mapInfo, "luggm1",         "73cd1e1d034c21b9c195fbe4b10494d5", "73cd1e1d034c21b9c195fbe4b10494d5" );   // luggm1/luggm1^C

    //Put( mapInfo, "gmraraujo",      "1c2e51cd1f115b5500b5e4651431d60a", "5cb765d06b82a39f295890b0f999715c" );   // gmraraujo/magc01
    //Put( mapInfo, "gmasanchez",     "6707aa795d20d8d0137381abb23a050d", "bf5cf0b5a74363104495ec17558b56d1" );   // gmasanchez/And2850
    //Put( mapInfo, "gmsdias",        "87a88d2b3cd5c7443cad75aac375d4ad", "18c8c8694cc353a8d2354ce3f9644a8a" );   // gmsdias/183054


    // 아래는 싱가폴인가???
    //Put( mapInfo, "gaadmin001",       "d201282ce62015a1aa81317581004f0a", "d201282ce62015a1aa81317581004f0a" );  // GAadmin001 anything001
    Put( mapInfo, "gaadmin001",     "d201282ce62015a1aa81317581004f0a", "775B37F50CE68B386B048DFE16542890" );  // GAadmin001 anything001
    Put( mapInfo, "gaadmin002",     "6b9f9ffa04a78e9bad0cf6b07ef3ca89", "4E9091577C2C0484F22300ABE6C04E34" );  // GAadmin002 anything002
    Put( mapInfo, "gaadmin003",     "c3b3f239df97a93d09bd0d486a34cc5d", "C3B3F239DF97A93D09BD0D486A34CC5D", 1 );  // GAadmin003 anything003
    Put( mapInfo, "gaadmin004",     "eb365fd4e8bb2ec6b770dafe75f3377f", "EB365FD4E8BB2EC6B770DAFE75F3377F", 1 );  // GAadmin004 anything004
    Put( mapInfo, "gaadmin005",     "a53b739ee5e263df55500ef53872f93f", "A53B739EE5E263DF55500EF53872F93F", 1 );  // GAadmin005 anything005
    Put( mapInfo, "gaadmin006",     "ba59da9bce781d1b3f4178416b1389b6", "BA59DA9BCE781D1B3F4178416B1389B6", 1 );  // GAadmin006 anything006
    Put( mapInfo, "gaadmin007",     "ffef39a7114c5ce6ef9a8e5364c3ae5a", "FFEF39A7114C5CE6EF9A8E5364C3AE5A", 1 );  // GAadmin007 anything007
    Put( mapInfo, "gaadmin008",     "434611c08d6760e950c511da804c7a8c", "434611C08D6760E950C511DA804C7A8C", 1 );  // GAadmin008 anything008
    Put( mapInfo, "gaadmin009",     "3312c4259839eeee523aa4671ff931a8", "3312C4259839EEEE523AA4671FF931A8", 1 );  // GAadmin009 anything009

    Put( mapInfo, "woosh",          "1dc727aebdf3d0db3885e03eaa2b92d6", "" );  // for internal test
    Put( mapInfo, "tslayer",        "f508ee8828f2b54230a027a5791b4236", "" );  // for internal test
    Put( mapInfo, "kogadmin",       "ffbd1d607132bafa721a643b2bb8b0e8",  "" ); // for us alpha test, pw = ntreev

    // NTreev Main
    Put( mapInfo, "ntreev011",      "18740b835c8747a2c648b0f02d014d15", "4eb9cf477365011d99d2b4704a7371bc" );  // kogadmin01/ntreev11
    Put( mapInfo, "ntreev012",      "18740b835c8747a2c648b0f02d014d15", "5118394f28be1c11f7322ab20540cf0a" );  // kogadmin02/ntreev11
    Put( mapInfo, "ntreev013",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin03/ntreev11
    Put( mapInfo, "ntreev014",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin04/ntreev11
    Put( mapInfo, "ntreev015",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin05/ntreev11
    Put( mapInfo, "ntreev016",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin06/ntreev11
    Put( mapInfo, "ntreev017",      "18740b835c8747a2c648b0f02d014d15", "18740b835c8747a2c648b0f02d014d15" );  // kogadmin07/ntreev11
    Put( mapInfo, "ntreev018",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin08/ntreev11
    Put( mapInfo, "ntreev019",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin09/ntreev11
    Put( mapInfo, "ntreev020",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin10/ntreev11
    Put( mapInfo, "ntreev021",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin11/ntreev11
    Put( mapInfo, "ntreev022",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin12/ntreev11
    Put( mapInfo, "ntreev023",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin13/ntreev11
    Put( mapInfo, "ntreev024",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin14/ntreev11
    Put( mapInfo, "ntreev025",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin15/ntreev11
    Put( mapInfo, "ntreev026",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin16/ntreev11
    Put( mapInfo, "ntreev027",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin17/ntreev11
    Put( mapInfo, "ntreev028",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin18/ntreev11
    Put( mapInfo, "ntreev029",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin19/ntreev11
    Put( mapInfo, "ntreev030",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin20/ntreev11
    Put( mapInfo, "ntreev031",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin21/ntreev11
    Put( mapInfo, "ntreev032",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin22/ntreev11
    Put( mapInfo, "ntreev033",      "18740b835c8747a2c648b0f02d014d15", "e95af119d98f6acaad7b7563244a16b5" );  // kogadmin23/ntreev11

    // 북미 추가계정 설정
    // 툴id, 툴pw(md5), 게임pw(md5)
    Put( mapInfo, "sgtool01",      "b35cafe4c9a4f4677e6edc05f1932446", "b35cafe4c9a4f4677e6edc05f1932446" );  // sgtool01/1gcgm!@#$
    Put( mapInfo, "sgtool02",      "df7cf9682011261e714fc44a44e23b83", "df7cf9682011261e714fc44a44e23b83" );  // sgtool02/2gcgm!@#$
    Put( mapInfo, "sgtool03",      "46f78c57166f6e0d03d5173097d3cf41", "46f78c57166f6e0d03d5173097d3cf41" );  // sgtool03/3gcgm!@#$
    Put( mapInfo, "sgtool04",      "b72a51091e72752e70c206cb4f68b5c0", "b72a51091e72752e70c206cb4f68b5c0" );  // sgtool04/4gcgm!@#$
    Put( mapInfo, "sgtool05",      "6c525370b3820ddc020195d867563fca", "6c525370b3820ddc020195d867563fca" );  // sgtool05/5gcgm!@#$

    // Philippine LevelUp
    Put( mapInfo, "gcphscreamer",   "d01607353f326d61139f848f2e8473ae", "d01607353f326d61139f848f2e8473ae" ); // GCPHScreamer/Dal2xngGM   --테섭
    Put( mapInfo, "gcgm01",         "17751b386d35c6fc69ea16e8a6d41c99", "17751b386d35c6fc69ea16e8a6d41c99" ); // GCGM01/ruth875 --본섭
    Put( mapInfo, "gcgm02",         "13443a4125c2a22926d42f44a0e9288f", "13443a4125c2a22926d42f44a0e9288f" ); // GCGM02/wsj35f --본섭
    Put( mapInfo, "gcgm03",         "d19d27e9960d69a27474f141da33bf69", "d19d27e9960d69a27474f141da33bf69" ); // GCGM03/kdk56s --본섭
    Put( mapInfo, "gcgm04",         "4a9eca3707e016439e9f75592327e45b", "4a9eca3707e016439e9f75592327e45b" ); // GCGM04/f3nr1s2011 --본섭
    Put( mapInfo, "gcgm05",         "60cfef16335ca12f1b7a2afd4bc86198", "60cfef16335ca12f1b7a2afd4bc86198" ); // GCGM05/enyavdum --본섭
    Put( mapInfo, "gcgm06",         "0656f1c9c0a51a4a2cfacf46632d5394", "0656f1c9c0a51a4a2cfacf46632d5394" ); // GCGM06/ash4k098 --본섭
    Put( mapInfo, "gcgm07",         "05e221d91613a44eb51ddab5dbcba2de", "05e221d91613a44eb51ddab5dbcba2de" ); // GCGM07/ro68yd --본섭
    Put( mapInfo, "gcgm08",         "b21583f80667ca96f9a201679516b04a", "6b2fc4aa4e56fa3a08cdff87d8cd3452" ); // GCGM08/she33n4 --본섭
    

    // 태국
    Put( mapInfo, "jonmaxmore.pp",  "f72c4a3873ebb548dfd473a0e8ad5be4", "a431a019f998118a0a7146208be23dc1" ); // jonmaxmore.PP / GmYellow
    Put( mapInfo, "peekacop.pp",    "da7384ded7a14a546a8f254d51b82068", "54ba5eab02a8d5992e8ce2ff178796bd" ); // peekacop.PP / GmGreen
    Put( mapInfo, "polar2night.pp", "8f3d465520f7c65deea459715529ac18", "1220d6e7d9f29d7942a5fd40f86312a9" ); // polar2night.PP / GmBlue
    Put( mapInfo, "tarororo.pp",    "90fa799662524988e5234ff77a6a8186", "5cc85b33df2c93ef44fbbae16507bf82" ); // tarororo.PP / GmRed
    Put( mapInfo, "wizzico.fb",     "122da2e8b45eff3c844cbb1c6cfb3fa0", "6a624de8c4db7c4e0af51bf83ffcd14d" ); // wizzico.FB / GmMagenta
    Put( mapInfo, "broadcast01.pp", "834e51e1855a38029cd624b8d8a8f839", "25f9e794323b453885f5181f1b624d0b" ); // broadcast01.PP / ppgm_01
    Put( mapInfo, "broadcast02.pp", "5c06bd255193ddf33f73e48e83456a10", "25f9e794323b453885f5181f1b624d0b" ); // broadcast02.PP / ppgm_02
    Put( mapInfo, "broadcast03.pp", "06705f5b32c036e19d41203e2daa8949", "25f9e794323b453885f5181f1b624d0b" ); // broadcast03.PP / ppgm_03
    Put( mapInfo, "broadcast04.pp", "fe7eb3de26d4be512b399bcd61beb53d", "25f9e794323b453885f5181f1b624d0b" ); // broadcast04.PP / ppgm_04
    Put( mapInfo, "broadcast05.pp", "4f0f3f90ed1bc4ac1988e0f89d151e83", "25f9e794323b453885f5181f1b624d0b" ); // broadcast05.PP / ppgm_05
    Put( mapInfo, "broadcast06.pp", "82137d76f8a63b8891f2e631720f733a", "25f9e794323b453885f5181f1b624d0b" ); // broadcast06.PP / ppgm_06
    Put( mapInfo, "ppayii30.pp",    "37ec3dd0bc264f550dcefac2c2be60b0", "37ec3dd0bc264f550dcefac2c2be60b0" ); // ppayii30.PP / payz3030
    Put( mapInfo, "jinkzero.pp",    "0450eb6af4d668c01dc44e12f0e3ece5", "0450eb6af4d668c01dc44e12f0e3ece5" ); // jinkzero.PP / 1153111130
    Put( mapInfo, "pique35.pp",     "9ae2be73b58b565bce3e47493a56e26a", "9ae2be73b58b565bce3e47493a56e26a" ); // pique35.PP / 963852741


    // 인도네시아
    Put( mapInfo, "megachase1",     "717d702197920eed375c7d1425f5b15a", "717d702197920eed375c7d1425f5b15a" ); // megachase1 / grandchase
    Put( mapInfo, "gcst01",         "d8b5c2c592e954389ac31d31d22ae02c", "d8b5c2c592e954389ac31d31d22ae02c" ); // gcst01 / canada818
    Put( mapInfo, "gcst02",         "ae37967798091f35f959c5e7a980e9c7", "ae37967798091f35f959c5e7a980e9c7" ); // gcst02 / corrado
    Put( mapInfo, "gcst03",         "020254c24244cf2b381a67d139dd20dc", "020254c24244cf2b381a67d139dd20dc" ); // gcst03 / slien9sqz
    Put( mapInfo, "gcst04",         "111bb7042ad29ada14c3ad0299f7d919", "111bb7042ad29ada14c3ad0299f7d919" ); // gcst04 / 7oueliews
    Put( mapInfo, "gcst05",         "753914c1d9cff0ba7cb306bed62a2ea1", "753914c1d9cff0ba7cb306bed62a2ea1" ); // gcst05 / ruwlkfc2
    Put( mapInfo, "gcst06",         "4dfb5ab9455306ab4c1eb2a73fe1230d", "4dfb5ab9455306ab4c1eb2a73fe1230d" ); // gcst06 / u48mniwz 
    Put( mapInfo, "gcst07",         "bd45d2f97e92382c15485c92e0fe1e64", "bd45d2f97e92382c15485c92e0fe1e64" ); // gcst07 / ijed2jnc
    Put( mapInfo, "gcst08",         "2767d8b92ea64bdab584fbd390031c0b", "2767d8b92ea64bdab584fbd390031c0b" ); // gcst08 / mocevru4 
    Put( mapInfo, "gcst09",         "3da3c99099cc559176b50e1210a557e3", "3da3c99099cc559176b50e1210a557e3" ); // gcst09 / qweasui0j
    Put( mapInfo, "gcst10",         "142a6303c737ebb6aa6e0b90a9d54f0a", "142a6303c737ebb6aa6e0b90a9d54f0a" ); // gcst10 / rtymgh12

    // 남미
    //Put( mapInfo, "ax5acc3s5gc",     "69d9b45f0cb863b82514800ad0bb3e5d", "69d9b45f0cb863b82514800ad0bb3e5d" ); // AX5ACC3S5GC / G00DW0RK!
    Put( mapInfo, "linal01",     "01013e5a1aa842c1cefe0953920a3ae6", "01013e5a1aa842c1cefe0953920a3ae6" ); // linal01 / tester123456


    // 유럽
    Put( mapInfo, "year2kilo",     "ffa50c6411fbdfa07efe2cf43619763f", "ffa50c6411fbdfa07efe2cf43619763f" ); // year2kilo / c00lc00l

    // 중국
    Put( mapInfo, "yhmxgm1@changyou.com",     "e7a90c36bf65b144880127dc435303c0", "e7a90c36bf65b144880127dc435303c0" ); // yhmxgm1@changyou.com / gchina1
    //Put( mapInfo, "yhmxgm1@changyou.com",     "e7a90<36bf65b144880127dc435303c0", "e7a90<36bf65b144880127dc435303c0" ); // yhmxgm1@changyou.com / gchina1
    Put( mapInfo, "yhmxgm2@changyou.com",     "be15f9fb9d111e03fde5a1d4cf731ce4", "be15f9fb9d111e03fde5a1d4cf731ce4" ); // yhmxgm2@changyou.com / gchina6
    Put( mapInfo, "yhmxgm3@changyou.com",     "e9a9539fc7aa0aeb4f0f7f92dcbcbd9d", "e9a9539fc7aa0aeb4f0f7f92dcbcbd9d" ); // yhmxgm3@changyou.com / gchina9
    Put( mapInfo, "yhmxgm4@changyou.com",     "842953e71369e6916c1004e5f1428e4c", "842953e71369e6916c1004e5f1428e4c" ); // yhmxgm4@changyou.com / gchina16
    Put( mapInfo, "yhmxgm5@changyou.com",     "74ff5fb213f70244c35e1747fde3647c", "74ff5fb213f70244c35e1747fde3647c" ); // yhmxgm5@changyou.com / gchina28
    Put( mapInfo, "yhmxgm6@changyou.com",     "ac044bda364643ca470b2c414a5d5c6a", "ac044bda364643ca470b2c414a5d5c6a" ); // yhmxgm6@changyou.com / gchina36
    Put( mapInfo, "pengpeng801@changyou.com",  "11B38B38D28249E2651C9C0E00B081A9", "11B38B38D28249E2651C9C0E00B081A9" ); // pengpeng801@changyou.com / woailvpeng

    /* 중국 로그인 계정 응답의 유저 계정 정보
    유저계정  로그인 계정
    548021379 yhmxgm1@changyou.com
    548055485 yhmxgm2@changyou.com
    548055910 yhmxgm3@changyou.com
    548056359 yhmxgm4@changyou.com
    548056788 yhmxgm5@changyou.com
    548057341 yhmxgm6@changyou.com
    pengpeng801@changyou.com pengpeng801@changyou.com
    */
    std::map< std::string, std::string > mapCNAccountTable;
    mapCNAccountTable.clear();
    mapCNAccountTable[ "yhmxgm1@changyou.com" ] = "548021379";
    mapCNAccountTable[ "yhmxgm2@changyou.com" ] = "548055485";
    mapCNAccountTable[ "yhmxgm3@changyou.com" ] = "548055910";
    mapCNAccountTable[ "yhmxgm4@changyou.com" ] = "548056359";
    mapCNAccountTable[ "yhmxgm5@changyou.com" ] = "548056788";
    mapCNAccountTable[ "yhmxgm6@changyou.com" ] = "548057341";
    mapCNAccountTable[ "pengpeng801@changyou.com" ] = "pengpeng801@changyou.com";

    // 입력한 정보를 가져온다.
    UpdateData( TRUE );

    if ( TRUE == m_cstrLoginID.IsEmpty() || TRUE == m_cstrLoginPW.IsEmpty() )
    {
        EndDialog( IDCANCEL );
        return;
    }

    // 국내 아이디 확인
    std::vector<CString>::iterator vit;
    for ( vit = m_vecKoreaLoginIDList.begin(); vit != m_vecKoreaLoginIDList.end(); vit++ ) {
        if ( vit->Compare(m_cstrLoginID) == 0 ) {
            m_bLangStateKorean = true;
            break;
        }
        m_bLangStateKorean = false;
    }

    // 아이디 확인
    std::map<std::string,KLoginInfo>::iterator mit;
    m_cstrLoginID.MakeLower();
    mit = mapInfo.find( (LPCSTR)(CStringA)m_cstrLoginID );
    if( mit == mapInfo.end() )
    {
        ::AfxMessageBox( L"Not registered ID." );
        CDialog::OnCancel();
        return;
    }

    // 중국 전용 로그인 계정 -> 유저 계정 변환하기
    std::string strAccount = (LPCSTR)(CStringA)m_cstrLoginID;
    std::map< std::string, std::string >::iterator mitCNAccountTable;
    mitCNAccountTable = mapCNAccountTable.find( strAccount );
    if ( mitCNAccountTable != mapCNAccountTable.end() ) {
        m_cstrLoginID = mitCNAccountTable->second.c_str();

        // 중국은 passwd 필드에 로그인 계정이 그대로 들어간다
        m_cstrLoginPW = mitCNAccountTable->first.c_str();
    }
    else {
        // 일반(중국 외) 패스워드 확인
        std::wstring strPW = KncUtil::toWideString( mit->second.strPW );
        std::wstring strCurrentPW = CMD5Checksum::GetMD5( (BYTE*)(LPCSTR)((CStringA)m_cstrLoginPW), m_cstrLoginPW.GetLength() );
        if ( strCurrentPW != strPW )
        {
            /* 비번 확인용
            CString strTemp;
            strTemp.Format( _T(" wrong password.\n %s\n %s\n"), strPW.c_str(), strCurrentPW.c_str() );
            ::AfxMessageBox( strTemp );
            */
            ::AfxMessageBox( _T(" wrong password.\n") );
            CDialog::OnCancel();
            return;
        }

        // 실제 접속에 필요한 패스워드를 만든다.
        m_cstrLoginPW = mit->second.strCode.c_str();
    }

    g_nLevel = mit->second.m_nLevel;

    // Login success
    EndDialog( IDOK );
}
void KLoginDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    OnOK();
}
