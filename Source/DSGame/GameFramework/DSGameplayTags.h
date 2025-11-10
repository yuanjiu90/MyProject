#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DSGameplayTags.generated.h"

UCLASS(config = Game, Blueprintable)
class DSGAME_API UDSGameplayTags : public UObject
{
	GENERATED_BODY()
public:
	virtual void BeginDestroy() override;
	
	static UDSGameplayTags& Get();

public:
	//测试代码
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DSGameTags")
	FGameplayTag DSGameTestTags;

private:
	static TObjectPtr<UDSGameplayTags> Singleton;
	void InitializeNativeTags();
};
