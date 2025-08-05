#ifndef _D3D_WND_CLASS_H_
#define _D3D_WND_CLASS_H_


//#include <string>
//#include <vector>
#include <d3dx9math.h>

class KD3DWnd;
class KWndDataPackage;

struct SWndTexUV
{
	float m_fu1, m_fv1, m_fu2, m_fv2;

	SWndTexUV()
		: m_fu1( 0.0f ), m_fu2( 0.0f ), m_fv1( 0.0f ), m_fv2( 0.0f )
	{

	}

	SWndTexUV( float u1, float v1, float u2, float v2 )
		: m_fu1( u1 ), m_fu2( u2 ), m_fv1( v1 ), m_fv2( v2 )
	{

	}

	SWndTexUV& operator=( SWndTexUV& arg )
	{
		m_fu1 = arg.m_fu1;
		m_fu2 = arg.m_fu2;
		m_fv1 = arg.m_fv1;
		m_fv2 = arg.m_fv2;
	}

	SWndTexUV& operator=( D3DXVECTOR4& arg )
	{
		m_fu1 = arg.x;
		m_fv1 = arg.y;
		m_fu2 = arg.z;
		m_fv2 = arg.w;
	}

	void operator() ( float u1, float v1, float u2, float v2 )
	{
		m_fu1 = u1;
		m_fu1 = u2;
		m_fv1 = v1;
		m_fv2 = v2;
	}
};

struct KD3DWndClass
{
    friend class KWndDataPackage;
public:
    KD3DWndClass( void ) : vCreatePos( 0.0f, 0.0f ),
                           vWindowPos( 0.0f, 0.0f ),
                           vDestroyPos( 0.0f, 0.0f ),
                           vDefTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vHovTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vSelTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vActTexCoords( 0.0f, 0.0f, 1.0f, 1.0f ),
                           vLocTexCoords( 0.0f, 0.0f, 1.0f, 1.0f )
    {
        strWndName.erase();
        strType.erase();
		strMotion.erase();
		strAlpha.erase();

        pParent  = NULL;
        dwWidth  = 0;
        dwHeight = 0;
        fMoveGap = 0.0f;

		bVisible = TRUE;
		bPixelValue = TRUE;
		bAutoCreate = TRUE;
		bPredraw = FALSE;

		iDefTextureIDx = -1;
		iActTextureIDx = -1;
		iHovTextureIDx = -1;
		iSelTextureIDx = -1;
		iLocTextureIDx = -1;

        nSortIndex = INT_MAX;
		
		vecChild.clear();
    }
    KD3DWndClass( const KD3DWndClass& src )
    {
		DataCopyExceptPointer( src );
        pParent = src.pParent;
		vecChild	= src.vecChild;
    }
    const KD3DWndClass& operator=( const KD3DWndClass& src )
    {
		DataCopyExceptPointer( src );
		pParent = src.pParent;
		vecChild	= src.vecChild;
        return *this;
    }

	virtual ~KD3DWndClass( void )
	{
	}

protected:
	int				bVisible;
	int				bPixelValue;
	int				bPredraw;
	int				bAutoCreate;

	std::string         strWndName;
	std::string         strType;
	std::string			strToolTip;
	std::string			strMotion;
	std::string			strAlpha;

	KD3DWndClass*       pParent;

	int               dwWidth;
	int               dwHeight;

	D3DXVECTOR2         vCreatePos;
	D3DXVECTOR2         vWindowPos;
	D3DXVECTOR2         vDestroyPos;

	float               fMoveGap;

	int					iDefTextureIDx;
	int					iHovTextureIDx;
	int					iSelTextureIDx;
	int					iActTextureIDx;
	int					iLocTextureIDx;

	D3DXVECTOR4         vDefTexCoords;
	D3DXVECTOR4         vHovTexCoords;
	D3DXVECTOR4         vSelTexCoords;
	D3DXVECTOR4         vActTexCoords;
	D3DXVECTOR4         vLocTexCoords;


    int                 nSortIndex;

	std::vector<KD3DWndClass*>	vecChild;

public:

    virtual int IsVisible() const { return bVisible; }
    virtual int IsPixelValue() const { return bPixelValue; }
    virtual int IsPreDraw() const { return bPredraw; }
    virtual int IsAutoCreate() const { return bAutoCreate; }

