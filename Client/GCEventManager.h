#pragma once
#include "KTDGParticleSystem.h"
#include "KNCSingleton.h"
//#include <set>

class KGCEventManager
{
private:
    std::map< int, int > m_mapEventPool;
    std::vector< CParticleEventSeqPTR > m_vecParticles;
public:
    DeclareSingleton( KGCEventManager );
public:
    KGCEventManager(void);
    ~KGCEventManager(void);


    enum{
        EP_LOADING = 1,
        EP_GAME = 2,
        EP_RESULT,
    };

    bool IsActiveEvent( int iEventID );
	void SetEvents( std::set<int>& setEvents );
	void SetEvents( std::map< DWORD, std::set<int> >& mapEvents );

    bool EventActivation( DWORD dwEventID_, int iPosition );

    void ClearResource();
    void ClearEvents();
private:
    //Bind in Lua
    bool RegisterLuabind();
    bool ParticleEffect( std::string strName_, float fx_ = -1 , float fy_ = -1, float fz_ = -1 );

    //private function

    //private Value
    std::set<int> m_setEvents;
};
DefSingletonInline( KGCEventManager );