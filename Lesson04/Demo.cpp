#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildTembok();
	BuildLantai();
	BuildPintu();
	BuildAtap();
	BuildPilar();
	BuildTv();
	BuildWindow();
	BuildCarpet();
	BuildSofa();
	BuildTable();
	BuildBerita();

	InitCamera();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// zoom camera
	// -----------
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		if (fovy < 90) {
			fovy += 0.0001f;
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		if (fovy > 0) {
			fovy -= 0.0001f;
		}
	}

	// update camera movement 
	// -------------
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		MoveCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		MoveCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		StrafeCamera(-CAMERA_SPEED);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		StrafeCamera(CAMERA_SPEED);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		VAO11 = VAO;
	}

	// update camera rotation
	// ----------------------
	double mouseX, mouseY;
	double midX = screenWidth/2;
	double midY = screenHeight/2;
	float angleY = 0.0f;
	float angleZ = 0.0f;

	// Get mouse position
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if ((mouseX == midX) && (mouseY == midY)) {
		return;
	}

	// Set mouse position
	glfwSetCursorPos(window, midX, midY);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angleY = (float)((midX - mouseX)) / 1000;
	angleZ = (float)((midY - mouseY)) / 1000;

	// The higher the value is the faster the camera looks around.
	viewCamY += angleZ * 2;

	// limit the rotation around the x-axis
	if ((viewCamY - posCamY) > 8) {
		viewCamY = posCamY + 8;
	}
	if ((viewCamY - posCamY) < -8) {
		viewCamY = posCamY - 8;
	}
	RotateCamera(-angleY);


	

}

