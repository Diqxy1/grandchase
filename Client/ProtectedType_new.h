#pragma once 

extern void                 ExitWithHackAlert( std::wstring strLog );


class CMemoryBlockBase
{
public:
	static std::list<CMemoryBlockBase*> m_listMemory;
	static void Clear()
    {
        while(!m_listMemory.empty())
        {
            delete m_listMemory.back();
            m_listMemory.pop_back();
        }
    }

	CMemoryBlockBase()
	{
		static bool b_first=true;
		if(b_first)
		{
			b_first=false;
			srand(time(NULL));
		}
		m_listMemory.push_back(this);
	}

	virtual ~CMemoryBlockBase()
	{

	}
};

template <class T>
class CMemoryBlock:public CMemoryBlockBase
{
public:	

	static CMemoryBlock<T> *m_pMemoryBlock;	
	
	static inline int AllocateBlock()
	{
		if(!m_pMemoryBlock)
			m_pMemoryBlock = new CMemoryBlock<T>();

		return m_pMemoryBlock->AllocateBlock_();
	}

	static inline void FreeBlock(int i)
	{
		if(m_pMemoryBlock)
			m_pMemoryBlock->FreeBlock_(i);
	}

	static inline T * GetBlock(int i){return m_pMemoryBlock->GetBlock_(i);}

	CMemoryBlock()		
	{
		size = 0;
		m_pFreeBlock = NULL;
		m_ppData= NULL;
		blocksize=0;
		m_ppBlock = NULL;
	}

	virtual ~CMemoryBlock()
	{
		m_pMemoryBlock = NULL;
		if(m_ppData)
		{
			free(m_ppData);
		}

		if(m_ppBlock)
		{
			for(int i=0;i<blocksize;i++)
			{
				if(m_ppBlock[i])
				{
					delete [] m_ppBlock[i];
					m_ppBlock[i]=NULL;
				}
			}
			free(m_ppBlock);
		}
	}

	inline int AllocateBlock_()
	{
		if(m_pFreeBlock == NULL)
			AddMemory(rand()%1024+1024);

		int i = m_pFreeBlock->index;
		m_pFreeBlock = m_pFreeBlock->pNext;
		m_ppData[i]->pNext = NULL;
		return i;
	}

	inline void FreeBlock_(int i)
	{
		if(m_ppData[i]->pNext || i<0 || i>=size)
		{
			ExitWithHackAlert(L"ProtectedTypeMgr");
			return;
		}		
		m_ppData[i]->pNext = m_pFreeBlock;
		m_pFreeBlock = m_ppData[i];
	}

	inline T * GetBlock_(int i){return m_ppData[i]->pData;}

protected:	

	/*template*/ class CBlockData
	{
	public:		
		T *pData;
		T Data;
		int index;
		CBlockData * pNext;
	};

	void AddMemory(int inc)
	{	
		blocksize++;

		m_ppData = (CBlockData**)realloc(m_ppData,(size+inc) * sizeof(void*));
		m_ppBlock = (CBlockData**)realloc(m_ppBlock,(blocksize) * sizeof(void*));			 

		m_ppBlock[blocksize-1] = new CBlockData[inc];

		int i = size;
		while(i<size+inc)
		{
			m_ppData[i] = &m_ppBlock[blocksize-1][i-size];
			m_ppData[i]->pData = &m_ppData[i]->Data;
			m_ppData[i]->index = i;
			m_ppData[i]->pNext = NULL;
			i++;
		}

		i=size;
		while(i<size+inc-1)
		{			
			m_ppData[i]->pNext = m_ppData[i+1];			
			i++;
		}	

		m_ppData[i]->pNext = m_pFreeBlock;
		m_pFreeBlock = m_ppData[size];

		i=size;
		while(i<size+inc)
		{
			std::swap<>(m_ppData[i]->pData,m_ppData[size+(rand()%inc)]->pData);
			i+=2;
		}
		size+=inc;
	}	


