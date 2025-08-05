#pragma once
//#include <vector>
//#include <windows.h>

class FRect_Quest
{
public:
    float left;
    float right;
    float top;
    float bottom;

    FRect_Quest();
    ~FRect_Quest();

    void SetRect( float _left, float _top, float _right, float _bottom );
    void Zoom( float Zoom );

    bool PtInRect( float fX, float fY );
    bool RectInRect( float fMinX, float fMinY, float fMaxX, float fMaxY );
};

class KQSFootHold;

struct SBoneTraceFootHold
{
    UCHAR ucBoneID;
    KQSFootHold* pHold;

    SBoneTraceFootHold() {}
    SBoneTraceFootHold( const SBoneTraceFootHold& kRight )
    {
        *this = kRight;
    }

    SBoneTraceFootHold& operator = (const SBoneTraceFootHold& kRight)
    {
        ucBoneID = kRight.ucBoneID;
        pHold = kRight.pHold;
        return *this;
    }
};

class KVirtualFootHold
{
public:
    KVirtualFootHold() {}
    ~KVirtualFootHold() {}

    UCHAR ucBoneIndex;
    float fRelX;
    float fRelY;
};

class KGCObj_Quest
{
public:
    KGCObj_Quest() {}
    ~KGCObj_Quest() {}

    typedef std::vector<SBoneTraceFootHold>::iterator iterFootHold;

    // [8/28/2007 breadceo] 발판 관련
public:
    virtual void AddVirtualFootHold( const KVirtualFootHold& kFootHold ) = 0;
    virtual void AddFoots( UCHAR ucBoneID, float fRelX, float fRelY, bool bFoots ) = 0;
    virtual void ClearFootHold() = 0;
    virtual bool IsDie() = 0;

    std::vector<SBoneTraceFootHold> m_vecTraceFootHold;
};
