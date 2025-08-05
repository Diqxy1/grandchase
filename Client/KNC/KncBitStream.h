#pragma once

////#include <vector>
#include <Serializer/SerBuffer.h>
//#include <windows.h>    // byte.

class KncBitStream;
SERIALIZE_DEFINE_TAG(KncBitStream, eTAG_USERCLASS);
SERIALIZE_DECLARE_PUTGET(KncBitStream);
class KncBitStream : public std::vector<byte>
{
    typedef value_type vec_element_type;
    enum { 
        vec_element_size = sizeof(value_type),
        vec_element_bit_size = sizeof(value_type) * 8,
    };

public:
    KncBitStream(void);
    virtual ~KncBitStream(void);

public:
    bool Get( size_t nIndex ) const;    // 해당 비트가 1인경우 true.
    void Set( size_t nIndex );          // 해당 비트를 1로 설정.
    void Reset( size_t nIndex );        // 해당 비트를 0으로 설정.

    void Dump( OUT std::ostream& stm );

    size_t GetBitSize()     { return size() * vec_element_bit_size; }
    size_t GetByteSize()    { return size() * vec_element_size; }

    SERIALIZE_DECLARE_FRIEND( KncBitStream );
};
