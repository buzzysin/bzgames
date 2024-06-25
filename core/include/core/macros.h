#pragma once

#define BZ_PLATFORM_UNIX 0b001
#define BZ_PLATFORM_WINDOWS 0b010
#define BZ_PLATFORM_MACOS 0b100

// Detect platform
#if defined(_WIN32) || defined(_WIN64)
#define BZ_PLATFORM BZ_PLATFORM_WINDOWS
#elif defined(__unix__) && !defined(__APPLE__)
#define BZ_PLATFORM BZ_PLATFORM_UNIX
#elif defined(__APPLE__)
#define BZ_PLATFORM BZ_PLATFORM_MACOS
#else
#error "Unsupported platform"
#endif

// #define bz_platform(X) (BZ_PLATFORM & X)
#define BZ_PLATFORM_(X) (BZ_PLATFORM & BZ_PLATFORM_##X)
constexpr bool bz_platform(int platform) { return BZ_PLATFORM & platform; }

// Detect compiler
#if defined(_MSC_VER)
#define BZ_COMPILER_MSVC
#elif defined(__GNUC__)
#define BZ_COMPILER_GCC
#elif defined(__clang__)
#define BZ_COMPILER_CLANG
#else
#error "Unsupported compiler"
#endif

template <typename... Args>
void bz_ignored(Args &&.../*unused*/) {}
#define BZ_IGNORED(...) bz_ignored(__VA_ARGS__)

#if defined(BZ_COMPILER_MSVC)
#define BZ_FORCE_INLINE __forceinline
#define BZ_NO_INLINE __declspec(noinline)
#define BZ_NO_VTABLE __declspec(novtable)
#define BZ_RESTRICT __restrict
#define BZ_ALIGN(X) __declspec(align(X))
#define BZ_DEPRECATED(X) __declspec(deprecated(X))
#define BZ_UNUSED __declspec(selectany)
#define BZ_NODISCARD _NODISCARD
#define BZ_LIKELY(X) X
#define BZ_UNLIKELY(X) X
#define BZ_IMPORT __declspec(dllimport)
#define BZ_EXPORT __declspec(dllexport)
#elif defined(BZ_COMPILER_GCC) || defined(BZ_COMPILER_CLANG)
#define BZ_FORCE_INLINE __attribute__((always_inline))
#define BZ_NO_INLINE __attribute__((noinline))
#define BZ_NO_VTABLE
#define BZ_RESTRICT __restrict
#define BZ_ALIGN(X) __attribute__((aligned(X)))
#define BZ_DEPRECATED(X) __attribute__((deprecated(X)))
#define BZ_UNUSED __attribute__((unused))
#define BZ_NODISCARD __attribute__((warn_unused_result))
#define BZ_LIKELY(X) __builtin_expect(!!(X), 1)
#define BZ_UNLIKELY(X) __builtin_expect(!!(X), 0)
#define BZ_IMPORT
#define BZ_EXPORT
#endif

// Detect architecture

#define BZ_ARCH_32 0b01
#define BZ_ARCH_64 0b10

// Detect architecture
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
#define BZ_ARCH BZ_ARCH_64
#else
#define BZ_ARCH BZ_ARCH_32
#endif

// Detect language standard
#if defined(__cplusplus)
#if __cplusplus == 199711L
#define BZ_CPP_98
#elif __cplusplus == 201103L
#define BZ_CPP_11
#elif __cplusplus == 201402L
#define BZ_CPP_14
#elif __cplusplus == 201703L
#define BZ_CPP_17
#elif __cplusplus == 202002L
#define BZ_CPP_20
#else
#error "Unsupported language standard"
#endif
#else
#error "Unsupported language standard"
#endif

// Detect debug mode
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(NDEBUG)
#define BZ_DEBUG
#else
#define BZ_RELEASE
#endif
