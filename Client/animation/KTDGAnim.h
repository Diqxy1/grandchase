// KTDGAnim.h: interface for the CKTDGAnim class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_KTDGANIM_H__B5C24276_6DAA_49BD_9B96_62F2C6E703C7__INCLUDED_)
//#define AFX_KTDGANIM_H__B5C24276_6DAA_49BD_9B96_62F2C6E703C7__INCLUDED_

#ifndef _KTDGANIM_H_
#define _KTDGANIM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include<vector>
//#include<map>
using std::vector;
using std::string;
using std::map;

//#include "KTDXDeviceManager.h"
//#include "GCDeviceManager2.h"
#include "GCMatrix.h"
#define D3DFVF_ANIM (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CKTDGAnim
{
	public:		
		class CAnimFrame
		{
			public:
                CAnimFrame( LPDIRECT3DDEVICE9 pd3dDevice, const char* pStrName, const char* pStrTexture, RECT* rect = NULL );
				virtual ~CAnimFrame();

				void BuildFrame( DWORD dwDiffuse = 0xFFFFFFFF );
				void Render( DWORD dwDiffuse = 0xFFFFFFFF );
				
				const string& GetName(){ return m_Name; }
                float GetWidth(){ return (float)m_pTexture->GetWidth(); }
                float GetHeight(){ return (float)m_pTexture->GetHeight(); }
				bool GetBuild(){ return m_bBuild; }
				std::string& GetTexFileName() { return m_strTextureFileName; }
				void SetTexture( GCDeviceTexture* pTexture ) { m_pTexture = pTexture; }
				GCDeviceTexture* GetTexture(){ return m_pTexture; }				

			private:
				LPDIRECT3DDEVICE9		m_pd3dDevice;
				
				GCDeviceTexture*		m_pTexture;
				std::string				m_strTextureFileName;

				KSafeArray<SLVERTEX,4> m_pFrameVB;
				RECT*					m_pRect;
				string					m_Name;
				bool					m_bBuild;
		};

		struct FrameData
		{
			CAnimFrame* frame;
			float		fElapsedTime;
		};

		class CAnimSequence
		{
			public:
				CAnimSequence( LPDIRECT3DDEVICE9 pd3dDevice, const char* pStrName_ );
				virtual ~CAnimSequence();

				void Render( int frame );

				void AddFrameData( FrameData* frameData ){ m_FrameList.push_back(frameData); m_MaxFrame = (int)m_FrameList.size()-1; }
				
				int GetMaxFrame(){ return m_MaxFrame; }
				const string& GetName(){ return m_Name; }
				vector<FrameData*>		m_FrameList;
                
                D3DBLEND			m_SrcBlendMode;
				D3DBLEND			m_DestBlendMode;

			private:
				LPDIRECT3DDEVICE9	m_pd3dDevice;
				int					m_MaxFrame;
				string				m_Name;
                
		};

		enum PlayType
		{
			PT_ONE_PASS				= 1,
			PT_ONE_PASS_AUTO_DIE,
			PT_COUNTER_PASS,
			PT_COUNTER_PASS_AUTO_DIE,
			PT_LOOP,
			PT_COUNTER_LOOP,
			PT_PING_PONG,
		};

		enum AnimState
		{
			AS_PLAY	= 1,
			AS_WAIT,
			AS_STOP,
			AS_DIE,
		};

		class CAnimInstance
		{
			public:
				CAnimInstance( LPDIRECT3DDEVICE9 pd3dDevice, CAnimSequence* animSequence );
				virtual ~CAnimInstance();

				void FrameMove( float fElapsedTime, void* pframeData = NULL );
				void Render( bool renderEnv = false );
				
				void Start( PlayType playtype, float speed = 1.0f );
				void Stop();
				void Wait();
				void Die(){ m_AnimState = AS_DIE; }
				void SetShow( bool show ){ m_bShow = show; }

                void AddMatrix( D3DXMATRIX matrix ){ m_UseAddMatrix = true; m_AddMatrix = matrix; }
                void RemoveMatrix(){ m_UseAddMatrix = false; }

				AnimState GetState(){ return m_AnimState; }
				GCMatrix* GetMatrix(){ return &m_KTDGMatrix; }
                int GetNowFrame(){ return m_NowFrame; }
                int GetMaxFrame(){ return m_pAnimSequence->GetMaxFrame(); }
				float GetCurrFrameTime(){ return m_TimeAdd; }

				void SetNowFrame( int iFrame )
				{ 
					m_NowFrame = iFrame; 
					m_TimeAdd = 0.0f;
				}
				void SetSrcBlendMode( D3DBLEND srcBlendMode ){ m_SrcBlendMode = srcBlendMode; }
				void SetDestBlendMode( D3DBLEND destBlendMode ){ m_DestBlendMode = destBlendMode; }
                void SetDiffuse( DWORD dwDiffuse );

                float GetRealWidth();
                float GetRealHeight();

                DWORD GetDiffuse(){ return m_dwDiffuse; }
				void ApplyDiffuse( void );



				CAnimSequence* GetSequence(){ return m_pAnimSequence; }

			private:				
				LPDIRECT3DDEVICE9	m_pd3dDevice;
				CAnimSequence*		m_pAnimSequence;
				GCMatrix 			m_KTDGMatrix;
				PlayType			m_PlayType;
				AnimState			m_AnimState;
				int					m_NowFrame;
				float				m_FrameSpeed;
				float				m_TimeAdd;
				bool				m_PingPongCounter;

				D3DBLEND			m_SrcBlendMode;
				D3DBLEND			m_DestBlendMode;
				bool				m_bShow;

                bool                m_UseAddMatrix;
                D3DXMATRIX          m_AddMatrix;	

                DWORD               m_dwDiffuse;
		};

	public:
		CKTDGAnim( LPDIRECT3DDEVICE9 pd3dDevice );
		virtual ~CKTDGAnim();

		virtual HRESULT FrameMove( float fElapsedTime, void* pframeData = NULL );
		virtual HRESULT Render( bool renderEnv = false );

		bool Compile( char* filename );

		void AddTexture( GCDeviceTexture* texture );
		void DeleteTexture( GCDeviceTexture* texture );
		GCDeviceTexture* GetTexture( const string& filename );		
		void ReleaseTexture();
		void LoadTexture();

		void DeleteFrame( CAnimFrame* frame );
        CAnimFrame* GetFrame( std::string name );
        CAnimFrame* GetFrame( const char* pStrName_ );
		void ClearFramePool();

		void AddSequence( CAnimSequence* sequence );
		void DeleteSequence( CAnimSequence* sequence );
		CAnimSequence* GetSequence( const string& name );
		void ClearSequencePool();

		CAnimInstance* CreateInst( CAnimSequence* sequence );
		void DeleteInst( CAnimInstance* instance );
		bool IsAliveInst( CAnimInstance* instance )
		{
			for( int i = 0; i < (int)m_AnimInstance.size(); i++ )
			{
				if( m_AnimInstance[i] == instance )
				{
					return true;
				}
			}
			return false;
		}
		void ClearInstList();
		
		void ToLast( CAnimInstance* instance );
		void LastToFirst()
		{ 
            if( m_AnimInstance.empty() )
                return;

			CAnimInstance* temp;
			temp = m_AnimInstance[m_AnimInstance.size()-1];
			m_AnimInstance[m_AnimInstance.size()-1] = m_AnimInstance[0];
			m_AnimInstance[0] = temp;
		}
		void Sort();

		virtual HRESULT	DeleteDeviceObjects();
        D3DBLEND StrToBlendOpt( std::string pStrBlend_ );
	private:
		LPDIRECT3DDEVICE9				m_pd3dDevice;
		vector<CAnimInstance*>			m_AnimInstance;		
		std::map<std::string, CAnimSequence*>	m_SequencePool;
		std::map<std::string, GCDeviceTexture*>	m_mapTexturePool;
		std::map<std::string, CAnimFrame*>		m_mapFramePool;

		struct LessZ 
		{

			bool operator()(CKTDGAnim::CAnimInstance *lhs, CKTDGAnim::CAnimInstance *rhs)  const 
			{
				return lhs->GetMatrix()->GetPos().z < rhs->GetMatrix()->GetPos().z;
			}

		};
};

#endif // _KTDGANIM_H_
