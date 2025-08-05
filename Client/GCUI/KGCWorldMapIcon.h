#ifndef _KGCWORLDMAPICON_H_
#define _KGCWORLDMAPICON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KGCWorldMapObject.h"
//#include "../uifx/ActionListener.h"
//#include <vector>

class KGCNameTag;

class KGCWorldMapIcon : public KGCWorldMapObject,
                        public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCWorldMapIcon );
    /// Default constructor
    KGCWorldMapIcon( void );
    /// Default destructor
    virtual ~KGCWorldMapIcon( void );
    /// Copy constructor
    KGCWorldMapIcon( const KGCWorldMapIcon& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCWorldMapIcon& operator=( const KGCWorldMapIcon& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum{
        EICON_OVER_LEVEL = 0, //내 레벨이 던전레벨보다 3이상 높을때 
        EICON_NORMAL_LEVEL, //내 레벨이 던전레벨과 +-2차이일때
        EICON_UNDER_LEVEL, //내 레벨이 던전레벨보다 3이상 낮을때

        EICON_MAX
    };

    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );

    virtual void PreDraw( void );    
    virtual void PostChildDraw( void );


    void SetIndex( int iIndex ){ m_iIndex = iIndex; }
    int GetIndex(){ return m_iIndex; }
	void CheckImages();
    void SetMissionIcon( bool bRender );
    void SetEpicMissionIcon( int iconType );
    void SetNameTag(const int& iDun_);
    void EffectContinentBlink( bool bRender );

    void RenderDungeonIcon( bool bRenderDungeonIcon );
    bool GetCurrentIconActived();
    void OnHoverImg(bool bInBound);
    void AllOffEpicMissionIcon();

private:
    // 아이콘의 인덱스
    int m_iIndex;
    bool m_bCurrentIconActived;
    
    KD3DWnd* m_pkBaseImg;    // 기본 이미지
    KD3DWnd* m_pkActiveImg;  // 활성 이미지
    KD3DWnd* m_pkHoverImg;   // 마우스 오버 이미지
    KD3DWnd* m_pkName;   // 이름
    
    KD3DWnd* m_pkOutLine;
    KD3DWnd* m_pkHoverImg1;

    KD3DWnd* m_pkMissionIcon;
    KGCNameTag* m_pkNameTag;

    // m_pkHoverImg2가 깜빡거리는걸 제어 하기 위한 변수
    int m_iToggleFrame;

    D3DXVECTOR2 m_vIconNamePos; // 아이콘 이름의 기본 위치.. 맨첨에 저장해두고 변하지 않는다..
    int m_iDirection;
    bool m_bRenderEffect;

    KSafeArray<KD3DWnd*,EICON_MAX> m_pkEpicMissionIcon;

};

DEFINE_WND_FACTORY( KGCWorldMapIcon );
DECLARE_WND_FACTORY( KGCWorldMapIcon );
DECLARE_WND_FACTORY_NAME( KGCWorldMapIcon );

#endif // _KGCWORLDMAPICON_H_
