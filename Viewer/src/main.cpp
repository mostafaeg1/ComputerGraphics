#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
	
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "MathUtils.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


double zoomFactor = 1;
int windowWidth = 1280;
int windowHeight = 720;
char* windowTitle = "OpenGL";
glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
bool zoomChanged = false;
std::shared_ptr<Scene> scene;
ImGuiIO* imgui;
GLFWwindow* window;

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup();

static void GlfwErrorCallback(int error, const char* description);
void RenderFrame(GLFWwindow* window, std::shared_ptr<Scene> scene, Renderer& renderer, ImGuiIO& io);

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

float GetAspectRatio();
void DrawImguiMenus();
void HandleImguiInput();


using std::string;

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);

/**
 * Fields
 */
//std::shared_ptr<Scene> scene;
static bool show_app_main_menu_bar = false;
bool show_demo_window = false;
bool show_another_window = false;

static glm::vec3 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

static glm::vec3 light_specular_color = glm::vec4(1.f, 1.f, 1.f, 1.00f);
static glm::vec3 light_diffuse_color = glm::vec4(1.f, 1.f, 1.f, 1.00f);
static glm::vec3 light_ambient_color = glm::vec4(1.f, 1.f, 1.f, 1.00f);


static glm::vec3 model_specular_color = glm::vec4(0.f, 0.f, 0.f, 1.0f);
static glm::vec3 model_diffuse_color = glm::vec4(0.21f,0.58f, 0.21f, 1.00f);
static glm::vec3 model_ambient_color = glm::vec4(0.32f, 0.41f, 0.3f, 1.00f);


static float viewport_width = 1280;
static float viewport_height = 720;

static float local_x_translate = 0;
static float local_y_translate = 0;
static float local_z_translate = 0;

static float world_x_translate = 0;
static float world_y_translate = 0;
static float world_z_translate = -20;


static float local_x_scale = 1;
static float local_y_scale = 1;
static float local_z_scale = 1;

static float world_x_scale = 1;
static float world_y_scale = 1;
static float world_z_scale = 1;
static float world_xyz_scale = 1;
static float local_xyz_scale = 1;


static float local_x_angle = 0;
static float local_y_angle = 0;
static float local_z_angle = 0;

static float world_x_angle = 0;
static float world_y_angle = 0;
static float world_z_angle = 0;

static float  Right = 0.0f;
static float  Left = 0.0f;
static float  Up = 0.0f;
static float  Down = 0.0f;

static float shininess = 1;
static glm::vec3 light_position = glm::vec3(0, 0,200);
static float x_light = 0;
static float y_light = 0;
static float z_light = 200;

static float  Aspect = GetAspectRatio();
static float  Fovy = 1.0f;
static float  Far1 = 1000.0f;
static float  Near = 1.f;

static int combo_item_current = 0;
static int light_current_index = 0;
static float world_local_current = 0;

static bool apply_toon_shading = 0;
static bool apply_texture_mapping = 0;
static int ortho = 0;


static char angle_type=' ';
static float angle_val = 0;



/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, const std::shared_ptr<Scene>& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus();
//void DrawImguiMenus(ImGuiIO& io, const std::shared_ptr<Scene>& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	if (!Setup(windowWidth, windowHeight, windowTitle))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}

	scene = std::make_shared<Scene>();	glm::vec3 eye = glm::vec3(0, 0, 10);
	glm::vec3 at = glm::vec3(0, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	scene->AddCamera(Camera(eye, at, up, GetAspectRatio()));

	scene->AddLight(glm::vec3(640.0f, 360.0f, 1000.0f));


	Renderer renderer;
	renderer.LoadShaders();
	renderer.LoadTexture();

	while (!glfwWindowShouldClose(window))
	{
		// Poll and process events
		glfwPollEvents();

		// Imgui stuff
		//start frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//-------------------
		DrawImguiMenus();
		ImGui::Render();
	    HandleImguiInput();

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render scene
		renderer.Render(scene);

		// Imgui stuff
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap front and back buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;

}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}

	imgui = &SetupImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
}

ImGuiIO& SetupImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Setup style
	ImGui::StyleColorsDark();

	glfwSetScrollCallback(window, glfw_OnMouseScroll);

	return io;
}

