// Precompile Header
#include "stdafx.h"

#include "KUITexture.h"
#include "KUIFontString.h"
#include "KUILayer.h"
#include "tinyxml.h"

//---------------------------------------------------------------------------
void KUILayer::Load(TiXmlNode* pkNode)
{
    KUIWidget::Creates<KUITexture>(this, pkNode, "Texture");
    KUIWidget::Creates<KUIFontString>(this, pkNode, "FontString");
}
//---------------------------------------------------------------------------
void KUILayer::Save(TiXmlNode* pkNode)
{
}
//---------------------------------------------------------------------------
void KUILayer::Render()
{
    KUIFrame::Render();
}
//---------------------------------------------------------------------------
void KUILayer::Update()
{
    KUIFrame::Update();
}
//---------------------------------------------------------------------------
KUIWidget* KUILayer::CreateObject()
{
    return new KUILayer;
}
//---------------------------------------------------------------------------
