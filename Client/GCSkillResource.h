#pragma once

//#include "GCEnum.h"
//#include <set>
#include "Damage.h"
#include "GCSkillEnum.h"

class GCSkill;

class GCSkillResource
{
public:
    GCSkillResource(void);
    ~GCSkillResource(void);

    // functions
    bool Load( KLuaManager& luaMgr );

    // getter/setter
    EGCSkillTree GetID() const { return m_eID; }
    void LoadRes( DWORD dwUID );
    void ReleaseRes( DWORD dwUID );
    void SetSkill( GCSkill* pSkill );
    void GetMotionID( std::set< MOTIONID >& setMotionID_ );

private:
    EGCSkillTree m_eID;
    GCSkill* m_pSkill;
    std::set< MOTIONID > m_setMotionID;
    std::set< DAMAGE_TYPE > m_setFireInfo;
    std::set< int > m_setSoundID;
    // 강화목걸이별로는 생각안하겠다!
    std::vector< std::string > m_vecAnimation;
    // Particle?
    // Sound?
};
