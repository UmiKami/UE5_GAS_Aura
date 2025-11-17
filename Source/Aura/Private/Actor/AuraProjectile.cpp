// Copyright By UmiKami


#include "Actor/AuraProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilityBlueprint.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");

	SetRootComponent(Sphere);

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionObjectType(ECC_PROJECTILE);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 550;
	ProjectileMovementComponent->MaxSpeed = 550;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
}


void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	PlayLoopingSound();
	SetLifeSpan(LifeSpan);
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	/*
	 * NOTE: Below function calls are for COSMETIC effects, MEANING, they will NOT replicate down to client if on Server.
	 * This why there is a chance that server overlaps BEFORE the client and calls Destroy() BEFORE the client can
	 * play the effects.
	 */
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	AudioComponent->Stop();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = Cast<IAbilitySystemInterface>(OtherActor)->GetAbilitySystemComponent())
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
		}

		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AAuraProjectile::PlayLoopingSound() const
{
	if (AudioComponent)
	{
		AudioComponent->SetSound(LoopingSound);
		AudioComponent->Play();
	}
}

void AAuraProjectile::Destroyed()
{
	/*
	 * NOTE: If the server calls Destroy() before the client can play the effects, then when the client receives the instruction to Destroy()
	 * , we will play the effects if the Hit had not been registered on the client at first.
	 */
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		AudioComponent->Stop();
	}

	Super::Destroyed();
}
