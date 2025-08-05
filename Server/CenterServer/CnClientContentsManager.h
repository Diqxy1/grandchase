#pragma once

#include <KNCSingleton.h>
#include <RTTI.H>
#include <ToString.h>
#include <Thread/Locker.h>
#include <map>
#include <vector>
#include <set>
#include "CnKairo.h"
#include "CommonPacket.h"
#include "NUserPacket.h"

class KLuaManager;
class KCnClientContentsManager
{
    DeclareSingleton( KCnClientContentsManager );
    NiDeclareRootRTTI( KCnClientContentsManager );
    DeclToStringW;

public:
    KCnClientContentsManager(void);
    ~KCnClientContentsManager(void);

    bool LoadScript();
    bool GetContentsInfo( OUT std::vector<std::pair<int, std::vector<int> > >& m_vecChannelCategories,
                          OUT std::vector<std::pair<int, std::vector<int> > >& m_vecCategoryModes,
                          OUT std::vector<std::pair<std::pair<int,bool>, std::vector<int> > >& vecCategoryInfo,
                          OUT std::map<int,std::set<char>>& mapCharactes,
                          OUT std::vector<int>& vecCharSP4OpenInfo,
                          OUT std::map<int,DWORD>& mapCharOpenType,
                          OUT std::map<int,GCITEMID>& mapCharCashGoods,
                          OUT std::vector<int>& vecDisableMenuList,
                          OUT GCITEMID& SlotItem
                          );
    int GetMaxLevel() { KLocker lock( m_csContents ); return m_nMaxLevel; };

    void GetChannelNewsImageNumber( OUT std::vector<int>& vecNumber );
    void GetLoadingImage( OUT std::vector<std::wstring>& vecFileName );
    void GetClientScriptName( OUT std::map<int, std::wstring >& mapScriptName );
    void GetExceptionMotionID( OUT std::vector<int>& vecNumber );
    void GetPVPLoadingImage( OUT std::map< int, std::vector<std::wstring> >& mapFileName );
    void GetEventBannerInfo( OUT std::vector< std::pair<std::wstring, int> >& mapEventBannerInfo );
    void GetCharSkillInfo( OUT std::set< DWORD >& setCharSkillEnable_ );

    void SetServerBenefitMsg( IN OUT std::vector< KServerInfo >& kServerList );
    void GetDLLBlackList( OUT std::set<std::wstring >& setBlackList );
    void GetChampionRatio( OUT std::map< int, float >& mapChampionRatio_ );
    void GetEventOpenInfo( OUT std::vector< KEventOpenInfo >& vecEventOpenInfo_ );
    void GetEventMonsterAbility( OUT std::map< int, KMonsterAbility >& mapMonsterList_ );
    void GetSmartPanelInfo( OUT bool& bSmartPanelMailListEnable, OUT std::vector<std::wstring>& vecSmartPanelMailList );
    void GetChannelImageDownloadURLList( OUT std::map<int, std::wstring>& mapChannelImageDownloadURLList );

protected:
    bool LoadVecPair( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::vector<std::pair<int, std::vector<int> > >& vec_pair );
    bool LoadMapInfo( IN KLuaManager& kLuaMng, OUT std::vector<std::pair<std::pair<int,bool>, std::vector<int> > >& vecModeInfo );
    bool LoadCharacterInfo( IN KLuaManager& kLuaMng, OUT std::map<int,std::set<char>>& mapCharactes );
    bool LoadCharacterSP4OpenInfo( IN KLuaManager& kLuaMng, OUT std::vector<int>& vecCharSP4OpenInfo );
    void LoadNumSequence( IN KLuaManager& kLuaMng, OUT std::vector<int>& vecSequence );

