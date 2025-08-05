#ifndef _GCGUILDSCENE_H_
#define _GCGUILDSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"

class KGCGuildInfo;
class KGCGuildMemo;
class KGCChatBox;
class KGCGuildUserList;

class KGCGuildScene : public KGCScene // extends
{
public:
	DECLARE_CLASSNAME( KGCGuildScene );
	/// Default constructor
	KGCGuildScene( void );
	/// Default destructor
	virtual ~KGCGuildScene( void );
	/// Copy constructor
	KGCGuildScene( const KGCGuildScene& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCGuildScene& operator=( const KGCGuildScene& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	void ActionPerformed( const KActionEvent& event );

protected:
	virtual void OnCreate( void );
	virtual void FrameMoveInEnabledState( void );
    virtual void OnEnterScene();

protected:

	KGCGuildInfo*		m_pkGuildInfo;
	KGCGuildUserList*	m_pkUserList;
	KGCChatBox*			m_pkChatBox;
	KGCGuildMemo*		m_pkGuildMemo;



};

DEFINE_WND_FACTORY( KGCGuildScene );
DECLARE_WND_FACTORY( KGCGuildScene );
DECLARE_WND_FACTORY_NAME( KGCGuildScene );

#endif // _GCGUILDSCENE_H_
