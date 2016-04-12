#pragma once
#include <type_traits>

namespace std
{
template<class...> class tuple;
}

namespace BMPL {
template <class...> struct Typelist;

namespace impl {
template <class... TLs> struct Merge;
template <class TL, class... Ts> struct AddBefore;
template <class TL, class... Ts> struct AddAfter;
template <class TL, class Ty> struct EraseFirstOf;
template <class TL, class Ty> struct EraseAllOf;
template <class TL, class Ty> struct Count;
template <class TL, class... Ts> struct ContainsAll;
template <class TTypelist, class... Checks> struct ContainsAll;
template <class TL, template<class> class Mapper> struct Map;
template <class TL, template<class> class Filt> struct Filter;
template <class TL> struct UniqueElements;
template <size_t RepeatN, class... Ts> struct Repeat;
template <class TL> struct TupleType;

template <class... Ts1, class... Ts2>
struct AddBefore<Typelist<Ts1...>, Ts2...>
{
	using Result = Typelist<Ts2..., Ts1...>;
};

template <class... Ts1, class... Ts2>
struct AddAfter<Typelist<Ts1...>, Ts2...>
{
	using Result = Typelist<Ts1..., Ts2...>;
};

template <class... Ts, class Erase>
struct EraseFirstOf<Typelist<Erase, Ts...>, Erase>
{
	using Result = Typelist<Ts...>;
};

template <class Ty, class... Ts, class Erase>
struct EraseFirstOf<Typelist<Ty, Ts...>, Erase>
{
	using Result = typename AddBefore<
		typename EraseFirstOf<Typelist<Ts...>, Erase>::Result,
		Ty>::Result;
};

template <class Ty, class Erase>
struct EraseAllOf<Typelist<Ty>, Erase>
{
	using Result = Typelist<Ty>;
};

template <class Ty, class Erase>
struct EraseAllOf<Typelist<Ty, Erase>, Erase>
{
	using Result = Typelist<Ty>;
};

template <class... Ts, class Erase>
struct EraseAllOf<Typelist<Erase, Ts...>, Erase>
{
	using Result = typename EraseAllOf<Typelist<Ts...>, Erase>::Result;
};

template <class Ty, class... Ts, class Erase>
struct EraseAllOf<Typelist<Ty, Ts...>, Erase>
{
	using Result = typename AddBefore<
		typename EraseAllOf<Typelist<Ts...>, Erase>::Result,
		Ty>::Result;
};

template <class... Ts1, class... Ts2>
struct Merge<Typelist<Ts1...>, Typelist<Ts2...>>
{
	using Result = Typelist<Ts1..., Ts2...>;
};

template <class... Ts1, class... Ts2, class... TLs>
struct Merge<Typelist<Ts1...>, Typelist<Ts2...>, TLs...>
{
	using Result = typename Merge<Typelist<Ts1..., Ts2...>, TLs...>::Result;
};

template <class T, class Ty>
struct Count<Typelist<T>, Ty>
{
	static constexpr size_t value = std::is_same_v<T, Ty>;
};

template <class T, class... Ts, class Ty>
struct Count<Typelist<T, Ts...>, Ty>
{
	static constexpr size_t value = std::is_same_v<T, Ty> + 
		Count<Typelist<Ts...>, Ty>::value;
};

template <class TL, class Check>
struct ContainsAll<TL, Check>
{
	static constexpr bool value = Count<TL, Check>::value;
};

template <class TL, class Check, class... Checks>
struct ContainsAll<TL, Check, Checks...>
{
	static constexpr bool value = Count<TL, Check>::value != 0
		? ContainsAll<TL, Checks...>::value
		: false;
};

template <class... Ts, template<class> class Mapper>
struct Map<Typelist<Ts...>, Mapper>
{
	using Result = Typelist<typename Mapper<Ts>::type...>;
};

template <class Ty, template<class> class Filt>
struct Filter<Typelist<Ty>, Filt>
{
	using Result = std::conditional_t<Filt<Ty>::value, Typelist<Ty>, Typelist<>>;
};

template <class Ty, class... Ts, template<class> class Filt>
struct Filter <Typelist<Ty, Ts...>, Filt>
{
	using Next = typename Filter<Typelist<Ts...>, Filt>::Result;
	using Result = std::conditional_t<Filt<Ty>::value,
		typename AddBefore<Next, Ty>::Result, 
		Next>;
};

template <class Ty>
struct UniqueElements<Typelist<Ty>>
{
	using Result = Typelist<Ty>;
};

template <class Ty, class... Ts>
struct UniqueElements<Typelist<Ty, Ts...>>
{
	using Result = typename AddBefore<
		typename UniqueElements<
		typename EraseAllOf<Typelist<Ts...>, Ty>::Result
		>::Result, Ty>::Result;
};

template <class... Ts>
struct Repeat<1, Ts...>
{
	using Result = Typelist<Ts...>;
};

template <size_t RepeatN, class... Ts>
struct Repeat
{
	static_assert(RepeatN > 1, "Cannot instantiate less than 1 times");
	using Result = typename Merge<Typelist<Ts...>, 
		typename Repeat<RepeatN - 1, Ts...>::Result>::Result;
};

template <class... Ts>
struct TupleType<Typelist<Ts...>>
{
	using Result = std::tuple<Ts...>;
};

} //namespace impl


//Interface: Alias and variable templates
template <class TL, class Erase>
using EraseFirstOf = typename impl::EraseFirstOf <TL, Erase> ::Result;

template <class TL, class Erase>
using EraseAllOf = typename impl::EraseAllOf<TL, Erase>::Result;

template <class... TLs>
using Merge = typename impl::Merge<TLs...>::Result;

template <class TL, class Ty>
static constexpr size_t Count = impl::Count<TL, Ty>::value;

template <class TL, class... Ts>
static constexpr bool Contains = impl::ContainsAll<TL, Ts...>::value;

template <class TL, class... Ts>
static constexpr bool Contains<TL, Typelist<Ts...>> = 
	impl::ContainsAll<TL, Ts...>::value;

template <class TL, template<class> class Mapper>
using Map = typename impl::Map<TL, Mapper>::Result;

template <class TL, template<class> class Filt>
using Filter = typename impl::Filter<TL, Filt>::Result;

template <class TL>
using UniqueElements = typename impl::UniqueElements<TL>::Result;

template <size_t RepeatN, class... Ts>
using Repeat = typename impl::Repeat<RepeatN, Ts...>::Result;

template <class TL>
using TupleType = typename impl::TupleType<TL>::Result;
}//namespace BMPL