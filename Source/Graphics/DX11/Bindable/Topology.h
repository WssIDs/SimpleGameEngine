#pragma once
#include "Bindable.h"


class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D_PRIMITIVE_TOPOLOGY type);
	virtual void Bind(Graphics& gfx) override;

protected:
	D3D_PRIMITIVE_TOPOLOGY Type;
};

