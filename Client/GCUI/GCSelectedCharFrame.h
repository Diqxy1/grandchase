#pragma once

class KGCIntroMainFrame;
class KGCIntroMissionFrame;
class KGCIntroNickNameFrame;

class KGCSelectedCharFrame : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
    DECLARE_CLASSNAME( KGCSelectedCharFrame );
    /// Default constructor
    KGCSelectedCharFrame( void );
    /// Default destructor
    virtual ~KGCSelectedCharFrame( void );
    /// Copy constructor
    KGCSelectedCharFrame( const KGCSelectedCharFrame& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCSelectedCharFrame& operator=( const KGCSelectedCharFrame& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	//이 부분이 아이콘에만 영향을 주는게 아니라. 그려지는 폼에도 영향을 미치기 때문에...
	enum{
		OPEN_FREE = 0,
		OPEN_CASH = 1,
		OPEN_HAVE = 2,
		OPEN_EVENT = 3,
		OPEN_NEW = 4,
		OPEN_SALE = 5,
        OPEN_SEALED = 6,
	};
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState();
    virtual void OnCreateComplete();
	virtual void OnInitialize();

	void SetSelectedCharFrame(const int& iCharType_ );
	void OnClickBuyBtn( );
	void OnCloseMovie( );
    void CheckParticleTime();
    void ShowParticle();

private:
	GCITEMID FindCashItemID_();

    KD3DSizingBox*				m_pkBG;
	KGCIntroMainFrame*			m_pkIntroMainFrame;

	KD3DWnd*		m_pkBtnConfirm;

	KD3DWnd*		m_pkNormalForm;

	D3DXVECTOR2		m_vNomalPos;
	D3DXVECTOR2		m_vSmallPos;

	int m_iCharType;
    int m_iParticleTime;
    bool m_bBlinkOn;
    bool m_bClickCompleBtn;


};

DEFINE_WND_FACTORY( KGCSelectedCharFrame );
DECLARE_WND_FACTORY( KGCSelectedCharFrame );
DECLARE_WND_FACTORY_NAME( KGCSelectedCharFrame );
