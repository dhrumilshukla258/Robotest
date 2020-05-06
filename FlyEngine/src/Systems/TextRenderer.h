#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Managers/GraphicsManager.h"

struct Character {
	unsigned int TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void RenderText(std::string text, glm::vec3 scale, glm::vec3 color);
	void Update();

private:
	Shader shader;
	std::map<char, Character> Characters;
	unsigned int VAO, VBO;
};