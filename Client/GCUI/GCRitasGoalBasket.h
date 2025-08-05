#pragma once

class KGCRitasGoalBasket : public KD3DWnd    // extends
                            ,public KActionListener   // implements
{
public:
	enum{
		OBJ_KIND_NUM = 3
	};
    DECLARE_CLASSNAME( KGCRitasGoalBasket );
    /// Default constructor
    KGCRitasGoalBasket( void );
    /// Default destructor
    virtual ~KGCRitasGoalBasket( void );
    /// Copy constructor
    KGCRitasGoalBasket( const KGCRitasGoalBasket& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCRitasGoalBasket& operator=( const KGCRitasGoalBasket& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

public:
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void OnCreate( void );


	void UpDateComboInfo();


private:
	KD3DWnd*		m_pkObjIcon[OBJ_KIND_NUM];
	KD3DStatic*		m_pkStaticObj[OBJ_KIND_NUM];

};

DEFINE_WND_FACTORY( KGCRitasGoalBasket );
DECLARE_WND_FACTORY( KGCRitasGoalBasket );
DECLARE_WND_FACTORY_NAME( KGCRitasGoalBasket );
