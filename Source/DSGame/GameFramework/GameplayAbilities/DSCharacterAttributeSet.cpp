#include <DSGame/GameFramework/GameplayAbilities/DSCharacterAttributeSet.h>

#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
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

//允许在整个GE执行前，就介入并修改提案或直接否决它。
// ****** 仅在GE即将修改BaseValue时被调用（Instant或带周期的GE） ******
//Return: true:GE应该被执行。false:GE应该被否决。
bool UDSCharacterAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	bool bReturn = Super::PreGameplayEffectExecute(Data);
	
	//允许在整个GameplayEffect执行前，就介入并修改提案或直接否决它。

	//AActor* TargetActor = Data.Target.GetAvatarActor();//获取目标
	//AActor* InstigatorAction = Data.EffectSpec.GetEffectContext().GetInstigator();//获取GE执行器

	

	return bReturn;
}

//在GE成功修改了BaseValue后，进行最终的游戏逻辑响应和事件触发
// ****** 仅在GE修改BaseValue后被调用（Instant或带周期的GE） ******
void UDSCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	
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
