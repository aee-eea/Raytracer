#include "AssetManager.h"
#include "IMaterial.h"
#include <memory>

AssetManager::AssetManager(){}

IMaterial& AssetManager::getMaterial(MaterialHandle handle) const{
    return *materialStore.at(handle);
}