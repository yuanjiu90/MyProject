#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DSCharacterAttributeSet.generated.h"

//属性设置宏
#define DS_GAMEPLAYATTRIBUTE_VALUE_SETTER(ClassName, PropertyName) \
	FORCEINLINE void Set##PropertyName(float NewVal) \
	{ \
		UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent(); \
		if (ensure(AbilityComp)) \
		{ \
			AbilityComp->SetNumericAttributeBase(Get##PropertyName##Attribute(), NewVal); \
			MARK_PROPERTY_DIRTY_FROM_NAME(ClassName, PropertyName, this); \
		}; \
	}

//属性初始化宏
#define DS_GAMEPLAYATTRIBUTE_VALUE_INITTER(ClassName, PropertyName) \
	FORCEINLINE void Init##PropertyName(float NewVal) \
	{ \
		PropertyName.SetBaseValue(NewVal); \
		PropertyName.SetCurrentValue(NewVal); \
		MARK_PROPERTY_DIRTY_FROM_NAME(ClassName, PropertyName, this); \
	}

//属性访问器宏
#define ATTRIBUTE_ACCESSORS_PRIMITIVE(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//属性访问器宏<网络版>  宏定义分别为：1.获取Attribute属性 2.获取CurrentValue 3.设置属性并标记为脏 4.设置BaseValue、CurrentValue并标记为脏
#define ATTRIBUTE_ACCESSORS_PRIMITIVE_WITH_REP(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	DS_GAMEPLAYATTRIBUTE_VALUE_SETTER(ClassName, PropertyName) \
	DS_GAMEPLAYATTRIBUTE_VALUE_INITTER(ClassName, PropertyName)


UCLASS()
class UDSCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UDSCharacterAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//属性值实际被修改前，对即将发生的改动进行预检查和约束
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//允许在整个GE执行前，就介入并修改提案或直接否决它。（即将修改BaseValue时被调用）
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData &Data) override;
	//在GE成功修改了BaseValue后，进行最终的游戏逻辑响应和事件触发
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
public:
	//生命值
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_PRIMITIVE_WITH_REP(UDSCharacterAttributeSet, Health);

	//最大生命值
	UPROPERTY(BlueprintReadOnly, Category = "Character", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_PRIMITIVE_WITH_REP(UDSCharacterAttributeSet, MaxHealth);
	

protected:
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty,bool bFillAffectedAttribute = false);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
};



