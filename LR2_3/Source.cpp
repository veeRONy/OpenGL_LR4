﻿#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "math_3d.h"
#include "pipeline.h"
#include "Camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "ogldev_util.h"
#include "mesh.h"
#include "shadow_map_fbo.h"
#include "shadow_map_technique.h"
#include "Magick++.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024


class Main : public ICallbacks
{
public:

    Main()
    {
        m_pEffect = NULL;
        m_pShadowMapTech = NULL;
        m_pGameCamera = NULL;
        m_pMesh = NULL;
        m_pQuad = NULL;
        m_scale = 0.0f;

        m_spotLight.AmbientIntensity = 0.0f;
        m_spotLight.DiffuseIntensity = 0.9f;
        m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_spotLight.Attenuation.Linear = 0.01f;
        m_spotLight.Position = Vector3f(-20.0, 20.0, 5.0f);
        m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
        m_spotLight.Cutoff = 20.0f;
    }

    ~Main()
    {
        SAFE_DELETE(m_pEffect);
        SAFE_DELETE(m_pShadowMapTech);
        SAFE_DELETE(m_pGameCamera);
        SAFE_DELETE(m_pMesh);
        SAFE_DELETE(m_pQuad);
    }

    bool Init()
    {
        if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
            return false;
        }

        m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

        m_pEffect = new LightingTechnique();

        if (!m_pEffect->Init()) {
            printf("Error initializing the lighting technique\n");
            return false;
        }

        m_pShadowMapTech = new ShadowMapTechnique();

        if (!m_pShadowMapTech->Init()) {
            printf("Error initializing the shadow map technique\n");
            return false;
        }

        m_pShadowMapTech->Enable();

        m_pQuad = new Mesh();

        if (!m_pQuad->LoadMesh("C:/Users/user/Desktop/Учеба/С++ программы/ИКГ/Open_GL_LR4/LR_3/LR2_3/quad.obj")) {
            return false;
        }

        m_pMesh = new Mesh();

        return m_pMesh->LoadMesh("C:/Users/user/Desktop/Учеба/С++ программы/ИКГ/Open_GL_LR4/LR_3/LR2_3/phoenix_ugv.md2");
    }


    void Run()
    {
        GLUTBackendRun(this);
    }

    virtual void RenderSceneCB()
    {
        m_pGameCamera->OnRender();
        m_scale += 0.05f;

        ShadowMapPass();
        RenderPass();

        glutSwapBuffers();
    }

    virtual void ShadowMapPass()
    {
        m_shadowMapFBO.BindForWriting();

        glClear(GL_DEPTH_BUFFER_BIT);

        Pipeline p;
        p.Scale(0.2f, 0.2f, 0.2f);
        p.Rotate(0.0f, m_scale, 0.0f);
        p.WorldPos(0.0f, 0.0f, 5.0f);
        p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
        m_pShadowMapTech->SetWVP(p.GetWVPTrans());
        m_pMesh->Render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    virtual void RenderPass()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pShadowMapTech->SetTextureUnit(0);
        m_shadowMapFBO.BindForReading(GL_TEXTURE0);

        Pipeline p;
        p.Scale(5.0f, 5.0f, 5.0f);
        p.WorldPos(0.0f, 0.0f, 10.0f);
        p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
        p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
        m_pShadowMapTech->SetWVP(p.GetWVPTrans());
        m_pQuad->Render();
    }

    virtual void IdleCB()
    {
        RenderSceneCB();
    }

    virtual void SpecialKeyboardCB(int Key, int x, int y)
    {
        m_pGameCamera->OnKeyboard(Key);
    }


    virtual void KeyboardCB(unsigned char Key, int x, int y)
    {
        switch (Key) {
        case 'q':
            glutLeaveMainLoop();
            break;
        }
    }


    virtual void PassiveMouseCB(int x, int y)
    {
        m_pGameCamera->OnMouse(x, y);
    }

private:

    LightingTechnique* m_pEffect;
    ShadowMapTechnique* m_pShadowMapTech;
    Camera* m_pGameCamera;
    float m_scale;
    SpotLight m_spotLight;
    Mesh* m_pMesh;
    Mesh* m_pQuad;
    ShadowMapFBO m_shadowMapFBO;
};

int main(int argc, char** argv)
{
    GLUTBackendInit(argc, argv);
    
    Magick::InitializeMagick(*argv);

    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 144, true, "OpenGL")) {
        return 1;
    }

    Main* pApp = new Main();


    if (!pApp->Init()) {
        return 1;
    }

    pApp->Run();

    delete pApp;

    return 0;
}
