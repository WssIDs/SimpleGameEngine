#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& consts)
	{
		D3D11_MAPPED_SUBRESOURCE msr;

		GetContext(gfx)->Map(
			m_pConstantBuffer.Get(), 0u,
			D3D11_MAP_WRITE_DISCARD, 0u,
			&msr
		);

		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u);
	}
	ConstantBuffer( Graphics& gfx, const C& consts)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( consts );
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		csd.SysMemPitch = 0;
		csd.SysMemSlicePitch = 0;
		
		GetDevice(gfx)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer);
	}
	ConstantBuffer(Graphics& gfx)
	{
		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( C );
		cbd.StructureByteStride = 0u;

		GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
	}

protected:

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_pConstantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::m_pConstantBuffer;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}
};
