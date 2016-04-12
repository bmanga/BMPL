#pragma once
#include "Typelist.h"
#include "TLAlgorithm.h"
#include <tuple>

namespace BMPL {
namespace impl {
template <class TL, template<class> class Fn> struct ForEachType;
template <class... Ts, template<class> class Fn>
struct ForEachType<Typelist<Ts...>, Fn>
{
	constexpr static void execute()
	{
		return (void)std::initializer_list<int> { (Fn<Ts>{}(), 0)... };
	}
};


template <class Fn, class TTuple, size_t...Is>
constexpr void ForEachInTuple(Fn&& fn, TTuple&& tuple,
	std::index_sequence<Is...>)
{
	return (void)std::initializer_list<int>
	{(std::forward<Fn>(fn)(std::get<Is>(tuple)), 0)...};
}

template <class Fn, class TTuple, size_t... Is>
constexpr decltype(auto) ApplyTuple(Fn&& fn, TTuple&& tuple,
	std::index_sequence<Is...>)
{
	return std::forward<Fn>(fn)(std::get<Is>(std::forward<TTuple>(tuple))...);
}

}//namespace impl

template <class TL, template<class> class Fn>
constexpr void ForEachType()
{
	return (void)impl::ForEachType<TL, Fn>::execute();
}

template <class Fn, class... Args>
constexpr void ForEachArg (Fn&& fn, Args&&... args)
{
	return (void)std::initializer_list<int>
	{
		((std::forward<Fn>(fn)(std::forward<Args>(args))), 0)...
	};
}

template <class Fn, class TTuple>
constexpr void ForEachInTuple(Fn&& fn, TTuple&& tuple)
{
	impl::ForEachInTuple(std::forward<Fn>(fn), std::forward<TTuple>(tuple),
		std::make_index_sequence<std::tuple_size<std::decay_t<TTuple>>::value>{});
}

//NOTE: there is a standard c++17 version (std::apply), replace soon
template <class Fn, class TTuple>
constexpr decltype(auto) ApplyTuple (Fn&& fn, TTuple&& tuple)
{
	return impl::ApplyTuple(std::forward<Fn>(fn), std::forward<TTuple>(tuple),
		std::make_index_sequence < std::tuple_size<std::decay_t<TTuple>>::value > {});
}
}