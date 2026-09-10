#include "Hooks.h"
#include "Menu.h"
#include "Settings.h"

#include "AdjustmentHandler.h"

namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			Settings::Initialize();
			Settings::ReadSettings();
			Settings::RequestAPIs();
			break;
		case SKSE::MessagingInterface::kPostLoadGame:
		case SKSE::MessagingInterface::kNewGame:
			Settings::OnPostLoadGame();
			AdjustmentHandler::OnPostLoadGame();
			break;
		default:
			break;
		}
	}

	void InitializeLog()
	{
		// Unknown CI build error
		//
		// #if defined(_WIN32)
		// 		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		// 		const auto level = spdlog::level::trace;

		// 		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		// 		log->set_level(level);
		// 		log->flush_on(level);
		// 		spdlog::set_default_logger(std::move(log));
		// #endif

		spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(1 << 8);

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	Hooks::Install();
	Menu::Initialize();

	return true;
}
