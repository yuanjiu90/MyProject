// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonLocalPlayer.h"

#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameUIManagerSubsystem.h"
#include "GameUIPolicy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonLocalPlayer)

class APawn;
class APlayerState;
class FViewport;
struct FSceneViewProjectionData;

UCommonLocalPlayer::UCommonLocalPlayer()
	: Super(FObjectInitializer::Get())
{
}

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());

	if (PC)
	{
		Delegate.Execute(this, PC);
	}

	return OnPlayerControllerSet.Add(Delegate);
}
FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APlayerState* PlayerState = PC ? PC->PlayerState : nullptr;

	if (PlayerState)
	{
		Delegate.Execute(this, PlayerState);
	}
	
	return OnPlayerStateSet.Add(Delegate);
}

FDelegateHandle UCommonLocalPlayer::CallAndRegister_OnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate)
{
	APlayerController* PC = GetPlayerController(GetWorld());
	APawn* Pawn = PC ? PC->GetPawn() : nullptr;

	if (Pawn)
	{
		Delegate.Execute(this, Pawn);
	}

	return OnPlayerPawnSet.Add(Delegate);
}

bool UCommonLocalPlayer::GetProjectionData(FViewport* Viewport, FSceneViewProjectionData& ProjectionData, int32 StereoViewIndex) const
{
	if (!bIsPlayerViewEnabled)
	{
		return false;
	}

	return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
}

bool UCommonLocalPlayer::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
#if !UE_BUILD_SHIPPING
	return Super::Exec(InWorld, Cmd, Ar);
#else
	// TODO : 发送错误调用 Exec 的日志，并记录调用栈，输入玩家，发送到服务器
	check(false);
	return false;
#endif
}

UPrimaryGameLayout* UCommonLocalPlayer::GetRootUILayout() const
{
	if (UGameUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
	{
		if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
		{
			return Policy->GetRootLayout(this);
		}
	}

	return nullptr;
}
