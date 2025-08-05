#ifndef _KGCOPTIONMUSIC_H_
#define _KGCOPTIONMUSIC_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCOptionMusic : public KD3DWnd,
					   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCOptionMusic );
	/// Default constructor
	KGCOptionMusic( void );
	/// Default destructor
	virtual ~KGCOptionMusic( void );
	/// Copy constructor
	KGCOptionMusic( const KGCOptionMusic& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCOptionMusic& operator=( const KGCOptionMusic& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	float GetVolume(){ return m_fVolume; }

	void SetVolume( float fVolume );
	void SetEnable(bool bEnable, float fVolume);
	void SetToggle();

private:
	void SetPosition( float fPos );
	void SetStartGagePos();
	void SetStartRangePos();

protected:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );

private:
	float m_fVolume;
	float m_fRange;
	float m_fStartX;
	bool m_bDrag;

	KD3DStatic* m_pkTitle;
	KD3DWnd*	m_pkCheckBox;
	KD3DWnd*	m_pkCheck;
	KD3DWnd*	m_pkThumb;
	KD3DWnd*	m_pkGage;//0x308
	KD3DWnd*	m_pkGageLeft;//0x30C
	KD3DWnd*	m_pkGageCenter;//0x310
	KD3DWnd*	m_pkGageRight;//0x314
};

extern KGCOptionMusic* g_pOptionMusic;

DEFINE_WND_FACTORY( KGCOptionMusic );
DECLARE_WND_FACTORY( KGCOptionMusic );
DECLARE_WND_FACTORY_NAME( KGCOptionMusic );

#endif //_KGCOPTIONMUSIC_H_