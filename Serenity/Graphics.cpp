#include "Graphics.h"

#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")

Graphics::Graphics(HWND hWnd, int width, int height, std::wstring_view gameWindowName) :
	renderList{}, fontWidth{ 12 }
{
	if ((m_hWnd = FindWindow(0, gameWindowName.data())) == NULL)
		throw std::runtime_error("Process not found");

	HRESULT hr;

	if (FAILED(hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &pObject)))
		throw WINEXCEPT(hr);

	D3DPRESENT_PARAMETERS pParams = {};
	pParams.Windowed = TRUE;
	pParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pParams.hDeviceWindow = hWnd;
	pParams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	pParams.BackBufferFormat = D3DFMT_A8R8G8B8;
	pParams.BackBufferWidth = width;
	pParams.BackBufferHeight = height;
	pParams.EnableAutoDepthStencil = TRUE;
	pParams.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(hr = pObject->CreateDeviceEx(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&pParams, 0, &pDevice)))
		throw WINEXCEPT(hr);

	if (FAILED(hr = pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)))
		throw WINEXCEPT(hr);

	if (FAILED(hr = pDevice->CreateVertexBuffer(
		sizeof(Vertex) * 1024,
		D3DUSAGE_DYNAMIC,
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT, &pVBuffer, nullptr)))
		throw WINEXCEPT(hr);

	if (FAILED(hr = D3DXCreateFont(
		pDevice.Get(), fontWidth, 0,
		FW_NORMAL, 1, true,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Verdana", &pFont)))
		throw WINEXCEPT(hr);

	if (FAILED(hr = pDevice->SetStreamSource(0, pVBuffer.Get(), 0, sizeof(Vertex))))
		throw WINEXCEPT(hr);

	if (FAILED(hr = pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		throw WINEXCEPT(hr);

	if (!GetClientRect(m_hWnd, &gameWindow))
		throw WINLASTEXCEPT;

	ClientToScreen(m_hWnd, (LPPOINT)&gameWindow.left);

	ClientToScreen(m_hWnd, (LPPOINT)&gameWindow.right);

	viewPort.Height = gameWindow.bottom - gameWindow.top;
	viewPort.Width = gameWindow.right - gameWindow.left;
	viewPort.X = gameWindow.left;
	viewPort.Y = gameWindow.top;

	if (FAILED(hr = pDevice->SetViewport(&viewPort)))
		throw WINEXCEPT(hr);
}

void Graphics::endFrame()
{
	HRESULT hr;

	if (renderList.vertices.size() > 0)
	{
		void* pVoid;

		if (FAILED(hr = pVBuffer->Lock(0, 0, &pVoid, D3DLOCK_DISCARD)))
			throw WINEXCEPT(hr);

		std::memcpy(pVoid, renderList.vertices.data(), sizeof(Vertex) * std::size(renderList.vertices));

		if (FAILED(hr = pVBuffer->Unlock()))
			throw WINEXCEPT(hr);

		int pos{ 0 };

		for (const auto& batch : renderList.batches)
		{
			if (FAILED(hr = pDevice->DrawPrimitive(batch.type, pos, batch.primitiveCount)))
				throw WINEXCEPT(hr);

			pos += batch.vertexCount;
		}
	
		renderList.clear();
	}

	if (textList.size() > 0)
	{
		RECT fontPos;

		for (const auto& text : textList)
		{
			fontPos.left = static_cast<int>(text.vertex.x);
			fontPos.top = static_cast<int>(text.vertex.y);
			pFont->DrawTextA(0, text.text.c_str(), strlen(text.text.c_str()), &fontPos, DT_NOCLIP, text.vertex.Color);
		}

		textList.clear();
	}

	if (FAILED(hr = pDevice->EndScene()))
		throw WINEXCEPT(hr);

	if (FAILED(hr = pDevice->PresentEx(0, 0, 0, 0, 0)))
		throw WINEXCEPT(hr);
}

int Graphics::getWidth() const
{
	return viewPort.Width;
}

int Graphics::getHeight() const
{
	return viewPort.Height;
}

HWND Graphics::getGameWindow() const
{
	return m_hWnd;
}

void Graphics::startFrame()
{
	HRESULT hr;
	RECT rect;
	
	if (GetClientRect(m_hWnd, (LPRECT)&rect))
	{
		ClientToScreen(m_hWnd, (LPPOINT)&rect.left);
		ClientToScreen(m_hWnd, (LPPOINT)&rect.right);

		if (gameWindow.right != rect.right || gameWindow.left != rect.left || gameWindow.top != rect.top || gameWindow.bottom != rect.bottom)
		{
			viewPort.Height = rect.bottom - rect.top;
			viewPort.Width = rect.right - rect.left;
			viewPort.X = rect.left;
			viewPort.Y = rect.top;

			if (FAILED(hr = pDevice->SetViewport(&viewPort)))
				throw WINEXCEPT(hr);
		}
	}

	if (FAILED(hr = pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0)))
		throw WINEXCEPT(hr);

	if (FAILED(hr = pDevice->BeginScene()))
		throw WINEXCEPT(hr);
}

void Graphics::drawFilledRect(float x, float y, float width, float height, D3DCOLOR color)
{
	x += viewPort.X;
	y += viewPort.Y;

	Vertex v[] =
	{
		{x, y + height, .0f, .0f, color},
		{x, y, .0f, .0f, color},
		{x + width, y + height, .0f, .0f, color},

		{x + width, y + height, .0f, .0f, color},
		{x, y, .0f, .0f, color},
		{x + width, y, .0f, .0f, color}
	};
	
	if (renderList.batches.back().type == D3DPT_TRIANGLELIST)
	{
		renderList.batches.back().primitiveCount += 2;
		renderList.batches.back().vertexCount += 6;
	}
	else
		renderList.batches.emplace_back(RenderList::Batch{ 6, 2, D3DPT_TRIANGLELIST });

	renderList.addVertices(v);
}

void Graphics::drawBorderBox(float x, float y, float width, float height, float thickness, D3DCOLOR color)
{
	drawFilledRect(x - 1.f, y - 1.f, width, thickness + 2.f, D3DCOLOR_XRGB(0, 0, 0));
	drawFilledRect(x - 1.f, y - 1.f, thickness + 2.f, height + 3.f, D3DCOLOR_XRGB(0, 0, 0));
	drawFilledRect(x + width - 1.f, y - 1.f, thickness + 2.f, height + 3.f, D3DCOLOR_XRGB(0, 0, 0));
	drawFilledRect(x, y + height - 1.f, width + thickness, thickness + 2.f, D3DCOLOR_XRGB(0, 0, 0));

	drawFilledRect(x, y, width, thickness, color);
	drawFilledRect(x, y, thickness, height, color);
	drawFilledRect(x + width, y, thickness, height, color);
	drawFilledRect(x, y + height, width + thickness, thickness, color);
}

void Graphics::drawCircle(float x, float y, float radius, int segments, D3DCOLOR color, bool filled)
{
	x += viewPort.X;
	y += viewPort.Y;

	static constexpr double pi = 3.14159265359;
	int numberVerts{ 0 };

	for (double angle = 0.0f; angle < 2.0 * pi; angle += (2.0 * pi) / segments)
	{
		numberVerts++;
		renderList.vertices.emplace_back(Vertex{ (float)(x + cos(angle) * radius), (float)(y + -sin(angle) * radius), .0f, .0f, color} );
	}

	if (renderList.batches.back().type == (filled ? D3DPT_TRIANGLEFAN : D3DPT_LINESTRIP))
	{
		renderList.batches.back().vertexCount += numberVerts;
		renderList.batches.back().primitiveCount += numberVerts - 1;
	}
	else
		renderList.batches.emplace_back(RenderList::Batch{ numberVerts, numberVerts - 1, filled ? D3DPT_TRIANGLEFAN : D3DPT_LINESTRIP });
}

float Graphics::centerText(float x, float width, float textWidth)
{
	x += width / 2;

	return x - (textWidth * (fontWidth / 2)) / 2;
}

void Graphics::drawString(const std::string& str, float x, float y, D3DCOLOR color)
{
	x += viewPort.X;
	y += viewPort.Y;

	textList.push_back(textStruct{ str, Vertex{x, y, .0f, .0f, color} } );
}

void Graphics::drawLine(float x, float y, float x2, float y2, D3DCOLOR color)
{
	x += viewPort.X;
	y += viewPort.Y;

	Vertex v[] =
	{
		Vertex{ x, y, .0f, .0f, color},
		Vertex{ x2, y2, .0f, .0f, color}
	};

	if (renderList.batches.back().type == D3DPT_LINELIST)
	{
		renderList.batches.back().vertexCount += 2;
		renderList.batches.back().primitiveCount += 1;
	}
	else
		renderList.batches.push_back(RenderList::Batch{ 2, 1, D3DPT_LINELIST });	

	renderList.addVertices(v);
}

IDirect3DDevice9Ex* Graphics::getDevice() noexcept
{
	return pDevice.Get();
}

