#ifndef _REPLAY_H_
#define _REPLAY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RP_NONE         0
#define RP_RECORD       1
#define RP_VIEW         2
#define RP_PREPARE      3


#include "Replay_Head.h"

class Replay
{   
public:
    Replay();

	ERPSpeed	m_eReplay_Speed;
	int			m_iCurrState; 
    int			m_iFeed;
    HANDLE		m_hFile;
    WCHAR		m_pFileName[MAX_PATH];
	float		m_fVersion;    
	bool		m_bEndOfFile;
    
	std::vector<std::wstring>	m_vecFileList;
	int			m_iFileNum;

    // 한번 저장될 부분...    
    Replay_Head_Data	m_HeadData;

    // 프레임마다 저장될 부분...
    unsigned char		Num_Player[GC_FPS_LIMIT];
    unsigned char		Num_Monster[GC_FPS_LIMIT];
    
    Replay_PlayerFrameData		m_PlayerFrame[GC_FPS_LIMIT][MAX_PLAYER_NUM];
    Replay_MonsterFrameData		m_MonsterFrame[GC_FPS_LIMIT][MAX_MONSTER];
	Replay_ChatFrameData		m_ChatFrame[GC_FPS_LIMIT];
	Replay_ItemFrameData		m_ItemFrame[GC_FPS_LIMIT];
	Replay_PetSkillFrameData	m_PetSKillFrame[GC_FPS_LIMIT];
    Replay_TagMatchFrameData    m_TagMatchFrame[GC_FPS_LIMIT];
    Replay_GameBoardData        m_GameBoardFrame[GC_FPS_LIMIT];
	
	KSafeArray<char,MAX_PLAYER_NUM> m_iDeath; //아레나 배틀 사용
	KSafeArray<char,MAX_PLAYER_NUM> m_iKills; //아레나 배틀 사용

    void Set_PlayerData(int);
    void Get_PlayerData(int);

    bool Replay_Compress();
//  bool Replay_Uncompress();

    bool Save_HeadData();
    bool Load_HeadData();

    bool Load_FrameData();

    void GetList();
    //void SetFileInfo();
	int  GetFileNum(); //리플레이 파일 저장 할 때 현재 파일 갯수를 알기 위해 만든 함수..	

	void BlankToZero(WCHAR* );

	//item 데이터 저장 위해서 추가.. 
	Replay_ItemFrameData	m_ItemFrameTemp;
	bool					m_bItemDataEntered;
	void Set_ItemData( KGC_PID_EFFECT_ITEM* pkPacket , char Who );

	// 펫 데이터 저장
	Replay_PetSkillFrameData	m_PetSkillFrameTemp;
	bool						m_bUsePetSkill;
	void Set_UsePetSkill( KGC_PID_USEPETSKILL* pkPacket, char cWho );

    // 태그매치용
    Replay_TagMatchFrameData	m_TagMatchFrameTemp;
    bool						m_bTagCharacter;
    void Set_TagCharacter( KGC_PID_TAG_CHARACTER* pkPacket, char cWho );

    // 데스매치용
    Replay_GameBoardData	m_GameBoardTemp;
    bool					m_bUpdateGameboard;
    void Set_GameBoard( KGC_PID_GAME_BOARD_UPDATE* pkPacket );

    void MakePlayerFrameData( bool bReplay );
    void MakeMonsterFrameData( bool bReplay );
    void MakeReplayChatData( bool bReplay );
    void MakeItemData( bool bReplay );
    void MakePetSkillData( bool bReplay );
    void MakeTagData( bool bReplay );
    void MakeGameBoardData( bool bReplay );
    void SetPlayerFrame( Replay_PlayerFrameData& PlayerFrame, int iPlayerIndex );
    void SetMonsterFrame( Replay_MonsterFrameData& MonsterFrame, int iMonIndex, int RIndex );

    void SetDummyReplayHeadData();
    // [8/3/2007 breadceo] Dummy Replay Data 생성
    void MakeDummyData();
    void WriteStream( RakNet::BitStream& stmReplay );
    void SavePetSkillFrameToStream( RakNet::BitStream& stmReplay );
    void SaveTagMatchFrameToStream( RakNet::BitStream& stmReplay );
    void SaveGameBoardFrameToStream( RakNet::BitStream& stmReplay );
    void SaveItemFrameToStream( RakNet::BitStream& stmReplay );
    void SaveChatFrameToStream( RakNet::BitStream& stmReplay );
    void SaveMonFrameToStream( RakNet::BitStream& stmReplay );
    void SavePlayerFrameToStream( RakNet::BitStream& stmReplay );

    // 2007.8.22 milennium9 구코드를 완전히 지우고 Dummy를 이용한 리플레이로 간다!!!
    bool SaveReplayStart();
    void SaveReplayEnd();
    bool ReadHeaderDataFromCompressedFile( Replay_Head_Data& sHeader_ );
    bool ReadFrameDataStreamFromCompressedFile( RakNet::BitStream& stmFrameData );

	// 2008.04.12 milennium9 리플레이 API 제공을 위한 헤더를 추가한다.
	void	IgnoreReplayAPIHeaderRegion();
	ReplayAPI_Header	GetReplayAPI_Header();
	void	GetReplayAPI_UserInfoHeader( ReplayAPI_UserInfo& userInfo, int iPlayerIndex );
};
#endif // _REPLAY_H_
