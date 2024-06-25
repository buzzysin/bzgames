#pragma once

#include "pimpl.h"

namespace bz::core {

template <typename T>
Pimpl<T>::Pimpl() : m_data(std::make_unique<T>()) {}

template <typename T>
template <typename... Args>
Pimpl<T>::Pimpl(Args &&...args)
	: m_data(std::make_unique<T>(std::forward<Args>(args)...)) {}

template <typename T>
Pimpl<T>::~Pimpl() = default;

template <typename T>
T *Pimpl<T>::operator->() {
	return m_data.get();
}

template <typename T>
const T *Pimpl<T>::operator->() const {
	return m_data.get();
}

template <typename T>
T &Pimpl<T>::operator*() {
	return *m_data;
}

} // namespace bz::core
