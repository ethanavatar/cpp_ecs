#ifndef _WORLD_HH
#define _WORLD_HH

#include <map>
#include <typeindex>
#include <vector>
#include <optional>

#include "component_storage.hh"
#include "component_handle.hh"

struct ComponentHandle;

struct World {
    std::size_t entity_count;
    std::map<std::type_index, ErasedComponentStorage &> components;

    std::size_t new_entity(void);

    template <typename T>
    ComponentHandle add_component(std::size_t entity, T component);

    template <typename T>
    std::optional<T> get_component(std::size_t entity);
};

#endif // _WORLD_HH

