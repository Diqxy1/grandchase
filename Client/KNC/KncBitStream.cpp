#include "stdafx.h"
#include "KncBitStream.h"



KncBitStream::KncBitStream(void)
{
}

KncBitStream::~KncBitStream(void)
{
}

bool KncBitStream::Get( size_t nIndex_ ) const
{
    size_t nVecIndex = nIndex_ / vec_element_bit_size; // 벡터의 인덱스
    size_t nBitIndex = nIndex_ % vec_element_bit_size;

    if( size() <= nVecIndex ) return false;

    return (at( nVecIndex ) & (1 << nBitIndex)) > 0;
}

void KncBitStream::Set( size_t nIndex_ )
{
    size_t nVecIndex = nIndex_ / vec_element_bit_size;
    size_t nBitIndex = nIndex_ % vec_element_bit_size;

    if( size() <= nVecIndex ) resize( nVecIndex + 1 );

    (*this)[nVecIndex] |= (1 << nBitIndex);
    //operator[]( nVecIndex ) |= (1 << nBitIndex);
}

void KncBitStream::Reset( size_t nIndex_ )
{
    size_t nVecIndex = nIndex_ / vec_element_bit_size;
    size_t nBitIndex = nIndex_ % vec_element_bit_size;

    if( size() <= nVecIndex ) return;

    (*this)[nVecIndex] &= ~(1 << nBitIndex);
    //operator[]( nVecIndex ) |= (1 << nBitIndex);

    std::vector<value_type>::reverse_iterator vit;
    for( vit = rbegin(); vit != rend(); vit++ )
    {
        if( *vit != 0 ) // 뒤에서부터 최초로 0이 아닌 값을 만나게 되면
        {
            // 현재 가르키는 값의 한칸 뒤(iter/rev_iter간 차이때문에 바로 뒤칸을 가르키게 된다) 부터
            // 컨테이너의 끝까지 모두 삭제.
            erase( vit.base(), end() );
            break;
        }
    }

    if( vit == rend() )     // 현재 모든 bit가 0이 된 경우. 위의 조건에서 compaction 되지 않는다.
        clear();
}

SERIALIZE_DEFINE_PUT( KncBitStream, obj, ks )
{
    return ks.Put( (std::vector<KncBitStream::vec_element_type>&)obj );
}

SERIALIZE_DEFINE_GET( KncBitStream, obj, ks )
{
    return ks.Get( (std::vector<KncBitStream::vec_element_type>&)obj );
}

void KncBitStream::Dump( OUT std::ostream& stm_ )
{
    stm_ << std::hex;
    std::copy( begin(), end(), std::ostream_iterator<int>( stm_, " " ) );
    stm_ << std::endl << std::dec;
}