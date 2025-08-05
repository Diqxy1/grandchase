#pragma once
//#include "../uifx/D3DWnd.h"
//#include "../uifx/D3DSayBalloonBox.h"
//#include "../uifx/D3DStatic.h"
//#include "UserPacket.h"

#define LINE_PER_CHAR      10
#define LINE_PER_CHAR_INC  10
#define MAX_BALLOON_COUNT  100  //최대 말풍선 수

class KGCSayBalloonSquare : public KD3DWnd,
    public KActionListener
{
public:
    DECLARE_CLASSNAME(KGCSayBalloonSquare);
    /// Default constructor
    KGCSayBalloonSquare(void);
    /// Default destructor
    virtual ~KGCSayBalloonSquare(void);
    /// Copy constructor
    KGCSayBalloonSquare(const KGCSayBalloonSquare&)
    {
        ASSERT(false && "Do not use copy constructor!");
    }
    /// Assignment operator
    const KGCSayBalloonSquare& operator=(const KGCSayBalloonSquare&)
    {
        ASSERT(false && "Do not use assignment operator!"); return *this;
    }

    enum {
        GC_SP_QUARTER_LEFT,
        GC_SP_QUARTER_RIGHT,
    };

    enum EArrowPos {
        GCSB_ARROW_POS_AUTO,
        GCSB_ARROW_POS_LEFT,
        GCSB_ARROW_POS_RIGHT,
    };
protected:
    virtual void OnCreate();
    virtual void ActionPerformed(const KActionEvent& event);
private:
    void AddBallonControl(int iIndex, int iballoontype = 0);      //풍선 컨트롤 생성,  말풍선 타입추가
    void DisableBalloon(int iIndex);
    void MoveBalloonInfo(int iSIndex, int iTIndex);
    void SetMsg(std::wstring strMsg, int iIndex, DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor);        //메시지 설정
    void SetNPCMsg(std::wstring strMsg, int iIndex, DWORD dwUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor);        //메시지 설정
    void ChooseArrow(int iIndex, D3DXVECTOR2 vPos, EArrowPos eArrow = GCSB_ARROW_POS_AUTO, int BalloonDistance = 50);      //화살표 방향을 계산한다.
    int GetfoldSize(D3DXVECTOR2 vVec11, D3DXVECTOR2 vVec12, D3DXVECTOR2 vVec21, D3DXVECTOR2 vVec22);       //겹치키는 부분 면적 구하는 함수
    void RemoveSpaceChar(std::wstring& strMessage);
    void SetMsgColor(int iIndex, KChatData::ENUM_CHAT_TYPE eMsgType);

    bool CheckIndex(const int iIndex_);
public:

    void SetWidthHeightFromMsgSize(std::wstring strMsg, int iIndex);
    void SetBalloonPos(D3DXVECTOR2 vPos, DWORD dwUID, EArrowPos eArrow = GCSB_ARROW_POS_AUTO, int BalloonDistance = 50);       //위치 설정    
    void DeleteUserMsg(DWORD dwCharUID);     //유저가 나가거나 할때 메시지 지우는 역활
    void DeleteNPCMsg(DWORD dwNPCUID);
    void NewUserMsg(std::wstring strMsg, DWORD dwCharUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor);     //새로운 메시지가 도착 했을 경우    
    void NewNPCMsg(std::wstring strMsg, DWORD dwUID, KChatData::ENUM_CHAT_TYPE eMsgType, DWORD dwChatColor);
    void DeleteAllBalloon();      // 생성 객체 모두 종료
    void TimeCheck();
    std::vector<DWORD> GetUserUIDList() { return m_vecdwUserUID; }

    inline int KGCSayBalloonSquare::GetBalloonControlCount() const //풍선수 리턴
    {
        return m_iBalloonCount;
    }

protected:
    //[MAX_BALLOON_COUNT]
    std::vector<KD3DBalloonBox*>  m_vecpkMessageBalloon;
    std::vector<KD3DStatic*>      m_vecpkSayMsg;
    std::vector<double>           m_vecfElapsedTime; // 풍선 오브젝트의 생성 시간
    std::vector<DWORD>            m_vecdwUserUID;
    std::vector<DWORD>				m_vecdwNPCUID;
    std::vector<KChatData::ENUM_CHAT_TYPE> m_veceChatType;
    std::vector<DWORD>            m_vecdwColor;
    DWORD            m_dwSayWhoPos;


    //	int m_eSayWhoPos;
    int m_iBalloonCount; //풍선 오브젝트수
    int m_iUseBalloonCount; //떠 있는 풍선수
    int m_iOtherBalloonIndex;

public:
    void SetOffset(DWORD dwUID, float fOffsetX, float fOffsetY);
    D3DXVECTOR2 GetOffset(DWORD dwUID);
    std::map<DWORD, D3DXVECTOR2>     m_mapOffset;

    void SetArrowDist(DWORD dwUID, int iArrowDist);
    int GetArrowDist(DWORD dwUID);
    std::map<DWORD, int>     m_mapDist;
};

DEFINE_WND_FACTORY(KGCSayBalloonSquare);
DECLARE_WND_FACTORY(KGCSayBalloonSquare);
DECLARE_WND_FACTORY_NAME(KGCSayBalloonSquare);

