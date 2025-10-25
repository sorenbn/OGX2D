#pragma once
#include <xtl.h>

class GraphicsDevice
{
public:
    GraphicsDevice();
    ~GraphicsDevice();

    bool Initialize();

    void Clear(D3DCOLOR color);
    void Begin();
    void End();
    void Present();

    void DrawQuad(float x, float y, float width, float height, D3DCOLOR color);
    void DrawTextSimple(const char* text, int x, int y, D3DCOLOR color);

    inline LPDIRECT3DDEVICE8 GetDevice() const { return device; }

private:
    LPDIRECT3D8 direct3D;              
    LPDIRECT3DDEVICE8 device;          
    D3DPRESENT_PARAMETERS parameters;
};
