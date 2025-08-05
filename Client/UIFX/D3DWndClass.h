#ifndef _D3DWNDCLASS_H_
#define _D3DWNDCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3dx9.h>
//#include <string>

class GCDeviceTexture;
class KD3DWnd;

class KD3DWndClass
{
public:
	int					iDeep;
    /*std::string         strWndName;
	std::string			strType;*/
#define MAX_WND_SIZE	50 //이게 50자 넘는 경우가 있긴 하겠음?	
	char				strWndName[MAX_WND_SIZE];
	char				strType[MAX_WND_SIZE];

    //KD3DWnd*          pParent;
	VOID*				pParent;

    DWORD               dwWidth;
    DWORD               dwHeight;

    D3DXVECTOR2         vCreatePos;
    D3DXVECTOR2         vWindowPos;
    D3DXVECTOR2         vDestroyPos;

    float               fMoveGap;
	bool				bAutoCreate;
	bool				bPredraw;
	bool				bRender;
	bool				bScene;		//Scene인지 아니지 안쓸테니 보구 지울꺼임..ㅡㅡ;
    

    int iDefTex;
    int iHovTex;
    int iSelTex;
    int iActTex;
    int iLocTex;

    D3DXVECTOR4         vDefTexCoords;
    D3DXVECTOR4         vHovTexCoords;
    D3DXVECTOR4         vSelTexCoords;
    D3DXVECTOR4         vActTexCoords;
    D3DXVECTOR4         vLocTexCoords;

    KD3DWndClass( void ) : vCreatePos( 0.0f, 0.0f ),
                           vWindowPos( 0.0f, 0.0f ),
                           vDestroyPos( 0.0f, 0.0f ),
                           vDefTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vHovTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vSelTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vActTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vLocTexCoords( 0.0f, 0.0f, 1.0f, 1.0f )
    {
		iDeep = -1;
        strcpy(strWndName,"");
		strcpy(strType,"");

        pParent  = NULL;
        dwWidth  = 0;
        dwHeight = 0;
        fMoveGap = 0.0f;        
        iDefTex = -1;
        iHovTex = -1;
        iSelTex = -1;
        iActTex = -1;
        iLocTex = -1;
		bAutoCreate = false;
		bPredraw = false;
		bRender = false;
		bScene	= false;
    }
    virtual ~KD3DWndClass( void )
    {
        // empty
    }
    KD3DWndClass( const KD3DWndClass& src )
    {
		iDeep = src.iDeep;
		strcpy(strWndName,src.strWndName);
		strcpy(strType,src.strType);
  
        pParent = src.pParent;

        dwWidth = src.dwWidth;
        dwHeight = src.dwHeight;

        vCreatePos = src.vCreatePos;
        vWindowPos = src.vWindowPos;
        vDestroyPos = src.vDestroyPos;

        fMoveGap = src.fMoveGap;
		bAutoCreate = src.bAutoCreate;
		bPredraw = src.bPredraw;
		bRender = src.bRender;
		bScene = src.bScene;
        

        iDefTex = src.iDefTex;
        iHovTex = src.iHovTex;
        iSelTex = src.iSelTex;
        iActTex = src.iActTex;
        iLocTex = src.iLocTex;

        vDefTexCoords = src.vDefTexCoords;
        vHovTexCoords = src.vHovTexCoords;
        vSelTexCoords = src.vSelTexCoords;
        vActTexCoords = src.vActTexCoords;
        vLocTexCoords = src.vLocTexCoords;
    }
    const KD3DWndClass& operator=( const KD3DWndClass& src )
    {
		iDeep = src.iDeep;
		strcpy(strWndName,src.strWndName);
		strcpy(strType,src.strType);

        pParent = src.pParent;

        dwWidth = src.dwWidth;
        dwHeight = src.dwHeight;

        vCreatePos = src.vCreatePos;
        vWindowPos = src.vWindowPos;
        vDestroyPos = src.vDestroyPos;

        fMoveGap = src.fMoveGap;
		bAutoCreate = src.bAutoCreate;
		bPredraw = src.bPredraw;
		bRender = src.bRender;
		bScene  = src.bScene;

        iDefTex = src.iDefTex;
        iHovTex = src.iHovTex;
        iSelTex = src.iSelTex;
        iActTex = src.iActTex;
        iLocTex = src.iLocTex;

        vDefTexCoords = src.vDefTexCoords;
        vHovTexCoords = src.vHovTexCoords;
        vSelTexCoords = src.vSelTexCoords;
        vActTexCoords = src.vActTexCoords;
        vLocTexCoords = src.vLocTexCoords;

        return *this;
    }
};

#endif // _D3DWNDCLASS_H_