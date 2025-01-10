#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"

void processKeyboardInput ();

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window window("Game Engine", 800, 800);
Camera camera;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, -200.0f);

glm::vec3 getArmPosition() {
	return camera.getCameraPosition() - glm::vec3(-6.0f, 3.0f, 5.0f);  //adjust the position of the arm
}

int main()
{
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

	//building and compiling shader program
	Shader shader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader skyboxShader("Shaders/skybox_vertex_shader.glsl", "Shaders/skybox_fragment_shader.glsl");

	std::vector<std::string> skyboxFaces = {
	"Resources/Textures/skybox/right.png",
	"Resources/Textures/skybox/left.png",
	"Resources/Textures/skybox/top.png",
	"Resources/Textures/skybox/bot.png",
	"Resources/Textures/skybox/front.png",
	"Resources/Textures/skybox/back.png"
	};


	//Textures
	GLuint tex = loadBMP("Resources/Textures/wood.bmp");
	GLuint tex2 = loadBMP("Resources/Textures/rock.bmp");
	GLuint tex3 = loadBMP("Resources/Textures/orange.bmp");
	GLuint tex4 = loadBMP("Resources/Textures/terrain.bmp");

	unsigned int  cubemapTexture = loadCubemap(skyboxFaces); //skybox

	glEnable(GL_DEPTH_TEST);

	float skyboxVertices[] = {
		// positions          
		-300.0f,  300.0f, -300.0f,
		-300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,
		 300.0f,  300.0f, -300.0f,
		-300.0f,  300.0f, -300.0f,

		-300.0f, -300.0f,  300.0f,
		-300.0f, -300.0f, -300.0f,
		-300.0f,  300.0f, -300.0f,
		-300.0f,  300.0f, -300.0f,
		-300.0f,  300.0f,  300.0f,
		-300.0f, -300.0f,  300.0f,

		 300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,

		-300.0f, -300.0f,  300.0f,
		-300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f, -300.0f,  300.0f,
		-300.0f, -300.0f,  300.0f,

		-300.0f,  300.0f, -300.0f,
		 300.0f,  300.0f, -300.0f,
		 300.0f,  300.0f,  300.0f,
		 300.0f,  300.0f,  300.0f,
		-300.0f,  300.0f,  300.0f,
		-300.0f,  300.0f, -300.0f,

		-300.0f, -300.0f, -300.0f,
		-300.0f, -300.0f,  300.0f,
		 300.0f, -300.0f, -300.0f,
		 300.0f, -300.0f, -300.0f,
		-300.0f, -300.0f,  300.0f,
		 300.0f, -300.0f,  300.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(1.0f, 1.0f);

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(1.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 1.0f);

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3,   
		1, 2, 3 };

	std::vector<Texture> textures;
	textures.push_back(Texture());
	textures[0].id = tex;
	textures[0].type = "texture_diffuse";

	std::vector<Texture> textures2;
	textures2.push_back(Texture());
	textures2[0].id = tex2;
	textures2[0].type = "texture_diffuse";

	std::vector<Texture> textures3;
	textures3.push_back(Texture());
	textures3[0].id = tex3;
	textures3[0].type = "texture_diffuse";

	std::vector<Texture> textures4;
	textures4.push_back(Texture());
	textures4[0].id = tex4;
	textures4[0].type = "texture_diffuse";

	//Mesh mesh(vert, ind, textures3);
	Mesh mesh(vert, ind, textures4);

	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh box = loader.loadObj("Resources/Models/cube.obj", textures);
	//Mesh plane = loader.loadObj("Resources/Models/plane.obj", textures3);
	Mesh plane1 = loader.loadObj("Resources/Models/plane1.obj", textures4);
	Mesh arm = loader.loadObj("Resources/Models/arm.obj");

	GLuint MatrixID = glGetUniformLocation(shader.getId(), "MVP");
	GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");

	GLuint MatrixID2 = glGetUniformLocation(sunShader.getId(), "MVP");
	GLuint ModelMatrixID2 = glGetUniformLocation(sunShader.getId(), "model");

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput();

		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressing mouse button" << std::endl;
		}
		 //// Code for the light ////

		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());


		//Test for one Obj loading = light source

		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);

		//// End code for the light ////

		shader.use();

		///// Test Obj files for box ////

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		box.draw(shader);

		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		plane1.draw(shader);
		
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, getArmPosition()); //translation
		ModelMatrix = glm::rotate(ModelMatrix, 130.0f, glm::vec3(0.0f, 0.0f, 1.0f)); //rotation
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f)); //scaling

		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]); //mvp matrix to shader part 2
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]); //model matrix to shader part 2

		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		arm.draw(shader);

		///// Test plane Obj file //////

		/*ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		plane.draw(shader);*/

		///// Test Obj files for plane1 ////

		// Skybox rendering
		glDepthFunc(GL_LEQUAL);  // Disable depth test for skybox
		skyboxShader.use();
		glm::mat4 SkyboxView = glm::mat4(glm::mat3(ViewMatrix));  // Remove translation part of view matrix
		glm::mat4 SkyboxMVP = ProjectionMatrix * SkyboxView;
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "MVP"), 1, GL_FALSE, &SkyboxMVP[0][0]);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS); // Restore depth test

		window.update();
	}
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glfwTerminate();
	return 0;
}

void processKeyboardInput()
{
	float cameraSpeed = 30 * deltaTime;
	float rotateSpeed = 45.0f * deltaTime; // Example rotation speed


	// Movement (relative to humanoid's forward direction)
	if (window.isPressed(GLFW_KEY_UP))  // Move forward
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))  // Move backward
		camera.keyboardMoveBack(cameraSpeed);

	// Rotation (around Y-axis)
	if (window.isPressed(GLFW_KEY_LEFT)) { // Rotate left
		float angle = 45.0f * deltaTime;
		camera.rotateOy(angle);
	}
	if (window.isPressed(GLFW_KEY_RIGHT)) { // Rotate right
		float angle = -45.0f * deltaTime;
		camera.rotateOy(angle);
	}
}
