// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonUIExtensions.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "CommonLocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"
#include "PrimaryGameLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "CommonUITypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonUIExtensions)

int32 UCommonUIExtensions::InputSuspensions = 0;

ECommonInputType UCommonUIExtensions::GetOwningPlayerInputType(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType();
		}
	}

	return ECommonInputType::Count;
}

bool UCommonUIExtensions::IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType() == ECommonInputType::Touch;
		}
	}
	return false;
}

bool UCommonUIExtensions::IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject)
{
	if (WidgetContextObject)
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(WidgetContextObject->GetOwningLocalPlayer()))
		{
			return InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;
		}
	}
	return false;
}

UCommonActivatableWidget* UCommonUIExtensions::PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSubclassOf<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(WidgetClass != nullptr))
	{
		return nullptr;
	}

	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				return RootLayout->PushWidgetToLayerStack(LayerName, WidgetClass);
			}
		}
	}

	return nullptr;
}

void UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, FGameplayTag LayerName, TSoftClassPtr<UCommonActivatableWidget> WidgetClass)
{
	if (!ensure(LocalPlayer) || !ensure(!WidgetClass.IsNull()))
	{
		return;
	}

	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				const bool bSuspendInputUntilComplete = true;
				RootLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass);
			}
		}
	}
}

TSharedPtr<FStreamableHandle> UCommonUIExtensions::PushStreamedContentToLayer_WithStateFunc(const ULocalPlayer* LocalPlayer, const FGameplayTag& LayerName,
	const TSoftClassPtr<UCommonActivatableWidget>& WidgetClass, TFunction<void(EAsyncWidgetLayerState, UCommonActivatableWidget*)> StateFunc)
{
	if (!ensure(LocalPlayer) || !ensure(!WidgetClass.IsNull()))
	{
		return nullptr;
	}

	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				const bool bSuspendInputUntilComplete = false;
				return RootLayout->PushWidgetToLayerStackAsync(LayerName, bSuspendInputUntilComplete, WidgetClass, StateFunc);
			}
		}
	}
	return nullptr;
}

void UCommonUIExtensions::PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget)
{
	if (!ActivatableWidget)
	{
		// Ignore request to pop an already deleted widget
		return;
	}

	if (const ULocalPlayer* LocalPlayer = ActivatableWidget->GetOwningLocalPlayer())
	{
		if (const UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
		{
			if (const UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
				{
					RootLayout->FindAndRemoveWidgetFromLayer(ActivatableWidget);
				}
			}
		}
	}
}

ULocalPlayer* UCommonUIExtensions::GetLocalPlayerFromController(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		return Cast<ULocalPlayer>(PlayerController->Player);
	}

	return nullptr;
}

FName UCommonUIExtensions::SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason)
{
	return SuspendInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendReason);
}

FName UCommonUIExtensions::SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		InputSuspensions++;
		FName SuspendToken = SuspendReason;
		SuspendToken.SetNumber(InputSuspensions);

		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, true);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, true);

		return SuspendToken;
	}

	return NAME_None;
}

void UCommonUIExtensions::ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken)
{
	ResumeInputForPlayer(PlayerController ? PlayerController->GetLocalPlayer() : nullptr, SuspendToken);
}

void UCommonUIExtensions::ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken)
{
	if (SuspendToken == NAME_None)
	{
		return;
	}

	if (UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
	{
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::MouseAndKeyboard, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Gamepad, SuspendToken, false);
		CommonInputSubsystem->SetInputTypeFilter(ECommonInputType::Touch, SuspendToken, false);
	}
}

void UCommonUIExtensions::SetLayerVisibility(ULocalPlayer* LocalPlayer, FGameplayTag LayerName, ESlateVisibility InVisibility)
{
	if (!ensure(LocalPlayer))
	{
		return;
	}

	if (const auto* GameInstance = LocalPlayer->GetGameInstance())
	{
		if (UGameUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UGameUIManagerSubsystem>())
		{
			if (const UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
				{
					UCommonActivatableWidgetContainerBase* Layer = RootLayout->GetLayerWidget(LayerName);
					if (IsValid(Layer))
					{
						Layer->SetVisibility(InVisibility);
					}
				}
			}
		}
	}
}

FKey UCommonUIExtensions::GetFirstKeyForInputType(ULocalPlayer* LocalPlayer, const UInputAction* InputAction)
{
	const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);
	if (CommonInputSubsystem)
	{
		return CommonUI::GetFirstKeyForInputType(CommonInputSubsystem->GetLocalPlayer(), CommonInputSubsystem->GetCurrentInputType(), InputAction);
	}
	return FKey();
}

void UCommonUIExtensions::RemoveAllWidgetFromLayer(ULocalPlayer* LocalPlayer, const FGameplayTag GameplayTag)
{
	if (const UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (const UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				RootLayout->RemoveAllWidgetFromLayer(GameplayTag);
			}
		}
	}
}

bool UCommonUIExtensions::LayerHasContent(ULocalPlayer* LocalPlayer, const FGameplayTag LayerName)
{
	if (!ensure(LocalPlayer))
	{
		return false;
	}

	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				UCommonActivatableWidgetContainerBase* Layer = RootLayout->GetLayerWidget(LayerName);
				if (IsValid(Layer))
				{
					return Layer->GetNumWidgets() > 0;
				}
			}
		}
	}
	return false;
}

bool UCommonUIExtensions::AboveLayerHasContent(ULocalPlayer* LocalPlayer, const FGameplayTag LayerName)
{
	if (!ensure(LocalPlayer))
	{
		return false;
	}

	TArray LayerPriority
	{
		// 不依赖DSGame
		FGameplayTag::RequestGameplayTag(FName("UI.Layer.Game")),
		FGameplayTag::RequestGameplayTag(FName("UI.Layer.GameMenu")),
		FGameplayTag::RequestGameplayTag(FName("UI.Layer.Menu")),
		FGameplayTag::RequestGameplayTag(FName("UI.Layer.Modal")),
		//FGameplayTag::RequestGameplayTag(FName("UI.Layer.Debug")),
		FGameplayTag::RequestGameplayTag(FName("UI.Layer.Queue")),
	};

	int32 LayerIndex = LayerPriority.Find(LayerName);
	if (LayerIndex == INDEX_NONE)
	{
		return false;
	}
	
	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer)))
			{
				for (int32 Index = LayerIndex; Index < LayerPriority.Num(); ++Index)
				{
					FGameplayTag LayerToCheck = LayerPriority[Index];
					UCommonActivatableWidgetContainerBase* Layer = RootLayout->GetLayerWidget(LayerName);
					if (IsValid(Layer) && Layer->GetNumWidgets() > 0)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

UPrimaryGameLayout* UCommonUIExtensions::GetRootLayout(ULocalPlayer* LocalPlayer)
{
	if (!ensure(LocalPlayer))
	{
		return nullptr;
	}
	if (UGameUIManagerSubsystem* UIManager = LocalPlayer->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			return Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(LocalPlayer));
		}
	}
	return nullptr;
}