// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomSetting.h"
#include "CustomSettingStyle.h"
#include "ToolMenus.h"
#include "ISettingsViewer.h"
#include "ISettingsModule.h"
#include "ISettingsContainer.h"
#include "ISettingsEditorModule.h"
#include "ISettingsEditorModel.h"
#include "ISettingsCategory.h"
#include <GameFramework/InputSettings.h>

static const FName CustomSettingTabName("CustomSetting");
static const FName CustomSettingContainerName("CustomSettingContainer");

#define LOCTEXT_NAMESPACE "FCustomSettingModule"

static void OpenSettings( FName ContainerName, FName CategoryName, FName SectionName )
{
	FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(ContainerName, CategoryName, SectionName);
}

void FCustomSettingModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FCustomSettingStyle::Initialize();
	FCustomSettingStyle::ReloadTextures();


	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([this]() {

		FToolMenuOwnerScoped OwnerScoped(this);
		{
			UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Edit");
			{
				FToolMenuSection& Section = Menu->FindOrAddSection("CustomSetting");
				Section.AddMenuEntry(
					"CustomSetting",
					FText::FromString("CustomSetting"),
					{},
					FSlateIcon(FCustomSettingStyle::GetStyleSetName(), "CustomSetting.Icon"),

					FUIAction(FExecuteAction::CreateStatic(&OpenSettings, CustomSettingContainerName, FName("Project"), FName("Maps")))
				);
			}
		}

	}));

	ISettingsModule* settingModlule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (settingModlule)
	{
// 		// GameMode & Map
// 		settingModlule->RegisterSettings(CustomSettingContainerName, "Project", "Maps",
// 			FText::FromString("Maps & Modes"),
// 			FText::FromString("Default maps, game modes and other map related settings."),
// 			GetMutableDefault<UGameMapsSettings>()
// 		);

		// Input settings
		settingModlule->RegisterSettings(CustomSettingContainerName, "Engine", "Input",
			LOCTEXT("EngineInputSettingsName", "Input"),
			LOCTEXT("ProjectInputSettingsDescription", "Input settings, including default input action and axis bindings."),
			GetMutableDefault<UInputSettings>()
		);

		settingModlule->RegisterViewer(CustomSettingContainerName, *this);
	}

	auto _handleSpawnTab = [this](const FSpawnTabArgs& SpawnTabArgs)->TSharedRef<SDockTab> {

		TSharedRef<SWidget> SettingsEditor = SNullWidget::NullWidget;
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule)
		{
			ISettingsEditorModule& SettingsEditorModule = FModuleManager::GetModuleChecked<ISettingsEditorModule>("SettingsEditor");
			ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer(CustomSettingContainerName);
			if (SettingsContainer.IsValid())
			{
				ISettingsEditorModelRef SettingsEditorModel = SettingsEditorModule.CreateModel(SettingsContainer.ToSharedRef());
				m_settingsEditorModel = SettingsEditorModel;
				SettingsEditor = SettingsEditorModule.CreateEditor(SettingsEditorModel);
			}
		}

		return SNew(SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				SettingsEditor
			];
	};

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(CustomSettingTabName, FOnSpawnTab::CreateLambda(_handleSpawnTab))
		.SetDisplayName(FText::FromString(TEXT("CustomSetting")))
		.SetMenuType(ETabSpawnerMenuType::Hidden)
		.SetIcon(FSlateIcon(FCustomSettingStyle::GetStyleSetName(), "CustomSetting.TitleIcon")
		);
}

void FCustomSettingModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FCustomSettingStyle::Shutdown();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(CustomSettingTabName);
}

void FCustomSettingModule::ShowSettings(const FName& CategoryName, const FName& SectionName)
{
	FGlobalTabmanager::Get()->TryInvokeTab(CustomSettingTabName);
	ISettingsEditorModelPtr SettingsEditorModel = m_settingsEditorModel.Pin();

	if (SettingsEditorModel.IsValid())
	{
		ISettingsCategoryPtr Category = SettingsEditorModel->GetSettingsContainer()->GetCategory(CategoryName);

		if (Category.IsValid())
		{
			SettingsEditorModel->SelectSection(Category->GetSection(SectionName));
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCustomSettingModule, CustomSetting)