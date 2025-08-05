#pragma once

//#include <string>
//#include <sstream>
#include "KFootHold.h"

class CFootHold : public KFootHold<CFootHold>
{
public:
	CFootHold(void);
	~CFootHold(void);

	virtual void            SetPos(const float& fX, const float& fY);

public:
	std::string ToXString()
	{
		std::stringstream stm;
		stm << m_fX;
		return stm.str();
	}

	std::string ToYString()
	{
		std::stringstream stm;
		stm << m_fY;
		return stm.str();
	}
};