/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "Control.h"

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

Control::Control()
{
	mComponentName = "Control";
}


Control::~Control()
{
}

void Control::SelfDelete() 
{
	gpComponentManager->mControl->Delete(this);
}

void Control::Clone(Component * rhs1) 
{
	Control* rhs = static_cast<Control*>(rhs1);
}