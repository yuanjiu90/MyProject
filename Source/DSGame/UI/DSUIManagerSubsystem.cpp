#include "DSUIManagerSubsystem.h"

UDSUIManagerSubsystem::UDSUIManagerSubsystem()
{
}

UDSUIManagerSubsystem* UDSUIManagerSubsystem::Get(const UObject* WorldContextObject)
{
	UWorld* MyWorld = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (MyWorld)
	{
		return UGameInstance::GetSubsystem<UDSUIManagerSubsystem>(MyWorld->GetGameInstance());
	}
	return nullptr;
}

void UDSUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}
