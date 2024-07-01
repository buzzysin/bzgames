#include "core/logger.h"
#include <iostream>

namespace bz::core::logger {

// constexpr std::string_view _consoleLight = "\033[1m";
// constexpr std::string_view _consoleDark = "\033[2m";
// constexpr std::string_view _consoleItalic = "\033[3m";
// constexpr std::string_view _consoleUnderline = "\033[4m";

constexpr std::string_view _consoleRed = "\033[31m";
constexpr std::string_view _consoleYellow = "\033[33m";
constexpr std::string_view _consoleGreen = "\033[32m";
constexpr std::string_view _consoleBlue = "\033[34m";
constexpr std::string_view _consoleMagenta = "\033[35m";
constexpr std::string_view _consoleCyan = "\033[36m";
constexpr std::string_view _consoleWhite = "\033[37m";

constexpr std::string_view _consoleLRed = "\033[91m";
constexpr std::string_view _consoleLYellow = "\033[93m";
constexpr std::string_view _consoleLGreen = "\033[92m";
constexpr std::string_view _consoleLBlue = "\033[94m";
constexpr std::string_view _consoleLMagenta = "\033[95m";
constexpr std::string_view _consoleLCyan = "\033[96m";
constexpr std::string_view _consoleLWhite = "\033[97m";

constexpr std::string_view _consoleDRed = "\033[41m";
constexpr std::string_view _consoleDYellow = "\033[43m";
constexpr std::string_view _consoleDGreen = "\033[42m";
constexpr std::string_view _consoleDBlue = "\033[44m";
constexpr std::string_view _consoleDMagenta = "\033[45m";
constexpr std::string_view _consoleDCyan = "\033[46m";
constexpr std::string_view _consoleDWhite = "\033[47m";

constexpr std::string_view _consoleReset = "\033[0m";

constexpr std::array _consoleColours = {
	_consoleRed,     _consoleYellow, _consoleGreen,    _consoleBlue,
	_consoleMagenta, _consoleCyan,   _consoleWhite,    _consoleLRed,
	_consoleLYellow, _consoleLGreen, _consoleLBlue,    _consoleLMagenta,
	_consoleLCyan,   _consoleLWhite, _consoleDRed,     _consoleDYellow,
	_consoleDGreen,  _consoleDBlue,  _consoleDMagenta, _consoleDCyan,
	_consoleDWhite};

Severity Logger::_globalSeverity{Severity::Info};

std::unordered_map<std::string_view, std::string_view> Logger::_fileColourMap{};

std::mutex Logger::_fileColourMapMutex{};

Logger::Logger(const char *filename, int line, const char *function,
               const std::optional<Severity> &severity)
	: _filename{filename}, _line{line}, _function{function},
	  _severity{severity.value_or(Severity::Info)} {}

Logger::~Logger() { _log(_severity); }

void Logger::setSeverity(Severity severity) { _globalSeverity = severity; }

void Logger::_log(Severity severity) {
	// Check if we should even log this message
	if (severity < _globalSeverity) {
		return;
	}

	std::string_view colour = _fetchColor(_filename);
	std::string_view severityStr;
	switch (severity) {
	case Severity::Trace:
		severityStr = "Trce";
		break;
	case Severity::Debug:
		severityStr = "Dbg ";
		break;
	case Severity::Info:
		severityStr = "Info";
		break;
	case Severity::Warning:
		severityStr = "Warn";
		break;
	case Severity::Error:
		severityStr = "Err ";
		break;
	}

	// Format log stting as [Severity] filename:line (function): message
	// The file name is the leaf file name, not the full path

	std::string leaf = _filename;
	std::size_t found = leaf.find_last_of("/\\");
	if (found != std::string::npos) {
		leaf = leaf.substr(found + 1);
	}

	std::cout << "[ " << severityStr << " ] " << colour << leaf << ":" << _line
			  << _consoleReset << " (" << _function << "): " << _stream.str()
			  << std::endl;
}

std::string_view Logger::_fetchColor(const std::string_view &filename) {
	std::lock_guard<std::mutex> lock(_fileColourMapMutex);
	if (_fileColourMap.find(filename) == _fileColourMap.end()) {

		static std::size_t colourIndex = 0;
		if (colourIndex < _consoleColours.size()) {
			_fileColourMap[filename] =
				_consoleColours.at(colourIndex % _consoleColours.size());
		} else {
			return _consoleReset;
		}

		colourIndex++;
		colourIndex %= _consoleColours.size();
	}
	return _fileColourMap[filename];
}

} // namespace bz::core::logger