#ifndef CHAINED_COMPARSIONS_H_
#define CHAINED_COMPARSIONS_H_

#include <utility>
#include <type_traits>


namespace chained_comparisons {
    template<class T>
    class chained_comparisons;

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
    class chained_comparisons {
    public:
        T&& of;
        bool value = true;
        explicit constexpr chained_comparisons(T&& of_, bool value_ = true) noexcept : of(::std::forward<T>(of_)), value(value_) {};

// Helper macro that expands DEFINE_CHAINED_COMPARISON_OPERATOR for all comparison operators
#define DEFINE_ALL_CHAINED_COMPARISON_OPERATORS \
    DEFINE_CHAINED_COMPARISON_OPERATOR(==) \
    DEFINE_CHAINED_COMPARISON_OPERATOR(!=) \
    DEFINE_CHAINED_COMPARISON_OPERATOR(>) \
    DEFINE_CHAINED_COMPARISON_OPERATOR(<) \
    DEFINE_CHAINED_COMPARISON_OPERATOR(>=) \
    DEFINE_CHAINED_COMPARISON_OPERATOR(<=)

// Define member functions to chain comparisons (1) generally and (2) with other chained_comparison<U>
#define DEFINE_CHAINED_COMPARISON_OPERATOR(op) \
        template<class U> \
        constexpr \
        chained_comparisons<U> operator op (U&& other) const \
        noexcept(noexcept(static_cast<bool>(::std::forward<T>(of) op ::std::forward<U>(other)))) { \
            return chained_comparisons<U>{ ::std::forward<U>(other), value && static_cast<bool>(::std::forward<T>(of) op ::std::forward<U>(other)) }; \
        } \
\
        template<class U> \
        constexpr \
        chained_comparisons<U> operator op (const chained_comparisons<U>& other) const \
        noexcept(noexcept(static_cast<bool>(::std::forward<T>(of) op ::std::forward<U>(other.of)))) { \
            return chained_comparisons<U>{ other.of, value && other.value && static_cast<bool>(::std::forward<T>(of) op ::std::forward<U>(other.of)) };\
        }

        DEFINE_ALL_CHAINED_COMPARISON_OPERATORS

#undef DEFINE_CHAINED_COMPARISON_OPERATOR

        // Convert to bool implicitly
        constexpr operator bool() const noexcept { return value; }
        constexpr bool operator!() const noexcept { return !value; }
    };

// Define functions to be able to have the chain_comparison<T> on the right side as well
#define DEFINE_CHAINED_COMPARISON_OPERATOR(op) template<class T, class U> \
    constexpr \
    typename ::std::enable_if<!detail::is_chained_comparison<U>::value, chained_comparisons<T>>::type \
    operator op (U&& other, const chained_comparisons<T>& self) \
    noexcept(noexcept(static_cast<bool>(::std::forward<U>(other) op ::std::forward<T>(self.of)))) { \
        return chained_comparisons<T>{ ::std::forward<T>(self.of), self.value && static_cast<bool>(::std::forward<U>(other) op ::std::forward<T>(self.of)) }; \
    }

    DEFINE_ALL_CHAINED_COMPARISON_OPERATORS

#undef DEFINE_CHAINED_COMPARISON_OPERATOR
#undef DEFINE_ALL_CHAINED_COMPARISON_OPERATORS

    struct C {
        template<class T>
        constexpr chained_comparisons<T> operator()(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of) };
        }

        // Alternative to c(...) is c >> ..., as >> has higher precedence
        // than comparison operators, and nothing has to go to the right of the first value.
        // And it looks like the rhs is being "imbued" with the ability to chain comparison.
        template<class T>
        constexpr chained_comparisons<T> operator>>(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of) };
        }

        // Provided to silence compilers that warn about precedence in "A >> B < C".
        // Use `A / B < C` or `A * B < C` instead.
        template<class T>
        constexpr chained_comparisons<T> operator/(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of) };
        }

        template<class T>
        constexpr chained_comparisons<T> operator*(T&& of) const noexcept {
            return chained_comparisons<T>{ ::std::forward<T>(of) };
        }
    };

    // c is a shorthand helper to begin chained comparisons.
    // c >> X <= Y <= Z, c / X <= Y <= Z, or c(X) <= Y <= Z.
    static constexpr const C c;
}

#endif
