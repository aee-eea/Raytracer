#include "AssetManager.h"
#include "IMaterial.h"
#include "ITexture.h"
#include <cassert>
#include <memory>




IMaterial& AssetManager::getMaterial(MaterialHandle handle) const{
    assert( (handle >= 1 && handle < nextMat) && "Invalid material handle");
    return *materialStore.at(handle);
}

ITexture& AssetManager::getTexture(TextureHandle handle) const{
    assert( (handle >= 1 && handle < nextTex) && "Invalid texture handle");
    return *textureStore.at(handle);
}