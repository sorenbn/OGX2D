#define _HAS_EXCEPTIONS 0
#include "GraphicsDevice.h"

// Simple 2D vertex (position + color)
struct Vertex2D
{
    float x, y, z, rhw;
    D3DCOLOR color;
};

// Flexible vertex format (FVF)
#define VERTEX_FORMAT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

GraphicsDevice::GraphicsDevice() : direct3D(NULL), device(NULL)
{
    ZeroMemory(&parameters, sizeof(parameters));
}

GraphicsDevice::~GraphicsDevice()
{
    if (device)
    {
        device->Release();
        device = NULL;
    }

    if (direct3D)
    {
        direct3D->Release();
        direct3D = NULL;
    }
}

bool GraphicsDevice::Initialize()
{
    direct3D = Direct3DCreate8(D3D_SDK_VERSION);
    if (!direct3D)
        return false;

    ZeroMemory(&parameters, sizeof(parameters));
    parameters.BackBufferWidth = 640;
    parameters.BackBufferHeight = 480;
    parameters.BackBufferFormat = D3DFMT_LIN_X8R8G8B8;
    parameters.BackBufferCount = 1;
    parameters.EnableAutoDepthStencil = TRUE;
    parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
    parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    parameters.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE_OR_IMMEDIATE;

    // On Xbox, the HWND is unused, so pass NULL
    HRESULT hr = direct3D->CreateDevice(
        0,
        D3DDEVTYPE_HAL,
        NULL,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &parameters,
        &device
    );

    return SUCCEEDED(hr);
}

void GraphicsDevice::Clear(D3DCOLOR color)
{
    device->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void GraphicsDevice::Begin()
{
    device->BeginScene();
}

void GraphicsDevice::End()
{
    device->EndScene();
}

void GraphicsDevice::Present()
{
    device->Present(NULL, NULL, NULL, NULL);
}

void GraphicsDevice::DrawQuad(float x, float y, float width, float height, D3DCOLOR color)
{
    Vertex2D vertices[4] = {
        { x,         y,          0.0f, 1.0f, color },
        { x + width, y,          0.0f, 1.0f, color },
        { x,         y + height, 0.0f, 1.0f, color },
        { x + width, y + height, 0.0f, 1.0f, color },
    };

    device->SetVertexShader(VERTEX_FORMAT);
    device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex2D));
}