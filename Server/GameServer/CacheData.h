#pragma once

template< class T >
class KCacheData
{
public:
    KCacheData():m_tInit((T)0),m_tAlive((T)0){}
    KCacheData( T tValue_ ):m_tInit(tValue_),m_tAlive(tValue_){}
    void SetInitValue( const T tValue_ ) { m_tInit     = tValue_; }
    void SetValue( const T tValue_ ) { m_tAlive    = tValue_; }

    void SetAllValue( const T tValue_ )
    {
        SetInitValue( tValue_ );
        SetValue( tValue_ );
    }

    T GetInitValue() { return m_tInit; }
    T GetValue() { return m_tAlive; }

    T GetChangeValue()  { return m_tAlive - m_tInit; }
    bool IsChanged()    { return (m_tAlive != m_tInit); }

    void IncreaseInitValue( const T tValue_ ) { m_tInit += tValue_; }
    void IncreaseValue( const T tValue_ ) { m_tAlive += tValue_; }

    void IncreaseAllValue( T tValue_ )
    {
        IncreaseInitValue( tValue_ );
        IncreaseValue( tValue_ );
    }

    void AdjustValue() { m_tInit = m_tAlive; }

    void operator+=( const T tValue_ ) { IncreaseValue( tValue_ ); }
    void operator-=( const T tValue_ ) { IncreaseValue( -tValue_ ); }

    operator T(void) const { return m_tAlive; }

private:
    T   m_tInit;
    T   m_tAlive;
};