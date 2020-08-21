#pragma once
#include "Shader.h"


//! ����� ������ ����������� ���������. 
//! ��������� ����� ������������ LightManager
class PixelLightShader : public tge::Shader
{
public:
	PixelLightShader();
	~PixelLightShader();

private:
	static sf::Shader my_shader;
	friend class tge::ShaderManager;
};
