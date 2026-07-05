#include "AssetManager.h"
#include "IMaterial.h"
#include <memory>

IMaterial& AssetManager::getMaterial(MaterialHandle handle) const{
    return *materialStore.at(handle);
}