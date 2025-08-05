// Precompile Header
#include "stdafx.h"

#include "KUIImage.h"
#include "KUIFrame.h"
#include "KUIEditor.h"
//#include "KLuabinder.h"
#include "tinyxml.h"
#include "KUIManager.h"

//NiImplementRTTI(KUIImage, KUIWidget);

//---------------------------------------------------------------------------
void KUIImage::RegisterLuabind()
{
    lua_tinker::def("CreateUIImage", KUIImage::CreateDef);
}
//---------------------------------------------------------------------------
KUIImage* KUIImage::CreateDef(char* strFilename)
{
    KUIImage* pkImage = new KUIImage;
    //pkImage->m_fTop = 0;
    //pkImage->m_fLeft = 0;
    //pkImage->m_fWidth = 256;
    //pkImage->m_fHeight = 256;
    //
    //KUITexture* pkUITex = new KUITexture;
    //pkUITex->Create(strFilename);
    //pkImage->m_spBackground = pkUITex;

    //g_pUIManager->GetCurFrame()->AttachWidget(pkImage);

    return pkImage;
}
//---------------------------------------------------------------------------
void KUIImage::Load(KUIFrame* pkUIFrame, TiXmlNode* pkNode)
{
    //KUIWidget::Load(pkUIFrame, pkNode);
	KUIWidget::Load( pkNode);

    TiXmlNode* pkBGNode = pkNode->FirstChild("Background");
    if (pkBGNode)
    {
        //m_spBackground = KUITexture::Create(pkBGNode, this);
    }
}
//---------------------------------------------------------------------------
void KUIImage::Save(TiXmlNode* pkNode)
{
    TiXmlElement kImageElt("Image");

    TiXmlElement kBGElt("Background");
    //m_spBackground->Save(&kBGElt);

    kImageElt.InsertEndChild(kBGElt);

    //KUIWidget::Save(&kImageElt);
    pkNode->InsertEndChild(kImageElt);
}
//---------------------------------------------------------------------------
bool KUIImage::UpdateInput()
{
    return false;
}
//---------------------------------------------------------------------------
void KUIImage::Update()
{
    KUIWidget::Render();

    m_spBackground->Update();
}
//---------------------------------------------------------------------------
void KUIImage::Render()
{
    KUIWidget::Render();

    m_spBackground->Render();
}
//---------------------------------------------------------------------------
//void KUIImage::Move(NiPoint2 kPos)
//{
//    KUIWidget::Move(kPos);
//
//    m_spBackground->Move(kPos);
//}
////---------------------------------------------------------------------------
