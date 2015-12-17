#ifndef _MAINCLASS_H
#define _MAINCLASS_H

//#include "GfxApi.hpp"

#include <string>
#include <MathGeoLib.h>
#include <memory>


void initText2D(const char * texturePath);
void printText2D(const char * text, int x, int y, int size);
void cleanupText2D();


struct GLFWwindow;

namespace GfxApi
{
    class Mesh;
    class Graphics;
    class RenderNode;
    //class Input;
}


class MainClass
{
public:
    MainClass();
    ~MainClass(void);
    int initGLFW(int screenWidth, int screenHeight);
    int initGLEW();

    virtual bool init();

    virtual void mainLoop();
    
    virtual void onTick();

    void exitApplication();

    void initCamera();

    void updateCamera(float deltaTime);

    void handleInput(float deltaTime);

    Frustum& getCamera();
    Frustum* getCameraPtr();

private:

    GLFWwindow* m_pWindow;

    std::shared_ptr<GfxApi::Graphics> m_graphics;

    bool m_exiting;

    bool isExiting();

    std::string m_windowTitle;

    Frustum m_camera;

    Frustum m_cameraFrozen;

    //std::shared_ptr<GfxApi::Input> m_pInput;

    tick_t m_lastTick;

    std::vector< std::shared_ptr<GfxApi::Mesh> > meshes;


};

#endif
