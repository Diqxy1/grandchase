#pragma once
//#include "stdafx.h"
//#include "UserPacket.h"
#include "IGCSquareObj.h"
#include "GCSquarePeople.h"
#include "Pet.h"

class GCSquarePeople;

class GCSquarePet
: public IGCSquareObj
{
public:
    GCSquarePet(void);
    virtual ~GCSquarePet(void);

    // functions
    virtual void FrameMove();
    virtual void ShadowRender();
    virtual bool Load();
    virtual bool TimeShareLoading();
    void RenderPetName();
    void OnLoadingComplete();

    // getter/setter
    void SetSimplePetInfo( const KSimplePetInfo& kPetInfo_ ) { m_kPetInfo = kPetInfo_; }
    void SetMaster( GCSquarePeople* pPeople );
    KPet* GetPetPtr() { return &m_kPet; }

private:
    KPet m_kPet;
    KSimplePetInfo m_kPetInfo;
    GCSquarePeople* m_pPetMaster;
    LOADING_STATE m_kLoadedState;
};
