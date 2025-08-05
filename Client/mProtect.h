#define MAX_SH_EVENT 64
#define MAX_MH_EVENT 64

#include <windows.h>

struct SPEED_HACK_EVENT
{
	SPEED_HACK_EVENT();
	bool  m_Enable;
	int   m_CurrentCount;
    int   m_MaxBoundCount;
    int   m_NumSpeedHackFind;
    DWORD m_CheckTime;
	DWORD m_StartTime;		
};

struct MEMORY_HACK_EVENT
{
	MEMORY_HACK_EVENT();
	bool  m_Enable;
    unsigned char *m_Buffer;
	int  m_Size;
	int  m_SecurityCode;	
};

class Security
{			
private:
	SPEED_HACK_EVENT  m_SHEvent[MAX_SH_EVENT];
	MEMORY_HACK_EVENT m_MHEvent[MAX_MH_EVENT];

public:
    int  AddSpeedHackSecurity(DWORD CheckTime, int MaxBoundNumber);
	int  CheckSpeedHackSecurity(int ID);
	void ClearSpeedHackSecurity();


	int  AddMemoryHackSecurity(unsigned char *Buffer, int Size);
	void  OverWrightMemoryHackSecurity(int ID, unsigned char *Buffer, int Size);
    void SetValueMemoryHackSecurity(int ID);
    bool CheckMemoryHackSecurity(int ID);
	void ClearMemoryHackSecurity();
};