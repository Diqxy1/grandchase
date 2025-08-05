#pragma once

class KD3DSizingBox;
class KD3DSizingBoxBtn :	public KD3DWnd // extends
{
public:
	DECLARE_CLASSNAME( KD3DSizingBoxBtn );
	/// Default constructor
	KD3DSizingBoxBtn( void );
	/// Default destructor
	virtual ~KD3DSizingBoxBtn( void );
	/// Copy constructor
	KD3DSizingBoxBtn( const KD3DSizingBoxBtn& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KD3DSizingBoxBtn& operator=( const KD3DSizingBoxBtn& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void OnCreate( void );
    virtual void OnSetHeight( DWORD dwHeight );
    virtual void OnSetWidth( DWORD dwWidth );
    virtual void FrameMoveInEnabledState();
    bool OnWndModeChange();

    ED3DWndMode m_eChildMode;
	KSafeArray<KD3DSizingBox*,NUM_WINDOW_MODE> m_pkStateBox;

public: // ±aº≫ Aⓒμμ¿i ½º¿O ¾O´I¸ÞAI¼C ±a´E Aß°¡
    KD3DSizingBox* m_pkSwapDef;
    void SwapDef( bool bChange = true );
};

DEFINE_WND_FACTORY( KD3DSizingBoxBtn );
