/* Copyright (C) 2017 Roman Koshelev */
#pragma once
#include <type_traits>
#include <cinttypes>

template <std::uint32_t Index>
using uint32_t_ = std::integral_constant<std::uint32_t, Index >;


template <class T> struct identity {
	using type = T;
};


#define REGISTER_TYPE_1(Type, Index)              \
    constexpr std::uint32_t type_to_id( identity<Type> ) noexcept { \
        return Index;                                           \
    }                                                           \
   /* constexpr auto id_to_type( uint32_t_<Index > ) noexcept {     \
        return identity<Type>{};                        \
    }    */                                                       \
    /**/

template <class... Types>
struct promotion;

template <class FirstType, class... Tail>
struct promotion<FirstType, Tail...>
{
	template <class Fun>
	static bool exec(const int k, Fun fun)
	{
		return promotion<FirstType>::exec(k, fun) || promotion<Tail...>::exec(k, fun);
	}
};

template <class FirstType>
struct promotion<FirstType>
{
	template <class Fun>
	static bool exec(const int k, Fun fun)
	{
		if (type_to_id(identity<FirstType>{}) == k)
		{
			fun(FirstType{});
			return true;
		}
		return false;
	}
};