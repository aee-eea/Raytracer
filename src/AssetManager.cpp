#include "AssetManager.h"
#include "IMaterial.h"
#include "ITexture.h"
#include <cassert>
#include <memory>




const IMaterial& rt::AssetManager::getMaterial(MaterialHandle handle) const{
    assert( (handle >= 1 && handle < nextMat) && "Invalid material handle");
    return *materialStore.at(handle);
}

const ITexture& rt::AssetManager::getTexture(TextureHandle handle) const{
    assert( (handle >= 1 && handle < nextTex) && "Invalid texture handle");
    return *textureStore.at(handle);
}

ImageHandle rt::AssetManager::addImage(const std::string& filePath){
    Image temp;
    if(!temp.load(filePath)) { return 0;}

    imageStore[nextImg] = std::move(temp);
    return nextImg++;
}
const Image& rt::AssetManager::getImage(ImageHandle handle) const{
    assert( (handle >= 1 && handle < nextImg) && "Invalid image handle");
    return imageStore.at(handle);
}