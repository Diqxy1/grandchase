#ifndef BOOST_PP_IS_ITERATING

#	ifndef _LUA_TINKER_PUSH_MEMBER_FUNCTOR_
#	define _LUA_TINKER_PUSH_MEMBER_FUNCTOR_

#		include <boost/preprocessor/enum_params.hpp>

#		define BOOST_PP_ITERATION_LIMITS	( 0, LUA_TINKER_FUNCTOR_ARG )
#		define BOOST_PP_FILENAME_1			"lua_tinker_push_member_functor.h"
#		include BOOST_PP_ITERATE()

#		undef BOOST_PP_FILENAME_1
#		undef BOOST_PP_ITERATION_LIMITS

#	endif

#else

#	define n	BOOST_PP_ITERATION()

template<typename RVal, typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)>
void push_functor(lua_State *L, RVal (T::*func)(BOOST_PP_ENUM_PARAMS(n, T)) ) 
{ 
	lua_pushcclosure(L, mem_functor<T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, T)>::invoke<RVal>, 1); 
}

template<typename RVal, typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)>
void push_functor(lua_State *L, RVal (T::*func)(BOOST_PP_ENUM_PARAMS(n, T)) const) 
{ 
	lua_pushcclosure(L, mem_functor<T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, T)>::invoke<RVal>, 1); 
}
#endif
