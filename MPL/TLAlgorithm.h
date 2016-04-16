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
template <class TL, template<class...> class> struct Rename;

template <class... Ts1, class... Ts2>
struct AddBefore<Typelist<Ts1...>, Ts2...>
{
	using type = Typelist<Ts2..., Ts1...>;
};

template <class... Ts1, class... Ts2>
struct AddAfter<Typelist<Ts1...>, Ts2...>
{
	using type = Typelist<Ts1..., Ts2...>;
};

template <class... Ts, class Erase>
struct EraseFirstOf<Typelist<Erase, Ts...>, Erase>
{
	using type = Typelist<Ts...>;
};

template <class Ty, class... Ts, class Erase>
struct EraseFirstOf<Typelist<Ty, Ts...>, Erase>
{
	using type = typename AddBefore<
		typename EraseFirstOf<Typelist<Ts...>, Erase>::type,
		Ty>::type;
};

template <class Ty, class Erase>
struct EraseAllOf<Typelist<Ty>, Erase>
{
	using type = Typelist<Ty>;
};

template <class Ty, class Erase>
struct EraseAllOf<Typelist<Ty, Erase>, Erase>
{
	using type = Typelist<Ty>;
};

template <class... Ts, class Erase>
struct EraseAllOf<Typelist<Erase, Ts...>, Erase>
{
	using type = typename EraseAllOf<Typelist<Ts...>, Erase>::type;
};

template <class Ty, class... Ts, class Erase>
struct EraseAllOf<Typelist<Ty, Ts...>, Erase>
{
	using type = typename AddBefore<
		typename EraseAllOf<Typelist<Ts...>, Erase>::type,
		Ty>::type;
};

template <class... Ts1, class... Ts2>
struct Merge<Typelist<Ts1...>, Typelist<Ts2...>>
{
	using type = Typelist<Ts1..., Ts2...>;
};

template <class... Ts1, class... Ts2, class... TLs>
struct Merge<Typelist<Ts1...>, Typelist<Ts2...>, TLs...>
{
	using type = typename Merge<Typelist<Ts1..., Ts2...>, TLs...>::type;
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
	using type = Typelist<typename Mapper<Ts>::type...>;
};

template <class Ty, template<class> class Filt>
struct Filter<Typelist<Ty>, Filt>
{
	using type = std::conditional_t<Filt<Ty>::value, Typelist<Ty>, Typelist<>>;
};

template <class Ty, class... Ts, template<class> class Filt>
struct Filter <Typelist<Ty, Ts...>, Filt>
{
	using Next = typename Filter<Typelist<Ts...>, Filt>::type;
	using type = std::conditional_t<Filt<Ty>::value,
		typename AddBefore<Next, Ty>::type, 
		Next>;
};

template <class Ty>
struct UniqueElements<Typelist<Ty>>
{
	using type = Typelist<Ty>;
};

template <class Ty, class... Ts>
struct UniqueElements<Typelist<Ty, Ts...>>
{
	using type = typename AddBefore<
		typename UniqueElements<
		typename EraseAllOf<Typelist<Ts...>, Ty>::type
		>::type, Ty>::type;
};

template <class... Ts>
struct Repeat<1, Ts...>
{
	using type = Typelist<Ts...>;
};

template <size_t RepeatN, class... Ts>
struct Repeat
{
	static_assert(RepeatN > 1, "Cannot instantiate less than 1 times");
	using type = typename Merge<Typelist<Ts...>, 
		typename Repeat<RepeatN - 1, Ts...>::type>::type;
};

template <class... Ts, template<class...>class RenameTo>
struct Rename<Typelist<Ts...>, RenameTo>
{
	using type = RenameTo<Ts...>;
};

} //namespace impl


//Interface: Alias and variable templates
template <class TL, class Erase>
using EraseFirstOf = typename impl::EraseFirstOf <TL, Erase> ::type;

template <class TL, class Erase>
using EraseAllOf = typename impl::EraseAllOf<TL, Erase>::type;

template <class... TLs>
using Merge = typename impl::Merge<TLs...>::type;

template <class TL, class Ty>
static constexpr size_t Count = impl::Count<TL, Ty>::value;

template <class TL, class... Ts>
static constexpr bool Contains = impl::ContainsAll<TL, Ts...>::value;

template <class TL, class... Ts>
static constexpr bool Contains<TL, Typelist<Ts...>> = 
	impl::ContainsAll<TL, Ts...>::value;

template <class TL, template<class> class Mapper>
using Map = typename impl::Map<TL, Mapper>::type;

template <class TL, template<class> class Filt>
using Filter = typename impl::Filter<TL, Filt>::type;

template <class TL>
using UniqueElements = typename impl::UniqueElements<TL>::type;

template <size_t RepeatN, class... Ts>
using Repeat = typename impl::Repeat<RepeatN, Ts...>::type;

template <class TL>
using TupleType = typename impl::Rename<TL, std::tuple>::type;

template <class TL, template<class...> class RenameTo>
using Rename = typename impl::Rename<TL, RenameTo>::type;
}//namespace BMPL