void glfw_OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	zoomFactor = glm::pow(1.1, -yoffset);
	zoomChanged = true;
}


GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}


void DrawImguiMenus()
{
	/**
	 * MeshViewer menu
	 */
	float f = 5;
	auto current_camera = scene->GetCamera(0);
	//ImGui::ShowDemoWindow(&show_demo_window);
	show_app_main_menu_bar = true;
	static int models_num = scene->GetModelCount();


	//Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{

				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene->AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Textures"))
		{
			string path = "../Data/Textures/";
			static std::list<string> list = { "Fire","Checker","crate","Stone","Leather" ,"Bricks","Grass","Sea"};

			for (const string& texture : list)
			{
				if (ImGui::MenuItem(texture.c_str()))
				{
					scene->updateTexturePath(path + texture + ".jpg");
				}
			}
			ImGui::EndMenu();
		}

		//	// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
		//}

		// Controls
		//// TODO: Add more controls as needed

		//ImGui::End();

		/**
		 * Imgui demo - you can remove it once you are familiar with imgui
		 */

		 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		//if (show_demo_window)
		//	ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Scene Controller");                          // Create a window called "Hello, world!" and append into it.
			int models_number = scene->GetModelCount();
			int lights_number = scene->GetLightCount();
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("Camera"))
				{
					ImGui::Text("View Type: "); ImGui::SameLine();
					ImGui::RadioButton("Orthographic", &ortho, 1); ImGui::SameLine();
					ImGui::RadioButton("Perspective", &ortho, 0);
					static float step = 5.0f;
					static bool inputs_step = true;

 
					if (!ortho) {
						
						ImGui::InputFloat("fovy", &Fovy, 0.1);
						if (Fovy <0 ) {
							Fovy += 0.001;
						}
						if (Fovy > glm::pi<float>()) {
							Fovy = glm::pi<float>();
						}
						ImGui::InputFloat("aspect", &Aspect, 0.02);
						ImGui::InputFloat("near", &Near, 0.05);
						ImGui::InputFloat("far", &Far1, step);
						scene->GetCamera(0).SetPerspectiveProjection(Fovy,Aspect, Near,Far1);
						scene->GetCamera(0).SwitchToPrespective();
					}
					else {
						ImGui::InputFloat("up", &Up, step);
						ImGui::InputFloat("down", &Down, step);
						ImGui::InputFloat("right", &Right, step);
						ImGui::InputFloat("left", &Left, step);
						scene->GetCamera(0).SwitchToOrthographic();
					}
					
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Model"))
				{
					const char** items;
					items = new const char* [models_number];
					for (int i = 0; i < models_number; i++) {
						items[i] = scene->GetModel(i).GetModelName().c_str();
					}

					if (models_number) {
						ImGui::Combo("Currently loaded mesh models", &combo_item_current, items, models_number);
					}

					if (models_number > 0) {
						if (ImGui::TreeNode("Material")) {
							ImGui::ColorEdit3("Ambient Color", (float*)&model_ambient_color); // Edit 3 floats representing a color
							ImGui::ColorEdit3("Diffuse Color", (float*)&model_diffuse_color); // Edit 3 floats representing a color
							ImGui::ColorEdit3("Specular  Color", (float*)&model_specular_color); // Edit 3 floats representing a color
							ImGui::SliderFloat("Shininess", &shininess, 0, 2);
							scene->GetModel(combo_item_current).updateShininess(shininess);

						}
						if (ImGui::TreeNode("Local Transformation")) {
							if (ImGui::TreeNode("Translation")) {
								float xret = ImGui::InputFloat("TranslateX", &local_x_translate, 0.1f, 1.0f, "%.3f");
								float yret = ImGui::InputFloat("TranslateY", &local_y_translate, 0.1f, 1.0f, "%.3f");
								float zret = ImGui::InputFloat("TranslateZ", &local_z_translate, 0.1f, 1.0f, "%.3f");
								glm::mat4 newtranslate = MathUtils::makeTranslateMat(local_x_translate, local_y_translate, local_z_translate);
								auto tt = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
								scene->GetModel(combo_item_current).setLocalTranslate(tt);
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Scaling")) {
								float xret = ImGui::SliderFloat("X ", &local_x_scale, 0.1f, 2.0f);
								float yret = ImGui::SliderFloat("Y ", &local_y_scale, 0.1f, 2.0f);
								float zret = ImGui::SliderFloat("Z ", &local_z_scale, 0.1f, 2.0f);
								float xyzret = ImGui::SliderFloat("X,Y,Z", &local_xyz_scale, 0.1f, 4.0f);
								if (xret == 1 || yret == 1 || zret == 1) {
									glm::mat4 newscale = MathUtils::makescaleMat(local_x_scale, local_y_scale, local_z_scale);
									scene->GetModel(combo_item_current).setLocalScale(newscale);
								}
								if (xyzret) {
									glm::mat4 newscale = MathUtils::makescaleMat(local_xyz_scale, local_xyz_scale, local_xyz_scale);
									scene->GetModel(combo_item_current).setLocalScale(newscale);
								}
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Rotation")) {
								float xret = ImGui::SliderFloat("X  ", &local_x_angle, -360, 360);
								float yret = ImGui::SliderFloat("Y  ", &local_y_angle, -360, 360);
								float zret = ImGui::SliderFloat("Z  ", &local_z_angle, -360, 360);
								glm::mat4 x_newrotataion;
								glm::mat4 y_newrotataion;
								glm::mat4 z_newrotataion;
								if (xret == 1) {
									x_newrotataion = MathUtils::makeRotationMat('x', local_x_angle);
									scene->GetModel(combo_item_current).setLocal_x_Rotation(x_newrotataion);

								}
								if (yret == 1) {
									y_newrotataion = MathUtils::makeRotationMat('y', local_y_angle);
									scene->GetModel(combo_item_current).setLocal_y_Rotation(y_newrotataion);
								}
								if (zret == 1) {
									z_newrotataion = MathUtils::makeRotationMat('z', local_z_angle);
									scene->GetModel(combo_item_current).setLocal_z_Rotation(z_newrotataion);
								}
								ImGui::TreePop();
							}
							ImGui::TreePop();
						}
						if (ImGui::TreeNode("World Transformation")) {
							if (ImGui::TreeNode("Translation")) {
								float xret = ImGui::InputFloat("TranslateX", &world_x_translate, 0.1f, 1.0f, "%.3f");
								float yret = ImGui::InputFloat("TranslateY", &world_y_translate, 0.1f, 1.0f, "%.3f");
								float zret = ImGui::InputFloat("TranslateZ", &world_z_translate, 0.1f, 1.0f, "%.3f");
								if (xret == 1 || yret == 1 || zret == 1) {
									glm::mat4 newtranslate = MathUtils::makeTranslateMat(world_x_translate, world_y_translate, world_z_translate);
									scene->GetModel(combo_item_current).setWorldTranslate(newtranslate);
								}
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Scaling")) {
								float xret = ImGui::SliderFloat("X ", &world_x_scale, 0.1f, 2.0f);
								float yret = ImGui::SliderFloat("Y ", &world_y_scale, 0.1f, 2.0f);
								float zret = ImGui::SliderFloat("Z ", &world_z_scale, 0.1f, 2.0f);
								float xyzret = ImGui::SliderFloat("X,Y,Z", &world_xyz_scale, 0.1f, 4.0f);

								if (xret == 1 || yret == 1 || zret == 1) {
									glm::mat4 newscale = MathUtils::makescaleMat(world_x_scale, world_y_scale, world_z_scale);
									scene->GetModel(combo_item_current).setWorldScale(newscale);
								}
								if (xyzret) {
									glm::mat4 newscale = MathUtils::makescaleMat(world_xyz_scale, world_xyz_scale, world_xyz_scale);
									scene->GetModel(combo_item_current).setWorldScale(newscale);
								}
								ImGui::TreePop();
							}
							if (ImGui::TreeNode("Rotation")) {
								float xret = ImGui::SliderFloat("X  ", &world_x_angle, -360, 360);
								float yret = ImGui::SliderFloat("Y  ", &world_y_angle, -360, 360);
								float zret = ImGui::SliderFloat("Z  ", &world_z_angle, -360, 360);
								glm::mat4 x_newrotataion;
								glm::mat4 y_newrotataion;
								glm::mat4 z_newrotataion;
								if (xret == 1) {
									x_newrotataion = MathUtils::makeRotationMat('x', world_x_angle);
									scene->GetModel(combo_item_current).setWorld_x_Rotation(x_newrotataion);

								}
								if (yret == 1) {
									y_newrotataion = MathUtils::makeRotationMat('y', world_y_angle);
									scene->GetModel(combo_item_current).setWorld_y_Rotation(y_newrotataion);
								}
								if (zret == 1) {
									z_newrotataion = MathUtils::makeRotationMat('z', world_z_angle);
									scene->GetModel(combo_item_current).setWorld_z_Rotation(z_newrotataion);
								}
								ImGui::TreePop();
							}
							ImGui::TreePop();

						}
						ImGui::Checkbox("Toon Shading", &apply_toon_shading);
					    if(apply_toon_shading) {
							scene->GetModel(combo_item_current).changeApplyToonShadingStatus(true);
							scene->GetModel(combo_item_current).changeApplyTextureMappingStatus(false);
							apply_texture_mapping = 0;
						}
						else {
							scene->GetModel(combo_item_current).changeApplyToonShadingStatus(false);
						}


						ImGui::Checkbox("Texture Mapping(sphere)", &apply_texture_mapping);
						if (apply_texture_mapping) {
							scene->GetModel(combo_item_current).changeApplyTextureMappingStatus(true);
							scene->GetModel(combo_item_current).changeApplyToonShadingStatus(false);
							apply_toon_shading = 0;
						}
						else {
							scene->GetModel(combo_item_current).changeApplyTextureMappingStatus(false);
						}

                         

						
						scene->GetModel(combo_item_current).updateAmbient(model_ambient_color);
						scene->GetModel(combo_item_current).updateDiffuse(model_diffuse_color);
						scene->GetModel(combo_item_current).updateSpecular(model_specular_color);
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Light")) {
					if (ImGui::Button("Add Light")) {
						/// to do
						scene->AddLight();
					}
					ImGui::Text("Select Light: ");

					std::vector<std::string> lights;

					for (int i = 0; i < lights_number; i++) {
						lights.push_back(scene->GetLight(i).GetName());
					}

					if (lights_number) {
						const char** lights_cstr = new const char* [lights_number];
						for (int i = 0; i < lights_number; i++) {
							lights_cstr[i] = lights[i].c_str();
						}
						ImGui::Combo("Current Lights", &light_current_index, lights_cstr, lights_number);
						delete[] lights_cstr;

					}



					if (lights_number) {
						scene->SetActiveLightIndex(light_current_index);
						auto current_light = scene->GetActiveLight();

						x_light = current_light.GetPosition().x;
						y_light = current_light.GetPosition().y;
						z_light = current_light.GetPosition().z;

						light_ambient_color = current_light.ambient;
						light_diffuse_color = current_light.diffuse;
						light_specular_color = current_light.specular;

						if (ImGui::TreeNode("Reflection")) {
							ImGui::ColorEdit3("Ambient", (float*)&light_ambient_color); // Edit 3 floats representing a color
							ImGui::ColorEdit3("Diffuse", (float*)&light_diffuse_color); // Edit 3 floats representing a color
							ImGui::ColorEdit3("Specular", (float*)&light_specular_color); // Edit 3 floats representing a color
						}

						current_light.ambient = light_ambient_color;
						current_light.diffuse = light_diffuse_color;
						current_light.specular = light_specular_color;

						auto step = 5;


						if (ImGui::TreeNode("Position")) {
							ImGui::InputFloat("X", &x_light, step);
							ImGui::InputFloat("Y", &y_light, step);
							ImGui::InputFloat("Z", &z_light, step);
						}
						light_position.x = x_light;
						light_position.y = y_light;
						light_position.z = z_light;
						scene->GetActiveLight().updatePosition(light_position);

						scene->GetActiveLight().ambient = light_ambient_color;
						scene->GetActiveLight().diffuse = light_diffuse_color;
						scene->GetActiveLight().specular = light_specular_color;
					}
					

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

				ImGui::End();
		}

		}
	}



