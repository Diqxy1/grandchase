#pragma once


class FRect
{
public:
	float left;
	float right;
	float top;
	float bottom;

	FRect();
	~FRect();

	void SetRect( float _left, float _top, float _right, float _bottom );
	void Zoom( float Zoom );

	bool PtInRect( float fX, float fY );
	bool RectInRect( float fMinX, float fMinY, float fMaxX, float fMaxY );

	void AdjustPtInRect( float& fX, float& fY, float fDelta = 0.0f );
};

template <class T>
class GCCollisionRect
{
public:
	GCCollisionRect()
	{
		m_Left		= 0;
		m_Top		= 0;
		m_Right		= 0;
		m_Bottom	= 0;
	}
	~GCCollisionRect()
	{
	}
public:
	void SetRect(T _left, T _top, T _right, T _bottom)
	{
		m_Left		= _left;
		m_Top		= _top;
		m_Right		= _right;
		m_Bottom	= _bottom;
	}
	void AdjustRect()	// left right top bottom이 안맞을까봐...
	{
		T Temp;
		if( m_Left > m_Right )
		{
			Temp = m_Left;
			m_Left = m_Right;
			m_Right = Temp;
		}

		if( m_Top < m_Bottom )
		{
			Temp = m_Top;
			m_Top = m_Bottom;
			m_Bottom = Temp;
		}
	}
	bool CheckCollision(GCCollisionRect<T>& comp,GCCollisionRect<T>* pCollision)
	{
		this->AdjustRect();
		comp.AdjustRect();

		if( m_Left > comp.m_Right )
			return 0;
		if( m_Right < comp.m_Left )
			return 0;
		if( m_Top < comp.m_Bottom )
			return 0;
		if( m_Bottom > comp.m_Top )
			return 0;

		if( comp > *this )
		{
			if(pCollision)
			{
				*pCollision = *this;
			}
			return true;
		}
		if( comp < *this )
		{
			if(pCollision)
			{
				*pCollision = comp;
			}
			return true;
		}
		if(pCollision == NULL )
			return true;

		pCollision->m_Left = GetMax( m_Left, comp.m_Left );
		pCollision->m_Right = GetMin( m_Right, comp.m_Right );
		pCollision->m_Top = GetMin( m_Top, comp.m_Top );
		pCollision->m_Bottom = GetMax( m_Bottom, comp.m_Bottom );
		return true;
	}

	bool CheckCollision(FRect& comp)
	{
		this->AdjustRect();

		if( m_Left > comp.right )
			return 0;
		if( m_Right < comp.left )
			return 0;
		if( m_Top < comp.bottom )
			return 0;
		if( m_Bottom > comp.top )
			return 0;
		
		return true;
	}

	void GetCenter(T& _CenterX, T& _CenterY)
	{
		_CenterX = (m_Left + m_Right) / 2;
		_CenterY = (m_Top + m_Bottom) / 2;
	}

	T GetCenterX() 
	{
		return (m_Left + m_Right) / 2;
	}

	T GetCenterY()
	{
		return (m_Top + m_Bottom) / 2;
	}

    T GetWidth()
    {
        return m_Right - m_Left;
    }

    T GetHeight()
    {
        return m_Top - m_Bottom;
    }

	T& GetMin(T& _x, T& _y)
	{
		if( _x < _y )
			return _x;
		return _y;
	}

	T& GetMax(T& _x, T& _y)
	{
		if( _x > _y )
			return _x;
		return _y;
	}

    bool PtInRect(T& _x, T& _y)
    {
        return _x >= m_Left && _x <= m_Right && _y <= m_Top && _y >= m_Bottom;
    }

    bool IsSetRect()
    {
        return ((0 != m_Left) || (0 != m_Right) || (0 != m_Top) || (0 != m_Bottom));
    }

public:		// Operators
	GCCollisionRect& operator = ( GCCollisionRect<T>& right )
	{
		m_Left = right.m_Left;
		m_Top  = right.m_Top;
		m_Right = right.m_Right;
		m_Bottom = right.m_Bottom;
		return *this;
	}
	bool operator == ( GCCollisionRect<T>& right )
	{
		if( m_Left		!= right.m_Left ) return false;
		if( m_Top		!= right.m_Top  ) return false;
		if( m_Right		!= right.m_Right  ) return false;
		if( m_Bottom	!= right.m_Bottom  ) return false;
		return true;
	}
	bool operator > ( GCCollisionRect<T>& right )
	{
		if( *this == right )
			return false;

		if( m_Left > right.m_Left ) return false;
		if( m_Right < right.m_Right ) return false;
		if( m_Top < right.m_Top ) return false;
		if( m_Bottom > right.m_Bottom ) return false;

		return true;
	}
	bool operator < ( GCCollisionRect<T>& right )
	{
		return right > *this;
	}
public:
	T m_Left;
	T m_Top;
	T m_Right;
	T m_Bottom;
};