void Demo::Update(double deltaTime) {
	angle += (float)((deltaTime * 1.5f) / 1000);
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(fovy, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(posCamX, posCamY, posCamZ), glm::vec3(viewCamX, viewCamY, viewCamZ), glm::vec3(upCamX, upCamY, upCamZ));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	DrawTembok();
	DrawLantai();
	DrawPintu();
	DrawAtap();
	DrawPilar();
	DrawTv();
	DrawWindow();
	DrawCarpet();
	DrawSofa();
	DrawTable();
	DrawBerita();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildTembok() {
	// load image into texture memory
	// ------------------------------
	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("wall_wood.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// tembok serong kiri
		-8.5,  3.0, 10.0, 0, 0,  // 0
		-6.5,  3.0, 11.5, 1, 0,   // 1
		-6.5,  6.0, 11.5, 1, 1,   // 2
		-8.5,  6.0, 10.0, 0, 1,  // 3

		// tembok kanan (posisi, tinggi, panjang)
		8.5,  6.0,  10.0, 0, 0,  // 4
		8.5,  6.0, -5.0,  1, 0,  // 5
		8.5, -0.5, -5.0,  1, 1,  // 6
		8.5, -0.5,  10.0, 0, 1,  // 7

		// tembok serong kanan
		8.5,  6.0, 10.0, 0, 0, // 8 
		6.5,  6.0, 11.5, 1, 0, // 9
		6.5,  3.0, 11.5, 1, 1, // 10
		8.5,  3.0, 10.0, 0, 1, // 11

		// tembok pintu masuk
		 6.5,  6.0, 11.5, 0, 0, // 12 
		-6.5,  6.0, 11.5, 1, 0, // 13
		-6.5,  3.0, 11.5, 1, 1, // 14
		 6.5,  3.0, 11.5, 0, 1, // 15

		// tembok kiri
		-8.5, -0.5,  -5.0, 0, 0, // 16
		-8.5, -0.5,  10.0, 1, 0, // 17
		-8.5,  6.0,  10.0, 1, 1, // 18
		-8.5,  6.0,  -5.0, 0, 1, // 19

		// tembok resepsionis
		-5.0, -0.5,  -5.0, 0, 0,  // 20
		 5.0, -0.5,  -5.0, 1, 0,  // 21
		 5.0,  6.0,  -5.0, 1, 1,  // 22
		-5.0,  6.0,  -5.0, 0, 1,  // 123

		// tembok resepsionis kiri
		-8.5, 3.0,  -5.0, 0, 0,  // 24
		-5.0, 3.0,  -5.0, 1, 0,  // 25
		-5.0, 6.0,  -5.0, 1, 1,  // 26
		-8.5, 6.0,  -5.0, 0, 1,  // 27

		// tembok resepsionis kanan
		8.5, 3.0, -5.0, 0, 0,  // 28
		5.0, 3.0, -5.0, 1, 0,  // 29
		5.0, 6.0, -5.0, 1, 1,  // 30
		8.5, 6.0, -5.0, 0, 1,  // 31

		//meja resepsionis
		2.5,  0.5,  -1.5, 0, 0,  // 32
	   -2.5,  0.5,  -1.5, 1, 0,  // 33
	   -2.5,  0.5,  -1.0, 1, 1,  // 34
		2.5,  0.5,  -1.0, 0, 1,  // 35
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22,   // bottom
		24, 26, 25, 24, 27, 26,
		28, 30, 29, 28, 31, 30,
		32, 34, 33, 32, 35, 34
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define texcoord pointer layout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawTembok()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildLantai()
{
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("marble_floor.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-8.5, -0.5, -5.0,   0,  0,  //1
		 8.5, -0.5, -5.0,  20,  0,  //2
		 8.5, -0.5,  11.5, 20, 20,  //3
		-8.5, -0.5,  11.5,  0, 20,  //4
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawLantai()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPintu()
{
	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("door.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		 4.0, -0.5, 11.5, 0, 0, // 0 
		-4.0, -0.5, 11.5, 1, 0, // 1
		-4.0,  3.0, 11.5, 1, 1, // 2
		 4.0,  3.0, 11.5, 0, 1, // 3

		-8.5, 3.0,  -5.0, 0, 0,  // 4
		-5.0, 3.0,  -5.0, 1, 0,  // 5
		-5.0,-0.5,  -5.0, 1, 1,  // 6
		-8.5,-0.5,  -5.0, 0, 1,  // 7

		 8.5, 3.0, -5.0, 0, 0,  // 8
		 5.0, 3.0, -5.0, 1, 0,  // 9
		 5.0,-0.5, -5.0, 1, 1,  // 10
		 8.5,-0.5, -5.0, 0, 1,  // 11

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8, 10,  9,  8,  11, 10
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawPintu()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO3); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildAtap()
{
	// Load and create a texture 
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("ceiling.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-8.5, 6.0, -5.0,   0,  0,  //1
		 8.5, 6.0, -5.0,  20,  0,  //2
		 8.5, 6.0,  11.5, 20, 20,  //3
		-8.5, 6.0,  11.5,  0, 20,  //4

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);

	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawAtap()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO4); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildPilar()
{
	// Load and create a texture 
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("pilar.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//pilar kiri 1
		-4.0, -0.5,  -0.5, 0, 0,  // 0
		-2.5, -0.5,  -0.5, 1, 0,  // 1
		-2.5,  6.0,  -0.5, 1, 1,  // 2
		-4.0,  6.0,  -0.5, 0, 1,  // 3

		-2.5, -0.5,  -2.0, 0, 0,  // 4
		-2.5, -0.5,  -0.5, 1, 0,  // 5
		-2.5,  6.0,  -0.5, 1, 1,  // 6
		-2.5,  6.0,  -2.0, 0, 1,  // 7

		-4.0, -0.5,  -0.5, 0, 0,  // 8
		-4.0, -0.5,  -2.0, 1, 0,  // 9
		-4.0,  6.0,  -2.0, 1, 1,  // 10
		-4.0,  6.0,  -0.5, 0, 1,  // 11

		-2.5, -0.5,  -2.0, 0, 0,  // 12
		-4.0, -0.5,  -2.0, 1, 0,  // 13
		-4.0,  6.0,  -2.0, 1, 1,  // 14
		-2.5,  6.0,  -2.0, 0, 1,  // 15

		//pilar kanan 1
		4.0, -0.5,  -0.5, 0, 0,  // 16
		2.5, -0.5,  -0.5, 1, 0,  // 17
		2.5,  6.0,  -0.5, 1, 1,  // 18
		4.0,  6.0,  -0.5, 0, 1,  // 19

		2.5, -0.5,  -2.0, 0, 0,  // 20
		2.5, -0.5,  -0.5, 1, 0,  // 21
		2.5,  6.0,  -0.5, 1, 1,  // 22
		2.5,  6.0,  -2.0, 0, 1,  // 23

		4.0, -0.5,  -0.5, 0, 0,  // 24
		4.0, -0.5,  -2.0, 1, 0,  // 25
		4.0,  6.0,  -2.0, 1, 1,  // 26
		4.0,  6.0,  -0.5, 0, 1,  // 27

		2.5, -0.5,  -2.0, 0, 0,  // 28
		4.0, -0.5,  -2.0, 1, 0,  // 29
		4.0,  6.0,  -2.0, 1, 1,  // 30
		2.5,  6.0,  -2.0, 0, 1,  // 31

		//meja resepsionis
		2.5, -0.5,  -0.7, 0, 0,  // 32
       -2.5, -0.5,  -0.7, 1, 0,  // 33
	   -2.5,  0.7,  -0.7, 1, 1,  // 34
		2.5,  0.7,  -0.7, 0, 1,  // 35

		2.5, -0.5,  -1.0, 0, 0,  // 36
	   -2.5, -0.5,  -1.0, 1, 0,  // 37
	   -2.5,  0.7,  -1.0, 1, 1,  // 38
		2.5,  0.7,  -1.0, 0, 1,  // 39

		2.5,  0.7,  -0.7, 0, 0,  // 40
	   -2.5,  0.7,  -0.7, 1, 0,  // 41
	   -2.5,  0.7,  -1.0, 1, 1,  // 42
		2.5,  0.7,  -1.0, 0, 1,  // 43

		2.5, -0.5,  -1.5, 0, 0,  // 44
	   -2.5, -0.5,  -1.5, 1, 0,  // 45
	   -2.5,  0.5,  -1.5, 1, 1,  // 46
		2.5,  0.5,  -1.5, 0, 1,  // 47
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8,  10, 9,  8,  11, 10,
						 12, 14, 13, 12, 15, 14,
						 16, 18, 17, 16, 19, 18,
						 20, 22, 21, 20, 23, 22,
						 24, 26, 25, 24, 27, 26,
						 28, 30, 29, 28, 31, 30,
						 32, 34, 33, 32, 35, 34,
						 36, 38, 37, 36, 39, 38,
						 40, 42, 41, 40, 43, 42,
						 44, 46, 45, 44, 47, 46
	};

	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);

	glBindVertexArray(VAO5);

	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawPilar()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO5); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 148, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTv()
{
	// Load and create a texture 
	glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("plastic.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//tv belakang resepsionis
		-2.5,  2.5,  -5.0,  0, 0,  // 0
		-2.5,  2.5,  -4.85, 1, 0,  // 1
		-2.5,  5.0,  -4.85, 1, 1,  // 2
		-2.5,  5.0,  -5.0,  0, 1,  // 3

		 2.5,  2.5,  -5.0,  0, 0,  // 4
		 2.5,  2.5,  -4.85, 1, 0,  // 5
		 2.5,  5.0,  -4.85, 1, 1,  // 6
		 2.5,  5.0,  -5.0,  0, 1,  // 7

		 2.5,  5.0,  -5.0,  0, 0,  // 8
		 2.5,  5.0,  -4.85, 1, 0,  // 9
		-2.5,  5.0,  -4.85, 1, 1,  // 10
		-2.5,  5.0,  -5.0,  0, 1,  // 11

		-2.5,  2.5,  -5.0,  0, 0,  // 12
		-2.5,  2.5,  -4.85, 1, 0,  // 13
		 2.5,  2.5,  -4.85, 1, 1,  // 14
		 2.5,  2.5,  -5.0,  0, 1,  // 15

		-2.5,  2.5,  -4.85,  0, 0,  // 16
		 2.5,  2.5,  -4.85,  1, 0,  // 17
		 2.5,  5.0,  -4.85,  1, 1,  // 18
		-2.5,  5.0,  -4.85,  0, 1,  // 19
								
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8,  10, 9,  8,  11, 10,
						 12, 14, 13, 12, 15, 14,
						 16, 18, 17, 16, 19, 18,
	};

	glGenVertexArrays(1, &VAO6);
	glGenBuffers(1, &VBO6);
	glGenBuffers(1, &EBO6);

	glBindVertexArray(VAO6);

	glBindBuffer(GL_ARRAY_BUFFER, VBO6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTv()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO6); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 148, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildWindow()
{
	// Load and create a texture 
	glGenTextures(1, &texture7);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("window.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//jendela pintu
		 6.5, -0.5, 11.5, 0, 0, // 0 
		 4.0, -0.5, 11.5, 1, 0, // 1
		 4.0,  3.0, 11.5, 1, 1, // 2
		 6.5,  3.0, 11.5, 0, 1, // 3

		 -6.5, -0.5, 11.5, 0, 0, // 4 
		 -4.0, -0.5, 11.5, 1, 0, // 5
		 -4.0,  3.0, 11.5, 1, 1, // 6
		 -6.5,  3.0, 11.5, 0, 1, // 7

		 //jendela serong kiri
		-8.5, -0.5, 10.0, 0, 0,  // 8
		-6.5, -0.5, 11.5, 1, 0,  // 9
		-6.5,  3.0, 11.5, 1, 1,  // 10
		-8.5,  3.0, 10.0, 0, 1,  // 11

		//jendela serong kanan
		8.5,  3.0, 10.0, 0, 0, // 8 
		6.5,  3.0, 11.5, 1, 0, // 9
		6.5, -0.5, 11.5, 1, 1, // 10
		8.5, -0.5, 10.0, 0, 1, // 11
		 		
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8, 10,  9,  8,  11, 10,
						 12, 14, 13, 12, 15, 14
	};

	glGenVertexArrays(1, &VAO7);
	glGenBuffers(1, &VBO7);
	glGenBuffers(1, &EBO7);

	glBindVertexArray(VAO7);

	glBindBuffer(GL_ARRAY_BUFFER, VBO7);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO7);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawWindow()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO7); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 148, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildCarpet()
{
	// Load and create a texture 
	glGenTextures(1, &texture8);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("red_carpet.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-8.5, -0.49,  3.0,   0,  0,  //1
		-2.3, -0.49,  3.0,  20,  0,  //2
		-2.3, -0.49,  11.5, 20, 20,  //3
		-8.5, -0.49,  11.5,  0, 20,  //4

		8.5, -0.49,  3.0,   0,  0,  //5
		2.3, -0.49,  3.0,  20,  0,  //6
		2.3, -0.49,  11.5, 20, 20,  //7
		8.5, -0.49,  11.5,  0, 20,  //8
	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6 };

	glGenVertexArrays(1, &VAO8);
	glGenBuffers(1, &VBO8);
	glGenBuffers(1, &EBO8);

	glBindVertexArray(VAO8);

	glBindBuffer(GL_ARRAY_BUFFER, VBO8);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO8);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawCarpet()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO8); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildSofa()
{
	// Load and create a texture 
	glGenTextures(1, &texture9);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("sofa.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//sofa panjang kanan
		 7.0,  0.7,  9.0,  0, 0,  // 0
		 7.0,  0.7,  5.0,  2, 0,  // 1
		 7.0, -0.5,  5.0,  2, 2,  // 2
		 7.0, -0.5,  9.0,  0, 2,  // 3

		 6.7,  0.7,  9.0,  0, 0,  // 4
		 6.7,  0.7,  5.0,  2, 0,  // 5
		 6.7, -0.5,  5.0,  2, 2,  // 6
		 6.7, -0.5,  9.0,  0, 2,  // 7

		 7.0,  0.7,  5.0,  0, 0,  // 8
		 6.7,  0.7,  5.0,  2, 0,  // 9
		 6.7, -0.5,  5.0,  2, 2,  // 10
		 7.0, -0.5,  5.0,  0, 2,  // 11

		 7.0,  0.7,  9.0,  0, 0,  // 12
		 6.7,  0.7,  9.0,  2, 0,  // 13
		 6.7, -0.5,  9.0,  2, 2,  // 14
		 7.0, -0.5,  9.0,  0, 2,  // 15

		 7.0,  0.7,  9.0,  0, 0,  // 16
		 7.0,  0.7,  5.0,  2, 0,  // 17
		 6.7,  0.7,  5.0,  2, 2,  // 18
		 6.7,  0.7,  9.0,  0, 2,  // 19

		 6.0,  0.4,  9.0,  0, 0,  // 20
		 6.7,  0.4,  9.0,  2, 0,  // 21
		 6.7, -0.5,  9.0,  2, 2,  // 22
		 6.0, -0.5,  9.0,  0, 2,  // 23

		 6.0,  0.4,  8.7,  0, 0,  // 24
		 6.7,  0.4,  8.7,  2, 0,  // 25
		 6.7, -0.5,  8.7,  2, 2,  // 26
		 6.0, -0.5,  8.7,  0, 2,  // 27

		 6.0,  0.4,  5.3,  0, 0,  // 28
		 6.7,  0.4,  5.3,  2, 0,  // 29
		 6.7, -0.5,  5.3,  2, 2,  // 30
		 6.0, -0.5,  5.3,  0, 2,  // 31

		 6.0,  0.4,  5.0,  0, 0,  // 32
		 6.7,  0.4,  5.0,  2, 0,  // 33
		 6.7, -0.5,  5.0,  2, 2,  // 34
		 6.0, -0.5,  5.0,  0, 2,  // 35

		 6.0,  0.4,  9.0,  0, 0,  // 36
		 6.0,  0.4,  8.7,  2, 0,  // 37
		 6.0, -0.5,  8.7,  2, 2,  // 38
		 6.0, -0.5,  9.0,  0, 2,  // 39

		 6.0,  0.4,  5.0,  0, 0,  // 40
		 6.0,  0.4,  5.3,  2, 0,  // 41
		 6.0, -0.5,  5.3,  2, 2,  // 42
		 6.0, -0.5,  5.0,  0, 2,  // 43

		 6.0,  0.4,  9.0,  0, 0,  // 44
		 6.0,  0.4,  8.7,  2, 0,  // 45
		 6.7,  0.4,  8.7,  2, 2,  // 46
		 6.7,  0.4,  9.0,  0, 2,  // 47

		 6.0,  0.4,  5.0,  0, 0,  // 48
		 6.0,  0.4,  5.3,  2, 0,  // 49
		 6.7,  0.4,  5.3,  2, 2,  // 50
		 6.7,  0.4,  5.0,  0, 2,  // 51

		 6.0,  0.1,  8.7,  0, 0,  // 52
		 6.0,  0.1,  5.3,  2, 0,  // 53
		 6.0, -0.5,  5.3,  2, 2,  // 54
		 6.0, -0.5,  8.7,  0, 2,  // 55

		 6.0,  0.1,  8.7,  0, 0,  // 56
		 6.0,  0.1,  5.3,  2, 0,  // 57
		 6.7,  0.1,  5.3,  2, 2,  // 58
		 6.7,  0.1,  8.7,  0, 2,  // 59

		 //sofa panjang kiri
		 -7.0,  0.7,  9.0,  0, 0,  // 60
		 -7.0,  0.7,  5.0,  2, 0,  // 61
		 -7.0, -0.5,  5.0,  2, 2,  // 62
		 -7.0, -0.5,  9.0,  0, 2,  // 63

		 -6.7,  0.7,  9.0,  0, 0,  // 64
		 -6.7,  0.7,  5.0,  2, 0,  // 65
		 -6.7, -0.5,  5.0,  2, 2,  // 66
		 -6.7, -0.5,  9.0,  0, 2,  // 67

		 -7.0,  0.7,  5.0,  0, 0,  // 68
		 -6.7,  0.7,  5.0,  2, 0,  // 69
		 -6.7, -0.5,  5.0,  2, 2,  // 70
		 -7.0, -0.5,  5.0,  0, 2,  // 71

		 -7.0,  0.7,  9.0,  0, 0,  // 72
		 -6.7,  0.7,  9.0,  2, 0,  // 73
		 -6.7, -0.5,  9.0,  2, 2,  // 74
		 -7.0, -0.5,  9.0,  0, 2,  // 75

		 -7.0,  0.7,  9.0,  0, 0,  // 76
		 -7.0,  0.7,  5.0,  2, 0,  // 77
		 -6.7,  0.7,  5.0,  2, 2,  // 78
		 -6.7,  0.7,  9.0,  0, 2,  // 79

		 -6.0,  0.4,  9.0,  0, 0,  // 80
		 -6.7,  0.4,  9.0,  2, 0,  // 81
		 -6.7, -0.5,  9.0,  2, 2,  // 82
		 -6.0, -0.5,  9.0,  0, 2,  // 83

		 -6.0,  0.4,  8.7,  0, 0,  // 84
		 -6.7,  0.4,  8.7,  2, 0,  // 85
		 -6.7, -0.5,  8.7,  2, 2,  // 86
		 -6.0, -0.5,  8.7,  0, 2,  // 87

		 -6.0,  0.4,  5.3,  0, 0,  // 88
		 -6.7,  0.4,  5.3,  2, 0,  // 89
		 -6.7, -0.5,  5.3,  2, 2,  // 90
		 -6.0, -0.5,  5.3,  0, 2,  // 91

		 -6.0,  0.4,  5.0,  0, 0,  // 92
		 -6.7,  0.4,  5.0,  2, 0,  // 93
		 -6.7, -0.5,  5.0,  2, 2,  // 94
		 -6.0, -0.5,  5.0,  0, 2,  // 95

		 -6.0,  0.4,  9.0,  0, 0,  // 96
		 -6.0,  0.4,  8.7,  2, 0,  // 97
		 -6.0, -0.5,  8.7,  2, 2,  // 98
		 -6.0, -0.5,  9.0,  0, 2,  // 99

		 -6.0,  0.4,  5.0,  0, 0,  // 100
		 -6.0,  0.4,  5.3,  2, 0,  // 101
		 -6.0, -0.5,  5.3,  2, 2,  // 102
		 -6.0, -0.5,  5.0,  0, 2,  // 103

		 -6.0,  0.4,  9.0,  0, 0,  // 104
		 -6.0,  0.4,  8.7,  2, 0,  // 105
		 -6.7,  0.4,  8.7,  2, 2,  // 106
		 -6.7,  0.4,  9.0,  0, 2,  // 107

		 -6.0,  0.4,  5.0,  0, 0,  // 108
		 -6.0,  0.4,  5.3,  2, 0,  // 109
		 -6.7,  0.4,  5.3,  2, 2,  // 110
		 -6.7,  0.4,  5.0,  0, 2,  // 111

		 -6.0,  0.1,  8.7,  0, 0,  // 112
		 -6.0,  0.1,  5.3,  2, 0,  // 113
		 -6.0, -0.5,  5.3,  2, 2,  // 114
		 -6.0, -0.5,  8.7,  0, 2,  // 115

		 -6.0,  0.1,  8.7,  0, 0,  // 116
		 -6.0,  0.1,  5.3,  2, 0,  // 117
		 -6.7,  0.1,  5.3,  2, 2,  // 118
		 -6.7,  0.1,  8.7,  0, 2,  // 119

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8, 10,  9,  8,  11, 10,
						 12, 14, 13, 12, 15, 14,
						 16, 18, 17, 16, 19, 18,
						 20, 22, 21, 20, 23, 22,
						 24, 26, 25, 24, 27, 26,
						 28, 30, 29, 28, 31, 30,
						 32, 34, 33, 32, 35, 34,
						 36, 38, 37, 36, 39, 38,
						 40, 42, 41, 40, 43, 42,
						 44, 46, 45, 44, 47, 46,
						 48, 50, 49, 48, 51, 50,
						 52, 54, 53, 52, 55, 54,
						 56, 58, 57, 56, 59, 58,
						 60, 62, 61, 60, 63, 62,
						 64, 66, 65, 64, 67, 66,
						 68, 70, 69, 68, 71, 70,
						 72, 74, 73, 72, 75, 74,
						 76, 78, 77, 76, 79, 78,
						 80, 82, 81, 80, 83, 82,
						 84, 86, 85, 84, 87, 86,
						 88, 90, 89, 88, 91, 90,
						 92, 94, 93, 92, 95, 94,
						 96, 98, 97, 96, 99, 98,
						 100, 102, 101, 100, 103, 102,
						 104, 106, 105, 104, 107, 106,
						 108, 110, 109, 108, 111, 110,
						 112, 114, 113, 112, 115, 114,
						 116, 118, 117, 116, 119, 118
	};

	glGenVertexArrays(1, &VAO9);
	glGenBuffers(1, &VBO9);
	glGenBuffers(1, &EBO9);

	glBindVertexArray(VAO9);

	glBindBuffer(GL_ARRAY_BUFFER, VBO9);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO9);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawSofa()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO9); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 400, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildTable()
{
	// Load and create a texture 
	glGenTextures(1, &texture10);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("table.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//meja kanan
		 5.5,  0.05,  8.7,  0, 0,  // 0
		 5.5,  0.05,  5.3,  1, 0,  // 1
		 5.5, -0.5,   5.3,  1, 1,  // 2
		 5.5, -0.5,   8.7,  0, 1,  // 3

		 4.0,  0.05,  8.7,  0, 0,  // 4
		 4.0,  0.05,  5.3,  1, 0,  // 5
		 4.0, -0.5,   5.3,  1, 1,  // 6
		 4.0, -0.5,   8.7,  0, 1,  // 7

		 4.0,  0.05,  8.7,  0, 0,  // 8
		 5.5,  0.05,  8.7,  1, 0,  // 9
		 5.5, -0.5,   8.7,  1, 1,  // 10
		 4.0, -0.5,   8.7,  0, 1,  // 11

		 4.0,  0.05,  5.3,  0, 0,  // 12
		 5.5,  0.05,  5.3,  1, 0,  // 13
		 5.5, -0.5,   5.3,  1, 1,  // 14
		 4.0, -0.5,   5.3,  0, 1,  // 15

		 4.0,  0.05,  8.7,  0, 0,  // 16
		 5.5,  0.05,  8.7,  1, 0,  // 17
		 5.5,  0.05,  5.3,  1, 1,  // 18
		 4.0,  0.05,  5.3,  0, 1,  // 19

		 //meja kiri
		 -5.5,  0.05,  8.7,  0, 0,  // 20
		 -5.5,  0.05,  5.3,  1, 0,  // 21
		 -5.5, -0.5,   5.3,  1, 1,  // 22
		 -5.5, -0.5,   8.7,  0, 1,  // 23

		 -4.0,  0.05,  8.7,  0, 0,  // 24
		 -4.0,  0.05,  5.3,  1, 0,  // 25
		 -4.0, -0.5,   5.3,  1, 1,  // 26
		 -4.0, -0.5,   8.7,  0, 1,  // 27

		 -4.0,  0.05,  8.7,  0, 0,  // 28
		 -5.5,  0.05,  8.7,  1, 0,  // 29
		 -5.5, -0.5,   8.7,  1, 1,  // 30
		 -4.0, -0.5,   8.7,  0, 1,  // 31

		 -4.0,  0.05,  5.3,  0, 0,  // 32
		 -5.5,  0.05,  5.3,  1, 0,  // 33
		 -5.5, -0.5,   5.3,  1, 1,  // 34
		 -4.0, -0.5,   5.3,  0, 1,  // 35

		 -4.0,  0.05,  8.7,  0, 0,  // 36
		 -5.5,  0.05,  8.7,  1, 0,  // 37
		 -5.5,  0.05,  5.3,  1, 1,  // 38
		 -4.0,  0.05,  5.3,  0, 1,  // 39

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8, 10,  9,  8,  11, 10,
						 12, 14, 13, 12, 15, 14,
						 16, 18, 17, 16, 19, 18,
						 20, 22, 21, 20, 23, 22,
						 24, 26, 25, 24, 27, 26,
						 28, 30, 29, 28, 31, 30,
						 32, 34, 33, 32, 35, 34,
						 36, 38, 37, 36, 39, 38,
						 40, 42, 41, 40, 43, 42,
						 44, 46, 45, 44, 47, 46,
						 48, 50, 49, 48, 51, 50,
						 52, 54, 53, 52, 55, 54,
						 56, 58, 57, 56, 59, 58,
						 60, 62, 61, 60, 63, 62,
						 64, 66, 65, 64, 67, 66,
						 68, 70, 69, 68, 71, 70,
						 72, 74, 73, 72, 75, 74,
						 76, 78, 77, 76, 79, 78,
						 80, 82, 81, 80, 83, 82,
						 84, 86, 85, 84, 87, 86,
						 88, 90, 89, 88, 91, 90,
						 92, 94, 93, 92, 95, 94,
						 96, 98, 97, 96, 99, 98,
						 100, 102, 101, 100, 103, 102,
						 104, 106, 105, 104, 107, 106,
						 108, 110, 109, 108, 111, 110,
						 112, 114, 113, 112, 115, 114,
						 116, 118, 117, 116, 119, 118
	};

	glGenVertexArrays(1, &VAO10);
	glGenBuffers(1, &VBO10);
	glGenBuffers(1, &EBO10);

	glBindVertexArray(VAO10);

	glBindBuffer(GL_ARRAY_BUFFER, VBO10);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO10);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawTable()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO10); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 400, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildBerita()
{
	// Load and create a texture 
	glGenTextures(1, &texture11);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("berita.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		//tayangan tv
		-2.30,  2.70,  -4.84,  0, 0,  // 16
		 2.30,  2.70,  -4.84,  1, 0,  // 17
		 2.30,  4.80,  -4.84,  1, 1,  // 18
		-2.30,  4.80,  -4.84,  0, 1,  // 19

	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2,
						 4,  6,  5,  4,  7,  6,
						 8,  10, 9,  8,  11, 10,
						 12, 14, 13, 12, 15, 14,
						 16, 18, 17, 16, 19, 18,
	};

	glGenVertexArrays(1, &VAO11);
	glGenBuffers(1, &VBO11);
	glGenBuffers(1, &EBO11);

	glBindVertexArray(VAO11);

	glBindBuffer(GL_ARRAY_BUFFER, VBO11);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO11);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}

void Demo::DrawBerita()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture11);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);

	glBindVertexArray(VAO11); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	glDrawElements(GL_TRIANGLES, 148, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::InitCamera()
{
	posCamX = 0.0f;
	posCamY = 1.0f;
	posCamZ = 8.0f;
	viewCamX = 0.0f;
	viewCamY = 1.0f;
	viewCamZ = 0.0f;
	upCamX = 0.0f;
	upCamY = 1.0f;
	upCamZ = 0.0f;
	CAMERA_SPEED = 0.0001f;
	fovy = 45.0f;
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Demo::MoveCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	// forward positive cameraspeed and backward negative -cameraspeed.
	posCamX = posCamX + x * speed;
	posCamZ = posCamZ + z * speed;
	viewCamX = viewCamX + x * speed;
	viewCamZ = viewCamZ + z * speed;
}

void Demo::StrafeCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	float orthoX = -z;
	float orthoZ = x;

	// left positive cameraspeed and right negative -cameraspeed.
	posCamX = posCamX + orthoX * speed;
	posCamZ = posCamZ + orthoZ * speed;
	viewCamX = viewCamX + orthoX * speed;
	viewCamZ = viewCamZ + orthoZ * speed;
}

void Demo::RotateCamera(float speed)
{
	float x = viewCamX - posCamX;
	float z = viewCamZ - posCamZ;
	viewCamZ = (float)(posCamZ + glm::sin(speed) * x + glm::cos(speed) * z);
	viewCamX = (float)(posCamX + glm::cos(speed) * x - glm::sin(speed) * z);
}


int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start(/*"Camera: Free Camera Implementation"*/"Hotel Lobby", 800, 600, false, false);
}