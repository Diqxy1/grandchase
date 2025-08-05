#ifndef _D3DSCROLLBAR_H_
#define _D3DSCROLLBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "D3DWnd.h"
//#include "ActionListener.h"
//#include <map>
//#include <string>

// assumption : vertical scrollbar

class KD3DScrollbar : public KD3DWnd,           // extends
                      public KActionListener    // implements
{
public:
    DECLARE_CLASSNAME( KD3DScrollbar );
    /// Default constructor
    KD3DScrollbar( void );
    /// Default destructor
    virtual ~KD3DScrollbar( void );
    /// Copy constructor
    KD3DScrollbar( const KD3DScrollbar& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KD3DScrollbar& operator=( const KD3DScrollbar& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    // 2012.04.18, shmhlove
    // 이 클래스의 Set함수들은 의존성이 높습니다.
    // 그래서 순서에 맞춰 호출하지 않으면 엉뚱한 동작을 하게 되어
    // 매번 사용할때마다 스크롤바 코드를 분석하게 끔 만드내요.
    //
    // 걍 InitScroll함수를 만들고 이것만 호출하면
    // 스크롤바 속성을 셋팅할수있게 할래요!!
    inline void InitScroll(int iPos, int iGap, int iMin, int iMax, int iXGab, int iYGab, int iPageSize, bool bStaticThumb)
    {
        m_iPos = iPos;              // 현재 Thumb의 위치입니다.
        m_iGap = iGap;              // Thumb이 한 번에 움직일때 단계수 입니다.(2이면 한번 스크롤될때 2단계씩 움직임)
        m_iMin = iMin;              // 최소단계입니다.
        m_iMax = iMax - iPageSize;              // 최대단계입니다.
        if ( m_iMax < m_iMin ) m_iMax = m_iMin;
        m_iXGab = iXGab;            // 스크롤바 윈도우 넓이로 부터 Thumb의 갭(넓이) 입니다.
        m_iYGab = iYGab;            // Thumb과 Up/Down버튼사이 간격입니다.
        m_uiPageSize = iPageSize;   // 코드분석을 대충해서 이건 뭐하려고 만들었는지 모르겠지만( Thumb크기 계산할때 사용하고 있습니다. )
        m_bStaticThumb = bStaticThumb;

        SetScrollGap( iGap );
        SetScrollRangeMin( iMin );
        SetScrollRangeMax( iMax );
        SetScrollPageSize( iPageSize );
        SetThumbXGab( iXGab );
        SetThumbYGab( iYGab );
        SetScrollPos( iPos );
    }

    void SetScrollPos( int iPos );
    void SetScrollGap( int iGap );
    void SetScrollRangeMin( int iMin );
    void SetScrollRangeMax( int iMax );
    void SetScrollPageSize( UINT uiPageSize );
    void SetThumbXGab( int iGab );
    void SetThumbYGab( int iGab );

    inline int  GetScrollPos( void )      const { return m_iPos; }
    inline int  GetScrollGap( void )      const { return m_iGap; }
    inline int  GetScrollRangeMin( void ) const { return m_iMin; }
    inline int  GetScrollRangeMax( void ) const { return m_iMax; }
    inline UINT GetScrollPageSize( void ) const { return m_uiPageSize; }
    DWORD GetButtonSize(void) const
    {
        //버튼은 정사각형이라고 처리함
        return m_pkBtnDown->GetWidth();
    }
    void ResizeBtn();
    void OnSetHeight( DWORD dwHeight );
    KD3DWnd* GetTargetWnd() const {
        if( m_pkTargetWnd == NULL )
            return GetParent();
        return m_pkTargetWnd;
    }
    void SetTargetWnd(KD3DWnd* val) { m_pkTargetWnd = val; }


protected:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
    DWORD CalcThumbSize( bool bEntire = false );
    DWORD CalcShaftSize( void );
    void OnBtnUp( DWORD dwCode );
    void OnBtnDown( DWORD dwCode );
    void OnBtnThumb( DWORD dwCode );

public:
    bool IsScrollUp()     {   return (m_iUpAndDown == 1);   }
    bool IsScrollDown()   {   return (m_iUpAndDown == 2);   }

protected:
    KD3DWnd*    m_pkBtnUp;
    KD3DWnd*    m_pkBtnDown;
    KD3DWnd*    m_pkBtnThumbTop;
    KD3DWnd*    m_pkBtnThumb;
    KD3DWnd*    m_pkBtnThumbBottom;
    KD3DWnd*    m_pkTargetWnd;

    int         m_iPos;
    int         m_iGap;
    int         m_iMin;
    int         m_iMax;
    int         m_iXGab;
    int         m_iYGab;

    int         m_iPressCount;

    UINT        m_uiPageSize;
    bool        m_bThumbDrag;
    float       m_fSavePosY;
    int         m_iUpAndDown;
    bool        m_bStaticThumb;

    typedef void (KD3DScrollbar::*HANDLER)( DWORD );
    std::map<std::string, HANDLER>  m_mapEvent;
};

DEFINE_WND_FACTORY( KD3DScrollbar );

#endif // _D3DSCROLLBAR_H_
