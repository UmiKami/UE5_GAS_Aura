// Copyright By UmiKami


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
