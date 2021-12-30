#ifndef CONCEPTS_H
#define CONCEPTS_H

#include <concepts>

template<typename T, typename... Ts>
concept any_of = ((std::same_as<T, Ts> || ...));

#endif
