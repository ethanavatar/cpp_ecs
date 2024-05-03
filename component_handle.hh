#ifndef COMPONENT_HANDLE_HH
#define COMPONENT_HANDLE_HH

#include "world.hh"

#include <optional>
#include <typeindex>

struct World;

struct ComponentHandle {
    std::size_t entity;
    std::type_index type;

    template <typename T>
    std::optional<T> get(World &world);
};

#endif // COMPONENT_HANDLE_HH
