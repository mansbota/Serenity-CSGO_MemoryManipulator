#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "WinException.h"
#include <wrl.h>
#include <vector>

using namespace Microsoft::WRL;

struct Vertex
{
	float x, y, z, ht;
	DWORD Color;
};

class Graphics
{
	struct textStruct
	{
		std::string text;
		Vertex vertex;
	};

	class RenderList
	{
	public:
		struct Batch {
			int vertexCount;
			int primitiveCount;
			D3DPRIMITIVETYPE type;
		};

		std::vector<Vertex> vertices;
		std::vector<Batch> batches;

		RenderList() {
			vertices.reserve(1000);
			batches.reserve(10);
		}

		template<std::size_t SIZE>
		void addVertices(Vertex (&v)[SIZE])
		{
			for (const auto& el : v)
				vertices.push_back(el);
		}

		void clear()
		{
			vertices.clear();
			batches.clear();
		}
	};

	ComPtr<IDirect3D9Ex> pObject;
	ComPtr<IDirect3DDevice9Ex> pDevice;
	ComPtr<IDirect3DVertexBuffer9> pVBuffer;
	ComPtr<ID3DXFont> pFont;

	HWND m_hWnd;
	RECT gameWindow;
	D3DVIEWPORT9 viewPort;
	RenderList renderList;
	std::vector<textStruct> textList;

public:
	Graphics(HWND hWnd, int width, int height, std::wstring_view gameWindowName);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void startFrame();
	void endFrame();
	int getWidth() const;
	int getHeight() const;
	HWND getGameWindow() const;

	void drawFilledRect(float x, float y, float width, float height, D3DCOLOR color);
	void drawBorderBox(float x, float y, float width, float height, float thickness, D3DCOLOR color);
	void drawCircle(float x, float y, float radius, int segments, D3DCOLOR color, bool filled);
	void drawString(const std::string& str, float x, float y, D3DCOLOR color);
	float centerText(float x, float width, float textWidth);
	void drawLine(float x, float y, float x2, float y2, D3DCOLOR color);
	IDirect3DDevice9Ex* getDevice() noexcept;
	int fontWidth;
};

