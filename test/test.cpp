#include "chained_comparisons.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <type_traits>

#ifdef __GNUC__
#pragma GCC diagnostic push
// Disable warning produced by X<=Y<=Z
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#define STREAM_WRITE(STREAM, STRING_LITERAL) \
    STREAM.write(STRING_LITERAL, sizeof(STRING_LITERAL) - static_cast<::std::size_t>(1))
#undef ASSERT
#define ASSERT(message, ...) do { \
    if (!static_cast<bool>(__VA_ARGS__)) { \
        STREAM_WRITE(::std::cerr, "Assertion failed: "); \
        ::std::cerr << message; \
        ::std::cerr.put('\n'); \
        ::std::cerr.flush(); \
        failed = true; \
    } \
} while (false)
#undef ASSERT_EQUAL
#define ASSERT_EQUAL(T, U) do { \
    if (!static_cast<bool>((T) == (U))) { \
        STREAM_WRITE(::std::cerr, "Assertion failed: (" #T ") should equal (" #U ") but does not.\n" #T " is: ("); \
        ::std::cerr << typeid(T).name(); \
        STREAM_WRITE(::std::cerr, ") "); \
        ::std::cerr << (T); \
        STREAM_WRITE(::std::cerr, "\n" #U " is: ("); \
        ::std::cerr << typeid(U).name(); \
        STREAM_WRITE(::std::cerr, ") "); \
        ::std::cerr << (U); \
        ::std::cerr.flush(); \
        ::std::cerr.put('\n'); \
        failed = true; \
    } \
} while (false)
#undef ASSERT_NOT_EQUAL
#define ASSERT_NOT_EQUAL(T, U) do { \
    if ((T) == (U)) { \
        STREAM_WRITE(::std::cerr, "Assertion failed: (" #T ") should not equal (" #U ") but does.\n" #T " is: ("); \
        ::std::cerr << typeid(T).name(); \
        STREAM_WRITE(::std::cerr, ") "); \
        ::std::cerr << (T); \
        STREAM_WRITE(::std::cerr, "\n" #U " is: ("); \
        ::std::cerr << typeid(U).name(); \
        STREAM_WRITE(::std::cerr, ") "); \
        ::std::cerr << (U); \
        ::std::cerr.put('\n'); \
        ::std::cerr.flush(); \
        failed = true; \
    } \
} while (false)

static bool is_in_range(int x) {
    using chained_comparisons::c;
    return c(0) <= x < 100;
}

static bool is_in_range_2(int x) {
    using chained_comparisons::c;
    return c / 100 > x > 0;
}

static bool rounds_down(double x) {
    using chained_comparisons::c;
    return c * 0 <= std::fmod(x, 1.) < 0.5;
}

static bool alias_chain(int x, int y) {
    constexpr chained_comparisons::C chain_alias;
    return chain_alias >> x == y == 1;
}

static constexpr bool constexpr_test(int x) {
    using chained_comparisons::c;
    return c(0) <= x < 100;
}

int main() {
    using ::chained_comparisons::c;

    ::std::boolalpha(::std::cout);

    bool failed = false;

    ASSERT_EQUAL(is_in_range(-1), false);
    ASSERT_EQUAL(is_in_range(0), true);
    ASSERT_EQUAL(is_in_range(1), true);
    ASSERT_EQUAL(is_in_range(99), true);
    ASSERT_EQUAL(is_in_range(100), false);

    ASSERT_EQUAL(is_in_range_2(-1), false);
    ASSERT_EQUAL(is_in_range_2(0), false);
    ASSERT_EQUAL(is_in_range_2(1), true);
    ASSERT_EQUAL(is_in_range_2(99), true);
    ASSERT_EQUAL(is_in_range_2(100), false);

    ASSERT_EQUAL(rounds_down(0.), true);
    ASSERT_EQUAL(rounds_down(1.), true);
    ASSERT_EQUAL(rounds_down(0.5), false);
    ASSERT_EQUAL(rounds_down(1.5), false);

    ASSERT_EQUAL(alias_chain(1, 1), true);
    ASSERT_EQUAL(alias_chain(0, 1), false);
    ASSERT_EQUAL(alias_chain(1, 0), false);
    ASSERT_EQUAL(alias_chain(0, 0), false);
    ASSERT_EQUAL(alias_chain(2, 2), false);

    ASSERT_EQUAL((std::integral_constant<bool, constexpr_test(0)>::value), true);

    return failed ? static_cast<int>(EXIT_FAILURE) : static_cast<int>(EXIT_SUCCESS);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
