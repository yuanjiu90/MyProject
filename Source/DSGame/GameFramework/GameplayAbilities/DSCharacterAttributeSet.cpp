#include <DSGame/GameFramework/GameplayAbilities/DSCharacterAttributeSet.h>

#include "AbilitySystemComponent.h"
#include "HLSLMathAliases.h"
#include "Net/UnrealNetwork.h"

UDSCharacterAttributeSet::UDSCharacterAttributeSet():
	Health(100),
	MaxHealth(1000)
{}

void UDSCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 同步给所有端的属性
	constexpr FDoRepLifetimeParams SharedParams{ COND_None, REPNOTIFY_Always, true };
	DOREPLIFETIME_WITH_PARAMS_FAST(UDSCharacterAttributeSet, Health, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(UDSCharacterAttributeSet, MaxHealth, SharedParams);

	// 只同步给主控端的属性
	//constexpr FDoRepLifetimeParams SharedParamsAutonomous{ COND_AutonomousOnly, REPNOTIFY_Always, true };
	//DOREPLIFETIME_WITH_PARAMS_FAST(UDSCharacterAttributeSet, HealthRecover, SharedParamsAutonomous);
}

//属性值实际被修改前，对即将发生的改动进行预检查和约束
//Attribute:即将被修改的属性
//NewValue:即将应用的新数值，可直接修改这个参数来钳制或调整最终生效的值
void UDSCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		//当血量发生变化时，将血量限制在0-MaxHealth之间
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	
	if (Attribute == GetMaxHealthAttribute())
	{
		//当最大血量发生变化时，将当前血量进行等比缩放
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue,GetHealthAttribute());
	}
}

void UDSCharacterAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty,
	bool bFillAffectedAttribute)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		if (bFillAffectedAttribute && CurrentValue == 0)
			return;
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}


void UDSCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	//1.首先，调用宏生成的父类函数
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDSCharacterAttributeSet, Health, OldHealth);

	//可以广播血量发生变化的消息
}

void UDSCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	//1.首先，调用宏生成的父类函数
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDSCharacterAttributeSet, MaxHealth, OldMaxHealth);
	
}
