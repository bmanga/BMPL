#pragma once

namespace BMPL {

template <class... Ts> struct Typelist;

namespace impl {

template <class TL, size_t Idx, size_t Cnt> struct TypeAt;
template <class TL, class Ty> struct IndexOf;


template <class Ty, class... Ts, size_t Idx>
struct TypeAt<Typelist<Ty, Ts...>, Idx, Idx>
{
	using type = Ty;
};

template <class Ty, class... Ts, size_t Idx, size_t Cnt>
struct TypeAt<Typelist<Ty, Ts...>, Idx, Cnt>
{
	using type = typename TypeAt<Typelist<Ts...>, Idx, Cnt + 1>::type;
};

template <class... Ts, class Ty>
struct IndexOf<Typelist<Ty, Ts...>, Ty>
{
	static constexpr size_t value = 0;
};

template <class T, class... Ts, class Ty>
struct IndexOf<Typelist<T, Ts...>, Ty>
{
	static constexpr size_t value = 1 + IndexOf<Typelist<Ts...>, Ty>::value;
};

}// namespace impl
template <class... Ts>
struct Typelist
{
	static constexpr size_t size = sizeof... (Ts);

	template <size_t Idx>
	using TypeAt = typename impl::TypeAt<Typelist<Ts...>, Idx, 0>::type;

	template <class Ty>
	static constexpr size_t indexOf = impl::IndexOf<Typelist<Ts...>, Ty>::value;
};


}