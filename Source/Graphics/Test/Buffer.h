#pragma once
#include <Graphics/DX11/Graphics.h>
#include <wrl/client.h>

class PixelBuffer
{
public:
	template<typename T>
	PixelBuffer(const T& Buffer, unsigned int Slot = 0u)
		:
		Slot(Slot)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(Buffer);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &Buffer;
		csd.SysMemPitch = 0;
		csd.SysMemSlicePitch = 0;

		Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&cbd, &csd, &pPixelBuffer);
	}
	
	template<typename T>
	void Update(const T& Buffer)
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		Graphics::GetGraphics().GetDeviceContext3D()->Map(
			pPixelBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);

		memcpy(msr.pData, &Buffer, sizeof(Buffer));
		Graphics::GetGraphics().GetDeviceContext3D()->Unmap(pPixelBuffer.Get(), 0u);
		Graphics::GetGraphics().GetDeviceContext3D()->PSSetConstantBuffers(Slot, 1u, pPixelBuffer.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pPixelBuffer;
	unsigned int Slot;
};


class VertexBuffer
{
public:
	template<typename T>
	VertexBuffer(const T& Buffer, unsigned int Slot = 0u)
		:
		Slot(Slot)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(Buffer);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &Buffer;
		csd.SysMemPitch = 0;
		csd.SysMemSlicePitch = 0;

		Graphics::GetGraphics().GetDevice3D()->CreateBuffer(&cbd, &csd, &pVertexBuffer);
	}

	template<typename T>
	void Update(const T& Buffer)
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		Graphics::GetGraphics().GetDeviceContext3D()->Map(
			pVertexBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);

		memcpy(msr.pData, &Buffer, sizeof(Buffer));
		Graphics::GetGraphics().GetDeviceContext3D()->Unmap(pVertexBuffer.Get(), 0u);
		Graphics::GetGraphics().GetDeviceContext3D()->VSSetConstantBuffers(Slot, 1u, pVertexBuffer.GetAddressOf());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	unsigned int Slot;
};