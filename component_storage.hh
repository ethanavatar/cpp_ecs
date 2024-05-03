#ifndef _COMPONENT_STORAGE_HH
#define _COMPONENT_STORAGE_HH

#include <vector>
#include <optional>

struct ErasedComponentStorage {
    virtual void push_nullopt(void) = 0;
};

template <typename T>
struct ComponentStorage: public ErasedComponentStorage {
    std::vector<std::optional<T>> components;
    void push_nullopt(void);
};

#endif // _COMPONENT_STORAGE_HH
