#pragma once

//#include "../uifx/D3DWnd.h"

class KD3DEdit;
class KD3DStatic;
class KD3DColorPalette;

class KGCNeonsignMessage : public KD3DWnd
{
private: // 객체 복사가 일어나면 컴파일 타임에 오류를 일으킨다.
    KGCNeonsignMessage( const KGCNeonsignMessage& );
    const KGCNeonsignMessage& operator=( const KGCNeonsignMessage& );

public:
    DECLARE_CLASSNAME( KGCNeonsignMessage );
    KGCNeonsignMessage(void);
    virtual ~KGCNeonsignMessage(void);

    DWORD GetMessageColor() const;
    void GetMessage( OUT std::wstring& strMessage );
    bool IsValidate();
    bool SetBytes();
    void SetEditText( bool bDefault );

protected:
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void FrameMoveInEnabledState();

private:
    KD3DColorPalette*   m_pkColor;
    KD3DEdit*           m_pkMessage;
    KD3DStatic*         m_pkBytes;
    bool                m_bDefaultMsg;
};

DEFINE_WND_FACTORY( KGCNeonsignMessage );
DECLARE_WND_FACTORY( KGCNeonsignMessage );
DECLARE_WND_FACTORY_NAME( KGCNeonsignMessage );