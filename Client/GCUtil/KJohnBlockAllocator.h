///////////////////////////////////////////////////////////////////////////////
/// @file   KJohnBlockAllocator.h
///         interface and implementation for the KJohnBlockAllocator class
/// @desc   customized fast small memory block allocator
/// @author original code by KOG BK server team
/// @author slight speed up for non thread safe version by seojt@kogstudios.com
/// @since  2005-01-05 오전 9:30:48
///

#ifndef _JohnBlockAllocator_
#define _JohnBlockAllocator_

#pragma warning(disable:4786)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#include <vector>
#include "KJohnGen.h"


template <class T, bool isMultiThreaded = false, size_t blocksPerBatch = 128, size_t blockAlignment = 4>
class KJohnBlockAllocator
{
private:
    template<int v>
    struct Int2Type
    {
        enum { value = v };
    };//struct Int2Type<>

private:
    template <class T, bool isMultiThreaded, size_t blocksPerBatch, size_t blockAlignment> 
    class BlockStore
    {
    private:
        typedef std::vector<unsigned char*> BatchPtrVector;

    private:
        unsigned char**     ppNextBlock;    ///< Pointer to the next available block pointer
        BatchPtrVector      batches;        ///< Array of pointers to batches
        CRITICAL_SECTION    m_cs;           ///< critical section for thread-safe version

    public:
        /// constructor.
        BlockStore()
        { 
            ppNextBlock = NULL;
            InitializeCriticalSection( &m_cs );
        }//BlockStore()

        /// destructor.
        ~BlockStore()
        {
            //* Check for memory leaks...
            DeleteBlocks();
            DeleteCriticalSection( &m_cs );
        }//~BlockStore()

        void Lock(Int2Type<true>) { EnterCriticalSection( &m_cs ); }
        void Unlock(Int2Type<true>) { LeaveCriticalSection( &m_cs ); }
        void Lock(Int2Type<false>) {}
        void Unlock(Int2Type<false>) {}

        T* AllocateBlock()
        {   
            Lock( Int2Type<isMultiThreaded>() );
        
            //* Is there any room?
            if ( !ppNextBlock || !*ppNextBlock )
            {
                // determine the allligned size of the blocks
                static const size_t blockSize = (sizeof(T)+blockAlignment-1)&(~(blockAlignment-1));

                // make a new batch 
                unsigned char*  pNewBatch = new unsigned char[blocksPerBatch*blockSize+15];
                batches.push_back( pNewBatch );

                //* Align the block on a 16-byte boundary
                unsigned char* pAlignedPtr =(unsigned char*)((unsigned int)(pNewBatch+15)&(~15));

                // fill the pointers with the new blocks
                // i.e. setup available-list
                ppNextBlock = (unsigned char**)pAlignedPtr;
                for (int i=0; i < blocksPerBatch-1; ++i)
                {
                    *((unsigned int*)(pAlignedPtr + i*blockSize)) = (unsigned int)(pAlignedPtr + (i+1)*blockSize);
                }//for
                // set end-of-list sentinel
                *((unsigned int*)(pAlignedPtr + (blocksPerBatch-1)*blockSize)) = (unsigned int)0;
            }//if

            unsigned char* pBlock = (unsigned char*)ppNextBlock;
            ppNextBlock = (unsigned char**)*ppNextBlock;
        
            Unlock( Int2Type<isMultiThreaded>() );

            return (T*)pBlock;
        }//AllocateBlock()

        void ReleaseBlock(T* pBlock)
        {
            Lock( Int2Type<isMultiThreaded>() );

            if ( pBlock )
            {
                *((unsigned int*)pBlock) = (unsigned int)ppNextBlock;
                ppNextBlock = (unsigned char**)((unsigned char*)pBlock);
            }//if
        
            Unlock( Int2Type<isMultiThreaded>() );
        }//ReleaseBlock()

        void DeleteBlocks()
        {
            Lock( Int2Type<isMultiThreaded>() );

            // Must clean up these pointers
            size_t iNum = batches.size();

            for (size_t i=0; i<iNum; ++i)
            {
                unsigned char* p = (unsigned char*)batches[i];
                delete[] p;
            }//for

            batches.clear();
            ppNextBlock = NULL;

            Unlock( Int2Type<isMultiThreaded>() );
        }//DeleteBlocks()
    };//struct BlockStore<>

public:
    void* operator new(size_t)
    {
        return s_Store.AllocateBlock();
    }//operator new()

    void* operator new[](size_t)
    {
        ASSERT( false );
        return NULL;
    }//operator new()

    void operator delete(void* pBlock)
    {
        s_Store.ReleaseBlock((T*)pBlock);
    }//operator delete()

