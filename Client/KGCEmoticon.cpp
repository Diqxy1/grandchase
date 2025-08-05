#include "stdafx.h"
#include ".\kgcemoticon.h"

// Kawan>
#include <iostream>
#include <algorithm>

//
//
#include "GCCameraOrtho.h"
//
//
//

ImplementSingleton(KGCEmoticonManager)

float KGCEmoticon::m_safTextureCoordList[4][4] =
{
    {   0.0f, 0.0f, 0.5f, 0.5f,    },
    {   0.5f, 0.0f, 1.0f, 0.5f,    },
    {   0.0f, 0.5f, 0.5f, 1.0f,    },
    {   0.5f, 0.5f, 1.0f, 1.0f,    },
};

GCDeviceTexture* KGCEmoticon::m_spBalloon = NULL;


KGCEmoticonManager::KGCEmoticonManager()
{
    if (g_pGCDeviceManager2)
        KGCEmoticon::m_spBalloon = g_pGCDeviceManager2->CreateTexture("EmoticonBase.dds");
}

KGCEmoticonManager::~KGCEmoticonManager()
{
    for (std::map< int, KGCEmoticon* >::iterator mit = m_mapEmoticon.begin(); mit != m_mapEmoticon.end(); mit++)
    {
        SAFE_DELETE(mit->second);
    }
    m_mapCommand.clear();

    for (std::vector<KGCEmoticonInstance*>::iterator vit = m_vecEmoticonInstance.begin(); vit != m_vecEmoticonInstance.end(); vit++)
    {
        SAFE_DELETE(*vit);
    }
    SAFE_DEVICE_RELEASE(KGCEmoticon::m_spBalloon);
}

void KGCEmoticonManager::LoadFromLuaScript(char* strFilename_)
{
    KLuaManager kLuaMgr;
    if (!GCFUNC::LoadLuaScript(kLuaMgr, strFilename_))
    {
        return;
    }
    kLuaMgr.BeginTable("Emoticon_Data");

    for (int i = 1; kLuaMgr.BeginTable(i) == S_OK; ++i)
    {
        KGCEmoticon* pEmoticon = new KGCEmoticon();
        pEmoticon->Load(&kLuaMgr);

        m_mapEmoticon[pEmoticon->m_iItemID] = pEmoticon;

        if (pEmoticon->m_iStrID != -1 && pEmoticon->m_iStrID < MAX_NUM_STRING)
        {
            std::wstring strCommand = g_pkStrLoader->GetString(pEmoticon->m_iStrID);
            std::transform(strCommand.begin(), strCommand.end(), strCommand.begin(), ::tolower); // Kawan> fix emoticon
            m_mapCommand[strCommand] = pEmoticon;
        }
        kLuaMgr.EndTable();
    }
    kLuaMgr.EndTable();

#ifdef EMOTICON_ON_BLOCKLIST
    kLuaMgr.BeginTable("CHASERS_ROOM_DATA");
    for (int i = 1; kLuaMgr.BeginTable(i) == S_OK; ++i)
    {
        KGCEmoticon* pEmoticon = new KGCEmoticon();
        pEmoticon->Load(&kLuaMgr);
        if (pEmoticon->m_strName.size() > 0)
        {
            std::wstringstream tmp;
            tmp << pEmoticon->m_strName.c_str();
            std::wstring str = tmp.str();
            std::transform(str.begin(), str.end(), str.begin(), ::tolower); // Kawan> emoticon
            m_mapCommand[str] = pEmoticon;
        }
        kLuaMgr.EndTable();
    }
    kLuaMgr.EndTable();
#endif
}

void KGCEmoticonManager::FrameMove()
{
    for (int i = m_vecEmoticonInstance.size() - 1; i >= 0; i--)
    {
        if (m_vecEmoticonInstance[i]->IsEnd())
        {
            SAFE_DELETE(m_vecEmoticonInstance[i]);
            m_vecEmoticonInstance.erase(m_vecEmoticonInstance.begin() + i);
        }

    }
    std::for_each(m_vecEmoticonInstance.begin(), m_vecEmoticonInstance.end(),
        boost::mem_fn(&KGCEmoticonInstance::FrameMove));
}

