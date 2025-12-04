#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DSGame/GameFramework/Input/DSEnhancedInputComponentType.h"
#include "DSAbilitySystemComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DSLogGAS, Log, All);

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class UDSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UDSAbilitySystemComponent();

	// 赋予角色技能。
	UFUNCTION(BlueprintCallable, Category = "DSAbilitySystemComponent|GameplayAbility")
	FGameplayAbilitySpecHandle GiveDSAbility(TSubclassOf<UDSGameplayAbility> AbilityClass, FActiveGameplayEffectHandle GameplayEffectHandle = FActiveGameplayEffectHandle(),
		int32 Level = 1,EDSGameplayAbilityInputBinds InputOverride = EDSGameplayAbilityInputBinds::NONE);

	
	//------------------------------- 输入相关 Start -------------------------------
	UFUNCTION(BlueprintCallable, Category = "DSAbilitySystemComponent")
	virtual void AbilityLocalInputPressed(int32 InputID) override;

	UFUNCTION(BlueprintCallable, Category = "DSAbilitySystemComponent")
	virtual void AbilityLocalInputReleased(int32 InputID) override;
	//------------------------------- 输入相关 End -------------------------------
};