float GetAspectRatio()
{
	return viewport_width / viewport_height;
}

void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	viewport_width = width;
	viewport_height = height;
	glViewport(0, 0, viewport_width, viewport_height);
}



void HandleImguiInput()
{
	if (!imgui->WantCaptureKeyboard && scene->GetModelCount())
	{
		if (imgui->KeysDown[49]) // 1
		{
			local_x_angle += 0.25;
			if (local_x_angle > 360) {
				local_x_angle = 0;
			}
			auto x_newrotataion = MathUtils::makeRotationMat('x', local_x_angle);
			scene->GetModel(combo_item_current).setLocal_x_Rotation(x_newrotataion);
		}

		if (imgui->KeysDown[50]) // 2
		{
			local_x_angle -= 0.25;
			if (local_x_angle <-360) {
				local_x_angle = 0;
			}
			auto x_newrotataion = MathUtils::makeRotationMat('x', local_x_angle);
			scene->GetModel(combo_item_current).setLocal_x_Rotation(x_newrotataion);
		}

		if (imgui->KeysDown[51]) // 3
		{
			local_y_angle += 0.25;
			if (local_y_angle > 360) {
				local_y_angle = 0;
			}
			auto y_newrotataion = MathUtils::makeRotationMat('y', local_y_angle);
			scene->GetModel(combo_item_current).setLocal_y_Rotation(y_newrotataion);
		}
		if (imgui->KeysDown[52]) // 4
		{
			local_y_angle -= 0.25;
			if (local_y_angle < -360) {
				local_y_angle = 0;
			}
			auto y_newrotataion = MathUtils::makeRotationMat('y', local_y_angle);
			scene->GetModel(combo_item_current).setLocal_y_Rotation(y_newrotataion);
		}

		if (imgui->KeysDown[53]) // 5
		{
			local_z_angle += 0.25;
			if (local_z_angle >360) {
				local_z_angle = 0;
			}			
			auto z_newrotataion = MathUtils::makeRotationMat('z', local_z_angle);
			scene->GetModel(combo_item_current).setLocal_z_Rotation(z_newrotataion);
		}

		if (imgui->KeysDown[54]) // 6
		{
			local_z_angle -= 0.25;
			if (local_z_angle < -360) {
				local_z_angle = 0;
			}
			auto z_newrotataion = MathUtils::makeRotationMat('z', local_z_angle);
			scene->GetModel(combo_item_current).setLocal_z_Rotation(z_newrotataion);
		}

		if (imgui->KeysDown[45]) // -
		{
			local_xyz_scale *= (1 / 1.005);
			glm::mat4 newscale = MathUtils::makescaleMat(local_xyz_scale, local_xyz_scale, local_xyz_scale);
			scene->GetModel(combo_item_current).setLocalScale(newscale);
		}

		if (imgui->KeysDown[61]) // +
		{
			local_xyz_scale *= (1.005 / 1);
			glm::mat4 newscale = MathUtils::makescaleMat(local_xyz_scale, local_xyz_scale, local_xyz_scale);
			scene->GetModel(combo_item_current).setLocalScale(newscale);
		}

		if (imgui->KeysDown[65]) // a
		{
			local_x_translate -= 0.01;
			auto newtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
			scene->GetModel(combo_item_current).setLocalTranslate(newtranslate);
		}

		if (imgui->KeysDown[68]) // d
		{
			local_x_translate += 0.01;
			auto newtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
			scene->GetModel(combo_item_current).setLocalTranslate(newtranslate);
		}

		if (imgui->KeysDown[69]) // e
		{
			local_z_translate += 0.01;
			auto newtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
			scene->GetModel(combo_item_current).setLocalTranslate(newtranslate);
		}

		if (imgui->KeysDown[83]) // s
		{
			local_y_translate -= 0.01;
			auto newtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
			scene->GetModel(combo_item_current).setLocalTranslate(newtranslate);
		}

		if (imgui->KeysDown[87]) // w
		{
			local_y_translate += 0.01;
			auto newtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
			scene->GetModel(combo_item_current).setLocalTranslate(newtranslate);
		}

		if (imgui->KeysDown[81]) // q
		{
			local_z_translate -= 0.01;
			auto newtranslate = glm::translate(glm::mat4(1.0f), glm::vec3(local_x_translate, local_y_translate, local_z_translate));
			scene->GetModel(combo_item_current).setLocalTranslate(newtranslate);
		}
	}

	if (!imgui->WantCaptureMouse)
	{
		if (zoomChanged)
		{
			scene->GetActiveCamera().Zoom(zoomFactor);
			Fovy = Fovy * zoomFactor;
			zoomChanged = false;
		}
	}
}
