// Copyright By UmiKami

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	USkeletalMeshComponent* MeshComponent = GetMesh();

	MeshComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	MeshComponent->SetCollisionResponseToChannel(ECC_PROJECTILE, ECR_Overlap);
	MeshComponent->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(MeshComponent, "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	// If we call MulticastHandleDeath_Implementation, it will execute only on Server side, character will die but ragdoll physics will not be visible on client. 
	MulticastHandleDeath();
}

/**
 * Whatever happens on this function for death, it is replicated to all other clients including the server.
 */
void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	const TObjectPtr<USkeletalMeshComponent> MeshComponent = GetMesh();

	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetEnableGravity(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::SetFacingWarpTarget_Implementation(const FVector& TargetLocation, const FName WarpTargetName)
{
	if (UMotionWarpingComponent* MotionWarping = FindComponentByClass<UMotionWarpingComponent>())
	{
		MotionWarping->AddOrUpdateWarpTargetFromLocation(WarpTargetName, TargetLocation);
	}
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::SetupAbilitySystemAndAttributeSet()
{
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	checkf(DefaultPrimaryAttributes, TEXT("Default primary attributes are not set for character."))
	checkf(DefaultSecondaryAttributes, TEXT("Default secondary attributes are not set for character."))

	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
	ApplyEffectToSelf(DefaultVitalAttributes);
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority())
	{
		return;
	}

	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	AuraASC->AddCharacterAbilities(StartUpAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeLine(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeLine(DynamicMatInst);
	}
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> InAttributes, const float Level) const
{
	UAbilitySystemComponent* TargetASC = GetAbilitySystemComponent();

	checkf(TargetASC, TEXT("Ability System Component is not valid for character base."))
	checkf(InAttributes, TEXT("Provided attributes are not set for character."))

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		InAttributes,
		Level,
		EffectContextHandle
	);

	TargetASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, TargetASC);
}
