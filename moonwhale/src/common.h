#pragma once
#include <cstdint>

using uint = unsigned int;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i32 = int32_t;

namespace common
{

#ifndef assert
#define assert(expression) do {if(!(expression)) {int* _assertionHappened_ = nullptr; *_assertionHappened_ = 0;}} while(0)
#else
#error assert macro is already defined
#endif

template <typename TArr, size_t N>
inline constexpr size_t arrayCount(TArr(&)[N])
{
    return N;
}

template <typename T>
struct Result
{
    bool success;
    T value;
};

#define noncopyable(type) \
type(const type&) = delete; \
type& operator=(const type&) = delete;

inline constexpr u64 kilobytes(u64 n) { return n * 1000LL; }
inline constexpr u64 megabytes(u64 n) { return kilobytes(n) * 1000LL; }
inline constexpr u64 gigabytes(u64 n) { return megabytes(n) * 1000LL; }
inline constexpr u64 terabytes(u64 n) { return gigabytes(n) * 1000LL; }

template <typename F>
class ScopeExit
{
public:
    F m_f;
    ScopeExit(F f) : m_f(f) {}
    ~ScopeExit() { m_f(); }
};
template <typename F>
inline ScopeExit<F> makeScopeExit(F f) { return ScopeExit<F>(f); }
#define make_scope_exit_name(prefix, line) make_scope_exit_name2(prefix, line)
#define make_scope_exit_name2(prefix, line) prefix ## line
#define scope_exit(code) \
    auto make_scope_exit_name(temp___scopeExit, __LINE__) = k::common::makeScopeExit([&](){code;})
}