#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DSDebugSystem_Def.h"
#include "DSDebugSystemSetting.generated.h"



UCLASS(config = Game, meta = (DisplayName = "DSDebugSystem Settings"))
class DSDEBUGSYSTEM_API UDSDebugSystemSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UDSDebugSystemSetting() {};

	UFUNCTION(Blueprintpure, Category = "Settings")
	static UDSDebugSystemSetting* GetDSDebugSystemSettings() { return Get(); }
	static UDSDebugSystemSetting* Get() { return GetMutableDefault<UDSDebugSystemSetting>(); }

public:

	virtual FName GetContainerName() const override
	{
		return FName(TEXT("CustomSettingContainer"));
	}

	virtual FName GetCategoryName() const override
	{
		return FName(TEXT("CustomSetting"));
	}

public:

	// Log 输出模式
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	EDSLogOption LogOption;
	// 窗口打印时间
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	float DisplayTime;
	// 窗口打印颜色
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	TMap<EDSLogType, FColor> LogColor;

	// 是否输出网络模式
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	bool bLogNetMode;
	// C++ 开启ensure
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	bool bEnableEnsure;
	// 是否输出Context
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	bool bLogContextObject;
	// C++是否输出类名及行数
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	bool bLogCodeLine;
	// 是否输出至文件
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category = "Log")
	bool bLogToFile;

};