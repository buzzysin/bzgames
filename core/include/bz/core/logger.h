#pragma once

#include <mutex>
#include <optional>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include "core/logger_traits.h"

namespace bz::core::logger {

enum class Severity { Debug, Info, Warning, Error };

class Logger {
public:
	Logger(const char *filename, int line, const char *function,
	       const std::optional<Severity> &severity = std::nullopt);

	~Logger();

	Logger(const Logger &) = delete;
	Logger &operator=(const Logger &) = delete;

	Logger(Logger &&) noexcept = default;
	Logger &operator=(Logger &&) noexcept = default;

	// Public interface
public:
	template <typename T, std::enable_if_t<IsFormattable<T>, int> = 0>
	Logger &operator<<(const T &value) {
		_stream << LogFormat<T>::format(value);
		return *this;
	}

	// Private interface
private:
	void _log(Severity severity);

	static std::string_view _fetchColor(const std::string_view &filename);

private:
	const char *_filename;
	int _line;
	const char *_function;
	Severity _severity;
	std::ostringstream _stream;

	// Map from filename to color (each file will always have the same
	// color)
	static std::unordered_map<std::string_view, std::string_view>
		_fileColourMap;
	static std::mutex _fileColourMapMutex;
};

} // namespace bz::core::logger

// NOLINTNEXTLINE
#define bzLog(SEVERITY)                                                        \
	::bz::core::logger::Logger(__FILE__, __LINE__, __FUNCTION__, {SEVERITY})
