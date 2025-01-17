#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void processKeyboardInput();

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window window("Escape from Planet X", 800, 800);
Camera camera;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, -200.0f);

glm::vec3 getArmPosition() {
	return camera.getCameraPosition() - glm::vec3(-6.0f, 3.0f, 5.0f);
}

int main()
{
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);

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
	GLuint tex1 = loadBMP("Resources/Textures/terrain.bmp"); //for the plane
	GLuint tex2 = loadBMP("Resources/Textures/mushroom.bmp"); //for the mushroom
	GLuint tex3 = loadBMP("Resources/Textures/colors.bmp"); //for the rocket
	GLuint tex4 = loadBMP("Resources/Textures/rusty_metal.bmp"); //for the arm
	GLuint tex5 = loadBMP("Resources/Textures/alien.bmp"); //for the alien

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

	std::vector<Texture> textures1;
	textures1.push_back(Texture());
	textures1[0].id = tex1;
	textures1[0].type = "texture_diffuse";

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

	std::vector<Texture> textures5;
	textures5.push_back(Texture());
	textures5[0].id = tex5;
	textures5[0].type = "texture_diffuse";

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); // Access ImGui input/output options
	ImGui::StyleColorsDark();    // Set the style to Dark

	// Setup platform/renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	Mesh mesh(vert, ind, textures1);

	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh box = loader.loadObj("Resources/Models/cube.obj", textures);
	Mesh plane1 = loader.loadObj("Resources/Models/plane1.obj", textures1);
	Mesh mushroom1 = loader.loadObj("Resources/Models/mushroom.obj", textures2);
	Mesh mushroom2 = loader.loadObj("Resources/Models/mushroom.obj", textures2);
	Mesh mushroom3 = loader.loadObj("Resources/Models/mushroom.obj", textures2);
	Mesh mushroom4 = loader.loadObj("Resources/Models/mushroom.obj", textures2);
	Mesh mushroom5 = loader.loadObj("Resources/Models/mushroom.obj", textures2);
	Mesh rocket = loader.loadObj("Resources/Models/roket.obj", textures3);
	Mesh rocket1 = loader.loadObj("Resources/Models/roket.obj", textures3);
	Mesh arm = loader.loadObj("Resources/Models/arm.obj", textures4);
	Mesh alien = loader.loadObj("Resources/Models/alien.obj", textures5);
	
	

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();

		//imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput();

		

		//sun

		sunShader.use();

		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());
		GLuint MatrixID = glGetUniformLocation(sunShader.getId(), "MVP");
		GLuint MatrixID2 = glGetUniformLocation(shader.getId(), "MVP");
		GLuint ModelMatrixID = glGetUniformLocation(shader.getId(), "model");


		glm::mat4 ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		sun.draw(sunShader);


		


		//shader for the rest of objects

		shader.use();
		
		
		glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//plane1
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -20.0f, 0.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		plane1.draw(shader);

		//arm
		/*ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, getArmPosition());
		ModelMatrix = glm::rotate(ModelMatrix, 130.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		arm.draw(shader);*/

		//mushroom1
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(100.0f, -25.0f, 40.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mushroom1.draw(shader);

		//mushroom2
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(200.0f, -20.0f, 50.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mushroom2.draw(shader);

		//mushroom3
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(30.0f, -25.0f, 170.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mushroom3.draw(shader);

		//mushroom4
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(130.0f, -15.0f, 175.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mushroom4.draw(shader);

		//mushroom5
		ModelMatrix = glm::mat4(1.0);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(200.0f, -17.0f, 150.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
		MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		mushroom5.draw(shader);

		static int taskNumber = 0;
		//static std::string currentTexture = "textures"; // Texture state
		static bool isFinished = false;
		ImGui::Begin("ESC From Planet X");
		if (!isFinished)
			ImGui::TextWrapped("You are a rogue cyborg, stranded on a desolate planet, pursued by the relentless space police determined to bring you in for leading a daring revolt. Branded as a dangerous fugitive, your only hope of survival lies in escaping this forsaken world.Scavenge the planet for scattered objects to repair your damaged ship and make your getaway.Time is running out ... Outsmart your hunters and fight for your freedom!");

		if (taskNumber == 1) {
			ImGui::Text("Task 1");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Find a nail box. Collect it to get to the next task.");
		}
		else if (taskNumber == 2) {
			ImGui::Text("Task 2");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Find the food supply box. Collect it to get to the next task.");
		}
		else if (taskNumber == 3) {
			ImGui::Text("Task 3");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Find the first aid box. Collect it to get to the next task.");
		}
		else if (taskNumber == 4) {
			ImGui::Text("Task 4");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Find the fuel crystals box. Collect it to get to the next task.");
		}
		else if (taskNumber == 5) {
			ImGui::Text("Task 5");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "This is task 5. Get to the spaceship. ");
		}
		else ImGui::Text("");


		if (taskNumber == 0)
			if (ImGui::Button("Start First Task")) {
				taskNumber = 1;

			}

		if (isFinished) {
			ImGui::Text("You have escaped from planet X");
			if (ImGui::Button("ESC")) {
				break;
			}
		}

		ImGui::End();


		if (taskNumber == 1)
		{
			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(45.0f, 5.0f, 75.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			box.draw(shader);
		}
		else if (taskNumber == 2)
		{
			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-50.0f, 5.0f, 170.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			box.draw(shader);
		}
		else if (taskNumber == 3)
		{
			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(160.0f, 5.0f, 160.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			box.draw(shader);
		}
		else if (taskNumber == 4)
		{
			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(90.0f, 5.0f, -200.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			box.draw(shader);
		}
		else if (taskNumber == 5)
		{
			//no box go to rocket
		}

		glm::vec3 poz = camera.getCameraPosition();
		printf("x: %f, y: %f, z: %f\n", (double)poz.x, (double)poz.y, (double)poz.z);

		if (taskNumber == 1) {
			glm::vec3 task = camera.getCameraPosition();
			if (task.x >= 30 && task.x <= 60 && task.z >= 60 && task.z <= 90) {

				taskNumber = 2;
			}

		}
		else if (taskNumber == 2) {
			glm::vec3 task = camera.getCameraPosition();
			if (task.x >= -65 && task.x <= -35 && task.z >= 155 && task.z <= 185) {

				taskNumber = 3;
			}

		}
		else if (taskNumber == 3) {
			glm::vec3 task = camera.getCameraPosition();
			if (task.x >= 145 && task.x <= 175 && task.z >= 145 && task.z <= 175) {

				taskNumber = 4;
			}

		}
		else if (taskNumber == 4) {
			glm::vec3 task = camera.getCameraPosition();
			if (task.x >= 75 && task.x <= 105 && task.z >= -215 && task.z <= -185) {

				taskNumber = 5;
			}

		}
		else if (taskNumber == 5) {
			glm::vec3 task = camera.getCameraPosition();
			if (task.x >= -15 && task.x <= 15 && task.z >= -15 && task.z <= 15) {

				taskNumber = -1;
				isFinished = true;
			}

		}


		
			float floatSpeed = 1.5f;
			float floatHeight = 7.0f;
			float floatOffset = sin(glfwGetTime() * floatSpeed) * floatHeight;

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f + floatOffset, 0.0f));
			float rotationAngle = glfwGetTime() * glm::radians(100.0f);
			ModelMatrix = glm::rotate(ModelMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(65.0f, 65.0f, 65.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			rocket1.draw(shader);

			float floatSpeedAlien = 5.0f;
			float floatHeightAlien = 3.0f;
			float floatOffsetAlien = sin(glfwGetTime() * floatSpeedAlien) * floatHeightAlien;

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(60.0f, 0.0f + floatOffsetAlien, 100.0f));
			float rotationAngleAlien = glfwGetTime() * glm::radians(100.0f);
			ModelMatrix = glm::rotate(ModelMatrix, rotationAngleAlien, glm::vec3(0.0f, 1.0f, 0.0f));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(65.0f, 65.0f, 65.0f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			alien.draw(shader);


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

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glfwTerminate();
	return 0;
}

void processKeyboardInput()
{
	float cameraSpeed = 30 * deltaTime;
	float rotateSpeed = 45.0f * deltaTime;

	if (window.isPressed(GLFW_KEY_UP))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.keyboardMoveBack(cameraSpeed);

	// Rotation (around Y-axis)
	if (window.isPressed(GLFW_KEY_LEFT)) {
		float angle = 45.0f * deltaTime;
		camera.rotateOy(angle);
	}
	if (window.isPressed(GLFW_KEY_RIGHT)) {
		float angle = -45.0f * deltaTime;
		camera.rotateOy(angle);
	}
}
