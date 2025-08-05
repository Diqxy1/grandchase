#ifndef _KGCINVENTORYFULLMSG_H_
#define _KGCINVENTORYFULLMSG_H_

class KGCInventoryFullMsg : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME( KGCInventoryFullMsg );
    /// Default constructor
    KGCInventoryFullMsg( void );
    /// Default destructor
    virtual ~KGCInventoryFullMsg( void );
    /// Copy constructor
    KGCInventoryFullMsg( const KGCInventoryFullMsg& )
    { ASSERT( false && "Do not use copy constructor!" ); }
    /// Assignment operator
    const KGCInventoryFullMsg& operator=( const KGCInventoryFullMsg& )
    { ASSERT( false && "Do not use assignment operator!" ); return *this; }

    enum MSG_TYPE
    {
        INVEN_FULL = 0,
        CALENDAR_INFO,
        EVNET_CREATE_INFO,
        NEW_SKILL_MSG,
    };

protected:
    virtual void OnCreate( void );
    virtual void ActionPerformed( const KActionEvent& event );
    virtual void FrameMoveInEnabledState();

private:
    KD3DWnd*                    m_pkBackground;
    KD3DStatic*                 m_pkMsg;

    int							m_iToggleFrame;
    int                         m_iToggleCount;

    MSG_TYPE                    m_eMsg_Type;

    int m_iVal1;
    int m_iVal2;

    enum 
    {   
        INVENTORY_FULL_WARNING_COUNT = 10,
        ALERT_TOGGLE_FRAME_MAX = 25, 
        ALERT_TOGGLE_COUNT_MAX = 5,
    };
private:
    void SetMsgColor(BYTE byteAlpha_);
public:
    void UpdateMsg();
    void SetMsgType( MSG_TYPE eMsgType_ ) { m_eMsg_Type = eMsgType_; }
	void SetExtraValue1( int val1_ = 0 ) { m_iVal1 = val1_; }
	void SetExtraValue2( int val2_ = 0 ) { m_iVal2 = val2_; }
};

DEFINE_WND_FACTORY( KGCInventoryFullMsg );
DECLARE_WND_FACTORY( KGCInventoryFullMsg );
DECLARE_WND_FACTORY_NAME( KGCInventoryFullMsg );

#endif //_KGCInventoryFullMsg_H_