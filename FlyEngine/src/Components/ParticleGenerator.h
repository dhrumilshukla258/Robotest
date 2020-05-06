#pragma once
#include "../pch.h"
#include "../Component.h"
#include "../Components/Particle.h"

class ParticleGenerator : public Component
{
public:
	ParticleGenerator();
	~ParticleGenerator();
	void Init();
	void SelfDelete();
	void Clone(Component *);

public:
	std::list<Particle_Struct*> particle_list;
	std::vector<glm::mat4> model_Matrix;
	std::vector<glm::vec4> color_Particles;
	std::string texturePath;

	unsigned int mVAO;

	unsigned int mTextureID;


	int max_Paticles;
;

	 GLuint vertexMatrix;
	GLuint vertexColor;

private:
	RTTR_ENABLE(Component)
};