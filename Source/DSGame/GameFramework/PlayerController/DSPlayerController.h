#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDSPlayerController, Log, All);

UCLASS()
class ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADSPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupInputComponent() override;
};
