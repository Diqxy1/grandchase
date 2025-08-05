#pragma once
#include "GCObject.h"

class KGCObjectAndMotion : public GCObject
{
public:
	KGCObjectAndMotion(void);
	virtual ~KGCObjectAndMotion(void);

	enum{
		OAM_ALL_MOTION_LOOP,
		OAM_SINGLE_MOTION_LOOP,
	};

	void FrameMove();
	void FrameMoveExtra();
    bool IsEmpty() { return m_vecMotions.empty(); }

	void AddMotion( std::string strMotion, bool bOneTimeMotion = false, bool bReserveNext = false );
    void AddMotion_Front( std::string strMotion, bool bOneTimeMotion = false );

	virtual void SetPosition( D3DXVECTOR3 vPos );
	virtual void SetScale( D3DXVECTOR3 vScale );
	void SetRotation( D3DXVECTOR3 vRot );
	void SetOffset( D3DXVECTOR3 vOffset );
    void SetEnable_FirstFrameForPosition(bool bEnable_ )
    {
        m_bFirstFrameForPosition = bEnable_;
        if( bEnable_ )
            m_iFirstFrame = 2;
        else
            m_iFirstFrame = 0;
    }
	void ClearMotion();
    void ClearOneTimeMotion();
	bool IsQueueMotion( std::string strMotion );

    bool SetWorldMatrix();

	D3DXVECTOR3 GetOriginalPosition()
	{
		return m_vOriginalPos;
	};

	D3DXVECTOR3 GetScale()
	{
		return m_vScale;
	};

	void NextMotion();
    void ReSetMotion();
    void SetRandomlyFrame( std::string strMotion, int iFrame );
    void SetCurrentFrame(int iFrame) { m_iFrame = iFrame; };
    int GetCurrentFrame() { return m_iFrame; };
    void SetLoopMode(int iMode){ m_iLoopMode = iMode; }
	DWORD dwGoodsID;

	int GetCurrentMotionIndex()
	{
		return m_iCurrentMotion;
	}
	const char* GetCurrentMotion()
	{
		if( m_iCurrentMotion < 0 )
		{
			return "";
		}

		if ( (int)m_vecMotions.size() > m_iCurrentMotion )
		{
			return m_vecMotions[m_iCurrentMotion].first->GetDeviceID().c_str();
		}

		return "";
	}

	void ReserveMotion( int iMotionIndex )
	{
		if ( (int)m_vecMotions.size() > iMotionIndex )
		{
			m_iReservedMotion = iMotionIndex;
		}
	}

	int GetVecMotionSize() { return (int)m_vecMotions.size(); }

private:
	bool SetFrameWithCheck();

private:
	int	m_iFrame;
	int	m_iCurrentMotion;
	int m_iLoopMode;
	int m_iReservedMotion;    
    bool m_bFirstFrameForPosition;
    int m_iFirstFrame;
	D3DXVECTOR3 m_vOriginalPos;
	D3DXVECTOR3	m_vPos;
	D3DXVECTOR3 m_vScale;
	D3DXVECTOR3	m_vRotation;
	D3DXVECTOR3 m_vOffset;
	std::vector< std::pair<GCDeviceMotionFRM*, bool> > m_vecMotions;	//pair의 second 값은 한번만 플레이하고 빠질 모션인지를 정합니다.
	std::map< std::string, GCDeviceMotionFRM* > m_mapTrash;
	
};
