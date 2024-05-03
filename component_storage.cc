#include "component_storage.hh"

template <typename T>
void ComponentStorage<T>::push_nullopt(void) {
    components.push(std::nullopt);
}
