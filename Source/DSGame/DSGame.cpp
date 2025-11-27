// Copyright Epic Games, Inc. All Rights Reserved.

#include "DSGame.h"
#include "Modules/ModuleManager.h"
#include "GameplayTagsManager.h"
#include "GameFramework/DSGameplayTags.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDSGameModule, DSGame, "DSGame");

void FDSGameModule::StartupModule()
{
	IModuleInterface::StartupModule();

	// Register native tags
	UGameplayTagsManager::OnLastChanceToAddNativeTags().AddLambda([](){							
		UDSGameplayTags::Get();
	});
}

void FDSGameModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
