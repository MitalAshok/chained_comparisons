# chained_comparisons

Adds relatively simple chained comparison (`X<=Y<=Z`) support for C++ 11

In C++, `A < B < C` is parsed as `(A < B) < C`, where you may be expecting `(A < B) && (B < C)`. This library fixes that.

The only important symbol exported is `::chained_comparisons::c`.

The usage is very simple. For the first or the second value being compared in a chain of `==`, `!=`, `<`, `>`, `<=` or `>=` comparisons, call `c(value)` (Or `c >> value`, `c / value` or `c * value`)

For example:

```cpp
bool is_in_range(int x) {
    using chained_comparisons::c;
    return c(0) <= x < 100;
}

bool rounds_down(double x) {
    using chained_comparisons::c;
    return c * 0 <= std::fmod(x, 1.) < 0.5;
}
```

You can use a different name by creating a `::chained_comparisons::C` object (recommended declaration: `static constexpr chained_comparisons::C my_chain_alias;`)

## Usage

This is a single header library. Download [include/chained_comparisons.h](include/chained_comparisons.h) and include it in your program. No extra compilation is required.

Works with C++11 and above.

## Note

Any half-decent compiler will optimise `c(a) < b < c` and `(a < b) && (b < c)` to the same thing. This library is `constexpr` friendly.

The `-Wparentheses` warning flag in GCC will warn when chained comparisons are used. Disable this warning with `-no-parentheses` or in a local section of code with:

```cpp
#ifdef __GNUC__
#pragma GCC diagnostic push
// Disable warning produced by X<=Y<=Z
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

// Code that uses chained comparisons

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
```

Or surround your code in the helper macros that do this for you:

```cpp
CHAINED_COMPARISONS_START

// Code that uses chained comparisons

CHAINED_COMPARISONS_END
```

Note that there is no semicolon after the macros, and they can be used in any position (including within an expression)
