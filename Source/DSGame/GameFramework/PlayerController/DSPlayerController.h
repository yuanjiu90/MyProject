#pragma once
#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDSPlayerController, Log, All);

UCLASS()
class ADSPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	ADSPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void SetupInputComponent() override;
};
