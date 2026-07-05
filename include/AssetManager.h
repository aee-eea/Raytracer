#pragma once
#include <cstdint>
#include <memory>
#include "IMaterial.h"
#include <vector>
#include "Handles.h"
#include <unordered_map>


class AssetManager{
public:
    template<typename T, typename... Args>
    MaterialHandle addMaterial(Args&&... args);

    IMaterial& getMaterial(MaterialHandle handle) const;


private:
    MaterialHandle next{0};
    std::unordered_map<MaterialHandle, std::unique_ptr<IMaterial>> materialStore;
};


template<typename T, typename... Args>
MaterialHandle AssetManager::addMaterial(Args&&... args){
    static_assert(std::is_base_of_v<IMaterial, T>, "Material class must inherit from IMaterial");
    materialStore[next] = std::make_unique<T>(std::forward<Args>(args)...);
    return next++;
}
