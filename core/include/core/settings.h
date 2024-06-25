#pragma once

#include <core/error.h>
#include <filesystem>

namespace bz::core {
class Settings {
public:
	Settings() = default;
	virtual ~Settings() = default;

	Settings(const Settings &) = delete;
	Settings &operator=(const Settings &) = delete;

	Settings(Settings &&) = default;
	Settings &operator=(Settings &&) = default;

public:
	virtual void load() = 0;
	virtual void save() = 0;

	// Where the setting folder is located
	virtual std::filesystem::path root() const = 0;

  // The name of the settings file
  virtual std::string filename() const = 0;

  
};

} // namespace bz::core