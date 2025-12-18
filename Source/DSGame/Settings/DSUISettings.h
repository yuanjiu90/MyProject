#pragma once
#include "CoreMinimal.h"
#include "DSGame/GameFramework/Input/DSEnhancedInputComponentType.h"
#include "Engine/DeveloperSettings.h"
#include "DSUISettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "DSUI Settings"))
class DSGAME_API UDSUISettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UDSUISettings();

	UFUNCTION(Blueprintpure, Category = "Settings")
	static UDSUISettings* GetDSGameSettings() {return Get();}

	static UDSUISettings* Get();

	virtual FName GetContainerName() const override
	{
		return FName(TEXT("CustomSettingContainer"));
	}
	
	virtual FName GetCategoryName() const override
	{
		return FName(TEXT("CustomSetting"));
	}
public:
	
	UPROPERTY(Config, EditAnywhere, Category = "UI")
	FText TestText;
};
