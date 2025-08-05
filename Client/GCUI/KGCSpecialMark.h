#ifndef _KGCSPECIALMARK_H_
#define _KGCSPECIALMARK_H_

//#include "../uifx/D3DWnd.h"

class KGCSpecialMark : public KD3DWnd 
{
public:
	DECLARE_CLASSNAME( KGCSpecialMark );
	/// Default constructor
	KGCSpecialMark( void );
	/// Default destructor
	virtual ~KGCSpecialMark( void );
	/// Copy constructor
	KGCSpecialMark( const KGCSpecialMark& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSpecialMark& operator=( const KGCSpecialMark& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
	void UpdateMark( int iIndex );

private:
	void CheckEvent( int iIndex );

protected:
	virtual void OnCreate( void );

private:
	KSafeArray<KD3DWnd*,4> m_ppMark;
};

DEFINE_WND_FACTORY( KGCSpecialMark );
DECLARE_WND_FACTORY( KGCSpecialMark );
DECLARE_WND_FACTORY_NAME( KGCSpecialMark );

#endif //_KGCSPECIALMARK_H_