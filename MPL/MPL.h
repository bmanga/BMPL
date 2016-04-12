#pragma once
#include "Typelist.h"
#include "TLAlgorithm.h"

namespace BMPL {
namespace impl {
template <class TL, template<class> class Fn> struct ForEachType;
template <class... Ts, template<class> class Fn>
struct ForEachType<Typelist<Ts...>, Fn>
{
	static void execute()
	{
		return (void)std::initializer_list<int> { (Fn<Ts>{}(), 0)... };
	}
};

}

template <class TL, template<class> class Fn>
constexpr void ForEachType()
{
	return (void)impl::ForEachType<TL, Fn>::execute();
}
}