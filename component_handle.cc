#include "component_handle.hh"

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
