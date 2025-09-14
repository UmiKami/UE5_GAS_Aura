// Copyright By UmiKami


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	checkf(AbilitySystemComponent, TEXT("AbilitySystem Component is NULL on AuraEnemy"))

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AAuraEnemy::HighlightActor()
{
	const TObjectPtr<USkeletalMeshComponent> CharMesh = GetMesh();
	
	CharMesh->SetRenderCustomDepth(true);
	CharMesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// in case there are enemies that swap weapons
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlightActor()
{
	const TObjectPtr<USkeletalMeshComponent> CharMesh = GetMesh();
	
	CharMesh->SetRenderCustomDepth(false);
	// in case there are enemies that swap weapons
	if (Weapon) Weapon->SetRenderCustomDepth(false);
}
