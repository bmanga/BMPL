#include "Typelist2.h"
#include "TLAlgorithm.h"
#include <iostream>
#include <cassert>
#include <typeinfo>
#include <type_traits>

template <class TL, class T>
struct foo
{
	void operator()() const
	{
		std::cout << "not a chance" << std::endl;
		std::cout << typeid(T).name() << std::endl;
	}
};

template <class T>
using foo2 = foo<BMPL::Typelist<int, char>, T>;


int main()
{
	using namespace BMPL;

	using tl = BMPL::Typelist<int, float, double, char, bool, int>;
	using ttl = Typelist<bool, char, float, int, double, char, int, bool, bool, unsigned>;

	using tt = UniqueElements<ttl>;


	impl::ForEachType<tt, foo2>{};
	//std::cout << tt::size << std::endl;
	using tl3 = EraseFirstOf<tl, int>;
	
	//std::cout << Size<tl>;

	using tl2 = Typelist<int,float, char>;
	//std::cout << SizeDeep<tl2>;
	using tl4 = Merge<tl, tl3, tl2>;
	using tl6 = Filter<tl, std::is_integral>;
	//std::cout << tl6::size << std::endl;
	//using t3 = EraseFirstOf<tl2, char>;
	//std::cout << tl3::size << std::endl;
	//std::cout << tl4::size << std::endl;
	//std::cout << Contains<tl4, tl2> << std::endl;

	using tl5 = Map<tl4, std::add_const>;
	static_assert(std::is_same_v<tl5::TypeAt<0>, const int>, "");
	//std::cout << Size<t4> << std::endl;
	//std::cout << typeid(tl3::TypeAt<0>).name() << std::endl;
	//assert(typeid(BMPL::TypeAt<tl, 2>) == typeid(double));
	system("PAUSE");
}
