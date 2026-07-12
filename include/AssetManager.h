#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "Handles.h"
#include <unordered_map>

namespace rt{
class IMaterial;
class ITexture;




class AssetManager{
public:
    template<typename T, typename... Args>
    MaterialHandle addMaterial(Args&&... args);

    IMaterial& getMaterial(MaterialHandle handle) const;


    template<typename T, typename... Args>
    TextureHandle addTexture(Args&&... args);

    ITexture& getTexture(TextureHandle handle) const;


private:
    MaterialHandle nextMat{1};
    std::unordered_map<MaterialHandle, std::unique_ptr<IMaterial>> materialStore;
    TextureHandle nextTex{1};
    std::unordered_map<TextureHandle, std::unique_ptr<ITexture>> textureStore;
};
}

using namespace rt;

template<typename T, typename... Args>
MaterialHandle rt::AssetManager::addMaterial(Args&&... args){
    static_assert(std::is_base_of_v<IMaterial, T>, "Material class must inherit from IMaterial");
    materialStore[nextMat] = std::make_unique<T>(std::forward<Args>(args)...);
    return nextMat++;
}

template<typename T, typename... Args>
TextureHandle rt::AssetManager::addTexture(Args&&... args){
    static_assert(std::is_base_of_v<ITexture, T>, "Texture class must inherit from ITexture");
    textureStore[nextTex] = std::make_unique<T>(std::forward<Args>(args)...);
    return nextTex++;
}
