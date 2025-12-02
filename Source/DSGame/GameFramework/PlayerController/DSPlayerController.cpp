#include "DSPlayerController.h"

#include "DSGame/GameFramework/Input/DSEnhancedInputComponent.h"

DEFINE_LOG_CATEGORY(LogDSPlayerController);

ADSPlayerController::ADSPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void ADSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UDSEnhancedInputComponent* DSEnhancedInputComponent = Cast<UDSEnhancedInputComponent>(InputComponent);
	if (DSEnhancedInputComponent)
	{
		DSEnhancedInputComponent->SetupPlayerInputComponent();
	}
}
