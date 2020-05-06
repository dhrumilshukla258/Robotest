#pragma once
class RenderCalculation
{
public:
	void CalculateModelingMatrix();
	void Calculate_Stencil_Toon_Modeling_Matrix();
	RenderCalculation();
	~RenderCalculation();
	void Update();
};

