#pragma once

namespace MtlCpp
{
	template <typename T>
	struct remove_reference
	{
		using type = T;
	};

	template <typename T>
	struct remove_reference<T&>
	{
		using type = T;
	};

	template <typename T>
	struct remove_reference<T&&>
	{
		using type = T;
	};

	template <typename T>
	using remove_reference_t = typename remove_reference<T>::type;

	template <typename T>
	constexpr T&& forward(remove_reference_t<T>& t) noexcept
	{
		return static_cast<T&&>(t);
	}

	template <typename T>
	constexpr T&& forward(remove_reference_t<T>&& t) noexcept
	{
		return static_cast<T&&>(t);
	}

	template <class T>
	struct remove_cv
	{
		typedef T type;
	};

	template <class T>
	struct remove_cv<T const>
	{
		typedef T type;
	};

	template <class T>
	struct remove_cv<T volatile>
	{
		typedef T type;
	};

	template <class T>
	struct remove_cv<T const volatile>
	{
		typedef T type;
	};

	template <bool B, class T = void>
	struct enable_if
	{};

	template <class T>
	struct enable_if<true, T>
	{
		typedef T type;
	};

	template <bool B, class T = void>
	using enable_if_t = typename enable_if<B, T>::type;

	template <typename T>
	constexpr bool is_void_v = __is_void(T);

	template <typename T1, typename T2>
	constexpr bool is_convertible_v = __is_convertible(T1, T2);
}