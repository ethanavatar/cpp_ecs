#ifndef _WORLD_HH
#define _WORLD_HH

#include <map>
#include <typeindex>
#include <vector>
#include <optional>

#include "component_storage.hh"

struct ComponentHandle;
#include "component_handle.hh"


struct World {
    std::size_t entity_count;
    std::map<std::type_index, ErasedComponentStorage> components;

    World(void):
        entity_count(0),
        components() {}

    std::size_t new_entity(void);

    template <typename T>
    ComponentHandle add_component(std::size_t entity, T component);

    template <typename T>
    std::optional<T> get_component(std::size_t entity);
};

#endif // _WORLD_HH

#ifdef WORLD_IMPLEMENTATION

template <typename T>
ComponentHandle World::add_component(std::size_t entity, T component) {
    const std::type_index type = typeid(T);
    if (this->components.find(type) == this->components.end()) {
        this->components.insert(std::pair<std::type_index, ErasedComponentStorage>(
            type,
            ComponentStorage<T>()
        ));
    }

    auto &storage = this->components.at(type);
    auto &typed_storage = dynamic_cast<ComponentStorage<T>&>(storage);
    typed_storage.components[entity] = component;
    return ComponentHandle {
        entity,
        type
    };

}

template <typename T>
std::optional<T> World::get_component(std::size_t entity) {
    const std::type_index type = typeid(T);
    auto &storage = this->components.at(type);
    try {
        auto &typed_storage = dynamic_cast<ComponentStorage<T>&>(storage);
        return typed_storage.components[entity];
    } catch (const std::bad_cast &e) {
        return std::nullopt;
    }
}

#endif // WORLD_IMPLEMENTATION
