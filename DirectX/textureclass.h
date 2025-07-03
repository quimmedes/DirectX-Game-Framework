////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <stdexcept>
#include <vector>

// Link necessary libraries
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d3d11.lib")

using Microsoft::WRL::ComPtr;



////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filePath);

	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	ID3D11ShaderResourceView* LoadTextureFromPNG(ID3D11Device*, ID3D11DeviceContext*, const wchar_t*);


	int GetWidth();
    int GetHeight();

private:
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	int m_width, m_height;
};

#endif