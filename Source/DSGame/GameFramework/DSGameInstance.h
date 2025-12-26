#pragma once
#include "CoreMinimal.h"
#include "CommonGameInstance.h"
#include "Engine/GameInstance.h"
#include "DSGameInstance.generated.h"

UCLASS(minimalapi)
class UDSGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UDSGameInstance(const FObjectInitializer& ObjectInitializer);
	
	
};
