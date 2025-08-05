#pragma once
#include "GCSkillEnum.h"
//#include "../uifx/D3DWnd.h"
//#include "../uifx/ActionListener.h"
#include "GCDummySkillBox.h"
#include "GCSkillBoxGroup.h"
#include "GCSkillBoxMain.h"
#include "GCSkillBoxSub.h"
#include "GCSkillBox.h"

class KGCSkilltreeField:	public KD3DWnd,
							public KActionListener
{
public:
	DECLARE_CLASSNAME( KGCSkilltreeField );
	/// Default constructor
	KGCSkilltreeField( void );
	/// Default destructor
	virtual ~KGCSkilltreeField( void );
	/// Copy constructor
	KGCSkilltreeField( const KGCSkilltreeField& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCSkilltreeField& operator=( const KGCSkilltreeField& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	virtual void ActionPerformed( const KActionEvent& event);

	void Update( int iSetID );
	void ClearAllChild();
	void CreateMainSkillBox( EGCSkillGroup eGroup, float x, float y, int iTitleID, bool bCreateSub, int iCharIdx, int iPromotionIdx );
	void CreateSkillBoxGroup( EGCSkillGroup eGroup, float x, float y, DWORD dwWidth, DWORD dwHeight, int iTitleID );

	void HideSubSkillBox();
	void ShowSubSkillBox();

	void UnEquipMainSkills();
    KGCSkillBox* GetTrainableSkillBox();
    KGCSkillBoxSub* GetTrainableSubSkillBox();

	KD3DWnd*	GetMouseRolloverBox();
	EGCSkillTree	GetMouseRolloverSkillID();

    virtual void OnLoad();
	static void OnSetDescBoxInfo( EGCSkillTree eSkill, KD3DWnd* pWnd );

	void ClearParticle();
private:
	void OnClickSubSkillBox( KGCSkillBoxSub* pWnd );
	void OnClickMainSkillBox( KGCSkillBoxMain* pWnd );
	void OnClickSkillBox( KGCSkillBox* pWnd );

	void OnHoverSubSkillBox( KGCSkillBoxSub* pWnd );
	void OnHoverMainSkillBox( KGCSkillBoxMain* pWnd );
	void OnHoverSkillBox( KGCSkillBox* pWnd );
	

	void CreateSubSkillBox( EGCSkillTree eSkillID, float x, float y, std::set<int> iLineType );
	void CreateSkillBox( EGCSkillTree eSkillID, float x, float y );
	void CreateDummySkillBox( EGCSkillGroup eGroupID, float x, float y );
	

	KGCDummySkillBox* GetDummySkillBox();
	KGCSkillBoxGroup* GetSkillBoxGroup();
	KGCSkillBoxMain* GetSkillBoxMain();
	KGCSkillBoxSub* GetSubSkillBox();
	KGCSkillBox* GetSkillBox();

	template< class T, class P >
	void ClearChildSkillBox( T _container, P _queue )
	{
		T::iterator mit;
		for( mit = _container.begin(); mit != _container.end() ; ++mit )
		{
			mit->second->ToggleRender( false );
			_queue.push( mit->second );
		}
		_container.clear();
	}


	void ToggleRender_SubSkillBox( bool bRender );

private:
	int m_iCurSet;
	EGCSkillTree	m_eRolloveredSkillID;
	std::map< int, KGCSkillBoxMain* > m_mapBaseSkill;
	std::map< int, KGCSkillBoxSub* > m_mapSubSkillBox;
	std::map< EGCSkillTree, KGCSkillBox*	> m_mapSkillBox;
	std::list< KGCSkillBox* > m_listDummySkillBox;
	std::list< KGCSkillBoxGroup* > m_listSkillGroup;

	std::queue< KGCSkillBoxMain* > m_qBaseSkillPool;
	std::queue< KGCSkillBoxSub* > m_qSubSkillBoxPool;
	std::queue< KGCSkillBox* > m_qSkillBoxPool;
	std::queue< KGCDummySkillBox* > m_qDummySkillBoxPool;
	std::queue< KGCSkillBoxGroup* > m_qSkillGroupPool;

	void MakeTopChild( KD3DWnd* pWnd );

	CParticleEventSeqPTR m_pSque;
};

DEFINE_WND_FACTORY( KGCSkilltreeField );
DECLARE_WND_FACTORY( KGCSkilltreeField );
DECLARE_WND_FACTORY_NAME( KGCSkilltreeField );