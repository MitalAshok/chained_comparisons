#ifndef CHAINED_COMPARSIONS_H_
#define CHAINED_COMPARSIONS_H_

#include <utility>
#include <type_traits>

#ifdef __GNUC__
#define CHAINED_COMPARISONS_START _Pragma("GCC diagnostics push") _Pragma("GCC diagnostic ignored \"-Wparentheses\"")  /* TODO: check this */
#define CHAINED_COMPARISONS_END _Pragma("GCC diagnostics pop")
#else
#define CHAINED_COMPARISONS_START
#define CHAINED_COMPARISONS_END
#endif


namespace chained_comparisons {
    template<class T>
    struct chained_comparisons;

    namespace detail {
        template<class T>
        struct is_chained_comparison_helper : ::std::false_type::type {};

        template<class T>
        struct is_chained_comparison_helper<chained_comparisons<T>> : ::std::true_type::type {};

        template<class T>
        struct is_chained_comparison : is_chained_comparison_helper<typename ::std::remove_cv<typename ::std::remove_reference<T>::type>::type>::type {};
    }

    // This class holds a reference and whether the comparisons chained so far have
    // succeeded. The only way for a comparison with this class to succeed is if
    // all previous comparisons have succeeded and comparison with the reference succeeds.
    template<class T>
    struct chained_comparisons {
        T&& of;
        bool value;
        // explicit constexpr chained_comparisons(T&& of_, bool value_ = true) noexcept : of(::std::forward<T>(of_)), value(value_) {};

        // Convert to bool implicitly
        constexpr operator bool() const noexcept { return value; }
        constexpr bool operator!() const noexcept { return !value; }
    };

// Define functions to chain comparisons (1) generally and (2) with other chained_comparison<U>
#define DEFINE_CHAINED_COMPARISON_OPERATOR(op) \
    template<class T, class U> \
    constexpr \
    typename ::std::enable_if<!detail::is_chained_comparison<U>::value, chained_comparisons<U>>::type \
    operator op (chained_comparisons<T> l, U&& r) \
    noexcept(noexcept(static_cast<bool>(::std::forward<T>(l.of) op ::std::forward<U>(r)))) { \
        return chained_comparisons<U>{ ::std::forward<U>(r), l.value && static_cast<bool>(::std::forward<T>(l.of) op ::std::forward<U>(r)) }; \
    } \
\
    template<class T, class U> \
    constexpr \
    typename ::std::enable_if<!detail::is_chained_comparison<U>::value, chained_comparisons<T>>::type \
    operator op (U&& l, const chained_comparisons<T>& r) \
    noexcept(noexcept(static_cast<bool>(::std::forward<U>(l) op ::std::forward<T>(r.of)))) { \
        return chained_comparisons<T>{ ::std::forward<T>(r.of), r.value && static_cast<bool>(::std::forward<U>(l) op ::std::forward<T>(r.of)) }; \
    } \
\
    template<class T, class U> \
    constexpr \
    chained_comparisons<U> \
    operator op (chained_comparisons<T> l, chained_comparisons<U> r) \
    noexcept(noexcept(static_cast<bool>(::std::forward<T>(l.of) op ::std::forward<U>(r.of)))) { \
        return chained_comparisons<U>{ l.of, l.value && r.value && static_cast<bool>(::std::forward<T>(l.of) op ::std::forward<U>(r.of)) }; \
    }

    DEFINE_CHAINED_COMPARISON_OPERATOR(==)
    DEFINE_CHAINED_COMPARISON_OPERATOR(!=)
    DEFINE_CHAINED_COMPARISON_OPERATOR(>)
    DEFINE_CHAINED_COMPARISON_OPERATOR(<)
    DEFINE_CHAINED_COMPARISON_OPERATOR(>=)
    DEFINE_CHAINED_COMPARISON_OPERATOR(<=)

#undef DEFINE_CHAINED_COMPARISON_OPERATOR

    struct C {
        template<class T>
        constexpr chained_comparisons<T> operator()(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of), true };
        }

        // Alternative to c(...) is c >> ..., as >> has higher precedence
        // than comparison operators, and nothing has to go to the right of the first value.
        // And it looks like the rhs is being "imbued" with the ability to chain comparison.
        template<class T>
        constexpr chained_comparisons<T> operator>>(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of), true };
        }

        // Provided to silence compilers that warn about precedence in "A >> B < C".
        // Use `A / B < C` or `A * B < C` instead.
        template<class T>
        constexpr chained_comparisons<T> operator/(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of), true };
        }

        template<class T>
        constexpr chained_comparisons<T> operator*(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of), true };
        }
    };

    // c is a shorthand helper to begin chained comparisons.
    // c >> X <= Y <= Z, c / X <= Y <= Z, or c(X) <= Y <= Z.
    static constexpr const C c;
}

#endif
