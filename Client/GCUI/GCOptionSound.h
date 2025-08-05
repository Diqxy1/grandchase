#ifndef _KGCOPTIONSOUND_H_
#define _KGCOPTIONSOUND_H_

//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"

class KD3DStatic;

class KGCOptionSound : public KD3DWnd,
					   public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCOptionSound );
	/// Default constructor
	KGCOptionSound( void );
	/// Default destructor
	virtual ~KGCOptionSound( void );
	/// Copy constructor
	KGCOptionSound( const KGCOptionSound& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCOptionSound& operator=( const KGCOptionSound& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	float GetVolume(){ return m_fVolume; }
	void SetVolume( float fVolume );
	void SetEnable( bool bEnable, float fVolume );
	void SetToggle();

private:
	void SetPosition( float fPos );
	void SetStartGagePos();
	void SetStartRangePos();

protected:
	virtual void OnCreate( void );
	virtual void ActionPerformed( const KActionEvent& event );

private:
	float m_fVolume;//0x2E8
	float m_fRange;//0x2EC
	float m_fStartX;//0x2F0
	bool m_bDrag;//0x2F4

	KD3DStatic* m_pkTitle;//0x2F8
	KD3DWnd*	m_pkCheckBox;//0x2FC
	KD3DWnd*	m_pkCheck;//0x300
	KD3DWnd*	m_pkThumb;//0x304
	KD3DWnd*	m_pkGage;//0x308
	KD3DWnd*	m_pkGageLeft;//0x30C
	KD3DWnd*	m_pkGageCenter;//0x310
	KD3DWnd*	m_pkGageRight;//0x314
};

extern KGCOptionSound* g_pOptionSound;

DEFINE_WND_FACTORY( KGCOptionSound );
DECLARE_WND_FACTORY( KGCOptionSound );
DECLARE_WND_FACTORY_NAME( KGCOptionSound );

#endif //_KGCOPTIONSOUND_H_