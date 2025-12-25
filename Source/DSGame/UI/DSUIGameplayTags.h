#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DSUIGameplayTags.generated.h"

UCLASS(config = Game, Blueprintable)
class DSGAME_API UDSUIGameplayTags : public UObject
{
	GENERATED_BODY()
public:
	virtual void BeginDestroy() override;
	
	static UDSUIGameplayTags& Get();

public:
	//Modal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI.Layer")
	FGameplayTag UI_Layer_Modal;
	//Menu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI.Layer")
	FGameplayTag UI_Layer_Menu;
	//GameMenu
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI.Layer")
	FGameplayTag UI_Layer_GameMenu;
	//Game
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI.Layer")
	FGameplayTag UI_Layer_Game;

private:
	static TObjectPtr<UDSUIGameplayTags> Singleton;
	void InitializeNativeTags();

	void AddNativeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
};
