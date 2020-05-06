/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Arnold George, arnold.george, 60001018
- End Header --------------------------------------------------------*/

#include "../pch.h"

#include "GraphicsManager.h"

#include "../Managers/ResourceManager.h"
#include "../Shaders/stb_image.h"
#include "../Shaders/Shader.h"
#include "../Shaders/Mesh.h"

using namespace std;

//
#define CustomFlags ( \
    aiProcess_CalcTangentSpace      |  \
    aiProcess_GenNormals            |  \
    aiProcess_JoinIdenticalVertices |  \
    aiProcess_Triangulate           |  \
    aiProcess_GenUVCoords           |  \
    aiProcess_SortByPType           |  \
aiProcess_OptimizeMeshes	        |\
aiProcess_OptimizeGraph             |\
    0 )

#define CustomFlags1 ( \
    aiProcess_CalcTangentSpace      |  \
    aiProcess_GenNormals            |  \
    aiProcess_JoinIdenticalVertices |  \
    aiProcess_Triangulate           |  \
    aiProcess_GenUVCoords           |  \
    aiProcess_SortByPType           |  \
	aiProcess_OptimizeMeshes	        |\
	aiProcess_OptimizeGraph             |\
	aiProcess_SplitLargeMeshes          |\
aiProcess_GenSmoothNormals				|\
aiProcess_ValidateDataStructure			|\
aiProcess_FindInvalidData				|\
aiProcess_OptimizeAnimations			|\
  aiProcess_FindInstances                  |\
aiProcess_Triangulate					|\
aiProcess_SortByPType					|\
aiProcess_ImproveCacheLocality			|\
aiProcess_RemoveRedundantMaterials		|\
aiProcess_TransformUVCoords				|\
aiProcess_LimitBoneWeights				|\
aiProcess_SplitByBoneCount				|\
    0 )













///
extern float SCR_WIDTH;
extern float SCR_HEIGHT;

extern ResourceManager* gpResourceManager;

Mesh ProcessMesh(aiMesh*, const aiScene*, std::string);
std::vector<Texture> LoadMaterialTextures(aiMaterial*, aiTextureType, std::string, std::string);
std::string loadFile(const char*);



GraphicsManager::GraphicsManager()
{
	shader2D = Shader("src/Shaders/shader2D.vert","src/Shaders/shader2D.frag");
	shader3D = Shader("src/Shaders/shader3D.vert", "src/Shaders/shader3D.frag");
	shader3D_Debug = Shader("src/Shaders/shader3D_Debug.vert", "src/Shaders/shader3D_Debug.frag");
	shader3D_Shadow = Shader("src/Shaders/shader3D_Shadow.vert", "src/Shaders/shader3D_Shadow.frag");
	shader3D_Raw = Shader("src/Shaders/shader3D_Raw.vert", "src/Shaders/shader3D_Raw.frag");
	shader_Particle = Shader("src/Shaders/shader_Particles.vert", "src/Shaders/shader_Particles.frag");
	shader_Stencil = Shader("src/Shaders/shader3D_Stencil.vert", "src/Shaders/shader3D_Stencil.frag");
	shader_HDR = Shader("src/Shaders/shader_HDR.vert", "src/Shaders/shader_HDR.frag");
	shader_Blur = Shader("src/Shaders/shader_Blur.vert", "src/Shaders/shader_Blur.frag");
}

GraphicsManager::~GraphicsManager()
{
}

int GraphicsManager::ShaderInitialize(std::string vs, std::string fs)
{
	int sP;

	std::string t1 = loadFile(vs.c_str());
	
	const char* vertexShaderSource = t1.c_str();
	std::string t2 = loadFile(fs.c_str());
	const char* fragmentShaderSource = t2.c_str();

	// --------------------------------------------------------------------------------------
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOG_WARN("FlyEngine", "File::GraphicsManager Function::ShaderInitialization Details::Error in Vertex Shader Compilation {} for Shader file {}", infoLog, vs);
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		LOG_WARN("FlyEngine", "File::GraphicsManager Function::ShaderInitialization Details::Error in Fragment Shader Compilation {} for Shader file {}", infoLog, fs);
	}

	sP = glCreateProgram();
	glAttachShader(sP, vertexShader);
	glAttachShader(sP, fragmentShader);
	glLinkProgram(sP);

	glGetProgramiv(sP, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(sP, 512, NULL, infoLog);
		LOG_WARN("FlyEngine", "File::GraphicsManager Function::ShaderInitialization Details::Error in Shader Program Linking Failed {} for Shader file {} and {}", infoLog, vs, fs);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return sP;
}

