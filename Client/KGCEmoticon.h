#pragma once

////#include <vector>
#include <d3dx9.h>
////#include <string>
////#include <map>
#include "KNCSingleton.h"

//class LPDIRECT3DTEXTURE9;

class GCDeviceTexture;
class GCObject;
class KGCEmoticon;
class KGCEmoticonInstance;
class KLuaManager;

class KGCEmoticonManager
{
    DeclareSingleton(KGCEmoticonManager);
public:
    KGCEmoticonManager(void);
    ~KGCEmoticonManager(void);

    void LoadFromLuaScript(char* strFilename_);

    void FrameMove();
    void Render();

    KGCEmoticonInstance* CreateEmoticon(int iItemID_);
    KGCEmoticon* GetEmoticonData(int iItemID_);
    KGCEmoticon* GetEmoticonDataByCommand(std::wstring& strCommnd); // Kawan> fix
    int GetEmoticonDataUsable(int iIndex);
    void SetEmoticonOpen(const int iItemID);

private:
    std::map< int, KGCEmoticon* > m_mapEmoticon;
    std::map< std::wstring, KGCEmoticon* > m_mapCommand;
    std::vector< KGCEmoticonInstance* > m_vecEmoticonInstance;
};

class KGCEmoticon
{
public:
    KGCEmoticon(void);
    ~KGCEmoticon(void);

    void Load(KLuaManager* pLuaMgr_);
    void Render(D3DXMATRIX* mat, int iIndex_, bool bIsRight_);
    void RenderForShop(float left, float top, float right, float bottom);
    void RenderForResult(float left, float top, float right, float bottom, int iIndex_);
    void RenderForChatbox(float left, float top, float right, float bottom);

    std::vector< char > m_vecEmoticonSequence;

    std::string m_strTextureName;
    GCDeviceTexture* m_pTexture;
    static GCDeviceTexture* m_spBalloon;
    std::string m_strName;
    bool m_bDirectional;
    bool m_bOpenedEmoticon;
    bool m_bIsVisibleInList;

    static float m_safTextureCoordList[4][4];

    D3DXVECTOR2 m_vSize;

    bool m_bBalloon;
    float m_fBackBalloonPos_X;
    float m_fBackBalloonPos_Y;
    int m_iItemID;
    int m_iCurrentSequence;
    DWORD m_dwEffectTime;
    int m_iStrID;
};

class KGCEmoticonInstance
{
public:
    //KGCEmoticonInstance( KGCEmoticon* pOriginal_ );
    KGCEmoticonInstance(int iEmoticonID_);
    ~KGCEmoticonInstance();

    void FrameMove();
    void SetFrame(int iFrame_);
    int GetFrame();
    void SetPos(D3DXVECTOR2* vPos) { m_vPos = *vPos; }
    void Render();
    void RenderForResult(float left, float top, float right, float bottom);
    bool IsEnd();
    void SetIsRight(bool bRight_) { m_bIsRight = bRight_; }

protected:

private:
    KGCEmoticonInstance() {}

    int m_iFrame;
    bool m_bIsRight;
    //GCObject* m_pTarget;
    D3DXVECTOR2 m_vPos;
    //KGCEmoticon* m_pOriginal;
    int m_iEmoticonID;
};

DefSingletonInline(KGCEmoticonManager);
