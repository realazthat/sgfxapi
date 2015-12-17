#include "gfxdemo.hpp"

#include "gfxapi.hpp"
#include "drawutils.hpp"

#include <GLFW/glfw3.h>

#include <memory>
#include <thread>
#include <chrono>



MainClass::MainClass() :
    m_exiting(false),
    m_windowTitle("GfxApiDemo")
{

}

MainClass::~MainClass(void)
{

}

int MainClass::initGLFW(int screenWidth, int screenHeight)
{
    if (!glfwInit())
        throw std::runtime_error("Error initializing GLFW");

    m_pWindow = glfwCreateWindow(screenWidth, screenHeight, m_windowTitle.c_str(), NULL, NULL);

    if (!m_pWindow)
    {
        glfwTerminate();
        throw std::runtime_error("Could not create GLFW window");
    }
   // glfwSetWindowUserPointer(window, &context);
   // thats useful for key-callbacks

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pWindow);

    glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return 0;
}


int MainClass::initGLEW()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::string msg = std::string("Error initializing glew: ");
        throw std::runtime_error(msg);
    }

    return 0;
}

void MainClass::initCamera()
{
   
    m_camera.SetPos(float3(0, 0, -10));
    m_camera.SetFront(float3(0,0,1));
    m_camera.SetUp(float3(0,1,0));
    m_camera.SetViewPlaneDistances(1.f, 2500.f);

    //m_camera.type = PerspectiveFrustum;
    float horizontalFov = 1;
    
    int width, height;
    glfwGetFramebufferSize(m_pWindow, &width, &height);

    float verticalFov = horizontalFov * (float)height / (float)width;
    m_camera.SetPerspective(horizontalFov,verticalFov);

    m_camera.SetKind(FrustumSpaceGL, FrustumRightHanded);
    assert(m_camera.Type() == PerspectiveFrustum);
}

void MainClass::exitApplication()
{
    glfwTerminate();
}

bool MainClass::init()
{
    initGLFW(1920, 1080);
    initGLEW();

    m_graphics = std::make_shared<GfxApi::Graphics>();


    initCamera();

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glDisable(GL_CULL_FACE);

    m_lastTick = Clock::Tick();

    //meshes.push_back(axes());
    //meshes.push_back(SSTQRGBD2(true));
    //meshes.push_back(simpletri());
    //meshes.push_back(reguizdemo());
    meshes.push_back(basictexture());

    return true;
}

double horizontalAngle = 0;
double verticalAngle = 0;

void MainClass::updateCamera(float deltaTime)
{
    int width, height;
    glfwGetFramebufferSize(m_pWindow, &width, &height);
    
    double xpos, ypos;
    glfwGetCursorPos(m_pWindow, &xpos, &ypos);

    double halfWidth = width/2;
    double halfHeight = height/2;
    
    // Reset mouse position for next frame
    glfwSetCursorPos(m_pWindow, halfWidth, halfHeight);
    
    double mouseSpeed = 0.0001;
    horizontalAngle += mouseSpeed * deltaTime * double(halfWidth - xpos );
    verticalAngle += mouseSpeed * deltaTime * double(halfHeight - ypos );

    Quat rotHorizontal = Quat(float3(0,1,0), horizontalAngle); 
    Quat rotVertical = Quat(float3(-1,0,0), verticalAngle);

    m_camera.SetUp(rotHorizontal * (rotVertical * float3(0,1,0)));
    m_camera.SetFront(rotHorizontal * (rotVertical * float3(0,0,1)));



}

void MainClass::mainLoop()
{
    init();

    while(!m_exiting)
    {

        onTick();
        //Sleep(1);
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }

    exitApplication();
}

