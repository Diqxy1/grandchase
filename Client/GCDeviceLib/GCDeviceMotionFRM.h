#pragma once

struct SFrame
{
	unsigned char	Option;
	float			Plus_x, Pos_y;
	unsigned short  Reserved1;
	unsigned short  Reserved2;
	D3DXMATRIX*		A_Bone;
	DWORD           dwNumAngleBone;

	SFrame()
	{
		Init();
	}

	~SFrame()
	{
		SAFE_DELETE_ARRAY( A_Bone );
	}

	void Init()
	{
		Option		= 0;
		Plus_x		= 0;
		Pos_y		= 0;
		A_Bone		= NULL;
		Reserved1	= 0;
		Reserved2	= 0;
		dwNumAngleBone = 0;
	}

	bool IsInit()
	{
		return ( A_Bone == NULL && dwNumAngleBone == 0 );
	}

	SFrame& operator=( const SFrame& In_ )
	{
		if( this->A_Bone == In_.A_Bone )
			return *this;

		this->Option =                    In_.Option;
		this->Plus_x =		             In_.Plus_x;
		this->Pos_y =		             In_.Pos_y;
		this->Reserved1 =	             In_.Reserved1;
		this->Reserved2 =		         In_.Reserved2;
		this->dwNumAngleBone =            In_.dwNumAngleBone;

		if( this->A_Bone )
			SAFE_DELETE_ARRAY( A_Bone );

		if( 0 < In_.dwNumAngleBone )
		{
			this->A_Bone = new D3DXMATRIX[In_.dwNumAngleBone];
			memcpy( this->A_Bone, In_.A_Bone, sizeof(D3DXMATRIX)*In_.dwNumAngleBone );
		}
		else
		{
#if !defined(__PATH__)
			MessageBoxA(0, "프레임 복사 중 본갯수에 생긴 문제", "Error", MB_OK);
#endif
		}

		return *this;
	}
};

struct SFrameOld
{
	unsigned char	Option;
	unsigned short  Reserved1;
	unsigned short  Reserved2;
	float			Plus_x, Pos_y;
	D3DXMATRIX*		A_Bone;
	SFrameOld()
	{
		Option		= 0;
		Reserved1	= 0;
		Reserved2	= 0;
		Plus_x		= 0;
		Pos_y		= 0;
		A_Bone		= NULL;
	}
};

enum EGCFrmVer
{
	FRM_VER_10, // 기존 frm 파일. 255프레임까지 가능
	FRM_VER_11, // 새 버전 frm 파일.
	FRM_VER_12,
};

class GCDeviceMotionFRM : public GCDevice
{
public:
	GCDeviceMotionFRM(std::string DeviceID);
	virtual ~GCDeviceMotionFRM(void);

	void OnResetDevice() {};
	void OnLostDevice() {};

	SFrame& GetFrameData(UINT uiFrame)
	{
		if( uiFrame >= m_dwNumFrame )
			uiFrame = 0;

		return m_aFrame[uiFrame];
	}

	inline int GetNum_Frame()		//Warning이 많아서 잠깐 수정..-_-ㅋ
	{
		if (m_dwNumFrame >= INT_MAX)
			m_dwNumFrame = INT_MAX;
		if (m_dwNumFrame < 0)
			m_dwNumFrame = 0;

		return static_cast<int>(m_dwNumFrame);
	}

	EGCFrmVer GetFrmVersion( void ) { return m_eFrmVer; }

public:
	static int ms_iSize;

protected:
	bool Load();
	void UnLoad();
	DWORD GetDeviceSize();

protected:
	DWORD m_dwNumFrame;
	DWORD m_dwNumAngleBone;
	SFrame* m_aFrame;

private:
	EGCFrmVer CheckFrmVer( const char* strVer_ ) const;
	void LoadNumFrameAndNumAnglebone( GCMemoryFile& file_ );
	EGCFrmVer m_eFrmVer;
};
