#ifndef _GCSTATEMATCH_H_
#define _GCSTATEMATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCState.h"
#include <WTypes.h>

class KGCStateMatch : public KGCState
{
public:
    KGCStateMatch( void );
    virtual ~KGCStateMatch( void );

    void SetChannelUID( DWORD dwChannelUID );
    DWORD GetChannelUID(){ return m_dwChannelUID; }

public:
    virtual bool OnInitBeforeStateChange( GAME_STATE eNowState_ );
    virtual bool OnDestroyBeforeStateChange();
    virtual bool FrameMove( float fElapsedTime );
    virtual bool Render();
    virtual bool OnInit();
    virtual void OnDestroy();
    virtual void OnSetupFSM();

private:
    int   m_bFirstInit;
    DWORD m_dwChannelUID;
};

#endif //_GCSTATEMATCH_H_