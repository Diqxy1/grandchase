#ifndef _GCGUILDMEMO_H_
#define _GCGUILDMEMO_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
//#include "UserPacket.h"

class KD3DListBox;
class KD3DScrollbar;

class KGCGuildMemo : public KD3DWnd,        // extends
                     public KActionListener // implements
{
public:
    DECLARE_CLASSNAME( KGCGuildMemo );
    /// Default constructor
    KGCGuildMemo( void );
    /// Default destructor
    virtual ~KGCGuildMemo( void );
    /// Copy constructor
    KGCGuildMemo( const KGCGuildMemo& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCGuildMemo& operator=( const KGCGuildMemo& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    enum
    {
        MAX_NUM_MEMO = 5,
    };

public:
    void ResetScrollBar( void );
    void AddMemo( const std::wstring& strMemo );
    void MemoGuildNotice( std::map<int, KGuildNoticeInfo>& rmapInfo );
    bool FilterGuildMemoCommand( const std::wstring& strMessage );
    virtual void ActionPerformed( const KActionEvent& event );

protected:
    virtual void OnCreate( void );

protected:
    std::vector<KGuildNoticeInfo>   m_vecNoticeInfo;
    KD3DListBox*                    m_pkMemoList;
    KD3DScrollbar*                  m_pkScrollbar;
};

DEFINE_WND_FACTORY( KGCGuildMemo );
DECLARE_WND_FACTORY( KGCGuildMemo );
DECLARE_WND_FACTORY_NAME( KGCGuildMemo );

extern KGCGuildMemo* g_pkGuildMemo;

#endif // _GCGUILDMEMO_H_