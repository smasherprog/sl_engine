#ifndef CAMERA_EXT_H_123
#define CAMERA_EXT_H_123
#include "..\glfw\include\GLFW\glfw3.h"
#include "Camera.h"

#define PRESSED(x)(x == GLFW_PRESS || x == GLFW_REPEAT)

namespace SL_Graphics{
	namespace _INTERNAL{
		void key_callback(GLFWwindow* window, Camera* cam){
			int leftstatus = glfwGetKey(window, GLFW_KEY_LEFT) | glfwGetKey(window, GLFW_KEY_A);
			int rightstatus = glfwGetKey(window, GLFW_KEY_RIGHT) | glfwGetKey(window, GLFW_KEY_D);
			int forwardstatus = glfwGetKey(window, GLFW_KEY_UP) | glfwGetKey(window, GLFW_KEY_W);
			int backstatus = glfwGetKey(window, GLFW_KEY_DOWN) | glfwGetKey(window, GLFW_KEY_S);
			int rightmousebuttonstatus = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

			if (PRESSED(leftstatus)){
				if (PRESSED(forwardstatus) || PRESSED(backstatus) || PRESSED(rightmousebuttonstatus)) cam->Set_Strafe(SL_Graphics::Camera::Strafing::STRAFING_LEFT);
				else cam->Heading(1.0f);
			}
			else {
				if (PRESSED(rightstatus)) {
					if (PRESSED(forwardstatus) || PRESSED(backstatus) || PRESSED(rightmousebuttonstatus)) cam->Set_Strafe(SL_Graphics::Camera::Strafing::STRAFING_RIGHT);
					else cam->Heading(-1.0f);
				}
				else cam->Set_Strafe(SL_Graphics::Camera::Strafing::STRAFING_NONE);
			}
			if (PRESSED(forwardstatus)) cam->Set_Move(SL_Graphics::Camera::Moving::MOVING_FORWARD);
			else if (PRESSED(backstatus)) cam->Set_Move(SL_Graphics::Camera::Moving::MOVING_BACKWARD);
			else cam->Set_Move(SL_Graphics::Camera::Moving::MOVING_NONE);
		}
	};
};

#endif