// Copyright By UmiKami


#include "Character/AuraEnemy.h"


void AAuraEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Highlight Actor"));
	DrawDebugSphere(GetWorld(), GetActorLocation(), 20, 30, FColor::Purple, true);
}

void AAuraEnemy::UnHighlightActor()
{
	FlushPersistentDebugLines(GetWorld());
}
