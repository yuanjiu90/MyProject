#pragma once
#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "DSUIManagerSubsystem.generated.h"

UCLASS()
class UDSUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
public:

	UDSUIManagerSubsystem();

	static UDSUIManagerSubsystem* Get(const UObject* WorldContextObject);

	// ~Begin UGameInstanceSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// ~End UGameInstanceSubsystem interface
};
