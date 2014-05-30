#ifndef SL_CAMERA_123
#define SL_CAMERA_123
#include "..\glm\glm.hpp"
#include "..\glm\ext.hpp"

namespace SL_Graphics{

	class Camera{
	public:

		enum Moving { MOVING_BACKWARD = -1, MOVING_NONE = 0, MOVING_FORWARD = 1 };
		enum Strafing { STRAFING_LEFT = -1, STRAFING_NONE = 0, STRAFING_RIGHT = 1 };

		Camera(){
			Position = glm::vec3(0.0f, 0.0f, 0.0f);
			Look = glm::vec3(0.0f, 0.0f, -1.0f);
			Right = glm::vec3(1.0f, 0.0f, 0.0f);
			Field_of_View = 45.0f;// 45 degree fov
			Near_Plane = 1.0f;
			Far_Plane = 1000.0f;
			View_Height = 480.0f;
			View_Width = 640.0f;

			Move = MOVING_NONE;
			Strafe = STRAFING_NONE;
			Look_Sentitivity = 2.0f;
			Heading_Degrees = Pitch_Degrees = 0.0f;
		}

		glm::mat4 Get_View() const { return View; }
		glm::mat4 Get_Projection() const { return Projection; }

		glm::vec3 Get_Position() const { return Position; }
		void Set_Position(glm::vec3 pos) { Position = pos; }

		glm::vec3 Get_Look() const { return Look; }
		void Set_Look(glm::vec3 lookat) { Look = lookat; }

		float Get_Field_of_View() const { return Field_of_View; }
		void Set_Field_of_View(float fov) { Field_of_View = fov; }

		float Get_Near_Plane() const { return Near_Plane; }
		void Set_Near_Plane(float np) { Near_Plane = np; }

		float Get_Far_Plane() const { return Far_Plane; }
		void Set_Far_Plane(float fp) { Far_Plane = fp; }

		float Get_View_Height() const { return View_Height; }
		void Set_View_Height(float s) { View_Height = s; }

		float Get_View_Width() const { return View_Width; }
		void Set_View_Width(float s) { View_Width = s; }

		void Set_View_Dimensions(int w, int h) { View_Width = (float)w; View_Height = (float)h; }

		void Set_Move(Moving m){ Move = m; }
		void Set_Strafe(Strafing m){ Strafe = m; }
		//Pitch is the amount of degrees of movement ABOUT the x axis. You can visualize this as nodding your head yes. That motion is Pitch
		void Pitch(float degrees){
			Pitch_Degrees = abs(degrees) > abs(Pitch_Degrees) ? degrees : Pitch_Degrees;
		}
		/*
		heading is the amount of radians of movment ABOUT the y axis. You can visual this as moving your head left to right. If you answer NO to a question using a head gesture, this is the movment of Heading.
		heading and yaw are used interchangeably, and are the same
		*/
		void Heading(float degrees){
			Heading_Degrees = abs(degrees) > abs(Heading_Degrees) ? degrees : Heading_Degrees;
		}

		void Update(double dt){
			Look = glm::normalize(Look);
			Right = glm::normalize(Right);
			if (Move != Moving::MOVING_NONE) {
				Position += Look * ((float)dt * (float)Move * 50.0f);
			}
			if (Strafe != STRAFING_NONE) {
				auto right = glm::normalize(glm::cross(Look, glm::vec3(0.0f, 1.0f, 0.0f)));
				Position += right * ((float)dt * (float)Strafe * 50.0f);
			}
			if (Heading_Degrees != 0.0f){
				Look = glm::rotateY(Look, Heading_Degrees);
				Right = glm::rotateY(Right, Heading_Degrees);
			}
			if (Pitch_Degrees != 0.0f){
				auto templook = glm::vec3(glm::rotate(glm::mat4(1.0f), Pitch_Degrees, Right) * glm::vec4(Look, 1.0f));
				if (templook.y < .9f && templook.y > -.9f) Look = templook;
			}
			Pitch_Degrees = 0.0f;
			Heading_Degrees = 0.0f;
			View = glm::lookAt(Position, Position + Look, glm::vec3(0.0f, 1.0f, 0.0f));

			Projection = glm::perspective(Field_of_View, View_Width / View_Height, Near_Plane, Far_Plane);
		}
	protected:
		glm::vec3 Position, Look, Right;
		glm::mat4 View, Projection;
		float Field_of_View, Near_Plane, Far_Plane, View_Height, View_Width, Look_Sentitivity, Heading_Degrees, Pitch_Degrees;
		Moving Move;
		Strafing Strafe;

	};
};


#endif