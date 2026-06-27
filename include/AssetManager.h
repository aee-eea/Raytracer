#pragma once
#include <cstdint>
#include <memory>
#include "IMaterial.h"
#include <vector>
#include "Handles.h"


class AssetManager{
public:
    AssetManager();

    template<typename T, typename... Args>
    MaterialHandle addMaterial(Args&&... args);

    IMaterial& getMaterial(MaterialHandle handle) const;


private:
    std::vector<std::unique_ptr<IMaterial>> materialStore;
};


template<typename T, typename... Args>
MaterialHandle AssetManager::addMaterial(Args&&... args){
    static_assert(std::is_base_of_v<IMaterial, T>, "Material class must inherit from IMaterial");
    materialStore.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    return materialStore.size() - 1;
}
