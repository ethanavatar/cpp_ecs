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

