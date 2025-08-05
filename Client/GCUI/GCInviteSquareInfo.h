#pragma once

//#include "../uifx/D3DWnd.h"
//#include "../Square/GCSquare.h"

class KD3DSizingBox;
class KD3DStatic;

class KGCInviteSquareInfo : public KD3DWnd
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCInviteSquareInfo( const KGCInviteSquareInfo& );
    const KGCInviteSquareInfo& operator=( const KGCInviteSquareInfo& );

public:
    DECLARE_CLASSNAME( KGCInviteSquareInfo );
    KGCInviteSquareInfo(void);
    virtual ~KGCInviteSquareInfo(void);

    // getter/setter
    bool IsEmpty() const { return m_bEmpty; }

    void InitSquareInfo();
    void SetSquareInfo( const KSquareInfo& kInfo );
    void SetArrow( bool bArrow );
    ED3DWndMode GetBackMode() const;
    void SetBackMode( ED3DWndMode eMode );
    DWORD GetSquareID() const { return m_kInfo.m_dwID; }
    const std::wstring& GetSquareName() const { return m_kInfo.m_strName; }

protected:
    virtual void OnCreate( void );

private:
    bool m_bEmpty;
    KD3DSizingBox*  m_pkBack;
    KD3DStatic*     m_pkInfo;
    KD3DWnd*        m_pkArrow;
    KSquareInfo     m_kInfo;
};

DEFINE_WND_FACTORY( KGCInviteSquareInfo );
DECLARE_WND_FACTORY( KGCInviteSquareInfo );
DECLARE_WND_FACTORY_NAME( KGCInviteSquareInfo );