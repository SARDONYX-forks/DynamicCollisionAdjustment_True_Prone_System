#include <SimpleIni.h>

#include "Settings.h"

void Settings::Initialize()
{
	logger::info("Initializing...");

	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
		kywd_NPC = dataHandler->LookupForm<RE::BGSKeyword>(0x13794, "Skyrim.esm");
		glob_trueHUD = dataHandler->LookupForm<RE::TESGlobal>(0x801, "DynamicCollisionAdjustment.esl");
	}

	logger::info("...success");
}

void Settings::ReadSettings()
{
	logger::info("Reading MCM .ini...");

	CSimpleIniA mcm;
	mcm.SetUnicode();

	mcm.LoadFile(Settings::FULL_FILE_PATH);

	// General
	ReadBoolSetting(mcm, "General", "bEnableActorScaleFix", bEnableActorScaleFix);
	ReadBoolSetting(mcm, "General", "bEnableStateAdjustments", bEnableStateAdjustments);

	ReadFloatSetting(mcm, "General", "fSneakControllerCapsuleHeightMultiplier", fSneakControllerShapeHeightMultiplier);
	ReadFloatSetting(mcm, "General", "fSwimmingControllerShapeHeightMultiplier", fSwimmingControllerShapeHeightMultiplier);
	ReadFloatSetting(mcm, "General", "fSwimmingControllerShapeRadiusMultiplier", fSwimmingControllerShapeRadiusMultiplier);

	// Debug
	ReadUInt32Setting(mcm, "Debug", "uDisplayDebugShapes", (uint32_t&)uDisplayDebugShapes);
	ReadBoolSetting(mcm, "Debug", "bDisplayCharacterBumper", bDisplayCharacterBumper);

	logger::info("...success");
}

SI_Error Settings::WriteSettings()
{
	CSimpleIniA mcm;
	mcm.SetUnicode();

	mcm.SetBoolValue("General", "bEnableActorScaleFix", bEnableActorScaleFix);
	mcm.SetBoolValue("General", "bEnableStateAdjustments", bEnableStateAdjustments);

	mcm.SetDoubleValue("General", "fSneakControllerCapsuleHeightMultiplier", fSneakControllerShapeHeightMultiplier);
	mcm.SetDoubleValue("General", "ffProneControllerShapeHeightMultiplier", fProneControllerShapeHeightMultiplier);
	mcm.SetDoubleValue("General", "fSwimmingControllerShapeHeightMultiplier", fSwimmingControllerShapeHeightMultiplier);
	mcm.SetDoubleValue("General", "fSwimmingControllerShapeRadiusMultiplier", fSwimmingControllerShapeRadiusMultiplier);

	mcm.SetLongValue("Debug", "uDisplayDebugShapes", static_cast<long>(uDisplayDebugShapes));
	mcm.SetBoolValue("Debug", "bDisplayCharacterBumper", bDisplayCharacterBumper);

	std::error_code ec;
	std::filesystem::create_directories(Settings::DIR, ec);
	if (ec) {
		logger::error("Failed to create settings directory: {}", ec.message());
	}
	const auto result = mcm.SaveFile(Settings::FULL_FILE_PATH);

	if (result < 0) {
		logger::error("Failed to save settings: {}", result);
	}

	return result;
}

void Settings::OnPostLoadGame()
{
	RequestAPIs();
	UpdateGlobals();
}

void Settings::UpdateGlobals()
{
	if (glob_trueHUD) {
		glob_trueHUD->value = g_trueHUD != nullptr ? 1.f : 0.f;
	}
}

void Settings::RequestAPIs()
{
	if (!g_trueHUD) {
		Settings::g_trueHUD = reinterpret_cast<TRUEHUD_API::IVTrueHUD4*>(TRUEHUD_API::RequestPluginAPI(TRUEHUD_API::InterfaceVersion::V4));
	}
}

void Settings::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
}

void Settings::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}

void Settings::ReadUInt32Setting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
	}
}
