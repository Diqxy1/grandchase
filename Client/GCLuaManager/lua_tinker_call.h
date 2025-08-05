#ifndef BOOST_PP_IS_ITERATING
#ifndef _LUA_TINKER_CALL_
#define _LUA_TINKER_CALL_

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define _POP_ARG( z, n, unused )	BOOST_PP_CAT(T,n) BOOST_PP_CAT( arg,n )
#define _PUSH_PARAM( r, state )     push(L, BOOST_PP_CAT(arg, BOOST_PP_TUPLE_ELEM( 2, 0, state ) ) );
#define _FOR_PRED( r, state )       BOOST_PP_NOT_EQUAL( BOOST_PP_TUPLE_ELEM( 2, 0, state ), BOOST_PP_INC( BOOST_PP_TUPLE_ELEM( 2, 1, state)))
#define _FOR_OP( r, state )         (BOOST_PP_INC(BOOST_PP_TUPLE_ELEM( 2, 0, state ) ), BOOST_PP_TUPLE_ELEM( 2, 1, state))
#define _FIRST_ITERATION

#define BOOST_PP_ITERATION_LIMITS	(0, LUA_TINKER_FUNCTOR_ARG)
#define BOOST_PP_FILENAME_1			"lua_tinker_call.h"
#include BOOST_PP_ITERATE()

#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS
#undef _POP_ARG
#undef _PUSH_PARAM
#undef _FOR_PRED
#undef _FOR_OP

#endif

#else

#define n	BOOST_PP_ITERATION()

template<typename RVal BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T)>
RVal call(lua_State* L, const char* name BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM( n, _POP_ARG, ~ ) )
{
    lua_pushcclosure(L, on_error, 0);
    int errfunc = lua_gettop(L);

    lua_pushstring(L, name);
    lua_gettable(L, LUA_GLOBALSINDEX);
    if(lua_isfunction(L,-1))
    {
        BOOST_PP_FOR( (0, BOOST_PP_DEC(n) ), _FOR_PRED, _FOR_OP, _PUSH_PARAM )
        if(lua_pcall(L, n, 1, errfunc) != 0)
        {
            lua_pop(L, 2);
            return RVal();
        }
    }
    else
    {
        print_error(L, "lua_tinker::call() attempt to call global `%s' (not a function)", name);
        lua_pop( L, 2 );
        return RVal();
    }

    lua_remove(L, -2);
    return pop<RVal>(L);
}
#endif
