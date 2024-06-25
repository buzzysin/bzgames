#include "test.h"
#include "core/result_traits.h"

#include <memory>
#include <string_view>

namespace bz::core::tests {

TEST(TestResult, ResultWithValue) {
    constexpr const Result<int, std::string_view> result{42};

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_EQ(result, 42);
}

TEST(TestResult, ResultWithError) {
    constexpr const Result<int, std::string_view> result{"error"};

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_EQ(result.error(), "error");
}

TEST(TestResult, ResultWithMoveOnlyValue) {
    const Result<std::unique_ptr<int>, std::string_view> result{
        std::make_unique<int>(42)};

    EXPECT_TRUE(static_cast<bool>(result));
    EXPECT_EQ(**result, 42); // WARNING: ownership was transferred
}

TEST(TestResult, ResultWithMoveOnlyError) {
    const Result<int, std::unique_ptr<std::string_view>> result{
        std::make_unique<std::string_view>("error")};

    EXPECT_FALSE(static_cast<bool>(result));
    EXPECT_EQ(*result.error(), "error");
}

TEST(TestResult, ResultFreeTransform) {
    constexpr Result<int, std::string_view> result{42};

    const auto transform = [](int value) {
        return Result<int, std::string_view>(value * 2);
    };

    constexpr auto transformed = result.andThen(transform);

    EXPECT_TRUE(static_cast<bool>(transformed));
    EXPECT_EQ(transformed, 84);

    constexpr auto transform2 = [](int value, int factor) {
        return Result<int, std::string_view>(value * factor);
    };

    constexpr auto transformed2 = result.andThen(transform2, 2);

    EXPECT_TRUE(static_cast<bool>(transformed2));
    EXPECT_EQ(transformed2, 84);
}

TEST(TestResult, ResultBoundTransform) {
    struct WrappedInt {
        int value;
        constexpr WrappedInt(int value) : value(value) {}
        constexpr Result<int, std::string_view> timesTwo() const {
            return value * 2;
        }
        constexpr Result<int, std::string_view> timesX(int factor) const {
            return value * factor;
        }
    };

    constexpr Result<WrappedInt, std::string_view> result{42};
    constexpr auto transform = &WrappedInt::timesTwo;

    // const auto transformed = result.andThen(transform);
    const auto transformed = result.andThen(transform);

    EXPECT_TRUE(static_cast<bool>(transformed));
    EXPECT_EQ(transformed, 84);

    const auto transformed2 = result.andThen(&WrappedInt::timesX, 2);

    EXPECT_TRUE(static_cast<bool>(transformed2));
    EXPECT_EQ(transformed2, 84);
}

TEST(TestResult, ResultContextTransform) {
    struct Context {
        constexpr Result<int, std::string_view> timesTwo(int value) const {
            return value * 2;
        }
        constexpr Result<int, std::string_view> timesX(int value,
                                                       int factor) const {
            return value * factor;
        }
    };

    constexpr Result<int, std::string_view> result{42};
    constexpr Context context;

    constexpr auto transform = &Context::timesTwo;

    constexpr auto transformed = result.andThen(&context, transform);

    EXPECT_TRUE(static_cast<bool>(transformed));
    EXPECT_EQ(transformed, 84);

    constexpr auto transformed2 = result.andThen(&context, &Context::timesX, 2);

    EXPECT_TRUE(static_cast<bool>(transformed2));
    EXPECT_EQ(transformed2, 84);
}

//
// Error transformers
//

TEST(TestResult, ResultFreeTransformErr) {
    constexpr Result<int, std::string_view> result{"error"};

    const auto transform = []([[maybe_unused]] std::string_view value) {
        // Fake a recovery
        return Result<int, std::string_view>(0);
    };

    const auto transformed = result.orElse(transform);

    EXPECT_TRUE(static_cast<bool>(transformed));
    EXPECT_EQ(transformed, 0);
}

TEST(TestResult, ResultBoundTransformErr) {
    struct WrappedError {
        std::string_view error;
        constexpr WrappedError(std::string_view error)
            : error(std::move(error)) {}
        constexpr WrappedError(const char *error) : error(error) {}

        constexpr Result<int, std::string_view> severity() const {
            return error.length();
        }
        constexpr Result<int, std::string_view>
        multiplySeverity(int factor) const {
            return error.length() * factor;
        }
    };

    constexpr Result<int, WrappedError> result{"error"};
    const auto transform = &WrappedError::severity;

    const auto transformed = result.orElse(transform);

    EXPECT_TRUE(static_cast<bool>(transformed));
    EXPECT_EQ(transformed, 5);

    const auto transformed2 = result.orElse(&WrappedError::multiplySeverity, 2);

    EXPECT_TRUE(static_cast<bool>(transformed2));
    EXPECT_EQ(transformed2, 10);
}

TEST(TestResult, ResultContextTransformErr) {
    struct Context {
        constexpr Result<int, std::string_view>
        severity(std::string_view error) const {
            return error.length();
        }
        constexpr Result<int, std::string_view>
        multiplySeverity(std::string_view error, int factor) const {
            return error.length() * factor;
        }
    };

    constexpr Result<int, std::string_view> result{"error"};
    constexpr Context context;

    constexpr auto transform = &Context::severity;

    constexpr auto transformed = result.orElse(&context, transform);

    EXPECT_TRUE(static_cast<bool>(transformed));
    EXPECT_EQ(transformed, 5);

    constexpr auto transformed2 =
        result.orElse(&context, &Context::multiplySeverity, 2);

    EXPECT_TRUE(static_cast<bool>(transformed2));
    EXPECT_EQ(transformed2, 10);
}

} // namespace bz::core::tests