	CBlockData* m_pFreeBlock;
	CBlockData** m_ppData;
	CBlockData** m_ppBlock;
	int size;
	int blocksize;
};

template <class T> CMemoryBlock<T> * CMemoryBlock<T>::m_pMemoryBlock = NULL;


template <class T>
class KProtectedType
{
public:	

	KProtectedType()
	{		
		m_index = CMemoryBlock<T>::AllocateBlock();		
		Set<T>((T)0);
	}

	KProtectedType( const T& data )
	{
	
		m_index = CMemoryBlock<T>::AllocateBlock();		
		Set<T>(data);
	}

	KProtectedType(  const KProtectedType< T >& rhs ) // 복사 생성자 안 넣으면 큰일난다 
	{
		m_index = CMemoryBlock<T>::AllocateBlock();		
		Set<T>(rhs.Get<T>());
	}

	~KProtectedType()
	{
		CMemoryBlock<T>::FreeBlock(m_index);		
		m_index = -1;		
	}

	// type-casting operator
	inline operator T( ) const 
	{		
		return Get<T>();
	}

	// assignment operators
	const KProtectedType< T >& operator = ( const T& rhs )
	{
		Set<T>( rhs );
		return *this;
	}
	const KProtectedType< T >& operator = ( const KProtectedType< T >& rhs )
	{
		Set<T>(rhs.Get<T>());
		return *this;
	}

	const KProtectedType< T >& operator += ( const T& rhs )
	{
		Set<T>( Get<T>() + rhs );
		return *this;
	}
	const KProtectedType< T >& operator -= ( const T& rhs )
	{
		Set<T>( Get<T>() - rhs );
		return *this;
	}
	const KProtectedType< T >& operator *= ( const T& rhs )
	{
		Set<T>( Get<T>() * rhs );
		return *this;
	}
	const KProtectedType< T >& operator /= ( const T& rhs )
	{
		Set<T>( Get<T>() / rhs );
		return *this;
	}
	const KProtectedType< T >& operator %= ( const T& rhs )
	{
		Set( Get<T>() % rhs );
		return *this;
	}
	const KProtectedType< T >& operator <<= ( int rhs )
	{
		Set<T>( Get<T>() << rhs );
		return *this;
	}
	const KProtectedType< T >& operator >>= ( int rhs )
	{
		Set<T>( Get<T>() >> rhs );
		return *this;
	}

	template <class T2> const bool operator == ( const T2& rhs )	
	{
		return Get<T>() == rhs;
	}

	template <class T2> const bool operator != ( const T2& rhs ) 
	{
		return Get<T>() != rhs;
	}

	template <class T2> const bool operator >= ( const T2& rhs )	
	{
		return Get<T>() >= (T)rhs;
	}

	template <class T2> const bool operator <= ( const T2& rhs )	
	{
		return Get<T>() <= (T)rhs;
	}

	template <class T2> const bool operator > ( const T2& rhs ) 
	{
		return Get<T>() > static_cast<T>(rhs);
	}

	template <class T2> const bool operator < ( const T2& rhs ) 
	{
		return Get<T>() < static_cast<T>(rhs);
	}

	const KProtectedType< T >& operator ++ ( void )
	{
		Set<T>(  Get<T>() + 1 );
		return *this;
	}
	// postfix increment operator
	const KProtectedType< T >& operator ++ ( int )
	{
		Set<T>(  Get<T>() + 1 );
		return *this;
	}
	// prefix decrement operator
	const KProtectedType< T >& operator -- ( void )
	{
		Set<T>(  Get<T>() - 1 );
		return *this;
	}
	// postfix decrement operator
	const KProtectedType< T >& operator -- ( int )
	{
		Set<T>( Get<T>() - 1 );
		return *this;
	}

protected:	

	union Udata
	{
		int int_;
		float float_;
		bool bool_;
		char char_;
		unsigned char uchar_;
	};

