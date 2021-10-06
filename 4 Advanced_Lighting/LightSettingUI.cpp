#include "LightSettingUI.h"

namespace LightSettingUI {
	static LightType display_ltype = LightType::DirectionLight;
	static int dir_light_idx = 0;
	static int point_light_idx = 0;
	static int spot_light_idx = 0;

	static bool show_my_window = true;
	static bool show_demo_window = false;
}


void LightSettingUI::display() {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;

	if (show_my_window) {
		ImGui::Begin("Set Light Color", &show_my_window,window_flags);
		ImGui::Text("here can set background color");
		ImGui::SliderFloat3("bg color slider:", value_ptr(background_color), 0.0, 1.0);
		ImGui::Checkbox("demo window", &show_demo_window);
		if (ImGui::CollapsingHeader("test collapsingheader")) {
			if (ImGui::TreeNode("test treenode")) {
				bool test_b[5];
				ImGui::BeginTable("the table", 2);
				ImGui::TableNextColumn(); ImGui::Text("123");
				ImGui::TableNextColumn(); ImGui::Checkbox("checkbox 0", &test_b[0]);
				ImGui::TableNextColumn(); ImGui::Checkbox("checkbox 1", &test_b[1]);
				ImGui::TableNextColumn(); ImGui::Checkbox("checkbox 2", &test_b[2]);
				ImGui::TableNextColumn(); ImGui::Checkbox("checkbox 3", &test_b[3]);
				ImGui::TableNextColumn(); ImGui::Checkbox("checkbox 4", &test_b[4]);
				ImGui::EndTable();
			}
		}
		if (ImGui::Button("Add direction light.")) {
			LightManager::create_direction_light(lightingshader,0.3f * vec3(1.0, 1.0, 1.0), vec3(0.0, -0.7, -0.7));
		}
		if (ImGui::Button("Add point light.")) {
			LightManager::create_point_light(light_box, vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 0.007, 0.00028));
		}
		if (ImGui::Button("Add spot light.")) {
			LightManager::create_spot_light(light_box, vec3(1.0, 1.0, 1.0), vec3(0.0, 0.0, 0.0), vec3(1.0, 0.007, 0.00028), vec3(0.0, 0.0, -1.0), 15.0, 17.0);
		}
		ImGui::BeginTabBar("Lights");
		if (ImGui::BeginTabItem("Direction")) {
			display_ltype = LightType::DirectionLight;
			ImGui::BeginListBox("Direction Light");
			for (size_t i = 0; i < LightManager::lights.size(); i++) {
				if (LightManager::lights[i]->type != LightType::DirectionLight)
					continue;

				const bool is_selected = (i == dir_light_idx);

				if (ImGui::Selectable(LightManager::lights[i]->name.c_str(), is_selected)) {
					dir_light_idx = i;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Point")) {
			display_ltype = LightType::PointLight;
			ImGui::BeginListBox("Point Light");
			for (size_t i = 0; i < LightManager::lights.size(); i++) {
				if (LightManager::lights[i]->type != LightType::PointLight)
					continue;

				const bool is_selected = (i == point_light_idx);

				if (ImGui::Selectable(LightManager::lights[i]->name.c_str(), is_selected)) {
					point_light_idx = i;
				}
				if (point_light_idx)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Spot")) {
			display_ltype = LightType::SpotLight;
			ImGui::BeginListBox("Spot Light");
			for (size_t i = 0; i < LightManager::lights.size(); i++) {
				if (LightManager::lights[i]->type != LightType::SpotLight)
					continue;

				const bool is_selected = (i == spot_light_idx);

				if (ImGui::Selectable(LightManager::lights[i]->name.c_str(), is_selected)) {
					spot_light_idx = i;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		switch (display_ltype) {
		case LightType::DirectionLight: {
			bool open_status_temp = LightManager::lights[dir_light_idx]->get_open_status();
			ImGui::Checkbox("Open", &open_status_temp);
			LightManager::lights[dir_light_idx]->set_open_status(open_status_temp);
			ImGui::SameLine(); ImGui::SliderFloat3("Color", value_ptr(LightManager::lights[dir_light_idx]->color), 0.0, 1.0, "%.2f");
			ImGui::SliderFloat3("Direction", value_ptr(static_cast<DirectionLight*>(LightManager::lights[dir_light_idx])->direction), 0.0, 1.0, "%.2f");
			break;
		}
		case LightType::PointLight: {
			bool open_status_temp = LightManager::lights[point_light_idx]->get_open_status();
			ImGui::Checkbox("Open", &open_status_temp);
			ImGui::SameLine(); LightManager::lights[point_light_idx]->set_open_status(open_status_temp);
			ImGui::SliderFloat3("Color", value_ptr(LightManager::lights[point_light_idx]->color), 0.0, 1.0, "%.2f");
			ImGui::SliderFloat3("Position", value_ptr(static_cast<PointLight*>(LightManager::lights[point_light_idx])->origin_position), -50.0, 50.0, "%.2f");
			ImGui::SliderFloat3("Attenuation_coefs", value_ptr(static_cast<PointLight*>(LightManager::lights[point_light_idx])->_attenuation_coefs), 0.0, 1.0, "%.5f", ImGuiSliderFlags_Logarithmic);
			break;
		}
		case LightType::SpotLight: {
			bool open_status_temp = LightManager::lights[spot_light_idx]->get_open_status();
			ImGui::Checkbox("Open", &open_status_temp);
			ImGui::SameLine(); LightManager::lights[spot_light_idx]->set_open_status(open_status_temp);
			ImGui::SliderFloat3("Color", value_ptr(LightManager::lights[spot_light_idx]->color), 0.0, 1.0, "%.2f");
			ImGui::SliderFloat3("Position", value_ptr(static_cast<SpotLight*>(LightManager::lights[spot_light_idx])->origin_position), -50.0, 50.0, "%.2f");
			ImGui::SliderFloat3("Attenuation_coefs", value_ptr(static_cast<SpotLight*>(LightManager::lights[spot_light_idx])->_attenuation_coefs), 0.0, 1.0, "%.5f",ImGuiSliderFlags_Logarithmic);
			
			ImGui::SliderFloat3("Spot Direction", value_ptr(static_cast<SpotLight*>(LightManager::lights[spot_light_idx])->origin_spot_dir), 0.0, 1.0, "%.2f");
			ImGui::SliderFloat("Inner Angle", &static_cast<SpotLight*>(LightManager::lights[spot_light_idx])->_inner_range_angle, 0.0, 90.0, "%.1f");
			ImGui::SliderFloat("Outer Angle", &static_cast<SpotLight*>(LightManager::lights[spot_light_idx])->_outer_range_angle, 0.0, 90.0, "%.1f");
			break;
		}					 
		default:
			break;
		}
		ImGui::End();
	}

	if (show_demo_window) {
		ImGui::ShowDemoWindow(&show_demo_window);
	}
}
