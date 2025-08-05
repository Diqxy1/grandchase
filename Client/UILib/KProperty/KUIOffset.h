#pragma once

#include "KUIDimension2.h"

class KUIWidget;
class TiXmlNode;
class TiXmlElement;

class KUIOffset : public KUIDimension2
{

public:
    enum EPosDir
    {
        eTopLeft,
        eTop,
        eTopRight,
        eRight,
        eBottomRight,
        eBottom,
        eBottomLeft,
        eLeft,
        eCenter,
        ePosdirMax
    };

public:
    KUIOffset(KUIWidget* pkOwner);

    void SetOwner(KUIWidget* pkOwner);

    virtual void Load(TiXmlNode* pkNode);

	virtual D3DXVECTOR2 GetDim();
    virtual void SetDim(D3DXVECTOR2 kDim);

    void LoadPointDir(TiXmlElement* pkElt);

protected:
    int m_iPointDir;
    int m_iReletivePointDir;

    KUIWidget* m_pkOwner;

};