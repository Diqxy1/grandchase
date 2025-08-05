#ifndef _GCSERVERSCENE_H_
#define _GCSERVERSCENE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GCUI/GCScene.h"

class KGCMainFrame;
class KGCNewsFrame;
class KGCServerList;
class KGCMoveModel;

class KGCServerScene : public KGCScene // extends
{
public:
    DECLARE_CLASSNAME( KGCServerScene );
    /// Default constructor
    KGCServerScene( void );
    /// Default destructor
    virtual ~KGCServerScene( void );
    /// Copy constructor
    KGCServerScene( const KGCServerScene& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCServerScene& operator=( const KGCServerScene& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    virtual void InitScene() {}
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate();
    virtual void FrameMoveInEnabledState( void );
    virtual void OnInitialize();
    virtual void OnCreateComplete();

	virtual void EnterScene();
    virtual void OnLeaveScene();

    void UpdateServerList();

    bool MoveServer(DWORD dwServerUID);
    void OnEventBtnClick();

	void Add_NewsFrame_Particle(std::string seq,float x=0.0f,float y=0.0f, float z = 0.5f);
	void RenderEffect( bool bRender );
	void ClearEffect();
    void ClearEffect( std::string sParticleName );
	void CreateEffect();

	void OnReplay();
	void OnTutorial();
    void OnClickRange();
    void OnPatch();

	virtual HRESULT Render( void );

    void NewReLoad();

private:
   // KGCMoveModel*  m_pkBack_Model;
    KGCServerList* m_pkServerList;
//	KGCNewsFrame * m_pkNewsFrame;

	D3DXVECTOR4         m_vTextureCoord;
	D3DXVECTOR2			m_vWindowPos;

	KD3DWnd* m_pkBtnTutorial;
    KD3DWnd* m_pkBtnPatch;

    KD3DWnd* m_pkClickRange;

	std::vector<KD3DWnd*> m_vecMoveModel;

	std::vector< CParticleEventSeqPTR > m_vecServerSceneEffect;
};

DEFINE_WND_FACTORY( KGCServerScene );
DECLARE_WND_FACTORY( KGCServerScene );
DECLARE_WND_FACTORY_NAME( KGCServerScene );

#endif // _GCSERVERSCENE_H_
