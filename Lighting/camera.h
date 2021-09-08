#pragma once
#include<iostream>
#include<format>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

enum class Move_direction {
	FORWARD,
	BACK,
	LEFT,
	RIGHT
};
	
class Camera {
public:
	Camera(vec3 campos,vec3 camdir,vec3 camup) {
		cam_pos = campos;
		cam_dir = camdir;
		cam_up = camup;
		old_xpos = 0.0;
		old_ypos = 0.0;
		yaw_ = -90;
		pitch_ = 0;
		fov = 60.0;
	}
	
	mat4 get_view_matrix() {
		//cout << "campos x:" << cam_pos.x << "y:" << cam_pos.y << "z:" << cam_pos.z << endl;
		//cout << "camdir x:"<<cam_dir.x<< "camdir y:"<<cam_dir.y<<"camdir z:" <<cam_dir.z << endl;
		return lookAt(cam_pos,cam_pos+cam_dir,cam_up);
	}

	void look(double xpos,double ypos) {
		if (old_xpos == 0.0 && old_ypos == 0.0) {
			old_xpos = xpos;
			old_ypos = ypos;
		}

		float x_offset = xpos - old_xpos;
		float y_offset = old_ypos - ypos;

		old_xpos = xpos;
		old_ypos = ypos;

		float rate = 0.1;
		x_offset *= rate;
		y_offset *= rate;
		yaw_ += x_offset;
		pitch_ += y_offset;

		//cout <<"yaw: "<< yaw_ << "pitch: "<< pitch_ << endl;

		if (pitch_ > 89)
			pitch_ = 89;
		if (pitch_ < -89)
			pitch_ = -89;

		//vec3 cam_dir;
		cam_dir.x = cos(radians(pitch_)) * cos(radians(yaw_));
		cam_dir.y = sin(radians(pitch_));
		cam_dir.z = cos(radians(pitch_)) * sin(radians(yaw_));
		cam_dir = normalize(cam_dir);
	}

	void move(Move_direction md,float delta_time) {
		float cam_speed = 20*delta_time;
		if (md== Move_direction::FORWARD) {
			cam_pos = cam_pos + cam_dir * cam_speed;
		}
		if (md == Move_direction::BACK) {
			cam_pos = cam_pos - cam_dir * cam_speed;
		}
		if (md == Move_direction::LEFT) {
			cam_pos = cam_pos - normalize(cross(cam_dir, cam_up)) * cam_speed;
		}
		if (md == Move_direction::RIGHT) {
			cam_pos = cam_pos + normalize(cross(cam_dir, cam_up)) * cam_speed;
		}
	}

	void zoom(double xoffset, double yoffset) {
		if (fov >= 1.0 && fov <= 90.0) {
			fov -= yoffset;
		}
		if (fov <= 1.0) {
			fov = 1.0;
		}
		if (fov >= 90.0) {
			fov = 90.0;
		}
	}
public:
	float fov;
	vec3 cam_pos;
	vec3 cam_dir;
	vec3 cam_up;
	float old_xpos;
	float old_ypos;
	float pitch_;
	float yaw_;
};