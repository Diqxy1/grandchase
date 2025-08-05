#pragma once

#include "../WOGMapTOol.h"
#include "ICommand.h"
#include "../LayerPart.h"

class CModifyLayerPartCmd
	: public ICommand
{
public:
	CModifyLayerPartCmd(void);
	virtual ~CModifyLayerPartCmd(void);

	virtual void Undo();
	virtual void Redo();

	// [8/20/2007 breadceo] Old Part 는 포인터로 관리
	// Old Part는 삭제시 지우지 않고
	// 실제 이 녀석을 지우려는 액션을 해도 지워지지 않고
	// 다른 ICommand 객체에 보존
	void SetOrigPart(CLayerPart* pPart) { m_pOrigPart = pPart; }
	void StoreOrigPart(CLayerPart* pPart);

private:
	CLayerPart* m_pOrigPart;
	CLayerPart* m_pBackPart;
};
