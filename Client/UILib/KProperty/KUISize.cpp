// Precompile Header
#include "stdafx.h"

#include "KUISize.h"
#include "tinyxml.h"

//---------------------------------------------------------------------------
void KUISize::Load(TiXmlNode* pkNode)
{
    TiXmlElement* pkSizeElt = pkNode->FirstChildElement("Size");
    if (pkSizeElt)
    {
        KUIDimension2::Load(pkSizeElt);
    }
}
//---------------------------------------------------------------------------
