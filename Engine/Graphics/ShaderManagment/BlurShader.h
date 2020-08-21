#pragma once
#include "Shader.h"
#include "ShaderManager.h"
//! ����� ������ "��������"
class BlurShader : public tge::Shader
{
public:
	BlurShader();
	~BlurShader();
	//! ������������ ������� ��������. ����� �����, ������� 0.01
	static void SetBlurRadius(float r);
	static float GetBlurRadius();

private:
	static float _radius;
	static sf::Shader my_shader;
	friend class tge::ShaderManager;
};
