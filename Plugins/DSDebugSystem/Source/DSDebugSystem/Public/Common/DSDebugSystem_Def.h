#pragma once
#include "CoreMinimal.h"
#include "DSDebugSystem_Def.generated.h"

// Log 类型
UENUM(BlueprintType)
enum class EDSLogType : uint8
{
	Fatal,
	Error,
	Warning,
	Display,
	Log,
	Verbose,
	VeryVerbose,
	All,
};

// 输出方式
UENUM(BlueprintType)
enum class EDSLogOption : uint8
{
	Default,
	Both,
	Viewport,
	Console,
	Disable
};