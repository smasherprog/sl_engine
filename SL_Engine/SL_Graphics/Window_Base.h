#ifndef SL_Window_Base_H_123
#define SL_Window_Base_H_123
#include <functional>
#include <vector>
#include <memory>

struct GLFWwindow;

namespace SL_Graphics{
	class Camera;
	namespace _INTERNAL{
		extern std::vector<std::function<bool(int, int, int, int)>> _Key_CallBacks;
		extern std::vector<std::function<bool(int, int, int)>> _Mouse_Button_CallBacks;
		extern std::vector<std::function<bool(double, double)>> _Mouse_Movement_CallBacks;
		extern std::vector<std::function<void(double, SL_Graphics::Camera*)>> _Update_CallBacks;
		extern std::vector<std::function<void(double, SL_Graphics::Camera*)>> _Draw_CallBacks;
		extern std::vector<std::function<void(int, int)>> _FrameBuffer_Change_CallBacks;
		extern std::function<void(int, int, int, int)> _Camera_Key_CallBack;
		extern std::function<void(double, double)> _Camera_Mouse_Movement_CallBack;

		extern double _Last_Mouse_X, _Last_Mouse_Y, _Delta_Mouse_X, _Delta_Mouse_Y, _Time_Now, _Last_Time;
	};
	
	class Window_Base{
		GLFWwindow *_Window;
		void* _OpenGL_Context;
		std::unique_ptr<Camera> _Camera;
		

	public:
		Window_Base(const char* window_title = "Window Title", int top_pos = 100, int left_pos = 100, int window_width = 640, int window_height = 480);
		virtual ~Window_Base();
		void Add_Key_Listener(std::function<bool(int, int, int, int)> f){ _INTERNAL::_Key_CallBacks.push_back(f); }
		void Add_Mouse_Button_Listener(std::function<bool(int, int, int)> f){ _INTERNAL::_Mouse_Button_CallBacks.push_back(f); }
		void Add_Mouse_Movement_Listener(std::function<bool(double, double)> f){ _INTERNAL::_Mouse_Movement_CallBacks.push_back(f); }
		void Add_Update_CallBack(std::function<void(double, SL_Graphics::Camera*)> f){ _INTERNAL::_Update_CallBacks.push_back(f); }
		void Add_Draw_CallBack(std::function<void(double, SL_Graphics::Camera*)> f){ _INTERNAL::_Draw_CallBacks.push_back(f); }
		void Add_FrameBuffer_Change_CallBack(std::function<void(int, int)> f){ _INTERNAL::_FrameBuffer_Change_CallBacks.push_back(f); }

		bool Begin();
		void End();
		void Update();
		void Draw();
		Camera* Get_Camera(){ return _Camera.get(); }
	};
};

#endif