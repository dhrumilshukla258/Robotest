/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Light.h"

#include "../Managers/ComponentManager.h"

extern ComponentManager* gpComponentManager;

RTTR_REGISTRATION
{
	rttr::registration::class_<Light>("Light")

	.property("Type", &Light::type)
	.property("Direction", &Light::direction)
	.property("Ambient", &Light::ambient)
	.property("Diffuse", &Light::diffuse)
	.property("Specular", &Light::specular)
	.property("Constant", &Light::constant)
	.property("Linear", &Light::linear)
	.property("Quadratic", &Light::quadratic)
	.property("CutOff", &Light::cutOff)
	.property("OuterCutOff", &Light::outerCutOff)
	
	
	.constructor();


}

Light::Light()
{
	mComponentName = "Light";
}

Light::~Light()
{
}

void Light::Init()
{
	

}

void Light::SelfDelete()
{
	gpComponentManager->mLight->Delete(this);
}

void Light::Clone(Component *rhs1) {
	Light* rhs = static_cast<Light*>(rhs1);

	type = rhs->type;
	point_Light_Position = rhs->point_Light_Position;
	 constant = rhs->constant;
	 linear = rhs->linear;
	 quadratic = rhs->quadratic;
	ambient = rhs->ambient;
	diffuse = rhs->diffuse;
	specular = rhs->specular;
	 direction = rhs->direction;
	 constant = rhs->constant;
	 linear=rhs->linear;
	 quadratic=rhs->quadratic;
	 cutOff= rhs->cutOff;
	outerCutOff=rhs->outerCutOff;
}