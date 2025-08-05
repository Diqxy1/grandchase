#include "stdafx.h"
#include "GCCharacterLockBox.h"
#include ".\gcskilltreedlg.h"

IMPLEMENT_CLASSNAME( KGCCharacterLockBox );
IMPLEMENT_WND_FACTORY( KGCCharacterLockBox );
IMPLEMENT_WND_FACTORY_NAME( KGCCharacterLockBox, "gc_character_lock_box" );

KGCCharacterLockBox::KGCCharacterLockBox( void )
:	m_pkBack(NULL)
{
	m_mapLockedChar.clear();
	for( int i = 0 ; i < GC_CHAR_NUM; ++i)
	{
		m_mapLockedChar[i] = NULL;
		char strChar[128] = {0,};
		sprintf(strChar, "character_q_icon%d",i);
		LINK_CONTROL( strChar,          m_mapLockedChar[i] );
	}

	LINK_CONTROL("back",m_pkBack);
	
}

KGCCharacterLockBox::~KGCCharacterLockBox( void )
{
}

void KGCCharacterLockBox::ActionPerformed( const KActionEvent& event )
{

}

void KGCCharacterLockBox::OnCreate( void )
{
	std::map<int,KD3DWnd*>::iterator it = m_mapLockedChar.begin();
	for(;it != m_mapLockedChar.end();it++){
		if(it->second != NULL)
			it->second->InitState(true,true,this);
	}
	m_pkBack->InitState(true,false,NULL);

}


D3DXVECTOR2 KGCCharacterLockBox::InitBox(int es,DWORD dwParam1, DWORD dwParam2)
{
	std::map<int, KD3DWnd*>::iterator mIter = m_mapLockedChar.begin();
	float movex = 46.0f;
	float movey = 46.0f;
	float movenx = 6.0f;
	float moveny = 8.0f;

	//KLuaManager luaMgr;
	//GCFUNC::LoadLuaScript( luaMgr, "Dev.lua" );


	//LUA_GET_VALUE_DEF( "lock_move_x", movex, 0.0f );
	//LUA_GET_VALUE_DEF( "lock_move_y", movey, 0.0f );
	//LUA_GET_VALUE_DEF( "lock_move_nx", movenx, 0.0f );
	//LUA_GET_VALUE_DEF( "lock_move_ny", moveny, 0.0f );

	D3DXVECTOR2 vpos;

	for(;mIter != m_mapLockedChar.end();mIter++)
	{
		if(mIter->second != NULL)
		{
			mIter->second->ToggleRender(false);
		}
	}
	int iLockedChar = 0;
	
	mIter =  m_mapLockedChar.begin();
	KCharInfoVector& vecChar = g_kGlobalValue.m_kUserInfo.vecCharInfo;

	int iArrayChar[GC_CHAR_NUM] = {0};
	KCharInfoVector::iterator vIter = vecChar.begin();
	for(;vIter != vecChar.end();vIter++)
		iArrayChar[vIter->iCharType] =1;

	for(;mIter != m_mapLockedChar.end();mIter++)
	{
		if( mIter->second == NULL) continue;
		if( !g_kGlobalValue.IsOpenedChar(mIter->first)) continue;
		if((iArrayChar[mIter->first] == 0)
			|| (es == 1 &&g_pkUIScene->m_pkSkillTree->CheckSkillOpenChar( mIter->first ) == false)	)
		{
			vpos.x = ((movex * (iLockedChar%5))+movenx) ;
			vpos.y = ((movey * (iLockedChar/5))+moveny);

			mIter->second->SetWindowPos(vpos);
			mIter->second->ToggleRender(true);
			iLockedChar++;
		}
	}

	//int x = 0;
	//int y = 0;
	//int with = 0;
	//int height = 0;
	//
	//LUA_GET_VALUE_DEF( "lock_pos_x", x, 0 );
	//LUA_GET_VALUE_DEF( "lock_pos_y", y, 0 );
	//LUA_GET_VALUE_DEF( "lock_size_x", with, 0 );
	//LUA_GET_VALUE_DEF( "lock_size_y", height, 0 );
	int sizey = (iLockedChar-1)/5;


	sizey = 62+ (46 *sizey);
	m_pkBack->SetSize(243,sizey);

	D3DXVECTOR2 vec(243,static_cast<float>(sizey));
	return vec;


}
