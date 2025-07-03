////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
	m_textureView = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if(m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}


	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filePath){

	m_textureView = LoadTextureFromPNG(device, context, filePath);

	if (m_textureView==0)
		return false;
	
	return true;
}


ID3D11ShaderResourceView* TextureClass::LoadTextureFromPNG(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filePath) {
	// Initialize WIC
	ComPtr<IWICImagingFactory> wicFactory;
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory)
	);
	if (FAILED(hr)) throw std::runtime_error("Failed to create WICImagingFactory.");

	// Load image
	ComPtr<IWICBitmapDecoder> decoder;
	hr = wicFactory->CreateDecoderFromFilename(filePath, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) throw std::runtime_error("Failed to load image.");

	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) throw std::runtime_error("Failed to get image frame.");

	// Convert to 32bpp RGBA
	ComPtr<IWICFormatConverter> converter;
	hr = wicFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) throw std::runtime_error("Failed to create format converter.");

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) throw std::runtime_error("Failed to initialize format converter.");

	// Get image size
	UINT width = 0, height = 0;
	converter->GetSize(&width, &height);

	// Allocate buffer
	std::vector<BYTE> pixels(width * height * 4);
	hr = converter->CopyPixels(
		nullptr,
		width * 4,
		static_cast<UINT>(pixels.size()),
		pixels.data()
	);
	if (FAILED(hr)) throw std::runtime_error("Failed to copy pixels.");

	// Create texture
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = pixels.data();
	initData.SysMemPitch = width * 4;

	ComPtr<ID3D11Texture2D> texture;
	hr = device->CreateTexture2D(&texDesc, &initData, &texture);
	if (FAILED(hr)) throw std::runtime_error("Failed to create texture.");

	

	// Create shader resource view
	ComPtr<ID3D11ShaderResourceView> srv;
	hr = device->CreateShaderResourceView(texture.Get(), nullptr, &srv);
	if (FAILED(hr)) throw std::runtime_error("Failed to create shader resource view.");

	return srv.Detach(); // caller must release it
}



int TextureClass::GetWidth()
{
    return m_width;
}


int TextureClass::GetHeight()
{
    return m_height;
}