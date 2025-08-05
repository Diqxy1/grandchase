#ifndef BOOST_PP_IS_ITERATING
#ifndef _LUA_TINKER_MEMBER_FUNCTOR_
#define _LUA_TINKER_MEMBER_FUNCTOR_

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

#define _POP_ARG( z, n, unused )	read<BOOST_PP_CAT(T,n)>(L,BOOST_PP_INC(BOOST_PP_INC(n)))
#define _FIRST_ITERATION

#define BOOST_PP_ITERATION_LIMITS	(LUA_TINKER_FUNCTOR_ARG, 0)
#define BOOST_PP_FILENAME_1			"lua_tinker_member_functor.h"
#include BOOST_PP_ITERATE()

#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS
#undef _POP_ARG

#endif

#else

#define n	BOOST_PP_ITERATION()

#if defined( _FIRST_ITERATION )
	#undef _FIRST_ITERATION
	template<typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT( n, typename T, void )>
	struct mem_functor
#else
	template<typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS( n, typename T )>
	struct mem_functor<T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS( n, T )>
#endif
	{
		template<typename RVal>
		static int invoke(lua_State *L) 
		{ 
			push(L,(read<T*>(L,1)->*upvalue_<RVal(T::*)(BOOST_PP_ENUM_PARAMS(n,T))>(L))(BOOST_PP_ENUM( n, _POP_ARG, ~ )));
			return 1; 
		}
		template<>
		static int invoke<void>(lua_State *L)  
		{ 
			(read<T*>(L,1)->*upvalue_<void(T::*)(BOOST_PP_ENUM_PARAMS(n,T))>(L))(BOOST_PP_ENUM( n, _POP_ARG, ~ )); 
			return 0;
		}
	};
#endif
