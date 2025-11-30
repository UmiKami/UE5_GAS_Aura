// Copyright By UmiKami


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"


AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBarComponent->SetupAttachment(RootComponent);
	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	checkf(AbilitySystemComponent, TEXT("AbilitySystem Component is NULL on AuraEnemy"))

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	SetupAbilitySystemAndAttributeSet();
	BindCallbacksToDependencies();
	BroadcastInitialValuesToUI();
	UAuraAbilitySystemFunctionLibrary::GiveStartUpAbilities(this, AbilitySystemComponent);
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
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	
	Super::Die();
}

void AAuraEnemy::SetupAbilitySystemAndAttributeSet()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemFunctionLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

/**
 * Private function only meant to send values to the Enemy Health Bar Widget.
 */
void AAuraEnemy::BroadcastInitialValuesToUI() const
{
	if (const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		HealthSignature.Broadcast(AuraAttributeSet->GetHealth());
		MaxHealthSignature.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::OnHitReactTagChanged(FGameplayTag GameplayTag, int NewCount)
{
	bHitReacting = NewCount > 0;

	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::BindCallbacksToDependencies()
{
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBarComponent->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).
		                        AddLambda(
			                        [this, AuraAttributeSet](const FOnAttributeChangeData& Data)
			                        {
				                        HealthSignature.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
		                        AddLambda(
			                        [this, AuraAttributeSet](const FOnAttributeChangeData& Data)
			                        {
				                        MaxHealthSignature.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags::Effects_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved)
		                      .AddUObject(this, &AAuraEnemy::OnHitReactTagChanged);
	}
}
