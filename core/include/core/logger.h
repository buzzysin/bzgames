#pragma once

#include <mutex>
#include <optional>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include "logger_traits.h"

namespace bz::core::logger {

enum class Severity { Trace, Debug, Info, Warning, Error };

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
		// This is a no-op if the severity is lower than the global severity
		if (_severity < _globalSeverity) {
			return *this;
		}

		_stream << LogFormat<T>::format(value);
		return *this;
	}

	// Public static interface
public:
	static void setSeverity(Severity severity);

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

	// Static members
	static Severity _globalSeverity;
	static std::unordered_map<std::string_view, std::string_view>
		_fileColourMap;
	static std::mutex _fileColourMapMutex;
};

} // namespace bz::core::logger

// NOLINTNEXTLINE
#define bzLog(SEVERITY)                                                        \
	::bz::core::logger::Logger(__FILE__, __LINE__, __FUNCTION__, {SEVERITY})

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define bzTrace() bzLog(::bz::core::logger::Severity::Trace)
#define bzDebug() bzLog(::bz::core::logger::Severity::Debug)
#define bzInfo() bzLog(::bz::core::logger::Severity::Info)
#define bzWarning() bzLog(::bz::core::logger::Severity::Warning)
#define bzError() bzLog(::bz::core::logger::Severity::Error)
// NOLINTEND(cppcoreguidelines-macro-usage)
