// Copyright By UmiKami

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::SetupAbilitySystemAndAttributeSet()
{
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

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	checkf(DefaultPrimaryAttributes, TEXT("Default primary attributes are not set for character."))
	checkf(DefaultSecondaryAttributes, TEXT("Default secondary attributes are not set for character."))

	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
	ApplyEffectToSelf(DefaultVitalAttributes);
}
