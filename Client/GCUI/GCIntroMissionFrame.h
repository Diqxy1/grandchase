#pragma once

class KGCIntroMissionFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCIntroMissionFrame );
    /// Default constructor
    KGCIntroMissionFrame( void );
    /// Default destructor
    virtual ~KGCIntroMissionFrame( void );
    /// Copy constructor
    KGCIntroMissionFrame( const KGCIntroMissionFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCIntroMissionFrame& operator=( const KGCIntroMissionFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	enum{
		NUM_DRAW_IMG = 2
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void PostChildDraw();

	void SetMissionFrame(const int iCharType_ );
private:


	GCITEMID		m_GCItemID;
	

	KD3DWnd	*		m_pkImgTypeCash;
	KD3DWnd	*		m_pkImgTypeEvent;

	KD3DWnd	*		m_pkTitle;

	KD3DStatic*		m_pkStaticDesc;

	//CharType , MissionGoodsID
	std::map<int,GCITEMID> m_mapItemInfo;

};

DEFINE_WND_FACTORY( KGCIntroMissionFrame );
DECLARE_WND_FACTORY( KGCIntroMissionFrame );
DECLARE_WND_FACTORY_NAME( KGCIntroMissionFrame );
