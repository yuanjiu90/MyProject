#pragma once
#include "CommonGameSettings.generated.h"

class UGameUIPolicy;

UCLASS(config=Editor, defaultconfig)
class COMMONGAME_API UCommonGameSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UCommonGameSettings(const FObjectInitializer& Initializer);

public:

	UPROPERTY(config, EditAnywhere, Category = General)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;

};
