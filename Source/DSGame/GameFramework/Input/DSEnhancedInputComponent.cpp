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
	InputDataConfig = UDSGameSettings::Get()->InputDataConfig;
	
	// 添加输入映射
	AddInputMapping();
	
	// 更新输入配置
	UpdateInputConfig();
}

void UDSEnhancedInputComponent::AddInputMapping() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (!IsValid(PlayerController))
	{
		DSLog(LogDSPlayerController, Error, TEXT("PlayerController Is Invalid"));
		return;
	}

	// 添加输入映射
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		FModifyContextOptions Options = FModifyContextOptions();
		Options.bNotifyUserSettings = true;
		Subsystem->AddMappingContext(InputDataConfig.InputMappingContext, 0, Options);
	}
}

void UDSEnhancedInputComponent::RemoveInputMapping() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (!IsValid(PlayerController))
	{
		DSLog(LogDSPlayerController, Error, TEXT("PlayerController Is Invalid"));
		return;
	}

	// 移除输入映射
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem)
	{
		FModifyContextOptions Options = FModifyContextOptions();
		Options.bNotifyUserSettings = true;
		Subsystem->RemoveMappingContext(InputDataConfig.InputMappingContext,Options);
	}
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
	
	if (nullptr == InputDataConfig.InputActionConfig)
	{
		DSLog(LogDSPlayerController, Error, TEXT("InputActionConfig Is Invalid"));
		return;
	}

	//移除所有绑定
	RemoveBinds();

	//绑定GA输入
	BindAbilityActions(InputDataConfig.InputActionConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased);	

	//绑定Native输入
	BindNativeAction(InputDataConfig.InputActionConfig, "Move", ETriggerEvent::Triggered, this, &UDSEnhancedInputComponent::Move);
	BindNativeAction(InputDataConfig.InputActionConfig, "Look", ETriggerEvent::Triggered, this, &UDSEnhancedInputComponent::Look);
	BindNativeAction(InputDataConfig.InputActionConfig, "Jump", ETriggerEvent::Started, this, &UDSEnhancedInputComponent::Jump);
	BindNativeAction(InputDataConfig.InputActionConfig, "Jump", ETriggerEvent::Completed, this, &UDSEnhancedInputComponent::StopJumping);
}

void UDSEnhancedInputComponent::RemoveBinds()
{
	for (const uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}

void UDSEnhancedInputComponent::Move(const FInputActionValue& InputActionValue)
{
	ADSCharacter* DSCharacter = GetDSCharacter();
	if (DSCharacter)
	{
		DSCharacter->Move(InputActionValue);
	}
}

void UDSEnhancedInputComponent::Look(const FInputActionValue& InputActionValue)
{
	ADSCharacter* DSCharacter = GetDSCharacter();
	if (DSCharacter)
	{
		DSCharacter->Look(InputActionValue);
	}
}

void UDSEnhancedInputComponent::Jump(const FInputActionValue& InputActionValue)
{
	ADSCharacter* DSCharacter = GetDSCharacter();
	if (DSCharacter)
	{
		DSCharacter->Jump();
	}
}

void UDSEnhancedInputComponent::StopJumping(const FInputActionValue& InputActionValue)
{
	ADSCharacter* DSCharacter = GetDSCharacter();
	if (DSCharacter)
	{
		DSCharacter->StopJumping();
	}
}

ADSCharacter* UDSEnhancedInputComponent::GetDSCharacter()
{
	ADSPlayerController* DSPlayerController = Cast<ADSPlayerController>(GetOwner());
	if (DSPlayerController)
	{
		APawn* LocalPawn = DSPlayerController->GetPawn();
		if (LocalPawn)
		{
			ADSCharacter* DSCharacter = Cast<ADSCharacter>(LocalPawn);
			return DSCharacter;
		}
	}
	return nullptr;
}

void UDSEnhancedInputComponent::Input_AbilityInputTagPressed(FName InputName, bool bCheckLongPress)
{
	if(!IsValid(InputDataConfig.InputActionConfig.Get()))
	{
		return;
	}

	//长按操作
	if (bCheckLongPress)
	{
		
	}

	ADSCharacter* DSCharacter = GetDSCharacter();
	if (DSCharacter)
	{
		//激活技能
		//DSCharacter->Input_AbilityInputTagPressed(InputName);
	}	
}

void UDSEnhancedInputComponent::Input_AbilityInputTagReleased(FName InputName)
{
	ADSCharacter* DSCharacter = GetDSCharacter();
	if (DSCharacter)
	{
		//结束技能
		//DSCharacter->Input_AbilityInputTagPressed(InputName);
	}
}
