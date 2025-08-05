#pragma once

class KGCResourceCollector
{
	friend class KGCCollectorObserver;
	DeclareSingleton( KGCResourceCollector );
public:
	KGCResourceCollector(void);
	~KGCResourceCollector(void);

	enum EGCResourceType{
        // 여기에 새로운 타입을 추가하려면!!
        // 생성자에 Feedback 테이블 이름과 Index의 Max 값을 넣어 줘야 합니다.
        // 테이블 이름은 Lua 에 사용됩니다.
        // Max 값은 Lua에서 Index를 읽어올 때 사용됩니다. (주의)그래서 Lua에 쓸때는 Index를 5로 하고 Max값이 4면 안읽어집니다.
        // 그리고 LoadTexure 함수를 타입에 따라 적절한 위치에 추가해 주면 됩니다.
		GCRC_PLAYER = 0,
		GCRC_MONSTER,
		GCRC_DUNGEON,
        GCRC_PET,
        GCRC_NOTYPE,

		GCRC_NUM,
	};

	typedef std::map<int, std::set<std::string> >	ResourceList;
	typedef std::map< EGCResourceType, ResourceList > ResourceListAll;

private:
	bool FeedbackToFile( EGCResourceType eType, int kind, const std::string& strName, ResourceListAll& RemoveList = ResourceListAll() );

	bool GetResourceList( KLuaManager& luaMgr, EGCResourceType eType, ResourceList& List );
	bool WriteResourceList( std::fstream& file, EGCResourceType eType, ResourceList& List, ResourceList& RemoveList = ResourceList() );
	void GetLoadList( KLuaManager& luaMgr, EGCResourceType eType, int kind, std::set<std::string>& List );

	void SetResouceType( EGCResourceType eType, int iValue );

	void StartCollection();
	void StopCollection();

    bool IsStarted() { return m_bStarted; }
    const std::pair<EGCResourceType,int>& GetCurrentState() { return m_CurrentState; }
    void AddToReportList( EGCResourceType eType, int kind, std::string strResourceName );
public:
	void AddNotLoadedParticleTexture( std::string strTextureName );
	void LoadTextures( EGCResourceType eType, int kind );
	void RefreshLocalScript();

    void ReportToServer();
    void ClearReportList();
    void SetColltectorActivation( bool bActive ) { m_bActivation = bActive; }
private:
	bool m_bStarted;
    bool m_bActivation;
	std::pair<EGCResourceType,int> m_CurrentState;
	std::map<EGCResourceType,std::string> m_mapTableName;
	std::map<EGCResourceType,int> m_mapKindMaxNum;
    KEVENT_EFFECT_TEX_REPORT m_ReportList;
};

DefSingletonInline( KGCResourceCollector );

class KGCCollectorObserver
{
private:
    KGCCollectorObserver();
    bool bStarted;
    std::pair<KGCResourceCollector::EGCResourceType,int> m_CurrentState;
public:
	KGCCollectorObserver( KGCResourceCollector::EGCResourceType eResourceType, int iValue )
	{
        bStarted = SiKGCResourceCollector()->IsStarted();
        m_CurrentState = SiKGCResourceCollector()->GetCurrentState();
		SiKGCResourceCollector()->SetResouceType( eResourceType, iValue );
		SiKGCResourceCollector()->StartCollection();
	}
	~KGCCollectorObserver()
    {
        SiKGCResourceCollector()->StopCollection();
        SiKGCResourceCollector()->SetResouceType( m_CurrentState.first, m_CurrentState.second );

        if( bStarted )
            SiKGCResourceCollector()->StartCollection();
	}
};