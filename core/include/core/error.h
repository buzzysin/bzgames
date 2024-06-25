#pragma once

#include <string>
#include <string_view>

namespace bz::core {

class Error {
public:
	Error() = default;
	virtual ~Error() = default;

public:
	Error(const char *what) : _message(what) {}
	Error(const std::string_view &what) : _message(what) {}
	Error(const std::string &what) : _message(what) {}

public:
	virtual const char *what() const noexcept { return _message.c_str(); }

private:
	std::string _message{"Error"};
};

} // namespace bz::core