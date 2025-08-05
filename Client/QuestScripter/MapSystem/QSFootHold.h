#pragma once

#include "KFootHold.h"
#include "MapHeader.h"

#define FOOTHOLDSIZE_REAL (20.0f)
#define TOINDEXFACTOR (20.0f) //400 / FOOTHOLDSIZE_REAL
#define TO_INDEX(p) (static_cast<int>(p*TOINDEXFACTOR+0.5f))

class KQSFootHold : public KFootHold<KQSFootHold>
{
public:
    KQSFootHold(void);
    ~KQSFootHold(void);

    inline void SetParent( KGCObj_Quest* pParent ) { m_pParent = pParent; }
    inline KGCObj_Quest* GetParent() { return m_pParent; }

    virtual void            SetPos( const float& fX, const float& fY );

private:
    // m_pParent == NULL 이면 Obj가 가지고 있지 않은 발판, 즉 맵, 마을용 발판임
    KGCObj_Quest* m_pParent;
};
