#pragma once
#include "TrueHUDAPI.h"

enum class DebugDrawMode : std::uint32_t
{
	kNone = 0,
	kAdjusted = 1,
	kAll = 2
};

struct Settings
{
	static constexpr auto DIR = L"Data/MCM/Settings";
	static constexpr auto FULL_FILE_PATH = L"Data/MCM/Settings/DynamicCollisionAdjustment.ini";

	static void Initialize();
	static void ReadSettings();
	static int WriteSettings();

	static void OnPostLoadGame();
	static void UpdateGlobals();

	static void RequestAPIs();

	static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
	static void ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);

	static void SetDefaults()
	{
		bEnableActorScaleFix = kDefaultEnableActorScaleFix;
		bEnableStateAdjustments = kDefaultEnableStateAdjustments;
		fSneakControllerShapeHeightMultiplier = kDefaultSneakControllerShapeHeightMultiplier;
		fProneControllerShapeHeightMultiplier = kDefaultProneControllerShapeHeightMultiplier;
		fSwimmingControllerShapeHeightMultiplier = kDefaultSwimmingControllerShapeHeightMultiplier;
		fSwimmingControllerShapeRadiusMultiplier = kDefaultSwimmingControllerShapeRadiusMultiplier;

		uDisplayDebugShapes = kDefaultDisplayDebugShapes;
		bDisplayCharacterBumper = kDefaultDisplayCharacterBumper;
	}

	// General defaults
	static inline constexpr bool kDefaultEnableActorScaleFix = true;
	static inline constexpr bool kDefaultEnableStateAdjustments = true;
	static inline constexpr float kDefaultSneakControllerShapeHeightMultiplier = 0.75f;
	static inline constexpr float kDefaultProneControllerShapeHeightMultiplier = 0.30f;
	static inline constexpr float kDefaultSwimmingControllerShapeHeightMultiplier = 0.75f;
	static inline constexpr float kDefaultSwimmingControllerShapeRadiusMultiplier = 2.f;

	// Debug defaults
	static inline constexpr DebugDrawMode kDefaultDisplayDebugShapes = DebugDrawMode::kNone;
	static inline constexpr bool kDefaultDisplayCharacterBumper = false;

	// General
	static inline bool bEnableActorScaleFix = kDefaultEnableActorScaleFix;
	static inline bool bEnableStateAdjustments = kDefaultEnableStateAdjustments;
	static inline float fSneakControllerShapeHeightMultiplier = kDefaultSneakControllerShapeHeightMultiplier;
	static inline float fProneControllerShapeHeightMultiplier = kDefaultProneControllerShapeHeightMultiplier;
	static inline float fSwimmingControllerShapeHeightMultiplier = kDefaultSwimmingControllerShapeHeightMultiplier;
	static inline float fSwimmingControllerShapeRadiusMultiplier = kDefaultSwimmingControllerShapeRadiusMultiplier;

	// Debug
	static inline DebugDrawMode uDisplayDebugShapes = kDefaultDisplayDebugShapes;
	static inline bool bDisplayCharacterBumper = kDefaultDisplayCharacterBumper;

	// Non-MCM
	static inline TRUEHUD_API::IVTrueHUD4* g_trueHUD = nullptr;
	static inline RE::TESGlobal* glob_trueHUD = nullptr;
	static inline RE::BGSKeyword* kywd_NPC = nullptr;
};