unsigned int GraphicsManager::CreateRekt() 
{
	unsigned int VAO, VBO, EBO;

	float vertices[] = { -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 
					0.5f,  0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
					0.5f, -0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-0.5f, -0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   0.0f, 0.0f // bottom left

	};
	int	size = sizeof(vertices);

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//	gpRenderManager->EBO = EBO;
	return VAO;


}

unsigned int GraphicsManager::CreateRekt_Particle()
{

	unsigned int VAO, VBO, EBO;

	float vertices[] = { -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 
					0.5f,  0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
					0.5f, -0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-0.5f, -0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   0.0f, 0.0f // bottom left

	};
	int	size = sizeof(vertices);

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &vertexMatrix);
	glGenBuffers(1, &vertexColor);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	//////////////////////////////////


	modelMatrices.resize(amount);

	
	glBindBuffer(GL_ARRAY_BUFFER, vertexMatrix);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);


	///////////////////////////////////////////

	
	color_Vec4.resize(amount);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexColor);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);


	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	glVertexAttribDivisor(7, 1);


	glBindVertexArray(0);
/////////////////////////////////////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return VAO;
}

unsigned int GraphicsManager::CreateTriangle()
{
	unsigned int VAO, VBO;
	float vertices[] = { -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
					0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
					0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.5f, 1.0f
	};
	int	size = sizeof(vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}

unsigned int GraphicsManager::CreateCircle()
{
	unsigned int VAO, VBO;
	float vertices[] = { 0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
			 0.433f,   0.25f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.353f,  0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			  0.25f,  0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			   0.0f,    0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
			 -0.25f,  0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.353f,  0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.433f,   0.25f,  0.0f, 1.0f, 1.0f, 0.0f,
			  -0.5f,    0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.433f,  -0.25f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.353f, -0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			 -0.25f, -0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			   0.0f,   -0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
			  0.25f, -0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.353f, -0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.433f,  -0.25f,  0.0f, 1.0f, 1.0f, 0.0f
	};
	int	size = sizeof(vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

unsigned int GraphicsManager::CreateLine()
{
	unsigned int VAO, VBO;
	float vertices[] = { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
						0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};
	int	size = sizeof(vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

unsigned int GraphicsManager::LoadTextureCall(std::string _filename)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(_filename.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		LOG_WARN("FlyEngine","File::GraphicsManager.cpp Function::LoadTextureCall Details::Failed to Load Texture for file {}:", _filename);
	}
	stbi_image_free(data);

	return textureId;
}

void GraphicsManager::Frame_Bubber_Set()
{
	glGenFramebuffers(1, &frame_buffer);


	glGenTextures(1, &frame_buffer_texture_buffer);

	glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_buffer);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frame_buffer_texture_buffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "frame buffer failure" << std::endl;
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

std::vector<Mesh> GraphicsManager::LoadModelCall(std::string _filename)
{
	std::string directory;
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(_filename, CustomFlags1);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		ASSERT("FlyEngine", nullptr, "File::GraphicsManager.cpp Function::LoadModelCall Details::Error in ASSIMP {}", importer.GetErrorString());
		std::vector<Mesh> error;
		return error;
	}

	directory = _filename.substr(0, _filename.find_last_of('\\'));
	if ("Resources/Assets/Object/Running.fbx" == directory)
	{
		int x = 0;
	}
	meshes.clear();
	/////////////////////anim
	m_global_inverse_transform = scene->mRootNode->mTransformation;
	m_global_inverse_transform.Inverse();
	
	if (scene->mAnimations != nullptr)
	{
		pScene = const_cast<aiScene*>(scene);
		animation = true;
		if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
		{
      			ticks_per_second = (float)scene->mAnimations[0]->mTicksPerSecond;
		}
		else
		{
			ticks_per_second = 30.0f;
		}


		/*cout << "scene->HasAnimations() 1: " << scene->HasAnimations() << endl;
		cout << "scene->mNumMeshes 1: " << scene->mNumMeshes << endl;
		cout << "scene->mAnimations[0]->mNumChannels 1: " << scene->mAnimations[0]->mNumChannels << endl;
		cout << "scene->mAnimations[0]->mDuration 1: " << scene->mAnimations[0]->mDuration << endl;
		cout << "scene->mAnimations[0]->mTicksPerSecond 1: " << scene->mAnimations[0]->mTicksPerSecond << endl << endl;

		cout << "		name nodes : " << endl;*/
		showNodeName(scene->mRootNode);
		//cout << endl;

		ProcessNode(scene->mRootNode, scene, directory);

		//cout << "		name nodes animation : " << endl;
		/*for (uint i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
		{
			cout << scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
		}*/
		//////////////////////////
		
		importer.GetOrphanedScene();
		return meshes;
		
	}
	else
	{
		ProcessNode(scene->mRootNode, scene, directory);
		animation = false;
		return meshes;
	}
	
	
}
void GraphicsManager::showNodeName(aiNode * node)
{
	//cout << node->mName.data << endl;
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		showNodeName(node->mChildren[i]);
	}
}
void GraphicsManager::ProcessNode(aiNode* node, const aiScene* scene, std::string _directory)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, _directory));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, _directory);
	}
}

