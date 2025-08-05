#pragma once 

template <class T>
class KProtectedType
{
public:	

    KProtectedType()
    {		
/*        m_data = 0;		*/
    }

    KProtectedType( const T& data )
    {

        m_data = data;		
    }

    KProtectedType(  const KProtectedType< T >& rhs ) // 복사 생성자 안 넣으면 큰일난다 
    {
        m_data = rhs.m_data;		
    }

    ~KProtectedType()
    {       
    }

    // type-casting operator
    inline operator T( ) const 
    {		
        return m_data;
    }

    // assignment operators
    const KProtectedType< T >& operator = ( const T& rhs )
    {
        m_data = rhs;
        return *this;
    }
    const KProtectedType< T >& operator = ( const KProtectedType< T >& rhs )
    {
        m_data = rhs.m_data;
        return *this;
    }

    const KProtectedType< T >& operator += ( const T& rhs )
    {
        m_data = m_data + rhs;
        return *this;
    }
    const KProtectedType< T >& operator -= ( const T& rhs )
    {
        m_data = m_data - rhs;
        return *this;
    }
    const KProtectedType< T >& operator *= ( const T& rhs )
    {
        m_data = m_data * rhs;
        return *this;
    }
    const KProtectedType< T >& operator /= ( const T& rhs )
    {
        m_data = m_data / rhs;
        return *this;
    }
    const KProtectedType< T >& operator %= ( const T& rhs )
    {
        m_data = m_data % rhs;
        return *this;
    }
    const KProtectedType< T >& operator <<= ( int rhs )
    {
        m_data = m_data << rhs;
        return *this;
    }
    const KProtectedType< T >& operator >>= ( int rhs )
    {
        m_data = m_data >> rhs;
        return *this;
    }

    template <class T2> const bool operator == ( const T2& rhs )	
    {
        return m_data == rhs;
    }

    template <class T2> const bool operator != ( const T2& rhs ) 
    {
        return m_data != rhs;
    }

    template <class T2> const bool operator >= ( const T2& rhs )	
    {
        return m_data >= (T)rhs;
    }

    template <class T2> const bool operator <= ( const T2& rhs )	
    {
        return m_data <= (T)rhs;
    }

    template <class T2> const bool operator > ( const T2& rhs ) 
    {
        return m_data > static_cast<T>(rhs);
    }

    template <class T2> const bool operator < ( const T2& rhs ) 
    {
        return m_data < static_cast<T>(rhs);
    }

    const KProtectedType< T >& operator ++ ( void )
    {
        m_data++;
        return *this;
    }
    // postfix increment operator
    const KProtectedType< T >& operator ++ ( int )
    {
        m_data++;
        return *this;
    }
    // prefix decrement operator
    const KProtectedType< T >& operator -- ( void )
    {
        m_data--;
        return *this;
    }
    // postfix decrement operator
    const KProtectedType< T >& operator -- ( int )
    {
        m_data--;
        return *this;
    }    
    mutable T m_data;
};