// Copyright By UmiKami


#include "AuraAssetManager.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	checkf(GEngine, TEXT("GEngine not valid."));

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