void KGCEmoticonManager::Render()
{
    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        g_pGCDeviceManager2->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        g_pGCDeviceManager2->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        std::for_each(m_vecEmoticonInstance.begin(), m_vecEmoticonInstance.end(),
            boost::mem_fn(&KGCEmoticonInstance::Render));

    }
    g_pGCDeviceManager2->PopState();
}

KGCEmoticonInstance* KGCEmoticonManager::CreateEmoticon(int iItemID_)
{
    std::map< int, KGCEmoticon* >::iterator mit = m_mapEmoticon.find(iItemID_);

    if (mit == m_mapEmoticon.end())
    {
        return NULL;
    }

    KGCEmoticonInstance* pEmoticonInstance = new KGCEmoticonInstance(iItemID_);

    m_vecEmoticonInstance.push_back(pEmoticonInstance);

    return pEmoticonInstance;
}

KGCEmoticon* KGCEmoticonManager::GetEmoticonData(int iItemID_)
{
    std::map< int, KGCEmoticon* >::iterator mit = m_mapEmoticon.find(iItemID_);

    if (mit == m_mapEmoticon.end())
        return NULL;


    if (!mit->second->m_pTexture)
    {
        mit->second->m_pTexture = g_pGCDeviceManager2->CreateTexture(mit->second->m_strTextureName);
    }

    return mit->second;
}

int KGCEmoticonManager::GetEmoticonDataUsable(int iIndex)
{
    // »ç¿ë °¡´ÉÇÑ ÀÌ¸ðÆ¼ÄÜÀ» ¸®ÅÏÇÑ´Ù!
    // n¹øÂ° ÀÌ¸ðÆ¼ÄÜÀ» ´Þ¶óÇÏ¸é ¾µ¼ö ÀÖ´Â ÀÌ¸ðÆ¼ÄÜÁß n¹øÂ° °ÍÀ» ÁØ´Ù~ ¾øÀ¸¸é false!
    std::map< int, KGCEmoticon* >::iterator mit = m_mapEmoticon.begin();
    for (int i = 0; mit != m_mapEmoticon.end(); ++mit)
    {
        // ¾µ ¼ö ÀÖ´Â ¾ÆÀÌÅÛÁß
        if (true == mit->second->m_bOpenedEmoticon)
        {
            // n¹øÂ° ÀÎ°¡?
            if (i == iIndex)
            {
                if (g_MyD3D->GetMyShortCutSlot()->IsSpecialSkillItem(mit->second->m_iItemID)) {
                    return -1;
                }
                else {
                    return mit->second->m_iItemID;
                }
            }
            ++i;
        }
    }

    return -1;
}

void KGCEmoticonManager::SetEmoticonOpen(const int iItemID_)
{
    std::map< int, KGCEmoticon* >::iterator mit = m_mapEmoticon.find(iItemID_);
    if (mit == m_mapEmoticon.end())
    {
        ASSERT(false && "ÇØ´ç ÀÌ¸ðÆ¼ÄÜÀÌ Á¸ÀçÇÏÁö ¾Ê½À´Ï´Ù?! ’o¹Ì?");
        return;
    }

    mit->second->m_bOpenedEmoticon = true;
}

KGCEmoticon* KGCEmoticonManager::GetEmoticonDataByCommand(std::wstring& strCommnd)
{
    /*std::map< std::wstring, KGCEmoticon* > x;
    x.clear();
    x = m_mapCommand;*/

    std::transform(strCommnd.begin(), strCommnd.end(), strCommnd.begin(), ::tolower); // Kawan> fix emoticon

    std::map< std::wstring, KGCEmoticon* >::iterator mit = m_mapCommand.find(strCommnd);

    if (mit == m_mapCommand.end())
        return NULL;

    if (false == mit->second->m_bOpenedEmoticon)
        return NULL;

    if (!mit->second->m_pTexture)
    {
        mit->second->m_pTexture = g_pGCDeviceManager2->CreateTexture(mit->second->m_strTextureName);
    }

    return mit->second;
}

