#pragma once
#include "damage.h"

class CHardDamageAmy4CannonBall :
	public CDamage
{
public:
    CHardDamageAmy4CannonBall(void){};
    virtual ~CHardDamageAmy4CannonBall(void){};
	virtual void FrameMove(CDamageInstance* pDamageInstance );
};


class CHardDamageAmy4RandomBoy :
    public CDamage
{
public:
    CHardDamageAmy4RandomBoy(void);
    virtual ~CHardDamageAmy4RandomBoy(void){};    

    virtual void Begin( CDamageInstance* pDamageInstance );    
    virtual void BeginWithLocate( CDamageInstance* pDamageInstance , float fStartX , float fStartY );
    std::vector<std::string> m_vecNameList;
};