void MainClass::handleInput(float deltaTime)
{
    float moveSpeed = 0.1; //units per second
    if(glfwGetKey(m_pWindow,GLFW_KEY_LEFT_SHIFT))
    {
        moveSpeed = 2.5;
    }
    if(glfwGetKey(m_pWindow,GLFW_KEY_S))
    {
        //m_camera.pos += deltaTime * moveSpeed * -m_camera.front;
        m_camera.SetPos(m_camera.Pos() + deltaTime * moveSpeed * -m_camera.Front());
    } 
    else if(glfwGetKey(m_pWindow,GLFW_KEY_W))
    {
        //m_camera.pos += deltaTime * moveSpeed * m_camera.front;
        m_camera.SetPos(m_camera.Pos() + deltaTime * moveSpeed * m_camera.Front());

    }

    if(glfwGetKey(m_pWindow,GLFW_KEY_A))
    {
        //m_camera.pos += deltaTime * moveSpeed * -m_camera.WorldRight();
        m_camera.SetPos(m_camera.Pos() + deltaTime * moveSpeed * -m_camera.WorldRight());
    } 
    else if(glfwGetKey(m_pWindow,GLFW_KEY_D))
    {
        //m_camera.pos += deltaTime * moveSpeed * m_camera.WorldRight();
        m_camera.SetPos(m_camera.Pos() + deltaTime * moveSpeed * m_camera.WorldRight());
    }

    if(glfwGetKey(m_pWindow,GLFW_KEY_Y))
    {
        //m_camera.pos += deltaTime * moveSpeed * -m_camera.up; 
        m_camera.SetPos(m_camera.Pos() + deltaTime * moveSpeed * -m_camera.Up());
    } 
    else if(glfwGetKey(m_pWindow,GLFW_KEY_X))
    {
        //m_camera.pos += deltaTime * moveSpeed * m_camera.up;
        m_camera.SetPos(m_camera.Pos() + deltaTime * moveSpeed * m_camera.Up());
    }

    if(glfwGetKey(m_pWindow,GLFW_KEY_O))
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else if(glfwGetKey(m_pWindow,GLFW_KEY_P))
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
   
}

void MainClass::onTick()
{

    double deltaTime = Clock::MillisecondsSinceD(m_lastTick);
    m_lastTick = Clock::Tick();


    if (glfwWindowShouldClose(m_pWindow))
    {
        throw std::runtime_error("window was closed");
    }

    if (glfwGetWindowAttrib(m_pWindow, GLFW_FOCUSED))
    {
        // handle all input
        handleInput(deltaTime);

        // perform camera updates
        updateCamera(deltaTime);
    }


    // Make the window's context current 
    glfwMakeContextCurrent(m_pWindow);

  
    m_graphics->Clear(true, true, true, 0.0, 0.0, 0.0);
    
    ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////
    // DRAW
    
    float4x4 xform = float4x4::FromTRS(float3(0,0,0), Quat::identity, float3(10,10,10));



    GfxApi::ClearBindings();
    for (auto& mesh_ptr : meshes)
    {
        auto& mesh = *mesh_ptr;
        
        mesh.Bind();
        mesh.sp->Use();
        int worldLocation = mesh.sp->GetUniformLocation("world");
        int worldViewProjLocation = mesh.sp->GetUniformLocation("worldViewProj");

        //std::cout << "worldLocation: " << worldLocation << ", worldViewProjLocation: " << worldViewProjLocation << std::endl;

        if (worldLocation != -1)
            mesh.sp->SetFloat4x4(worldLocation, xform);
        if (worldViewProjLocation != -1)
            mesh.sp->SetFloat4x4(worldViewProjLocation, m_camera.ViewProjMatrix());

        for (auto& mesh_texture_ptr : mesh.textures)
        {
            auto& mesh_texture = *mesh_texture_ptr;
            
            auto& texture_unit = *mesh_texture.texture_unit;
            texture_unit.Activate();
            mesh_texture.texture->Bind();

            if (!mesh_texture.sampler)
                texture_unit.UnBindSampler();
            else
                texture_unit.BindSampler(*mesh_texture.sampler);

            mesh.sp->BindTexture(texture_unit.Index(), texture_unit, *mesh_texture.texture, mesh_texture.sampler_name);
        }

        mesh.Draw();
    }
    GfxApi::ClearBindings();

    // Swap front and back buffers 
    glfwSwapBuffers(m_pWindow);

    // poll the input system
    //m_pInput->poll();
    glfwPollEvents();

    
}


Frustum& MainClass::getCamera()
{
    return m_camera;
}

Frustum* MainClass::getCameraPtr()
{
    return &m_camera;
}





#include <stdio.h>
#include <iostream>



MainClass* g_pMainClass;

int main(int argc, char *argv[])
{

    try
    {
        MainClass* g_pMainClass = new MainClass();
        g_pMainClass->mainLoop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

