// Precompile Header
#include "stdafx.h"
#include "KUIModel.h"
#include "tinyxml.h"

//NiImplementRTTI(KUIModel, KUITexture);

//---------------------------------------------------------------------------
void KUIModel::Load(TiXmlNode* pkNode)
{
    KUITexture::Load(pkNode);
}
//---------------------------------------------------------------------------
void KUIModel::CreateTexture()
{
	/*
    NiFilename kFilename(m_strFilename.c_str());
    std::string strExt = kFilename.GetExt();
    std::transform(strExt.begin(), strExt.end(),
        strExt.begin(), toupper);

    // Model
    if (strExt == ".KFM")
    {
        m_spActorMgr = KActorManager::Create(m_strFilename.c_str());
        m_spModel = m_spActorMgr->GetNIFRoot();
    }
    else if (strExt == ".NIF")
    {
        m_spModel = KLoadNIF(m_strFilename);
    }
    else
    {
        KAssert(!"Not support file type");
    }
    m_spModel->UpdateEffects();
    m_spModel->UpdateProperties();
    //m_spModel->UpdateNodeBound();
    //m_spModel->UpdateWorldBound();
    m_spModel->Update(0.f);
    const NiBound& kBound = m_spModel->GetWorldBound();

    // Renderer
    m_spRenderer = _KAppHolder()->GetRenderer();
    
    // Camera
    NiFrustum kFrustum(-0.25f, 0.25, 0.1875f, -0.1875f, 1.0f, 1000.0f);
    NiRect<float> kPort(0.0f, 1.0f, 1.0f, 0.0f);
    m_spCamera = new NiCamera;
    m_spCamera->SetViewFrustum(kFrustum);
    m_spCamera->SetViewPort(kPort);
    m_spCamera->SetTranslate(kBound.GetCenter() + 
        -NiPoint3::UNIT_Y * kBound.GetRadius() * 10.f);
    m_spCamera->Update(0.f);
    m_spCamera->LookAtWorldPoint(NiPoint3::ZERO, NiPoint3::UNIT_Z);
        
    // Rendered Screen Texture
    NiTexture::FormatPrefs kPrefs;
    kPrefs.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;
    kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;
    kPrefs.m_eAlphaFmt = NiTexture::FormatPrefs::BINARY;

    m_spRenTexture = NiRenderedTexture::Create(
        512, 512, m_spRenderer, kPrefs);
    m_spScrTexture = new NiScreenTexture(m_spRenTexture);
    m_spScrTexture->AddNewScreenRect(
        GetWorldPos().x, GetWorldPos().y, 
        GetSize().x, GetSize().y, m_fTexLeft, m_fTexTop);

    m_spRenderTargetGroup = NiRenderTargetGroup::Create(
        m_spRenTexture->GetBuffer(), m_spRenderer, true, true);
    assert(m_spRenderTargetGroup);
	*/
}
//---------------------------------------------------------------------------
void KUIModel::Render()
{
    KUIFrame::Render();
	/*
    KBasedApp::KRenderTargetInfo kRTInfo;
    kRTInfo.m_pkCamera = m_spCamera;
    kRTInfo.m_pkRTGroup = m_spRenderTargetGroup;
    kRTInfo.m_pkTarget = m_spModel;
    _KAppHolder()->InsertRenderTargetInfo(kRTInfo);
    
    // Scr Texture
    m_spScrTexture->Draw(_KAppHolder()->GetRenderer());
	*/
}
//---------------------------------------------------------------------------
void KUIModel::Update()
{
    KUIFrame::Update();
	/*
    m_spActorMgr->UpdateData(0.f);
    m_spActorMgr->UpdateAnimation(0.f);
    m_spModel->Update(0.f);

    // Render our in scene screen texture
    m_spRenderer->BeginUsingRenderTargetGroup(m_spRenderTargetGroup, 
        NiRenderer::CLEAR_ALL);
    NiDrawScene(m_spCamera, m_spModel, _KAppHolder()->GetCuller());
    m_spRenderer->EndUsingRenderTargetGroup();
	*/
}
//---------------------------------------------------------------------------
bool KUIModel::UpdateInput()
{
    KUITexture::UpdateInput();

    return false;
}
//---------------------------------------------------------------------------
KUIWidget* KUIModel::CreateObject()
{
    return new KUIModel;
}
//---------------------------------------------------------------------------
void KUIModel::CopyMember(KUIWidget* pkWidget)
{
    KUITexture::CopyMember(pkWidget);
}
//---------------------------------------------------------------------------