    virtual void SetVisible( int ivalue ) { bVisible = ivalue; }
    virtual void SetPixelValue( int ivalue ) { bPixelValue = ivalue; }
    virtual void SetPreDraw( int ivalue ) { bPredraw = ivalue; }
    virtual void SetAutoCreate( int ivalue ) { bAutoCreate = ivalue; }

    virtual std::string GetWndName() const { return strWndName; }
    virtual std::string GetWndType() const { return strType; }
    virtual std::string GetToolTip() const { return strToolTip; }
    virtual std::string GetMotion() const { return strMotion; }
    virtual std::string GetAlpha() const { return strAlpha; }

    virtual void SetWndName( const std::string& str ) { strWndName = str; }
    virtual void SetWndType( const std::string& str ) { strType = str; }
    virtual void SetToolTip( const std::string& str ) { strToolTip = str; }
    virtual void SetMotion( const std::string& str ) { strMotion = str; }
    virtual void SetAlpha( const std::string& str ) { strAlpha = str; }

    virtual KD3DWndClass*   GetParent() const { return pParent; }
    virtual void SetParent( KD3DWndClass* pWnd ) { pParent = pWnd; }

    virtual int GetWidth() const { return dwWidth; }
    virtual int GetHeight() const { return dwHeight; }

    virtual void SetWidth( int iWidth ) { dwWidth = iWidth; }
    virtual void SetHeight( int iHeight) { dwHeight = iHeight; }

    virtual D3DXVECTOR2 GetCreatedPos() const { return vCreatePos; }
    virtual D3DXVECTOR2 GetWindowPos() const { return vWindowPos; }
    virtual D3DXVECTOR2 GetDestroyPos() const { return vDestroyPos; }
    virtual void SetCreatedPos( D3DXVECTOR2& pos ) { vCreatePos = pos; }
    virtual void SetWindowPos( D3DXVECTOR2& pos ) { vWindowPos = pos; }
    virtual void SetDestroyPos( D3DXVECTOR2& pos ) { vDestroyPos = pos; }

    virtual float GetMoveGap() const { return fMoveGap; }
    virtual void SetMoveGap( float fValue ) { fMoveGap = fValue; }

    virtual int GetDefTextureIDx() const { return iDefTextureIDx; }
    virtual int GetHovTextureIDx() const { return iHovTextureIDx; }
    virtual int GetSelTextureIDx() const { return iSelTextureIDx; }
    virtual int GetActTextureIDx() const { return iActTextureIDx; }
    virtual int GetLocTextureIDx() const { return iLocTextureIDx; }

    virtual void SetDefTextureIDx( int iIDx, bool bInit = false );
    virtual void SetHovTextureIDx( int iIDx, bool bInit = false ) { iHovTextureIDx = iIDx; }
    virtual void SetSelTextureIDx( int iIDx, bool bInit = false ) { iSelTextureIDx = iIDx; }
    virtual void SetActTextureIDx( int iIDx, bool bInit = false ) { iActTextureIDx = iIDx; }
    virtual void SetLocTextureIDx( int iIDx, bool bInit = false ) { iLocTextureIDx = iIDx; }

    virtual D3DXVECTOR4 GetDefTexCoords() const { return  vDefTexCoords; }
    virtual D3DXVECTOR4 GetHovTexCoords() const { return  vHovTexCoords; }
    virtual D3DXVECTOR4 GetSelTexCoords() const { return  vSelTexCoords; }
    virtual D3DXVECTOR4 GetActTexCoords() const { return  vActTexCoords; }
    virtual D3DXVECTOR4 GetLocTexCoords() const { return  vLocTexCoords; }

    virtual void SetDefTexCoords( D3DXVECTOR4& coords, bool bInit = false );
    virtual void SetHovTexCoords( D3DXVECTOR4& coords, bool bInit = false ) { vHovTexCoords = coords; }
    virtual void SetSelTexCoords( D3DXVECTOR4& coords, bool bInit = false ) { vSelTexCoords = coords; }
    virtual void SetActTexCoords( D3DXVECTOR4& coords, bool bInit = false ) { vActTexCoords = coords; }
    virtual void SetLocTexCoords( D3DXVECTOR4& coords, bool bInit = false ) { vLocTexCoords = coords; }