KGCEmoticon::KGCEmoticon(void)
{
    m_pTexture = NULL;
    m_iCurrentSequence = 0;

    m_vSize = D3DXVECTOR2(1.0f, 1.0f);
    m_dwEffectTime = ::GetTickCount();
    m_bIsVisibleInList = true;
}

KGCEmoticon::~KGCEmoticon(void)
{
    if (m_pTexture)
    {
        SAFE_DEVICE_RELEASE(m_pTexture);
    }
}

void KGCEmoticon::Load(KLuaManager* pLuaMgr_)
{
    pLuaMgr_->GetValue("Name", m_strName);
    pLuaMgr_->GetValue("X_Size", m_vSize.x);
    pLuaMgr_->GetValue("Y_Size", m_vSize.y);
    m_vSize *= 1.3f;
    pLuaMgr_->GetValue("ItemID", m_iItemID);
    pLuaMgr_->GetValue("Directional", m_bDirectional);
    pLuaMgr_->GetValue("OpenedEmoticon", m_bOpenedEmoticon);
    pLuaMgr_->GetValue("VisibleList", m_bIsVisibleInList);


    m_iStrID = -1;
    pLuaMgr_->GetValue("Command", m_iStrID);
    if (FAILED(pLuaMgr_->GetValue("BackBalloon", m_bBalloon))) {
        m_bBalloon = true;
    }
    if (FAILED(pLuaMgr_->GetValue("BackBalloonPos_X", m_fBackBalloonPos_X))) {
        m_fBackBalloonPos_X = -2.0f;    //  ¿ø·¡ ¹ÚÇôÀÖ´ø »ó¼ö°ªÀ» Á¸ÁßÇØÁÜ
    }
    if (FAILED(pLuaMgr_->GetValue("BackBalloonPos_Y", m_fBackBalloonPos_Y))) {
        m_fBackBalloonPos_Y = -15.0f;   //  ¿ø·¡ ¹ÚÇôÀÖ´ø »ó¼ö°ªÀ» Á¸ÁßÇØÁÜ
    }
    if (m_pTexture)
    {
        m_pTexture->Release();
    }

    pLuaMgr_->GetValue("Texture", m_strTextureName);

    if (!GC_GLOBAL_DEFINE::bRealTimeLoadEtcTex)
    {
        m_pTexture = g_pGCDeviceManager2->CreateTexture(m_strTextureName);
    }

    m_vecEmoticonSequence.clear();
    if (pLuaMgr_->BeginTable("FrameSequence") == S_OK)
    {
        for (int iIndex = 1; pLuaMgr_->BeginTable(iIndex) == S_OK; ++iIndex)
        {
            int iSequence;
            int iDuration;
            pLuaMgr_->GetValue(1, iSequence);
            pLuaMgr_->GetValue(2, iDuration);

            for (int i = 0; i < iDuration; i++)
            {
                m_vecEmoticonSequence.push_back(static_cast<char>(iSequence));
            }

            pLuaMgr_->EndTable();
        }
        pLuaMgr_->EndTable();
    }
}

void KGCEmoticon::Render(D3DXMATRIX* mat, int iCurrentSequence, bool bIsRight_)
{
    if (iCurrentSequence >= static_cast<int> (m_vecEmoticonSequence.size())) {
        return;
    }

    int iIndex = m_vecEmoticonSequence[iCurrentSequence];

    if (m_bBalloon) {
        g_pGCDeviceManager2->DrawInWorld(m_spBalloon, mat,
            80.0f, 80.0f,
            0.0f, 0.0f, 1.0f, 1.0f,
            m_fBackBalloonPos_X, m_fBackBalloonPos_Y, 1.0f);
    }

    if (m_vecEmoticonSequence.empty() == true) {
        return;
    }

    if (!m_bDirectional && !bIsRight_)
    {
        g_pGCDeviceManager2->DrawInWorld(m_pTexture, mat,
            50.0f, 50.0f,
            m_safTextureCoordList[iIndex][2],
            m_safTextureCoordList[iIndex][1],
            m_safTextureCoordList[iIndex][0],
            m_safTextureCoordList[iIndex][3]);
    }
    else
    {
        g_pGCDeviceManager2->DrawInWorld(m_pTexture, mat,
            50.0f, 50.0f,
            m_safTextureCoordList[iIndex][0],
            m_safTextureCoordList[iIndex][1],
            m_safTextureCoordList[iIndex][2],
            m_safTextureCoordList[iIndex][3]);
    }
}

