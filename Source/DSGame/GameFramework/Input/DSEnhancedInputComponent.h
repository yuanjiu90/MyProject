#pragma once
#include "CoreMinimal.h"
#include "DSEnhancedInputComponentType.h"
#include "EnhancedInputComponent.h"
#include "DSEnhancedInputComponent.generated.h"


UCLASS(Config = Input)
class UDSEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	UDSEnhancedInputComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void SetupPlayerInputComponent();

	const UEnum* GetInputEnumPtr() const {return EnumPtr;};

	void UpdateInputConfig();

	template<class UserClass, typename FuncType>
	uint32 BindNativeAction(const UDSInputConfig* InputConfig, const FName InputName, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UDSInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc);

	void RemoveBinds();
private:
	void Input_AbilityInputTagPressed(FName InputName, bool bCheckLongPress);
	void Input_AbilityInputTagReleased(FName InputName);
private:
	
	UPROPERTY(Transient)
	TObjectPtr<const UEnum> EnumPtr;

	UPROPERTY(Transient)
	TObjectPtr<UDSInputConfig> InputActionConfig;
	
	//缓存绑定的输入事件
	TArray<uint32> BindHandles;
};

template<class UserClass, typename FuncType>
uint32 UDSEnhancedInputComponent::BindNativeAction(const UDSInputConfig* InputConfig, const FName InputName, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForName(InputName, bLogIfNotFound))
	{
		return BindHandles.Add(BindAction(IA, TriggerEvent, Object, Func).GetHandle());
	}
	return 0;
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UDSEnhancedInputComponent::BindAbilityActions(const UDSInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
{
	check(InputConfig);

	for (const FDSInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputName.IsValid())
		{
			if (PressedFunc && Action.bIsTrigger)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputName, Action.bCheckLongPress).GetHandle());
			}else
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputName, Action.bCheckLongPress).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputName).GetHandle());
			}
		}
	}
}
