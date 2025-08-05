#pragma once

#include "KFootHold.h"

class KGCObj;

#define FOOTHOLDSIZE_REAL (20.0f)
#define TOINDEXFACTOR (20.0f) //400 / FOOTHOLDSIZE_REAL
#define TO_INDEX(p) (static_cast<int>(p*TOINDEXFACTOR+0.5f))

class KGCFootHold : public KFootHold<KGCFootHold>
{
public:
    KGCFootHold(void);
    virtual ~KGCFootHold(void);

    inline void SetParent( KGCObj* pParent ) { m_pParent = pParent; }
    inline KGCObj* GetParent() { return m_pParent; }

    virtual void SetPos( const float& fX, const float& fY );

private:
    // m_pParent == NULL �̸� Obj�� ������ ���� ���� ����, �� ��, ������ ������
    KGCObj* m_pParent;
};
