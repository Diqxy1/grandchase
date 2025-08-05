#pragma once

template <class T,int thesize>
class KSafeArray
{
private:	
    T v[thesize];
public:
    KSafeArray(){}
    ~KSafeArray(){}

    // 타입 정의
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // 반복자 지원
    iterator begin() { return v; }
    const_iterator begin() const { return v; }
    iterator end() { return v+thesize; }
    const iterator end() const { return v+thesize; }

    // 직접적인 원소 액세스
    reference operator[](std::size_t i) {

        if(i<0 || i>=thesize)
        {
#if defined(DEBUG)
            throw new std::exception("Invalid Index");
#elif !defined( __PATH__ )
            terminate();
#endif
        }

        return v[i];
    }
    const_reference operator[] (std::size_t i) const {

        if(i<0 || i>=thesize)
        {
#if defined(DEBUG)
            throw new std::exception("Invalid Index");
#elif !defined( __PATH__ )
            terminate();
#endif
        }
        return v[i];
    }

    // 사이즈 관련
    size_type size() const { return thesize; }
    size_type max_size() const { return thesize; }

    // 기존 배열로의 전환
    T* as_array() { return v; }
};

template <class T>
class KSafeDArray
{
private:	
    T* v;
    std::size_t m_iSize;
    KSafeDArray():v(NULL){}
public:
    KSafeDArray( int iSize_ ):v(NULL){ v = new T[iSize_]; m_iSize = iSize_; }
    KSafeDArray( const KSafeDArray<T>& arg ) {
        if( v )
            delete[] v;

        m_iSize = arg.size();
        v = new T[m_iSize];
        for( int i = 0 ; i < (int)m_iSize : ++i )
        {
            v[i] = arg[i];
        }
    }
    ~KSafeDArray(){}

    // 타입 정의
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // 반복자 지원
    iterator begin() { return v; }
    const_iterator begin() const { return v; }
    iterator end() { return v+m_iSize; }
    const iterator end() const { return v+m_iSize; }

    // 직접적인 원소 액세스
    reference operator[](std::size_t i) {

        if(i<0 || i>=m_iSize)
        {
#if defined(DEBUG)
            throw new std::exception("Invalid Index");
#elif !defined( __PATH__ )
            terminate();
#endif
        }

        return v[i];
    }
    const_reference operator[] (std::size_t i) const {

        if(i<0 || i>=m_iSize)
        {
#if defined(DEBUG)
            throw new std::exception("Invalid Index");
#elif !defined( __PATH__ )
            terminate();
#endif
        }
        return v[i];
    }

    const KSafeDArray<T>& operator = ( const KSafeDArray<T>& arg )
    {
        if( v )
            delete[] v;

        m_iSize = arg.size();
        v = new T[m_iSize];
        for( int i = 0 ; i < (int)m_iSize ; ++i )
        {
            v[i] = arg[i];
        }

        return *this;
    }

    // 사이즈 관련
    size_type size() const { return m_iSize; }
    size_type max_size() const { return m_iSize; }

    // 기존 배열로의 전환
    T* as_array() { return v; }
};
