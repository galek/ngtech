#pragma once

namespace NGTech {
	
	/*
	*/
	void error_callback(int error, const char* description);

	/*
	*/
	const char* get_key_name(int key);

	/*
	*/
	const char* get_action_name(int action);

	/*
	*/
	const char* get_button_name(int button);

	/*
	*/
	const char* get_mods_name(int mods);

	/*
	*/
	const char* get_character_string(int codepoint);

	/*
	*/
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/*
	*/
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	/*
	*/
	void cursor_position_callback(GLFWwindow* window, double x, double y);
	
	/*
	*/
	void window_size_callback(GLFWwindow* window, int width, int height);
	
	/*
	*/
	void window_close_callback(GLFWwindow* window);
}