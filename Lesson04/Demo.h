#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2, 
		   VBO3, VAO3, EBO3, texture3, VBO4, VAO4, EBO4, texture4, VBO5, VAO5, EBO5, texture5,
		   VBO6, VAO6, EBO6, texture6, VBO7, VAO7, EBO7, texture7, VBO8, VAO8, EBO8, texture8,
		   VBO9, VAO9, EBO9, texture9, VBO10, VAO10, EBO10, texture10, VBO11, VAO11, EBO11, texture11;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	float angle = 0;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);

	void BuildTembok();
	void BuildLantai();
	void BuildPintu();
	void BuildAtap();
	void BuildPilar();
	void BuildTv();
	void BuildWindow();
	void BuildCarpet();
	void BuildSofa();
	void BuildTable();
	void BuildBerita();

	void DrawTembok();
	void DrawLantai();
	void DrawPintu();
	void DrawAtap();
	void DrawPilar();
	void DrawTv();
	void DrawWindow();
	void DrawCarpet();
	void DrawSofa();
	void DrawTable();
	void DrawBerita();

	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();
};

