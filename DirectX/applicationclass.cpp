////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"
#include "math.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	std::string modelFilename = "Engine/data/sphere.txt";
	std::wstring textureFilename = L"Engine/data/brick.png";
	bool result;


	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;

	// Set the initial position of the camera.
	m_Camera->SetPosition(0, 0, -5);
	m_Camera->SetRotation(-20, 30, 0);


	// Create and initialize the model object.
	m_Model = new ModelClass;

	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename.c_str(), textureFilename.c_str());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light shader object.
	m_LightShader = new LightShaderClass;

	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light object.
	m_Light = new LightClass;
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}


void ApplicationClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


//Update loop?
bool ApplicationClass::Frame()
{
	static float rotation = 0.0f;
	bool result;


	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.25f;
	if(rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the graphics scene.

	static float time = 0.0f;
	
	time= time + 0.001f;
	
	result = Render(time);
	if(!result)
	{
		return false;
	}

	return true;
}


bool ApplicationClass::Render(float time)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	for(int i=0;i<32;i++){
		for (int j= 0;j< 32;j++) {

			scaleMatrix = XMMatrixScaling(1, 1, 1);  // Build the scaling matrix.
			rotateMatrix = XMMatrixRotationY(time);  // Build the rotation matrix.
			translateMatrix = XMMatrixTranslation(std::sinf((float)i*5)+ (float)i * 5, (std::sin((i+j)*2+time)+i*j)+time, std::cosf((i + j) * 2 + time) + (float)j*5+time );  // Build the translation matrix.

			// Multiply the scale, rotation, and translation matrices together to create the final world transformation matrix.
			srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
			worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			m_Model->Render(m_Direct3D->GetDeviceContext());

			// Render the model using the light shader.
			result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
				m_Light->GetDirection(),m_Light->GetAmbientColor(), XMFLOAT4(i+j,j+j,i+j,1), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());


			if (!result)
			{
				return false;
			}
		}

	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}