#pragma once
#include <KNCSingleton.h>

#define LIMIT_NUM_LOADICON 500 //아이콘 100개이상 로드 하지 않는다.

class KGCIconImageManager
{
	DeclareSingleton( KGCIconImageManager );
public:
	KGCIconImageManager(void);
	~KGCIconImageManager(void);
	void InsertImageIcon( std::wstring strImageName, std::wstring strFileURL);
	void LoadIconImageFromHTTP(std::wstring strImageName);
	void LoadIconImageFromMassFile( std::wstring strImageName);
	void LoadIconImageFromRealPath( std::wstring strImageName,bool bGuildMark = false);
	void RemoveIconImage(std::wstring strImageName);
	GCImageIconTexture* GetTexture(std::wstring strImageName);
private:
    struct SIconImageInfo{
        GCImageIconTexture  *pTexture;
        time_t              tTime;
        SIconImageInfo(){pTexture = NULL; tTime = 0;}
    };
    std::map<std::wstring, SIconImageInfo>  m_mapIconImageInfo;

};
DefSingletonInline( KGCIconImageManager );