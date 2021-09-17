/**
 * Partially imported from libstdc++ system implementation
 */

/// integral_constant
template<typename _Tp, _Tp __v>
struct integral_constant
{
    static constexpr _Tp value = __v;
    typedef _Tp value_type;
    typedef integral_constant<_Tp, __v> type;
    constexpr operator value_type() const noexcept
    {
        return value;
    }
#if __cplusplus > 201103L

#define __cpp_lib_integral_constant_callable 201304

    constexpr value_type operator()() const noexcept
    {
        return value;
    }
#endif
};

template<typename _Tp, _Tp __v>
constexpr _Tp integral_constant<_Tp, __v>::value;

/// The type used as a compile-time boolean with true value.
typedef integral_constant<bool, true> true_type;

/// The type used as a compile-time boolean with false value.
typedef integral_constant<bool, false> false_type;

/// remove_cv
template<typename _Tp>
struct remove_cv
{
    using type = _Tp;
};

template<typename _Tp>
struct remove_cv<const _Tp>
{
    using type = _Tp;
};

template<typename _Tp>
struct remove_cv<volatile _Tp>
{
    using type = _Tp;
};

template<typename _Tp>
struct remove_cv<const volatile _Tp>
{
    using type = _Tp;
};

// __remove_cv_t (std::remove_cv_t for C++11).
template<typename _Tp>
using __remove_cv_t = typename remove_cv<_Tp>::type;

template<typename>
struct __is_integral_helper : public false_type
{};

template<>
struct __is_integral_helper<bool> : public true_type
{};

template<>
struct __is_integral_helper<char> : public true_type
{};

template<>
struct __is_integral_helper<signed char> : public true_type
{};

template<>
struct __is_integral_helper<unsigned char> : public true_type
{};

// We want is_integral<wchar_t> to be true (and make_signed/unsigned to work)
// even when libc doesn't provide working <wchar.h> and related functions,
// so check __WCHAR_TYPE__ instead of _GLIBCXX_USE_WCHAR_T.
#ifdef __WCHAR_TYPE__
template<>
struct __is_integral_helper<wchar_t> : public true_type
{};
#endif

#ifdef _GLIBCXX_USE_CHAR8_T
template<>
struct __is_integral_helper<char8_t> : public true_type
{};
#endif

template<>
struct __is_integral_helper<char16_t> : public true_type
{};

template<>
struct __is_integral_helper<char32_t> : public true_type
{};

template<>
struct __is_integral_helper<short> : public true_type
{};

template<>
struct __is_integral_helper<unsigned short> : public true_type
{};

template<>
struct __is_integral_helper<int> : public true_type
{};

template<>
struct __is_integral_helper<unsigned int> : public true_type
{};

template<>
struct __is_integral_helper<long> : public true_type
{};

template<>
struct __is_integral_helper<unsigned long> : public true_type
{};

template<>
struct __is_integral_helper<long long> : public true_type
{};

template<>
struct __is_integral_helper<unsigned long long> : public true_type
{};

template<typename>
struct __is_floating_point_helper : public false_type
{};

template<>
struct __is_floating_point_helper<float> : public true_type
{};

template<>
struct __is_floating_point_helper<double> : public true_type
{};

template<>
struct __is_floating_point_helper<long double> : public true_type
{};

/// is_integral
template<typename _Tp>
struct is_integral : public __is_integral_helper<__remove_cv_t<_Tp>>::type
{};

/// is_floating_point
template<typename _Tp>
struct is_floating_point : public __is_floating_point_helper<__remove_cv_t<_Tp>>::type
{};
