#include "DSDebugSystem.h"
#include <Common/DSDebugSystemSetting.h>
#if WITH_EDITOR
#include "ISettingsModule.h"
#endif


#define LOCTEXT_NAMESPACE "FDSDebugSystemModule"



void FDSDebugSystemModule::StartupModule()
{
#if WITH_EDITOR
	// 注册Setting
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Engine", "DSDebug System",
			LOCTEXT("RuntimeSettingsName", "DSDebug System"),
			LOCTEXT("RuntimeSettingsDescription", "DSDebug System"),
			GetMutableDefault<UDSDebugSystemSetting>());
	}
#endif
}

void FDSDebugSystemModule::ShutdownModule()
{
#if WITH_EDITOR
	// 解注册Setting
	if (UObjectInitialized())
	{
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "Engine", "DSDebug System");
		}
	}
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDSDebugSystemModule, DSDebugSystem)