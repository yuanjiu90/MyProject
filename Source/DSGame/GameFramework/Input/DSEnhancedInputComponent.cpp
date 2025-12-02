#include "DSEnhancedInputComponent.h"

#include "DSEnhancedInputComponentType.h"
#include "EnhancedInputSubsystems.h"
#include "DSGame/GameFramework/Character/DSCharacter.h"
#include "DSGame/GameFramework/PlayerController/DSPlayerController.h"
#include "DSGame/Settings/DSGameSettings.h"
#include "LogSystem/DSLogHelper.h"

UDSEnhancedInputComponent::UDSEnhancedInputComponent(const FObjectInitializer& ObjectInitializer)
{
	EnumPtr = StaticEnum<EDSGameplayAbilityInputBinds>();
}

void UDSEnhancedInputComponent::SetupPlayerInputComponent()
{
	UpdateInputConfig();
}

void UDSEnhancedInputComponent::UpdateInputConfig()
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(GetOwner());
	if (!IsValid(DSPlayerController))
	{
		DSLog(LogDSPlayerController, Error, TEXT("PlayerController Is Invalid"));
		return;
	}
		
	UDSEnhancedInputComponent* DSEnhancedInputComponent = Cast<UDSEnhancedInputComponent>(DSPlayerController->InputComponent);
	if (nullptr == DSEnhancedInputComponent)
	{
		DSLog(LogDSPlayerController, Error, TEXT("DSEnhancedInputComponent Is Invalid"));
		return;
	}
	
	const ULocalPlayer* LP = DSPlayerController->GetLocalPlayer();
	if (nullptr == LP)
	{
		DSLog(LogDSPlayerController, Error, TEXT("LocalPlayer Is Invalid"));
		return;
	}
	
	const UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (nullptr == Subsystem)
	{
		DSLog(LogDSPlayerController, Error, TEXT("EnhancedInputLocalPlayerSubsystem Is Invalid"));
		return;
	}

	InputActionConfig = UDSGameSettings::Get()->InputActionConfig;
	if (nullptr == InputActionConfig)
	{
		DSLog(LogDSPlayerController, Error, TEXT("InputActionConfig Is Invalid"));
		return;
	}

	//移除所有绑定
	RemoveBinds();

	//绑定GA输入
	BindAbilityActions(InputActionConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased);	
}

void UDSEnhancedInputComponent::RemoveBinds()
{
	for (const uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}

void UDSEnhancedInputComponent::Input_AbilityInputTagPressed(FName InputName, bool bCheckLongPress)
{
	if(!IsValid(InputActionConfig.Get()))
	{
		return;
	}

	//长按操作
	if (bCheckLongPress)
	{
		
	}
	
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(GetOwner());
	if (DSPlayerController)
	{
		APawn* LocalPawn = DSPlayerController->GetPawn();
		if (LocalPawn)
		{
			ADSCharacter* DSCharacter = Cast<ADSCharacter>(LocalPawn);
			if (DSCharacter)
			{
				//激活技能
				//DSCharacter->Input_AbilityInputTagPressed(InputName);
			}
		}
	}
	
}

void UDSEnhancedInputComponent::Input_AbilityInputTagReleased(FName InputName)
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(GetOwner());
	if (DSPlayerController)
	{
		APawn* LocalPawn = DSPlayerController->GetPawn();
		if (LocalPawn)
		{
			ADSCharacter* DSCharacter = Cast<ADSCharacter>(LocalPawn);
			if (DSCharacter)
			{
				//结束技能
				//DSCharacter->Input_AbilityInputTagPressed(InputName);
			}
		}
	}
}