    void operator delete[](void* pBlock)
    {
        ASSERT( false );
    }//operator delete[]()

    //* These can be called directly, but be warned, the constructors 
    //* and destructors on the blocks will not be called!
    static T*   AllocateBlock()         { return s_Store.AllocateBlock(); }
    static void ReleaseBlock(T* pBlock) { s_Store.ReleaseBlock(pBlock); }
    static void CleanUp()               { s_Store.DeleteBlocks(); }

public:
    typedef BlockStore<T,isMultiThreaded,blocksPerBatch,blockAlignment>  BLOCK_STORE;

private:
    static BLOCK_STORE  s_Store;
};//class KJohnBlockAllocator

template<class T, bool isMultiThreaded, size_t blocksPerBatch, size_t blockAlignment> 
typename KJohnBlockAllocator<T,isMultiThreaded,blocksPerBatch,blockAlignment>::BLOCK_STORE
KJohnBlockAllocator<T,isMultiThreaded,blocksPerBatch,blockAlignment>::s_Store;

#endif // _JohnBlockAllocator_


/** @example    class KJohnBlockAllocator

    #include <stdio.h>
    #include <vector>
    #include <list>
    #include <conio.h>
    #include "KJohnBlockAllocator.h"

    //#define STEP_BY_STEP()
    #define STEP_BY_STEP()      getch()


    class CSmallObject : public KJohnBlockAllocator<CSmallObject>
    {
    public:
        static int  ms_iObjectCounter;

    private:
        int         m_iInt;
        float       m_fFloat;

    public:
        CSmallObject()
        {
            m_iInt   = ms_iObjectCounter;
            m_fFloat = ms_iObjectCounter * 10.f;
            ++ms_iObjectCounter;
            printf( "CSmallObject(): # of objects=%i, %i, %g\n", ms_iObjectCounter, m_iInt, m_fFloat );
        }//CSmallObject()

        ~CSmallObject()
        {
            --ms_iObjectCounter;
            printf( "~CSmallObject(): # of objects=%i, %i, %g\n", ms_iObjectCounter, m_iInt, m_fFloat );
        }//~CSmallObject()

        void Print()
        {
            printf( "Print(): # of objects==%i, %i,%g\n", ms_iObjectCounter, m_iInt, m_fFloat );
        }//Print()
    };//class CSmallObject

    int  CSmallObject::ms_iObjectCounter = 0;

    void main()
    {
        typedef std::vector<CSmallObject*>              OBJECTVECTOR;
        typedef std::vector<CSmallObject*>::iterator    OBJECTVECTOR_ITOR;
        typedef std::list<CSmallObject*>                OBJECTLIST;
        typedef std::list<CSmallObject*>::iterator      OBJECTLIST_ITOR;

        const int           c_iNumAllocation  = 10;
        CSmallObject*       pSmallObject;

        // example 1: disable new[] operation
        //pSmallObject = new CSmallObject[100]; // doesn't work!

        // example 2: sequential allocation and deletion
        OBJECTVECTOR        objVector;
        OBJECTVECTOR_ITOR   vitor;

        for (int i = 0; i < c_iNumAllocation; ++i)
        {
            pSmallObject = new CSmallObject();
            objVector.push_back( pSmallObject );
        }//for
        STEP_BY_STEP();

        for (vitor = objVector.begin(); vitor != objVector.end(); ++vitor)
        {
            (*vitor)->Print();
        }//for
        STEP_BY_STEP();

        for (vitor = objVector.begin(); vitor != objVector.end(); ++vitor)
        {
            delete (*vitor);
        }//for
        STEP_BY_STEP();

        // example 3: random allocation and deletion
        OBJECTLIST          objList;
        OBJECTLIST_ITOR     litor;

        for (i = 0; i < c_iNumAllocation; ++i)
        {
            objList.push_back( new CSmallObject() );
        }//for
        STEP_BY_STEP();

        // random deletion
        int iCounter = 0;

        litor = objList.begin();
        while ( litor != objList.end() )
        {
            if ( iCounter % 3 == 0 )
            {
                delete (*litor);
                litor = objList.erase( litor );
            }//if

            ++litor;
            ++iCounter;
        }//while
        STEP_BY_STEP();

        litor = objList.begin();
        while ( litor != objList.end() )
        {
            (*litor)->Print();
            ++litor;
        }//while
        STEP_BY_STEP();

        litor = objList.begin();
        while ( litor != objList.end() )
        {
            delete (*litor);
            ++litor;
        }//while
        STEP_BY_STEP();

        objList.clear();

        // final example: manual clean up
        CSmallObject::CleanUp(); // can be commented out
    }//main()

*/
