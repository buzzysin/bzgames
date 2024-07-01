#include "test.h"

#include <core/logger.h>
#include <gtest/gtest.h>


namespace bz::core::logger::tests {

TEST(TestLogger, LoggerDebug) {
  bzLog() << "Default";
	bzLog(Severity::Debug) << "Debug";
	bzLog(Severity::Info) << "Info";
	bzLog(Severity::Warning) << "Warning";
	bzLog(Severity::Error) << "Error";
}

} // namespace bz::core::tests