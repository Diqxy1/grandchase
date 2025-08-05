#pragma once

//#include "../uifx/D3DWnd.h"
//#include <boost/noncopyable.hpp>
#include "GCBuddyTree.h"

class KD3DStatic;

class GCBuddyChild : public KD3DWnd,
                     public boost::noncopyable
{
public:
    DECLARE_CLASSNAME( GCBuddyChild );
    GCBuddyChild(void);
    virtual ~GCBuddyChild(void);

    // functions
    void SetData( const FRIEND_UI_DATA& kData, bool bChangePosition = true );
    void GetData( FRIEND_UI_DATA& kData ) { kData = m_kUIData; }

public:
	void SetGuildGradeMark( char cGrade_ );

protected:
    virtual void OnCreate();
    virtual void OnCreateComplete();
    virtual void PostChildDraw();

private:
    KD3DWnd* m_pkICon;
    KD3DStatic* m_pkName;
    FRIEND_UI_DATA m_kUIData;
    bool m_bFirst;
    KSafeArray<D3DXVECTOR2,2> m_vPos;
    KD3DWnd* m_pkNoEmoticon;
	KD3DStatic* m_pkLocation;

	//길드메신저 길드원 등급마크
private:	
	KD3DWnd* m_pkGuildGradeMaster;
	KD3DWnd* m_pkGuildGradeGeneral;

public:
	void SetTextColor( DWORD dwColor_ );
};

DEFINE_WND_FACTORY( GCBuddyChild );
DECLARE_WND_FACTORY( GCBuddyChild );
DECLARE_WND_FACTORY_NAME( GCBuddyChild );
