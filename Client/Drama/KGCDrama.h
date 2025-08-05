#pragma once
//#include <map>

//드라마는 자기가 할꺼 다 햇으면 return false 한다.
typedef bool (*DramaFunc)();
class MONSTER;

class KGCDrama
{
public:
    KGCDrama(void);    
    virtual ~KGCDrama(void);

    bool IsEnd(){ return ms_bDramaEnd; };
    
	bool Start( int iQuest_, int iDramaIndex_ );
	void End();
    void FrameMove();
    //void AddDrama( int iQuest_, int iStage_, int iStep_, DramaFunc Drama_);

    void AddDrama( int iQuest_, int iDramaIndex_, DramaFunc Drama_ );

private:
	static bool LuaDrama();
    
    static bool DramaQuest6_0();    //엘숲
    static bool DramaQuest6_1();    //엘숲 라이언 없애기
    static bool DramaQuest7_0();    //맹계

    
    static bool DramaQuestNeo2_4();
    static bool DramaQuestNeo2_5();
    static bool DramaQuestNeo2_6();
    static bool DramaQuestNeo2_7();

    
    static int ms_iFrame;
    static bool ms_bDramaEnd;
    static DramaFunc    ms_DramaFunc;

    // 퀘스트 id, 스테이지, dialog index 까지 3가지를 한 맵으로 처리 하고싶어서 -ㅅ-;
    //std::map< std::pair< int,int >, std::map< int, DramaFunc > > ms_mapDrama;

    // quest id, dialog index 만으로 커버할 수 잇을것 같다.
    std::map< std::pair< int , int > , DramaFunc > ms_mapDrama;
private:
	void BindLua();
	int GetFrame();
	void CreateParticleToMonster( char* strName, float offetX, float offsetY );
	void ChangeMonster( int fromIndex, int toIndex, int iMonType, bool bBoss = false );
	MONSTER* FindMonster( int iMonsterType );
	void SetDialogue( bool bRight, int iSpeeker, int iSpeech );
	DramaFunc GetDrama( int iQuest_, int iDramaIndex_ );
	D3DXVECTOR2 GetPos( int iMonsterIndex );

};
