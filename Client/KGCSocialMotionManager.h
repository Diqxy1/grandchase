#pragma once

struct KSosicalMotionInfo 
{
    std::pair<int, int>     prMotionID; // 모션ID< 남캐ID, 여캐 ID >
    bool                    bUseSpace;  // Space키로 발동되는 모션인가?

    KSosicalMotionInfo() {
        prMotionID.first = -1;
        prMotionID.second = -1;
        bUseSpace = false;
    }
};

class KGCSocialMotionManager
{	
    DeclareSingleton( KGCSocialMotionManager );

public:
    KGCSocialMotionManager( void );
    virtual ~KGCSocialMotionManager( void );

public:
    bool LoadSocialMotionInfo( const char* szFileName_ );
    void SetMotion( const std::wstring& strCommand_ );
    void SetMotion( const int iMotionID_ );
    void SetMotion( const GCITEMID itemID_, const int iPlayerIndex_ = -1 );

    bool IsSocialMotionCommand( const std::wstring& strMsg_, bool bIsCoordiMotion_ = false );
    bool GetSocialMotion( std::set<int>& setMotionList_ );

    void SetEmoticonMotionList( void );
    void GetEmoticonMotionList( OUT std::vector< int >& vecList_ );
    
private:
    std::map< std::string, KSosicalMotionInfo >   m_mapSocilMotionInfo;      // < 모션 명령, 모션 MID<남, 여> >
    std::map< std::string, KSosicalMotionInfo >   m_mapOtherMotionInfo;      // < 모션 명령, 모션 MID<남, 여> >

    std::map< GCITEMID, std::vector< std::vector< int > > >  m_mapEmoticonMotion;   // < 이모티콘 ID, 모션 리스트< 모션 이름 리스트 > > 
    std::vector< int >                                       m_vecEmoticonMotion;   // 이모티콘 모션 전체 리스트 
};

DefSingletonInline( KGCSocialMotionManager );