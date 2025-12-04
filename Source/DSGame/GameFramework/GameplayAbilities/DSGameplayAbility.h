#pragma once
#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DSGameplayAbility.generated.h"

enum class EDSGameplayAbilityInputBinds : uint8;

UCLASS(abstract)
class UDSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UDSGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DSGameplayAbility")
	EDSGameplayAbilityInputBinds DefaultInputBind;
};
