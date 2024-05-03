#include "world.hh"
#include "component_storage.hh"
#include "component_handle.hh"

#include <typeinfo>
#include <optional>
#include <map>
#include <string>
#include <vector>

std::size_t World::new_entity() {
    for (auto &[t, c]: this->components) {
        c.push_nullopt();
    }

    return this->entity_count++;
}

template <typename T>
ComponentHandle World::add_component(std::size_t entity, T component) {
    const std::type_index type = typeid(T);
    auto &storage = this->components[type];
    auto &typed_storage = dynamic_cast<ComponentStorage<T> &>(storage);
    typed_storage.components[entity] = component;
    return ComponentHandle { entity, type };
}

template <typename T>
std::optional<T> World::get_component(std::size_t entity) {
    const std::type_index type = typeid(T);
    auto &storage = this->components[type];
    try {
        auto &typed_storage = dynamic_cast<ComponentStorage<T> &>(storage);
        return typed_storage.components[entity];
    } catch (const std::bad_cast &e) {
        return std::nullopt;
    }
}
