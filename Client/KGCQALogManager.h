#pragma once

class KGCQALogManager
{	
    DeclareSingleton( KGCQALogManager );

public:
    KGCQALogManager( void );
    virtual ~KGCQALogManager( void );

public:
    void SetQALogState( bool bSet_ );
    bool IsQALog( void ) { return m_bQALog; }
    void EndQALog( void );

    void StartGameLog( const KChangeRoomInfo roomInfo_ );
    void EndGameLevelUpLog( const std::wstring& strNick_, const bool bLvUP, const int iStartLv_, const int iResultLv_,
                                                   const bool bSLvUp_, const int iStartSLv_, const int iResultSLv_ );
    void EndGameExpLog( const std::wstring& strNick_, const int iBaseExp_, const int iExtraExp_, const int iTotalExp, const int iSPExp_ );
    void DungeonDropDiceLog( const std::wstring& strNick_, const DWORD dwDiceNumber_ );
    void DungeonDropGetLog( const std::wstring& strNick_, const GCItem* pItemInfo_, const std::wstring& strItemCharType_ );
    void RecvItemLog( const std::vector< std::pair< GCITEMUID, int > >& vecItem_ );
    
    void MonsterChangeHPLog( const std::wstring& strNick_, const std::wstring& strMonsterName_, const int iMonsterIndex_,
                             const float fOldMonHP_, const float fNewMonHP_, const float fDamage_, 
                             const bool bPetDamage_, const bool bSpecial_, const bool bCritical_, const bool bSpear_ );
    void PlayerChangeHPLog( const std::wstring& strNick_, const PLAYER::DamageInfo& damage_, const int iAttackerIndex_, 
                            const float fHP_, const float fDamage_, 
                            const bool bSpecial_, const bool bCritical_, const bool bSpear_ );

private:
    void WriteLogFile( const std::wstring& strLog_ );

private:
    bool            m_bQALog;
    std::ofstream  m_ofLog;

};

DefSingletonInline( KGCQALogManager );