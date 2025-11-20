// Copyright By UmiKami


#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"

#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemFunctionLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemFunctionLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController* PlayerController = WorldContextObject->GetWorld()->GetFirstPlayerController())
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PS = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PlayerController, PS, ASC, AS);

			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}

	return nullptr;
}

void UAuraAbilitySystemFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	
	if (!AuraGameModeBase) return;

	const TObjectPtr<UCharacterClassInfo> CharacterClassInfo = AuraGameModeBase->CharacterClassInfo;
	
	checkf(CharacterClassInfo, TEXT("Data Asset for character class info not set in AuraGameMode."))
	
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	AActor* Avatar = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle PrimaryEffectContext = ASC->MakeEffectContext();
	PrimaryEffectContext.AddSourceObject(Avatar);
	
	const FGameplayEffectSpecHandle PrimaryEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data);

	FGameplayEffectContextHandle SecondaryEffectContext = ASC->MakeEffectContext();
	SecondaryEffectContext.AddSourceObject(Avatar);
	
	const FGameplayEffectSpecHandle SecondaryEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data);

	FGameplayEffectContextHandle VitalEffectContext = ASC->MakeEffectContext();
	VitalEffectContext.AddSourceObject(Avatar);
	
	const FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data);
}
