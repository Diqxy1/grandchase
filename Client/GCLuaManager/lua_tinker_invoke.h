#ifndef BOOST_PP_IS_ITERATING
#ifndef _LUA_TINKER_INVOKE_
#define _LUA_TINKER_INVOKE_

	#include <boost/preprocessor/control/if.hpp>
	#include <boost/preprocessor/arithmetic/inc.hpp>
	#include <boost/preprocessor/repetition/enum.hpp>
	#include <boost/preprocessor/comparison/equal.hpp>
	#include <boost/preprocessor/iteration/iterate.hpp>
	#include <boost/preprocessor/repetition/enum_params.hpp>
	#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>

	#define _POP_ARG( z, n, unused )	read<BOOST_PP_CAT(T,n)>(L,BOOST_PP_INC(n))
	#define _FIRST_ITERATION

	#define BOOST_PP_ITERATION_LIMITS	(LUA_TINKER_FUNCTOR_ARG, 0)
	#define BOOST_PP_FILENAME_1			"lua_tinker_invoke.h"
	#include BOOST_PP_ITERATE()

	#undef BOOST_PP_FILENAME_1
	#undef BOOST_PP_ITERATION_LIMITS
	#undef _POP_ARG

#endif

#else

	#define n	BOOST_PP_ITERATION()

	#if defined( _FIRST_ITERATION )
	#undef _FIRST_ITERATION
		template<BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(n, typename T, void)>
		struct functor
	#else
		template<BOOST_PP_ENUM_PARAMS(n, typename T)>
		struct functor<BOOST_PP_ENUM_PARAMS(n,T)>
	#endif
	{
		template<typename RVal>
		static int invoke(lua_State *L) 
		{
			push(L,upvalue_<RVal(*)(BOOST_PP_ENUM_PARAMS(n,T))>(L)(BOOST_PP_ENUM( n, _POP_ARG, ~ )) ); return 1; 
		}
		template<>
		static int invoke<void>(lua_State *L) 
		{ 
			upvalue_<void(*)(BOOST_PP_ENUM_PARAMS(n,T))>(L)(BOOST_PP_ENUM( n, _POP_ARG, ~ ));	return 0; 
		}
	};
#endif