    bool LoadChannelNews( IN KLuaManager& kLuaMng, OUT std::vector<int>& vecNumber );
    bool LoadLoadingImage( IN KLuaManager& kLuaMng, OUT std::map< int, std::vector< std::wstring > >& mapFileName, IN std::string& LoadingImageTableName, OUT CnKairo& cnKairo );
    bool LoadClientScriptName( IN KLuaManager& kLuaMng, OUT std::map<int, std::wstring >& mapScriptName );
    bool LoadExceptionMotionID( IN KLuaManager& kLuaMng, OUT std::vector<int>& vecNumber );
    bool DoImageGamble( OUT int& nNum );
    bool LoadPVPLoadingImage( IN KLuaManager& kLuaMng, OUT std::map< int, std::vector< std::wstring > >& mapFileName, IN std::string& LoadingImageTableName );
    bool LoadEventBannerInfo( IN KLuaManager& kLuaMng, OUT std::vector< std::pair<std::wstring, int> >& vecEventBannerInfo );
    bool LoadCharSkillInfo( IN KLuaManager& kLuaMng, OUT std::set<DWORD>& setCharSkillEnable );
    bool LoadTimeS4( IN KLuaManager& kLuaMng_, IN std::string& strTable_, OUT time_t& tmTime_ );
    bool LoadBenefitMsg( IN KLuaManager& kLuaMng, OUT std::map<int, std::wstring >& mapScriptName );
    bool LoadDLLBlackList( IN KLuaManager& kLuaMng_, OUT std::set<std::wstring >& setBlackList_ );
    bool LoadChampionRatio( IN KLuaManager& kLuaMng, OUT std::map<int,float>& mapChampionRatio );
    bool LoadEvnetOpenInfo( IN KLuaManager& kLuaMng, OUT std::vector<KEventOpenInfo>& vecEventOpenInfo );
    bool LoadEventMonsterAbility( IN KLuaManager& kLuaMng_, OUT std::map<int, KMonsterAbility>& mapMonsterList_ );
    bool LoadSetInfo( IN KLuaManager& kLuaMng, IN std::string& strTable, OUT std::set<char>& setCharPromotion );
    bool LoadCharOpenType( IN KLuaManager& kLuaMng_, OUT std::map< int, DWORD >& mapCharOpenType_ );
    bool LoadCharCashGoods( IN KLuaManager& kLuaMng_, OUT std::map< int, GCITEMID >& mapCharCashGoods_ );
    bool LoadDisableMenuList( IN KLuaManager& kLuaMng_, OUT std::vector<int>& vecDisableMenuList );
    bool LoadSmartPanelMailList( IN KLuaManager& kLuaMng, OUT std::vector<std::wstring>& vecSmartPanelMailList );
    bool LoadChannelImageDownloadURLList( IN KLuaManager& kLuaMng, OUT std::map<int, std::wstring>& mapChannelImageDownloadURLList );
protected:
    mutable KncCriticalSection      m_csContents;
    // 아래 컨테이너에 vector를 쓴 이유는 순서가 의미가 있기 때문이다.
    // 아래 데이터에 대한 별도의 타입을 만들어야 할지는 나도 결정을 내리기 힘들다.. ㅜㅜ.
    // 하지만 가독성은 매우 떨어 진다는건 확힐하다.. 
    std::vector<std::pair<int, std::vector<int> > >                     m_vecChannelCategories; // Channel,<Category>
    std::vector<std::pair<int, std::vector<int> > >                     m_vecCategoryModes; // Category,<Modes>
    std::vector<std::pair<std::pair<int,bool>, std::vector<int> > >     m_vecCategoryInfo; // <Category,Random>,<Maps>
    std::map<int,std::set<char>>                                        m_mapCharactes;
    std::vector<int>                                                    m_vecCharSP4OpenInfo; //캐릭터 4필 오픈 정보
    int                                                                 m_nMaxLevel;

    // 클라이언트 추가 정보
    std::vector<int>                                                    m_vecChannelNewsImageNumber; // 채널뉴스 이미지 번호
    std::map<int, std::vector<std::wstring> >                           m_mapLoadingImageName; // <Index,pair<ScriptName,ScriptName> > 메인로딩 이미지 파일이름
    std::map<int, std::wstring >                                        m_mapClientScriptName; // <Index,ScriptName> 클라이언트 스크립트 이름
    std::vector<int>                                                    m_vecExceptionMotionID; // 예외 모션 번호
    std::map<int, std::vector<std::wstring> >                           m_mapPVPLoadingImageName; // <Index,pair<ScriptName,ScriptName> > 대전로딩 이미지 파일이름
    CnKairo                                                             m_CnKairoInfo;
    std::vector< std::pair<std::wstring, int> >                         m_vecEventBannerInfo;   // <banner file name, 사용여부> 이벤트 배너 정보.
    std::set<DWORD>                                                     m_setCharSkillEnable; // 캐릭터 스킬트리 ON/OFF 여부
    std::map<int, std::wstring >                                        m_mapServerBenefitMsg; // <ServerPart,Message> 서버 혜택 메세지(툴팁용)
    std::set<std::wstring>                                              m_setDLLBlackList; // BlackDllList
    std::map<int,float>                                                 m_mapChampionRatio; // 챔피언 모드 적용 능력치 확률
    std::vector<KEventOpenInfo>                                         m_vecEventOpenInfo; // 이벤트 알리미 적용될 내용.
    std::map<int, KMonsterAbility>                                      m_mapMonsterAbility; // 몬스터 능력치 비율을 바꿈.
    std::map<int, DWORD>                                                m_mapCharOpenType; // 케릭터 무료지급, 캐쉬&미션 지급 여부 체크를 위한 컨테이너
    std::map<int, GCITEMID>                                             m_mapCharCashGoods;  // 케릭터 무료지급, 캐쉬&미션 지급 여부 체크를 위한 컨테이너
    std::vector<int>                                                    m_vecDisableMenuList; //활성화 되지 않아야할 메뉴 버튼 리스트
    bool                                                                m_bSmartPanelMailListEnable;
    std::vector<std::wstring>                                           m_vecSmartPanelMailList; //스마트 패널 리스트.
    std::map<int, std::wstring>                                         m_mapChannelImageDownloadURLList; //채널 이미지 다운로드 URL 리스트.
    GCITEMID                                                            m_SlotItem;
};

DefSingletonInline( KCnClientContentsManager );
DeclOstmOperatorA( KCnClientContentsManager );
