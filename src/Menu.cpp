#include <SKSEMenuFramework.h>

#include "AdjustmentHandler.h"
#include "Menu.h"
#include "Settings.h"

namespace
{
	std::string g_message{};
	bool g_messageSuccess = false;

	inline void SetMessage(const char* a_message, bool a_success)
	{
		g_message = a_message;
		g_messageSuccess = a_success;
	}

	inline void RefreshAdjustment()
	{
		AdjustmentHandler::ForEachController([](const std::shared_ptr<AdjustmentHandler::ControllerData>& a_controller) {
			a_controller->AdjustConvex();
		});
	}

	constexpr float kMultiplierMin = 0.1f;
	constexpr float kMultiplierMax = 20.0f;
	constexpr float kSliderWidth = 350.0f;

	void SliderFloatWithDefault(const char* a_label, float* a_value, float a_default)
	{
		ImGuiMCP::SetNextItemWidth(kSliderWidth);
		if (ImGuiMCP ::SliderFloat(a_label, a_value, kMultiplierMin, kMultiplierMax, "%.2f")) {
			RefreshAdjustment();
		};
		ImGuiMCP::SameLine();

		std::string defaultLabel = std::string("Default##") + a_label;
		if (ImGuiMCP::Button(defaultLabel.c_str())) {
			*a_value = a_default;
			RefreshAdjustment();
		}
	}

	void RenderSettings()
	{
		ImGuiMCP::SeparatorText("General");
		ImGuiMCP::Checkbox("Enable Actor Scale Fix", &Settings::bEnableActorScaleFix);
		ImGuiMCP::Checkbox("Enable State Adjustments", &Settings::bEnableStateAdjustments);

		ImGuiMCP::Text("Shape Controller");
		SliderFloatWithDefault("Sneak Height", &Settings::fSneakControllerShapeHeightMultiplier, 0.75f);
		SliderFloatWithDefault("Prone Height", &Settings::fProneControllerShapeHeightMultiplier, 0.30f);
		SliderFloatWithDefault("Swimming Height", &Settings::fSwimmingControllerShapeHeightMultiplier, 0.75f);
		SliderFloatWithDefault("Swimming Radius", &Settings::fSwimmingControllerShapeRadiusMultiplier, 2.0f);
		ImGuiMCP::Spacing();

		ImGuiMCP::SeparatorText("Debug");
		if (Settings::g_trueHUD) {
			// Installed: #33dd33
			ImGuiMCP::TextColored(ImGuiMCP::ImVec4(0.2f, 0.87f, 0.2f, 1.0f), "TrueHUD plugin: Installed");
		} else {
			// Not installed: #dd3333
			ImGuiMCP::TextColored(ImGuiMCP::ImVec4(0.87f, 0.2f, 0.2f, 1.0f), "TrueHUD plugin: Not installed!");
		}

		constexpr std::array<const char*, 3> DEBUG_DRAW_MODES = { "None", "Adjusted", "All" };
		auto currentMode = static_cast<int>(Settings::uDisplayDebugShapes);
		ImGuiMCP::SetNextItemWidth(200.0f);
		if (ImGuiMCP::Combo("Display Debug Shapes", &currentMode, DEBUG_DRAW_MODES.data(), static_cast<int>(DEBUG_DRAW_MODES.size()))) {
			Settings::uDisplayDebugShapes = static_cast<DebugDrawMode>(currentMode);
		}
		ImGuiMCP::Checkbox("Display Character Bumper", &Settings::bDisplayCharacterBumper);

		ImGuiMCP::Spacing();
		ImGuiMCP::Separator();

		if (ImGuiMCP::Button("Save Settings")) {
			auto result = Settings::WriteSettings();
			if (result == SI_OK) {
				// Saved: #33dd33
				::SetMessage("Saved settings.", true);
			} else {
				// Failed: #dd3333
				::SetMessage("Failed to save settings.", false);
			}
		}

		ImGuiMCP::SameLine(0.0f, 12.0f);
		if (ImGuiMCP::Button("Reset All")) {
			Settings::SetDefaults();
			::SetMessage("Reset settings.", true);
		}

		if (!g_message.empty()) {
			if (g_messageSuccess) {
				// Success: #33dd33
				ImGuiMCP::TextColored(ImGuiMCP::ImVec4(0.2f, 0.87f, 0.2f, 1.0f), g_message.c_str());
			} else {
				// Error: #dd3333
				ImGuiMCP::TextColored(ImGuiMCP::ImVec4(0.87f, 0.2f, 0.2f, 1.0f), g_message.c_str());
			}
		}
	}
}

namespace Menu
{
	void Initialize()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			logger::warn("SKSE Menu Framework is not installed; menu integration disabled");
			return;
		}

		SKSEMenuFramework::SetSection("Dynamic Collision Adjustment");
		SKSEMenuFramework::AddSectionItem("Settings", ::RenderSettings);

		logger::info("SKSE Menu Framework integration initialized");
	}
}
