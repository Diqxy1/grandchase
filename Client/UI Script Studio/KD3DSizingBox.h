#include "D3DWndClass.h"

class KD3DSizingBox
{
public:
	KD3DSizingBox();
	~KD3DSizingBox();

	static void SetSize( KD3DWndClass* pkWnd, int iWidth, int iHeight );
    static void SetDefTexCoords( KD3DWndClass* pkWnd, D3DXVECTOR4& coords );
    static void SetDefTextureIDx( KD3DWndClass* pkWnd, int iIDx );
};