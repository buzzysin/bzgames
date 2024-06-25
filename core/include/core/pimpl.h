#pragma once

#include <memory>

namespace bz::core {

template <typename T>
class Pimpl {
public:
	Pimpl();

	Pimpl(const Pimpl &other) = delete;
	Pimpl &operator=(const Pimpl &other) = delete;

	Pimpl(Pimpl &&other) = default;
	Pimpl &operator=(Pimpl &&other) = default;

	template <typename... Args>
	Pimpl(Args &&...args);

	~Pimpl();

public:
	T *operator->();
	const T *operator->() const;

	T &operator*();
	const T &operator*() const;

private:
	std::unique_ptr<T> m_data;
};

} // namespace bz::core

#include "pimpl.tpp" // IWYU pragma: export

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define BZ_PIMPL(Class)                                                        \
public:                                                                        \
	class Class##Private;                                                      \
                                                                               \
private:                                                                       \
	::bz::core::Pimpl<Class##Private> _pImpl;
// NOLINTEND(cppcoreguidelines-macro-usage)