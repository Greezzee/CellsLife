#pragma once
#include "Shader.h"

//! ������ �������� ���������
//! ��������� ����� ������������ LightManager
class SmoothLightShader : public tge::Shader
{
public:
	SmoothLightShader();
	~SmoothLightShader();

private:
	static sf::Shader my_shader;
	friend class tge::ShaderManager;
};
