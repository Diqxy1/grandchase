#pragma once
#include "gcdevicetexture.h"
#include "GCDeviceFontTH.h" //TH부르면 그냥은 알아서 불림

#define	SEXEL_UNIT			64
#define	SEXEL_UNIT_SHIFT	6
#define	PIXEL_TO_SEXEL(p)	((p)>>SEXEL_UNIT_SHIFT)
#define	SEXEL_TO_PIXEL(p)	((p)<<SEXEL_UNIT_SHIFT)

struct STextureSection
{
	STextureSection()
	{
		memset(&rect,0,sizeof(rect));
		memset(&uv,0,sizeof(uv));
	}
	void Set(int left,int top,int width,int height,int tex_width=512,int tex_height=512);
	RECT rect;
	D3DXVECTOR4 uv;
};

class GCTmpTexture:public GCDevice,public GCBaseTexture
{
public:
	GCTmpTexture(LPDIRECT3DDEVICE9 d3ddevice, std::string strFileName);
	virtual ~GCTmpTexture(void);

	void OnResetDevice() {};
	void OnLostDevice() {};

	virtual void SetWithLoad(int stage =0);

	void CopyTo(LPDIRECT3DSURFACE9 dest_surface, RECT *dest_rect ,RECT *src_rect=NULL,D3DCOLOR col = 0);

	std::string GetFileName(){return m_strFileName;}
	D3DFORMAT GetFileFormat() { return m_dwFormat; }

	virtual void Release(){GCDevice::Release();}
protected:
	virtual bool Load();
	virtual void UnLoad();
	DWORD GetDeviceSize();

	std::string			m_strFileName;
	D3DFORMAT			m_dwFormat;
};

class INotUseCountable
{
public:
	INotUseCountable()
		:m_NotUseCount(0)
	{}

	struct LessNotUse
	{
		bool operator()(INotUseCountable *l, INotUseCountable *r)  const
		{
			return l->GetNotUseCount()<r->GetNotUseCount();
		}
	};

	virtual void Use(){m_NotUseCount--;}
	virtual void Update(){m_NotUseCount++;}
	virtual bool IsNotUse(int i){return m_NotUseCount>i;}
	virtual int GetNotUseCount(){return m_NotUseCount;}
protected:
	int m_NotUseCount;
};

class GCBuildTexture : // 요놈은 따로 빌드 안한다. 그냥 512*512 짜리에다가 쑤셔넣는다
	public GCBaseTexture, public INotUseCountable
{
public:
	GCBuildTexture(LPDIRECT3DDEVICE9 d3ddevice, D3DFORMAT format=D3DFMT_DXT1, bool usetext=true , DWORD width=512, DWORD height=512);
	virtual ~GCBuildTexture(void);

	bool AddTexture(std::string texture,STextureSection* section,bool precalculated_section=false);
	bool AddTexture(GCTmpTexture* src_Texture ,STextureSection* section,D3DXVECTOR4 * uv);
	void Release(){delete this;}

	void TextDraw(GCDeviceFontTH* font, int left,int top,std::wstring str,D3DCOLOR color = 0xffffffff,
		RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000 );
	void TextDraw(GCDeviceFontBase* font, int left,int top,std::wstring str,D3DCOLOR color = 0xffffffff,
		RECT* pRt = NULL, DWORD dwFlag = DT_LEFT|DT_TOP, D3DCOLOR colorOutLine = 0xff000000 );

	virtual void SetWithLoad(int stage=0);

	bool GetFreeTextureSection( STextureSection *section,DWORD width ,DWORD height);
	void SetFreeTextureSection( STextureSection *section);

	bool IsUseSection(int x,int y,int width,int height);
	void SetSection(int x,int y,int width,int height,bool use);

	bool IsUseBuildNickNameTex( void ) { return m_bBuildNickNameTex; }

protected:

	bool									m_bBuildNickNameTex;
	bool									*m_bUseTexture;

	DWORD									m_section_width;
	DWORD									m_section_height;
};
