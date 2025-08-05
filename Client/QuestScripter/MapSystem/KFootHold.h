#pragma once

#include "../GCUtil/GCUtil.h"
#include "KMap.h"

template <class T0>
class KFootHold
{
public:
    KFootHold(void);
    virtual ~KFootHold(void);

    // 밟는 놈인가? 밟히는 놈인가?
    enum FOOTHOLDTYPE
    {
        FHT_INIT = 0x00000000,      // 아무것도 아니오~!
        //FHT_FOOT = 0x00000001,      // 발 ( 발은 주로 유닛, 몬스터에 있을 듯? )
        FHT_HEAD = 0x00000002,      // 밟히는 부분 ( 맵 블록이나 엘리시스 머리, 고르고스 머리 등에 위치 )
    };

    enum FOOTHOLDPROP
    {
        FHP_JUST_FOOTHOLD   = 0x00000000,       // 그런거 없다. 올라가는 발판임
        FHP_NO_JUMP_DOWN    = 0x00000001,       // ↓ 키를 눌러서 못내려간다
        FHP_NO_JUMP_UP      = 0x00000002,       // ↑ 키를 눌러서 못올라간다
        FHP_NO_COME_RIGHT   = 0x00000004,       // ← 키를 눌러서 못감
        FHP_NO_COME_LEFT    = 0x00000008,       // → 키를 눌러서 못감
		FHP_DEAD_FOOTHOLD	= 0x00000010,		// Dead
        FHP_NO_ACTION       = 0x00000020,       // 그냥 발판. 아무것도 하지 않는다. 아지트 오브젝트 발판용르로
    };

    virtual inline float    GetX() const { return m_fX; }
    virtual inline float    GetY() const { return m_fY; }

    virtual inline float    GetPrevX() const { return m_fPrevX; }
    virtual inline float    GetPrevY() const { return m_fPrevY; }

    virtual inline int      GetPrevIndexX() const { return m_iPrevIndexX; }
    virtual inline int      GetPrevIndexY() const { return m_iPrevIndexY; }

    virtual inline int      GetIndexX() { if( m_iIndexX < 0 ) m_iIndexX = 0; return m_iIndexX; }
    virtual inline int      GetIndexY() { if( m_iIndexY < 0 ) m_iIndexY = 0; return m_iIndexY; }

    virtual inline DWORD    GetFootHoldProp() const { return m_dwFHProp; }
    virtual inline DWORD    GetFootHoldType() const { return m_dwFHType; }

    virtual inline void     SetFootHoldProp( const DWORD& _Prop ) { m_dwFHProp = _Prop; }
    virtual inline void     SetFootHoldType( const DWORD& _Type ) { m_dwFHType = _Type; }

    virtual inline void     SetLocalPos( const float& fLocalX, const float& fLocalY ) { m_fLocalX = fLocalX; m_fLocalY = fLocalY; }
    virtual inline float    GetLocalX() const { return m_fLocalX; }
    virtual inline float    GetLocalY() const { return m_fLocalY; }

    // SetPosition
    virtual void            SetPos( const float& fX, const float& fY ) = 0;

    virtual void            SetValue( const std::string& strValue ) { m_strFHValue = strValue; }
    virtual std::string     GetValue() const { return m_strFHValue; }

    virtual void Clear()
    {
        m_fX = 0;
        m_fY = 0;

        m_dwFHProp = KFootHold::FHP_JUST_FOOTHOLD;
        m_dwFHType = KFootHold::FHT_INIT;

        m_fLocalX = 0;
        m_fLocalY = 0;

        m_iPrevIndexX = 0;
        m_iPrevIndexY = 0;
        m_iIndexX = 0;
        m_iIndexY = 0;

        m_strFHValue = "";
    }

    KFootHold& operator = ( const KFootHold& rvar )
    {
        this->m_fX                  = rvar.m_fX;
        this->m_fY                  = rvar.m_fY;
        this->m_dwFHProp            = rvar.m_dwFHProp;
        this->m_dwFHType            = rvar.m_dwFHType;

        this->m_fLocalX             = rvar.m_fLocalX;
        this->m_fLocalY             = rvar.m_fLocalY;

        this->m_iPrevIndexX         = rvar.m_iPrevIndexX;
        this->m_iPrevIndexY         = rvar.m_iPrevIndexY;

        this->m_iIndexX             = rvar.m_iIndexX;
        this->m_iIndexY             = rvar.m_iIndexY;

        this->m_strFHValue          = rvar.m_strFHValue;

        return *this;
    }

protected:
    // X Pos
    float           m_fX;
    // Y Pos
    float           m_fY;

    // Local X
    float           m_fLocalX;
    // Local Y
    float           m_fLocalY;

    // X Prev Pos
    float           m_fPrevX;
    // Y Prev Pos
    float           m_fPrevY;

    // Property
    DWORD           m_dwFHProp;
    DWORD           m_dwFHType;

    int             m_iIndexX;
    int             m_iIndexY;

    int             m_iPrevIndexX;
    int             m_iPrevIndexY;

    std::string     m_strFHValue;
};

#include "KFootHold.inl"
