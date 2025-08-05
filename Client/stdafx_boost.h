#pragma once

#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/multiplies.hpp>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/random.hpp>
#include <boost/foreach.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_array.hpp>
#include <boost/scope_exit.hpp>
#include <boost/signal.hpp>
#include <boost/signals2.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>	//	숫자를 문자열로, 문자열을 숫자로 바꿀때 편하다.
#include <boost/circular_buffer.hpp>    // 환형큐

#pragma warning( push )
#pragma warning( disable:4800 )
#include <boost/pool/pool_alloc.hpp>
#pragma warning( pop )