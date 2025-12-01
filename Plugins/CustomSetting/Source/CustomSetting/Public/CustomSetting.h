// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISettingsEditorModel.h"
#include "ISettingsViewer.h"

class FToolBarBuilder;
class FMenuBuilder;

class FCustomSettingModule
	: public IModuleInterface
	, public ISettingsViewer
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** ISettingsViewer **/
	virtual void ShowSettings(const FName& CategoryName, const FName& SectionName) override;

private:

	TWeakPtr<ISettingsEditorModel> m_settingsEditorModel;

	TSharedPtr<class FUICommandList> PluginCommands;
};
