#include "DSGame/GameFramework/GameplayAbilities/DSAbilitySystemComponent.h"

#include "DSGameplayAbility.h"
#include "DSGame/GameFramework/Input/DSEnhancedInputComponentType.h"
#include "LogSystem/DSLogHelper.h"

DEFINE_LOG_CATEGORY(DSLogGAS);

UDSAbilitySystemComponent::UDSAbilitySystemComponent()
{
	
}

FGameplayAbilitySpecHandle UDSAbilitySystemComponent::GiveDSAbility(TSubclassOf<UDSGameplayAbility> AbilityClass,
	FActiveGameplayEffectHandle GameplayEffectHandle, int32 Level, EDSGameplayAbilityInputBinds InputOverride)
{
	FGameplayAbilitySpecHandle Handle;

	if (AbilityClass == nullptr)
	{
		DSLog(DSLogGAS, Warning, TEXT("GiveDSAbility Ability Class is nullptr!, 检查配置！！！"));
		return Handle;
	}

	// 检查是否是主控端
	if (!IsOwnerActorAuthoritative())
	{
		DSLog(DSLogGAS, Warning, TEXT("GiveDSAbility called on ability %s on the client, not allowed!"),*AbilityClass->GetDefaultObject<UDSGameplayAbility>()->GetName());
		return Handle;
	}

	UDSGameplayAbility* CDO= AbilityClass->GetDefaultObject<UDSGameplayAbility>();
	if (CDO->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalOnly)
	{
		DSLog(DSLogGAS, Warning, TEXT("GiveDSAbility called on ability %s that is non instanced or won't execute on server, not allowed!"),*CDO->GetName());
		return Handle;
	}

	int32 InputID = -1;
	if (InputOverride == EDSGameplayAbilityInputBinds::NONE)
	{
		InputID = CDO->DefaultInputBind == EDSGameplayAbilityInputBinds::NONE ? InputID : static_cast<int32>(CDO->DefaultInputBind) ;
	}
	else
	{
		InputID = static_cast<int32>(InputOverride);
	}

	FGameplayAbilitySpec AbilitySpec{ AbilityClass, Level, InputID, this };
	if(GameplayEffectHandle.IsValid())
	{
		AbilitySpec.GameplayEffectHandle = GameplayEffectHandle;
	}
	Handle = GiveAbility(AbilitySpec);
	
	return Handle;
}

void UDSAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	Super::AbilityLocalInputPressed(InputID);
	//可以执行一些自定义的逻辑
}

void UDSAbilitySystemComponent::AbilityLocalInputReleased(int32 InputID)
{
	Super::AbilityLocalInputReleased(InputID);
	//可以执行一些自定义的逻辑
}
