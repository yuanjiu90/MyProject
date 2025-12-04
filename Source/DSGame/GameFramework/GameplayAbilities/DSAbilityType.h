#pragma once
#include "CoreMinimal.h"
#include "DSAbilityType.generated.h"

class UDSGameplayAbility;
// 角色通用技能
UCLASS(Blueprintable)
class UDSGeneralSkillsAssets : public UDataAsset
{
	GENERATED_BODY()

public:
	//角色通用技能
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UDSGameplayAbility>> GeneralSkills;
};