// Copyright By UmiKami


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	checkf(GEngine, TEXT("GEngine not valid."));

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	// This is required to use target data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
