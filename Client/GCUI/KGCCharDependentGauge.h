#pragma once

class KGCCharDependentGauge :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCCharDependentGauge );
    /// Default constructor
    KGCCharDependentGauge( void );
    /// Default destructor
    virtual ~KGCCharDependentGauge( void );
    /// Copy constructor
    KGCCharDependentGauge( const KGCCharDependentGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCCharDependentGauge& operator=( const KGCCharDependentGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:
    void SetAmount( float fAmount );

    void SetBlink( bool bBlink )
    {
        m_bBlink = bBlink;
    };

    bool m_bBlink;
    bool m_bLevel;

protected:

    virtual void OnCreate( void );
    virtual void FrameMoveInEnabledState( void );
private:
    // �� - ���� ������
    D3DXVECTOR4 m_vBurnningGaugeCoords; // m_pkBurnningGauge �ؽ��� ��ǥ
    D3DXVECTOR4 m_vBurnningActiveCoords; // m_pkBurnningActive �ؽ��� ��ǥ
    D3DXVECTOR4 m_vBurnningActiveLightCoords; // m_pkBurnningGauge �ؽ��� ��ǥ
    D3DXVECTOR4 m_vBurnningGaugeLightCoords; // m_pkBurnningActive �ؽ��� ��ǥ
    KD3DWnd*    m_pkBurnningBar; // ���װ����� �� ���� UI
    KD3DWnd*    m_pkBurnningGauge; // ������ ��ư ������ �� UI
    KD3DWnd*    m_pkBurnningGaugeLight; // ������ ��ư ������ �� ��½�̴� ȿ���ֱ����� UI
    KD3DWnd*    m_pkBurnningGaugeLight2; // ������ ��ư ������ �� ��½�̴� ȿ���ֱ����� UI
    KD3DWnd*    m_pkBurnningGaugeLight3; // ������ ��ư ������ �� ��½�̴� ȿ���ֱ����� UI
    KD3DWnd*    m_pkBurnningActive; // ���׸�� �� �� UI
    KD3DWnd*    m_pkBurnningActiveLight; // ���׸�� �� �� ��½�̴� ȿ���ֱ����� UI	
    KD3DWnd*    m_pkBurnningActiveLight2; // ���׸�� �� �� ��½�̴� ȿ���ֱ����� UI	
    KD3DWnd*    m_pkBurnningActiveLight3; // ���׸�� �� �� ��½�̴� ȿ���ֱ����� UI	
    KD3DWnd*    m_pkBurnningLightLine; // ��½�� �� ��ü ������ �ܰ�����
    KD3DWnd*	m_pkBurnningBlackLine; // ������ �ܰ�����
    KD3DWnd*    m_pkBurnningEdge; 
    KD3DWnd*    m_pkBurnningIcon; // ���� ������ �Ϲ�
    KD3DWnd*    m_pkBurnningIconLight1; // ���� ������ ������ ��ư ������ �� ��½�̴� UI 1
    KD3DWnd*    m_pkBurnningIconLight2; // ���� ������ ������ ��ư ������ �� ��½�̴� UI 2
    KD3DWnd*    m_pkBurnningIconActive1; // ���� ������ ���׸�� �� �� ��½�̴� UI 1
    KD3DWnd*    m_pkBurnningIconActive2; // ���� ������ ���׸�� �� �� ��½�̴� UI 2

    
};


DEFINE_WND_FACTORY( KGCCharDependentGauge );
DECLARE_WND_FACTORY( KGCCharDependentGauge );
DECLARE_WND_FACTORY_NAME( KGCCharDependentGauge );

class KGCQPGauge :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( KGCQPGauge );
    /// Default constructor
    KGCQPGauge( void );
    /// Default destructor
    virtual ~KGCQPGauge( void );
    /// Copy constructor
    KGCQPGauge( const GCSPGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCQPGauge& operator=( const KGCQPGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum {
        MAX_QP_GAUGE = 5,
    };

public:
    void SetAmount( float fAmount );
    void InitQPGauge();

protected:
    virtual void OnCreate( void );
    virtual void PostChildDraw( void );

private:
    KSafeArray< KD3DWnd*, MAX_QP_GAUGE >        m_pkQPGaugeActive;
    KSafeArray< D3DXVECTOR4, MAX_QP_GAUGE >    m_vQPGaugeActiveCoords;
    KSafeArray< D3DXVECTOR4, MAX_QP_GAUGE >    m_vQPOriginalCoords;

    bool m_bInit;

};


DEFINE_WND_FACTORY( KGCQPGauge );
DECLARE_WND_FACTORY( KGCQPGauge );
DECLARE_WND_FACTORY_NAME( KGCQPGauge );

class KGCBloodGauge :
	public KD3DWnd
{
public:
	DECLARE_CLASSNAME( KGCBloodGauge );
	/// Default constructor
	KGCBloodGauge( void );
	/// Default destructor
	virtual ~KGCBloodGauge( void );
	/// Copy constructor
	KGCBloodGauge( const KGCBloodGauge& )
	{ ASSERT( false && "Do not use copy constructor!" ); }
	/// Assignment operator
	const KGCBloodGauge& operator=( const KGCBloodGauge& )
	{ ASSERT( false && "Do not use assignment operator!" ); return *this; }

	enum {
		MAX_BLOOD_GAUGE = 6,
	};

public:
	void SetAmount( float fAmount );
	void InitBloodGauge();

protected:
	virtual void OnCreate( void );
	virtual void PostChildDraw( void );

private:
	KSafeArray< KD3DWnd*, MAX_BLOOD_GAUGE >			m_pkBloodGaugeActive;//0x2E4
	KSafeArray< D3DXVECTOR4, MAX_BLOOD_GAUGE >		m_vBloodGaugeActiveCoords;
	KSafeArray< D3DXVECTOR4, MAX_BLOOD_GAUGE >		m_vBloodOriginalCoords;

	bool																			m_bInit;//0x3BC

};


DEFINE_WND_FACTORY( KGCBloodGauge );
DECLARE_WND_FACTORY( KGCBloodGauge );
DECLARE_WND_FACTORY_NAME( KGCBloodGauge );

class GCSPGauge :
    public KD3DWnd
{
public:
    DECLARE_CLASSNAME( GCSPGauge );
    /// Default constructor
    GCSPGauge( void );
    /// Default destructor
    virtual ~GCSPGauge( void );
    /// Copy constructor
    GCSPGauge( const GCSPGauge& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const GCSPGauge& operator=( const GCSPGauge& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }
public:   
	void InitBloodGauge();
    void InitQPGauge();
    void SetAmount( float fAmount );
    void SetBlink( bool bBlink )
    {
        m_pkJin->SetBlink( bBlink );
        m_pkSieg->SetBlink( bBlink );
        m_pkMari->SetBlink( bBlink );
        m_pkMarbleGauge->SetBlink( bBlink );
        m_pkMarbleGaugeRin->SetBlink( bBlink );
        m_pkMarbleGaugeZero->SetBlink( bBlink );
        m_pkMarbleGaugeRonan->SetBlink( bBlink );
#ifdef DECANEE
        m_pkMarbleGaugeDecanee->SetBlink( bBlink );
#endif
    };

    bool m_bBlink;

    virtual void ToggleRender( bool bRender );

protected:
    virtual void OnCreate( void );
    
private:
    KGCCharDependentGauge* m_pkJin;
    KGCCharDependentGauge* m_pkSieg;    
    KGCCharDependentGauge* m_pkMari;
    KGCCharDependentGauge* m_pkMarbleGauge;
    KGCCharDependentGauge* m_pkMarbleGaugeRin;
    KGCCharDependentGauge* m_pkMarbleGaugeZero;
    KGCCharDependentGauge* m_pkMarbleGaugeRonan;

#ifdef DECANEE
    KGCCharDependentGauge* m_pkMarbleGaugeDecanee;
#endif

    KGCQPGauge* m_pkQPGauge;   
	KGCBloodGauge* m_pkBloodGauge;
};


DEFINE_WND_FACTORY( GCSPGauge );
DECLARE_WND_FACTORY( GCSPGauge );
DECLARE_WND_FACTORY_NAME( GCSPGauge );