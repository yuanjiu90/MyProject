// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameUIManagerSubsystem.h"

#include "CommonGameSettings.h"
#include "CommonLocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GameUIPolicy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIManagerSubsystem)

class FSubsystemCollectionBase;
class UClass;

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UCommonGameSettings* CommonGameSettings = GetDefault<UCommonGameSettings>();
	if (!CurrentPolicy && CommonGameSettings &&  !CommonGameSettings->DefaultUIPolicyClass.IsNull())
	{
		TSubclassOf<UGameUIPolicy> PolicyClass = CommonGameSettings->DefaultUIPolicyClass.LoadSynchronous();
		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
	}

	GetGameInstance()->OnLocalPlayerAddedEvent.AddUObject(this, &UGameUIManagerSubsystem::NotifyPlayerAdded);
	GetGameInstance()->OnLocalPlayerRemovedEvent.AddUObject(this, &UGameUIManagerSubsystem::NotifyPlayerDestroyed);
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	GetGameInstance()->OnLocalPlayerAddedEvent.RemoveAll(this);
	GetGameInstance()->OnLocalPlayerRemovedEvent.RemoveAll(this);
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
}

void UGameUIManagerSubsystem::NotifyPlayerAdded(ULocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(LocalPlayer));
	}
}

// void UGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
// {
// 	if (LocalPlayer && CurrentPolicy)
// 	{
// 		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
// 	}
// }

void UGameUIManagerSubsystem::NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer)
{
	if (LocalPlayer && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(LocalPlayer));
	}
}

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