void KGCEmoticon::RenderForShop(float left, float top, float right, float bottom)
{

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

        if (m_iCurrentSequence >= (int)m_vecEmoticonSequence.size() || m_iCurrentSequence < 0)
        {
            m_iCurrentSequence = 0;
        }

        if (m_vecEmoticonSequence.empty() == true)
        {
            g_pGCDeviceManager2->PopState();
            return;
        }

        int iIndex = m_vecEmoticonSequence[m_iCurrentSequence];

        if (m_dwEffectTime + 5 <= ::GetTickCount())
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTexture,
                left + 10.f, top + 10.f, right - 10.f, bottom - 10.f,
                m_safTextureCoordList[iIndex][0],
                m_safTextureCoordList[iIndex][1],
                m_safTextureCoordList[iIndex][2],
                //m_safTextureCoordList[iIndex][3],false,0.1f,0xffffffff);
                m_safTextureCoordList[iIndex][3], false, 10.0f, 0xffffffff);

            m_dwEffectTime = ::GetTickCount();
            m_iCurrentSequence++;
        }
        else
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTexture,
                left + 10.f, top + 10.f, right - 10.f, bottom - 10.f,
                m_safTextureCoordList[iIndex][0],
                m_safTextureCoordList[iIndex][1],
                m_safTextureCoordList[iIndex][2],
                //m_safTextureCoordList[iIndex][3],false,0.1f,0xffffffff);
                m_safTextureCoordList[iIndex][3], false, 10.0f, 0xffffffff);

        }
    }
    g_pGCDeviceManager2->PopState();
}

void KGCEmoticon::RenderForResult(float left, float top, float right, float bottom, int iIndex_)
{
    // [3/17/2008] breadceo. bound check
    if (iIndex_ < 0 || (int)iIndex_ >= (int)m_vecEmoticonSequence.size())
        return;

    int iIndex = m_vecEmoticonSequence[iIndex_];

    float gapX;
    float gapY = (bottom - top) * 1.f / 5.f;

    if (m_bBalloon)
    {
        g_pGCDeviceManager2->DrawInScreen(m_spBalloon,
            left, top + gapY, right, bottom + gapY, 0, 0, 1, 1);
    }


    gapX = (right - left) / 6.f;
    gapY = (bottom - top) / 6.f;

    g_pGCDeviceManager2->DrawInScreen(m_pTexture,
        left + gapX, top + gapY, right - gapX, bottom - gapY,
        m_safTextureCoordList[iIndex][0],
        m_safTextureCoordList[iIndex][1],
        m_safTextureCoordList[iIndex][2],
        m_safTextureCoordList[iIndex][3]);
}

void KGCEmoticon::RenderForChatbox(float left, float top, float right, float bottom)
{
    //static int iCurrentSequence = 0;
    //static float fEffectTime = DXUtil_Timer( TIMER_GETAPPTIME );

    g_pGCDeviceManager2->PushState();
    {
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        g_pGCDeviceManager2->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


        if (m_iCurrentSequence >= (int)m_vecEmoticonSequence.size() || m_iCurrentSequence < 0)
        {
            m_iCurrentSequence = 0;
        }
        if (m_vecEmoticonSequence.empty() == true)
        {
            g_pGCDeviceManager2->PopState();
            return;
        }

        int iIndex = m_vecEmoticonSequence[m_iCurrentSequence];
        m_pTexture->SetDeviceTexture();

        if (m_dwEffectTime + 5 <= ::GetTickCount())
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTexture,
                left, top, right, bottom,
                m_safTextureCoordList[iIndex][0],
                m_safTextureCoordList[iIndex][1],
                m_safTextureCoordList[iIndex][2],
                m_safTextureCoordList[iIndex][3]);

            m_dwEffectTime = ::GetTickCount();
            m_iCurrentSequence++;
        }
        else
        {
            g_pGCDeviceManager2->DrawInScreen(m_pTexture,
                left, top, right, bottom,
                m_safTextureCoordList[iIndex][0],
                m_safTextureCoordList[iIndex][1],
                m_safTextureCoordList[iIndex][2],
                m_safTextureCoordList[iIndex][3]);
        }
    }
    g_pGCDeviceManager2->PopState();

}

