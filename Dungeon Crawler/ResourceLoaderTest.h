#ifndef RESOURCE_LOADER_TEST_H
#define RESOURCE_LOADER_TEST_H

#include "State.h"

#include "Texture.h"
#include "Shader.h"
#include "ResourceLoader.h"

class ResourceLoaderTest : public State
{
private:
    GLuint vao;
    glm::mat4 ortho;
    
    ShaderProgram* shader;
    Texture* texture;
    
    ResourceLoader<Texture> textureLoader;
    ResourceLoader<ShaderProgram> shaderLoader;
    
public:
    ResourceLoaderTest(Game* game);
    ~ResourceLoaderTest();
    
    bool Init();
    void Shutdown();
    
    void HandleEvents(SDL_Event &e);
    void Update(double dt);
    void Render(double alpha);
};

#endif
