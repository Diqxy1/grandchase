#include "mProtect.h"

SPEED_HACK_EVENT::SPEED_HACK_EVENT()
{
	m_Enable=false;
	m_MaxBoundCount=0;
	m_CurrentCount=0;
    m_CheckTime=0;
	m_StartTime=0;
    m_NumSpeedHackFind=0;
}

MEMORY_HACK_EVENT::MEMORY_HACK_EVENT()
{		
	m_Enable=false;
	m_Buffer=NULL;
	m_Size=0;
	m_SecurityCode=0;
}

// 스피드핵 컴퍼넌트를 클리어한다.
void Security::ClearSpeedHackSecurity()
{
    // 스피드핵 컴퍼넌트를 추가할 빈 슬롯을 찾는다.
    for(int i=0;i<MAX_SH_EVENT;i++)
    {
        m_SHEvent[i].m_Enable=false;
    }
}

// 스피드핵 컴퍼넌트를 추가한다.
int  Security::AddSpeedHackSecurity(DWORD CheckTime, int MaxBoundNumber)
{
	// 스피드핵 컴퍼넌트를 추가할 빈 슬롯을 찾는다.
	for(int i=0;i<MAX_SH_EVENT;i++)
	{
		if(!m_SHEvent[i].m_Enable) break;
	}
	if(i==MAX_SH_EVENT) return -1;
	m_SHEvent[i].m_Enable=true;
	m_SHEvent[i].m_MaxBoundCount=MaxBoundNumber;
    m_SHEvent[i].m_CurrentCount=0;
	m_SHEvent[i].m_NumSpeedHackFind=0;
    m_SHEvent[i].m_CheckTime=CheckTime;
	m_SHEvent[i].m_StartTime=timeGetTime();	
	return i;
}

// 스피드 해킹 당하지 않았나 검사한다.
int Security::CheckSpeedHackSecurity(int ID){
	// 작동중인 스피드핵 컴퍼넌트가 아니므로 안전함
	if(!m_SHEvent[ID].m_Enable) return false;
    
    m_SHEvent[ID].m_CurrentCount++;

    
    // 체크 시간이 지났다.
    if(m_SHEvent[ID].m_StartTime+m_SHEvent[ID].m_CheckTime<=timeGetTime()){
        // 예상된 패킷 수를 초과했음
        if(m_SHEvent[ID].m_CurrentCount>m_SHEvent[ID].m_MaxBoundCount)
        {    
            m_SHEvent[ID].m_NumSpeedHackFind++;            
        }else{
            m_SHEvent[ID].m_StartTime=timeGetTime();
            m_SHEvent[ID].m_CurrentCount=0;
        }
    }       
    return m_SHEvent[ID].m_NumSpeedHackFind;// 해킹 당하지 않았음
}



void Security::ClearMemoryHackSecurity()
{   
    // 스피드핵 컴퍼넌트를 추가할 빈 슬롯을 찾는다.
    for(int i=0;i<MAX_MH_EVENT;i++)
    {
        m_MHEvent[i].m_Enable=false;
    }
}

int  Security::AddMemoryHackSecurity(unsigned char *Buffer, int Size)
{
    // 스피드핵 컴퍼넌트를 추가할 빈 슬롯을 찾는다.
    for(int i=0;i<MAX_MH_EVENT;i++)
    {
        if(!m_MHEvent[i].m_Enable) break;
    }
    if(i==MAX_MH_EVENT) return -1;
    m_MHEvent[i].m_Enable=true;
    m_MHEvent[i].m_Buffer=Buffer;
    m_MHEvent[i].m_Size=Size;
    
    m_MHEvent[i].m_SecurityCode=0;
    for(int j=0;j<Size;j++){
        m_MHEvent[i].m_SecurityCode+=m_MHEvent[i].m_Buffer[j]%17;
    }   
    return i;    
}

void Security::OverWrightMemoryHackSecurity(int ID, unsigned char *Buffer, int Size)
{       
    if(ID<0) return;
    m_MHEvent[ID].m_Enable=true;
    m_MHEvent[ID].m_Buffer=Buffer;
    m_MHEvent[ID].m_Size=Size;
    
    m_MHEvent[ID].m_SecurityCode=0;
    for(int j=0;j<Size;j++){
        m_MHEvent[ID].m_SecurityCode+=m_MHEvent[ID].m_Buffer[j]%17;
    }           
}


// 값이 변화했고 그 값을 적용시키고 싶다면...
void Security::SetValueMemoryHackSecurity(int ID)
{
    m_MHEvent[ID].m_SecurityCode=0;
    for(int j=0;j<m_MHEvent[ID].m_Size;j++){
        m_MHEvent[ID].m_SecurityCode+=m_MHEvent[ID].m_Buffer[j]%17;
    }   
}

// 현재 ID의 컴퍼넌트가 해킹당하지 않았나 검사...
bool Security::CheckMemoryHackSecurity(int ID)
{
	//크래쉬리포트 처리
	return false;
	
    if(ID<0) return false;
    // 작동중인 메모리핵 컴퍼넌트가 아니므로 안전함
    if(!m_MHEvent[ID].m_Enable) return false;
    
    int SecurityCode=0;
    for(int j=0;j<m_MHEvent[ID].m_Size;j++){
        SecurityCode+=m_MHEvent[ID].m_Buffer[j]%17;
    }   
    
    if(m_MHEvent[ID].m_SecurityCode!=SecurityCode)
    {
        return true;// 해킹 당했음
    }
                               
    return false;// 해킹 당하지 않았음
}

