#ifndef COMPONENT_HANDLE_HH
#define COMPONENT_HANDLE_HH

struct World;
#include "world.hh"

#include <optional>
#include <typeindex>

struct ComponentHandle {
    std::size_t entity;
    std::type_index type;

    template <typename T>
    std::optional<T> get(World &world);

    template <typename T>
    ComponentHandle to(std::size_t entity);
};

#endif // COMPONENT_HANDLE_HH

#ifdef COMPONENT_HANDLE_IMPLEMENTATION

template <typename T>
std::optional<T> ComponentHandle::get(World &world) {
    if (typeid(T) != this->type) {
        return std::nullopt;
    }

    auto &storage = world.components[typeid(T)];
    try {
        auto &typed_storage = dynamic_cast<ComponentStorage<T>&>(storage);
        return typed_storage.components[entity];
    } catch (const std::bad_cast &e) {
        return std::nullopt;
    }
}

template <typename T>
ComponentHandle ComponentHandle::to(std::size_t entity) {
    return ComponentHandle {
        entity,
        typeid(T)
    };
}

#endif // COMPONENT_HANDLE_IMPLEMENTATION