	template <class T2> inline void Set(T data) const
	{
		m_data = data;
		(*CMemoryBlock<T>::GetBlock(m_index))=(T)(m_data^m_index);
	}

	template <class T2> inline T Get() const
	{
		if((*CMemoryBlock<T>::GetBlock(m_index)) != (m_data^m_index))
		{
			ExitWithHackAlert(L"ProtectedTypeMgr");
		}
		return m_data;
	}


	template <> inline void Set<float>(T data) const
	{
		m_data = data;
		Udata tmp;
		tmp.float_ = m_data;
		tmp.int_^= m_index;
		(*CMemoryBlock<T>::GetBlock(m_index)) = tmp.float_;
	}

	template <> inline T Get<float>() const
	{
		Udata tmp;
		tmp.float_ = (*CMemoryBlock<T>::GetBlock(m_index));
		tmp.int_^=m_index;
		if(tmp.float_ != m_data)
		{
			ExitWithHackAlert(L"ProtectedTypeMgr");
		}
		return m_data;
	}

	template <> inline void Set<bool>(T data) const
	{
		m_data = data;
		Udata tmp;
		tmp.bool_ = m_data;
		tmp.int_^= m_index;
		(*CMemoryBlock<T>::GetBlock(m_index)) = tmp.bool_;
	}

	template <> inline T Get<bool>() const
	{
		Udata tmp;
		tmp.bool_ = (*CMemoryBlock<T>::GetBlock(m_index));
		tmp.int_^=m_index;
		if(tmp.bool_ != m_data)
		{
			ExitWithHackAlert(L"ProtectedTypeMgr");
		}
		return m_data;
	}

	template <> inline void Set<char>(T data) const
	{
		m_data = data;
		Udata tmp;
		tmp.char_ = m_data;
		tmp.int_^= m_index;
		(*CMemoryBlock<T>::GetBlock(m_index)) = tmp.char_;
	}

	template <> inline T Get<char>() const
	{
		Udata tmp;
		tmp.char_ = (*CMemoryBlock<T>::GetBlock(m_index));
		tmp.int_^=m_index;
		if(tmp.char_ != m_data)
		{
			ExitWithHackAlert(L"ProtectedTypeMgr");
		}
		return m_data;
	}

	template <> inline void Set<unsigned char>(T data) const
	{
		m_data = data;
		Udata tmp;
		tmp.uchar_ = m_data;
		tmp.int_^= m_index;
		(*CMemoryBlock<T>::GetBlock(m_index)) = tmp.uchar_;
	}

	template <> inline T Get<unsigned char>() const
	{
		Udata tmp;
		tmp.uchar_ = (*CMemoryBlock<T>::GetBlock(m_index));
		tmp.int_^=m_index;
		if(tmp.uchar_ != m_data)
		{
			ExitWithHackAlert(L"ProtectedTypeMgr");
		}
		return m_data;
	}
	mutable int m_index;
	mutable T m_data;
};



template <class T>
class KDynamicPointer:public boost::noncopyable
{

public:
	KDynamicPointer()
		:pMem(NULL)
	{
		Init();
	}

	~KDynamicPointer()
	{		
		Release();
	}

	void Init()
	{
		if(!pMem)
		{
			static bool b_first=true;
			if(b_first)
			{
				b_first=false;
				srand(time(NULL));
			}

			int begin = (rand()%255+1);		
			pMem = (unsigned char*)malloc(sizeof(T)+begin);		
			pMem[0]=begin;		
			new ((T*)(pMem+begin)) T();
		}		
	}
	void Release()
	{
		if(pMem)
		{
			((T*)(pMem+pMem[0]))->~T();
			free(pMem);
			pMem=NULL;
		}		
	}

	T & Get()
	{
		return *(T*)(pMem+pMem[0]);
	}

	inline operator T* () const
	{
		return ((T*)(pMem+pMem[0]));
	}

	inline T* operator->( ) const 
	{		
		return ((T*)(pMem+pMem[0]));
	}

	unsigned char * pMem;
};
