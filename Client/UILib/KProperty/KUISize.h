#pragma once

#include "KUIDimension2.h"

class TiXmlNode;

class KUISize : public KUIDimension2
{

public:
    virtual void Load(TiXmlNode* pkNode);

};