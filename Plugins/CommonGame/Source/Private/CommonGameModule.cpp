// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonGameSettings.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
/**
 * Implements the FCommonGameModule module.
 */
class FCommonGameModule : public IModuleInterface
{
public:
	FCommonGameModule();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

};


FCommonGameModule::FCommonGameModule()
{
}

void FCommonGameModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Common Game",
										 NSLOCTEXT("CommonGamePlugin","CommonGameSettingsName", "Common Game"),
										 NSLOCTEXT("CommonGamePlugin","RuntimeSettingsDescription", "Configure Common Game Settings defaults."),
										 GetMutableDefault<UCommonGameSettings>());
	}
}

void FCommonGameModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FCommonGameModule, CommonGame);