KGCEmoticonInstance::KGCEmoticonInstance(int iEmoticonID_)
    : m_vPos(D3DXVECTOR2(0.0f, 0.0f)), m_iEmoticonID(iEmoticonID_), m_iFrame(0), m_bIsRight(false)
{
}


KGCEmoticonInstance::~KGCEmoticonInstance()
{

}

void KGCEmoticonInstance::SetFrame(int iFrame_)
{
    /* ASSERT( !( iFrame_ >= pOriginal->m_vecEmoticonSequence.size() || iFrame_ < 0) );

     if ( iFrame_ >= pOriginal->m_vecEmoticonSequence.size() || iFrame_ < 0)
     {
         m_iFrame = pOriginal->m_vecEmoticonSequence.size() - 1;
     }
     else
     {*/
    m_iFrame = iFrame_;
    //}
}

int KGCEmoticonInstance::GetFrame()
{
    return m_iFrame;
}

void KGCEmoticonInstance::FrameMove()
{
    m_iFrame++;
    //if ( m_iFrame >= pOriginal->m_vecEmoticonSequence.size() || m_iFrame < 0)
    //{
    //    //m_iFrame = pOriginal->m_vecEmoticonSequence.size() - 1;
    //    m_iFrame = 0;
    //}
    //m_vPos.x = g_MyD3D->GetMyPlayer()->m_pObject->GetWorldMat()->_41;
    //m_vPos.y = g_MyD3D->GetMyPlayer()->m_pObject->GetWorldMat()->_42;
}

void KGCEmoticonInstance::Render()
{
    //ÀÏ´Ü ´ëÃæ ±×·Áº¸ÀÚ.
    D3DXMATRIX matWorld, matTranslate, matScale;
    KGCEmoticon* pOriginal = SiKGCEmoticonManager()->GetEmoticonData(m_iEmoticonID);

    if (!pOriginal)
    {
        return;
    }

    if (m_bIsRight)
    {
        D3DXMatrixScaling(&matScale, pOriginal->m_vSize.x, pOriginal->m_vSize.y, 0.5f);
        D3DXMatrixTranslation(&matTranslate, m_vPos.x + 0.13f, m_vPos.y + 0.15f, 0.5f);
    }
    else
    {
        D3DXMatrixScaling(&matScale, -pOriginal->m_vSize.x, pOriginal->m_vSize.y, 0.5f);
        D3DXMatrixTranslation(&matTranslate, m_vPos.x - 0.13f, m_vPos.y + 0.15f, 0.5f);
    }

    D3DXMatrixMultiply(&matWorld, &matScale, &matTranslate);
    D3DXMatrixMultiply(&matWorld, &matWorld, &g_kCamera.m_matCamera);
    pOriginal->Render(&matWorld, m_iFrame, m_bIsRight);
}

void KGCEmoticonInstance::RenderForResult(float left, float top, float right, float bottom)
{
    KGCEmoticon* pOriginal = SiKGCEmoticonManager()->GetEmoticonData(m_iEmoticonID);
    if (NULL != pOriginal)
    {
        pOriginal->RenderForResult(left, top, right, bottom, m_iFrame);
    }
}

bool KGCEmoticonInstance::IsEnd()
{
    KGCEmoticon* pOriginal = SiKGCEmoticonManager()->GetEmoticonData(m_iEmoticonID);
    if (!pOriginal)
    {
        return true;
    }

    return m_iFrame >= ((int)pOriginal->m_vecEmoticonSequence.size() - 1);
}