    virtual std::vector<KD3DWndClass*>::size_type GetChildNum() { return vecChild.size(); }
    virtual const std::vector<KD3DWndClass*>& GetChildContainer() const { return vecChild; }
    virtual void ClearChildContainer() { vecChild.clear(); };
    virtual void AddChildWnd( KD3DWndClass* pWnd ) { vecChild.push_back( pWnd ); }
	virtual void AddChildWnd( KD3DWndClass* pWnd, UINT nIndex ); // 2008. 08. 04
    virtual void RemoveChildWnd( int iIndex ) { vecChild.erase( vecChild.begin() + iIndex ); }
    virtual void SwapChildPos( int iLeft, int iRight ) {
        std::swap(vecChild[iLeft],vecChild[iRight]);
    }
    virtual void SetChild( int iIndex, KD3DWndClass* pkWnd ) { vecChild[iIndex] = pkWnd; }

    virtual void SetSortIndex( int _nSortIndex ) { nSortIndex = _nSortIndex; }
    virtual int GetSortIndex() const { return nSortIndex; }
    void CalcSortIndex();
    void SortByName();
    void SortByIndex();

    KD3DWndClass* FindWndWithName( std::string strWndName )
    {
        std::vector< KD3DWndClass* >::iterator vit;

        for( vit = vecChild.begin() ; vit != vecChild.end() ; ++vit )
        {
            if( (*vit)->GetWndName().compare( strWndName ) == 0 )
                return *vit;
        }

        return NULL;
    }

    KD3DWndClass* GetChild( int iIndex ) const {
        if( iIndex < 0 || iIndex >= (int)vecChild.size() )
            return NULL;
        return vecChild[iIndex];
    }

	void SetSize(int iWidth, int iHeight );

    void DeleteChildren()
    {
        for(UINT i = 0 ; i < vecChild.size(); i++)
        {
            if( vecChild[i] )
            {
                vecChild[i]->DeleteChildren();
                delete vecChild[i];
            }
        }
    }

	//------------------------------------------------------------------------
	/*20091019 이동석*/
	void DataCopyExceptPointer( const KD3DWndClass& src )
	{
		bVisible = src.bVisible;
		bPixelValue = src.bPixelValue;
		bAutoCreate = src.bAutoCreate;
		bPredraw = src.bPredraw;

		strWndName = src.strWndName;
		strType = src.strType;
		strAlpha = src.strAlpha;
		strMotion = src.strMotion;

		dwWidth = src.dwWidth;
		dwHeight = src.dwHeight;

		vCreatePos = src.vCreatePos;
		vWindowPos = src.vWindowPos;
		vDestroyPos = src.vDestroyPos;

		fMoveGap = src.fMoveGap;

		iDefTextureIDx = src.iDefTextureIDx;
		iHovTextureIDx = src.iHovTextureIDx;
		iSelTextureIDx = src.iSelTextureIDx;
		iActTextureIDx = src.iActTextureIDx;
		iLocTextureIDx = src.iLocTextureIDx;

		vDefTexCoords = src.vDefTexCoords;
		vHovTexCoords = src.vHovTexCoords;
		vSelTexCoords = src.vSelTexCoords;
		vActTexCoords = src.vActTexCoords;
		vLocTexCoords = src.vLocTexCoords;
	}

	void DeepCopy( KD3DWndClass* src )
	{
		DataCopyExceptPointer( *src );
		
		const size_t iLimit = src->vecChild.size();
		for( size_t i=0; i<iLimit; ++i )
		{
			vecChild.push_back( new KD3DWndClass );
			vecChild[i]->DeepCopy( src->vecChild[i] );
			vecChild[i]->pParent = this;
		}
	}
	//////////////////////////////////////////////////////////////////////////

};


struct KD3DNumClass
{
	std::string			strType;
    
	int					iWidth , iHeight;
	int					iTexIDs;

	float				fTu1 , fTv1 , fTu2 , fTv2;

	float				offset;

};

class SortByNameComp
{
public:	
    SortByNameComp() {}
    ~SortByNameComp() {}

    bool operator() ( const KD3DWndClass* pLeft, const KD3DWndClass* pRight )
    {
        if ( !pLeft || !pRight )
            return false;

        if ( strcmp( pLeft->GetWndName().c_str(), pRight->GetWndName().c_str() ) < 0  )
            return true;
        else 
            return false;
    }
};

class SortByIndexComp
{
public:	
    SortByIndexComp() {}
    ~SortByIndexComp() {}

    bool operator() ( const KD3DWndClass* pLeft, const KD3DWndClass* pRight )
    {
        if ( !pLeft || !pRight )
            return false;

        if ( pLeft->GetSortIndex() < pRight->GetSortIndex() )
            return true;
        else 
            return false;
    }
};









#endif // _D3D_WND_CLASS_H_



