#include "DSGame/GameFramework/GameplayAbilities/DSGameplayAbility.h"

#include "DSGame/GameFramework/Input/DSEnhancedInputComponentType.h"

UDSGameplayAbility::UDSGameplayAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DefaultInputBind = EDSGameplayAbilityInputBinds::NONE;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	
}