Mesh GraphicsManager::ProcessMesh(aiMesh * mesh, const aiScene * scene, std::string _directory)
{
	//std::cout << "bones: " << mesh->mNumBones << " vertices: " << mesh->mNumVertices << std::endl;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	vector<VertexBoneData> bones_id_weights_for_each_vertex;
	
	vertices.reserve(mesh->mNumVertices); 
	indices.reserve(mesh->mNumVertices);

	//Vertex Creation
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{



		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;

			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		//vector.x = mesh->mTangents[i].x;
		//vector.y = mesh->mTangents[i].y;
		//vector.z = mesh->mTangents[i].z;
		//vertex.Tangent = vector;

		//vector.x = mesh->mBitangents[i].x;
		//vector.y = mesh->mBitangents[i].y;
		//vector.z = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}

	//Indices Creation
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//Texture Creation
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	{
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", _directory);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", _directory);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", _directory);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", _directory);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	if (mesh->mNumBones > 0)
	{
		bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);
	}
	// load bones
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		int bone_index = 0;
		string bone_name(mesh->mBones[i]->mName.data);

		//cout << mesh->mBones[i]->mName.data << endl;

		if (m_bone_mapping.find(bone_name) == m_bone_mapping.end()) // ��������� ��� �� � ������� ��������
		{
			// Allocate an index for a new bone
			bone_index = m_num_bones;
			m_num_bones++;
			BoneMatrix bi;
			m_bone_matrices.push_back(bi);
			m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
			m_bone_mapping[bone_name] = bone_index;

			//cout << "bone_name: " << bone_name << "			 bone_index: " << bone_index << endl;
		}
		else
		{
			bone_index = m_bone_mapping[bone_name];
		}

		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			int vertex_id = mesh->mBones[i]->mWeights[j].mVertexId; // �� ������� �� ������ ����� �������� �����
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight); // � ������ ������� ����� ����� � �� ���

			// ������ ������� vertex_id �� ������ ����� � �������� bone_index  ����� ��� weight
			//cout << " vertex_id: " << vertex_id << "	bone_index: " << bone_index << "		weight: " << weight << endl;
		}
	}


	return Mesh(vertices, indices, textures, bones_id_weights_for_each_vertex);
}

unsigned int GraphicsManager::Create_Frame_Buffer_Pointer()
{
	
	


	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	//////////////
	glGenTextures(2, colorBuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, (GLint)0, GL_RGB16F, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT, (GLint)0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach texture to framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
	}
	
	
	/////////////////////
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT );
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, (GLsizei)SCR_WIDTH, (GLsizei)SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		//unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0};
		//glDrawBuffers(1, attachments);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	return hdrFBO;
}

unsigned int GraphicsManager::CreateQuad()
{
	
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	
		return quadVAO;
}




//Non Class Functions 

std::vector<Texture> LoadMaterialTextures(aiMaterial * _mat, aiTextureType _type, std::string _typeName, std::string _directory)
{
	
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < _mat->GetTextureCount(_type); i++)
	{
		Texture texture;
		aiString str;
		_mat->GetTexture(_type, i, &str);
		
		std::string textureFile = std::string(str.C_Str());

		

		texture.id = gpResourceManager->LoadTexture(textureFile);
		texture.type = _typeName;
		textures.push_back(texture);
	}
	return textures;
}

std::string loadFile(const char *fname) 
{
	std::string out, line;
	std::ifstream in(fname);
	getline(in, line);
	while (in) {
		out += line + "\n";
		getline(in, line);
	}
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,face_buffer_size,torus.faceArray(),GL_STATIC_DRAW);
	return out;
}

