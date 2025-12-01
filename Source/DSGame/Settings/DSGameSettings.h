#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DSGameSettings.generated.h"

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "DSGame Settings"))
class DSGAME_API UDSGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UDSGameSettings();

	UFUNCTION(Blueprintpure, Category = "Settings")
	static UDSGameSettings* GetDSGameSettings() {return Get();}

	static UDSGameSettings* Get();

	virtual FName GetContainerName() const override
	{
		return FName(TEXT("CustomSettingContainer"));
	}
	
	virtual FName GetCategoryName() const override
	{
		return FName(TEXT("CustomSetting"));
	}
public:
	UPROPERTY(EditAnywhere, Config,BlueprintReadOnly, Category = "Gameplay")
	float TestFloat = 100.f;
};
