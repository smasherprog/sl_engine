#include "stdafx.h"
#include "Window_Base.h"
#include "..\SL_Utilities\Debug_Output.h"
#include "Texture_Factory.h"
#include "..\SL_Terrain_Graphics\Terrain_Graphics.h"
#include "Camera.h"
#include "Camera_Ext.h"
#include "..\SL_Utilities\Imaging.h"
#include "..\SL_Font_Utilities\Font_Factory.h"

std::vector<std::function<bool(int, int, int, int)>> SL_Graphics::_INTERNAL::_Key_CallBacks;
std::vector<std::function<bool(int, int, int)>> SL_Graphics::_INTERNAL::_Mouse_Button_CallBacks;
std::vector<std::function<bool(double, double)>> SL_Graphics::_INTERNAL::_Mouse_Movement_CallBacks;
std::vector<std::function<void(double, SL_Graphics::Camera*)>> SL_Graphics::_INTERNAL::_Update_CallBacks;
std::vector<std::function<void(double, SL_Graphics::Camera*)>> SL_Graphics::_INTERNAL::_Draw_CallBacks;
std::vector<std::function<void(int, int)>> SL_Graphics::_INTERNAL::_FrameBuffer_Change_CallBacks;
std::function<void(int, int, int, int)> SL_Graphics::_INTERNAL::_Camera_Key_CallBack;
std::function<void(double, double)> SL_Graphics::_INTERNAL::_Camera_Mouse_Movement_CallBack;

double SL_Graphics::_INTERNAL::_Last_Mouse_X = 0;
double SL_Graphics::_INTERNAL::_Last_Mouse_Y = 0;
double SL_Graphics::_INTERNAL::_Delta_Mouse_X = 0;
double SL_Graphics::_INTERNAL::_Delta_Mouse_Y = 0;
double SL_Graphics::_INTERNAL::_Last_Time = 0;
double SL_Graphics::_INTERNAL::_Time_Now = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	for (auto& x : SL_Graphics::_INTERNAL::_FrameBuffer_Change_CallBacks) x(width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto key_caught = false;
	for (auto& x : SL_Graphics::_INTERNAL::_Key_CallBacks){
		key_caught |= x(key, scancode, action, mods);
		if (key_caught) break;
	}
	if (!key_caught) {
		SL_Graphics::_INTERNAL::_Camera_Key_CallBack(key, scancode, action, mods);
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	for (auto& x : SL_Graphics::_INTERNAL::_Mouse_Button_CallBacks) if (x(button, action, mods)) break;
}
void mouse_movement_callback(GLFWwindow* window, double xpos, double ypos)
{
	SL_Graphics::_INTERNAL::_Delta_Mouse_X = xpos - SL_Graphics::_INTERNAL::_Last_Mouse_X;
	SL_Graphics::_INTERNAL::_Last_Mouse_X = xpos;
	SL_Graphics::_INTERNAL::_Delta_Mouse_Y = ypos - SL_Graphics::_INTERNAL::_Last_Mouse_Y;
	SL_Graphics::_INTERNAL::_Last_Mouse_Y = ypos;
	auto key_caught = false;
	for (auto& x : SL_Graphics::_INTERNAL::_Mouse_Movement_CallBacks) {
		key_caught |= x(xpos, ypos);
		if (key_caught) break;
	}
	if (!key_caught) SL_Graphics::_INTERNAL::_Camera_Mouse_Movement_CallBack(xpos, ypos);
}
SL_Graphics::Window_Base::Window_Base(const char* window_title, int top_pos, int left_pos, int window_width, int window_height){
	DEBUG_MSG("Begin Window_Base");
	
	DEBUG_MSG("Calling glfwInit");
	if (glfwInit()){
		DEBUG_MSG("glfwCreateWindow");
		_Window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
		if (!_Window)
		{
			DEBUG_MSG("glfwTerminate");
			glfwTerminate();
		}
		else {
			DEBUG_MSG("glfwMakeContextCurrent");
			glfwMakeContextCurrent(_Window);
			glfwSetFramebufferSizeCallback(_Window, framebuffer_size_callback);
			int width, height;
			glfwGetFramebufferSize(_Window, &width, &height);
			SL_Graphics::_INTERNAL::_Last_Mouse_X = (double)(width / 2);
			SL_Graphics::_INTERNAL::_Last_Mouse_Y = (double)(height / 2);
			glfwSetCursorPos(_Window, SL_Graphics::_INTERNAL::_Last_Mouse_X, SL_Graphics::_INTERNAL::_Last_Mouse_Y);

			framebuffer_size_callback(_Window, width, height);
			glfwSetKeyCallback(_Window, key_callback);
			glfwSetMouseButtonCallback(_Window, mouse_button_callback);
			glfwSetCursorPosCallback(_Window, mouse_movement_callback);
			DEBUG_MSG("Calling glewInit");

			GLenum err = glewInit();
			if (GLEW_OK != err) DEBUG_MSG("Error Calling glewInit() %", std::string((char*)glewGetErrorString(err)));
			
			Texture_Factory::Init();
			_Camera = std::make_unique<Camera>();
			_Camera->Set_Position(glm::vec3(0, 100, 250));
			_Camera->Set_Look(glm::vec3(0, 0, -100.0f));
			_Camera->Set_Far_Plane(1000.0f);
			_Camera->Set_Field_of_View(45.0f);
			_Camera->Set_Near_Plane(1.0f);

			Add_Update_CallBack(std::bind(&SL_Graphics::Camera::Update, _Camera.get(), std::placeholders::_1));
			Add_FrameBuffer_Change_CallBack(std::bind(&SL_Graphics::Camera::Set_View_Dimensions, _Camera.get(), std::placeholders::_1, std::placeholders::_2));
			Camera* c = _Camera.get();
			SL_Graphics::_INTERNAL::_Camera_Key_CallBack = [=](int, int, int, int){ SL_Graphics::_INTERNAL::key_callback(_Window, c); };
			SL_Graphics::_INTERNAL::_Camera_Mouse_Movement_CallBack = [=](double, double){
				if (glfwGetMouseButton(_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){// camera mouse look 
					c->Heading((float)-SL_Graphics::_INTERNAL::_Delta_Mouse_X);
					c->Pitch((float)-SL_Graphics::_INTERNAL::_Delta_Mouse_Y);
				}
			};
			SL_Font::Init("c:/windows/fonts/");
		}
	}
	DEBUG_MSG("End Window_Base");
}

SL_Graphics::Window_Base::~Window_Base(){
	Texture_Factory::DeInit();
	glfwTerminate();
}
bool SL_Graphics::Window_Base::Begin(){
	SL_Graphics::_INTERNAL::_Time_Now = glfwGetTime();
	if (glfwWindowShouldClose(_Window)) return false;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}
void SL_Graphics::Window_Base::End(){
	glfwSwapBuffers(_Window);
	glfwPollEvents();
	SL_Graphics::_INTERNAL::_Last_Time = SL_Graphics::_INTERNAL::_Time_Now;//swap
}
void SL_Graphics::Window_Base::Update(){
	auto c = _Camera.get();
	auto t = SL_Graphics::_INTERNAL::_Time_Now - SL_Graphics::_INTERNAL::_Last_Time;
	for (auto& x : SL_Graphics::_INTERNAL::_Update_CallBacks) x(t, c);
}
void SL_Graphics::Window_Base::Draw(){
	auto t = SL_Graphics::_INTERNAL::_Time_Now - SL_Graphics::_INTERNAL::_Last_Time;
	auto c = _Camera.get();
	for (auto& x : SL_Graphics::_INTERNAL::_Draw_CallBacks) x(t, c);
}