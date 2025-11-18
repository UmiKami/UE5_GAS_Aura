// Copyright By UmiKami


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
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

	SetupAbilitySystemAndAttributeSet();
	BindCallbacksToChangesOnHealth();
	BroadcastInitialValues();
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

void AAuraEnemy::SetupAbilitySystemAndAttributeSet()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AAuraEnemy::BroadcastInitialValues() const
{
	if (const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		HealthSignature.Broadcast(AuraAttributeSet->GetHealth());
		MaxHealthSignature.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::BindCallbacksToChangesOnHealth()
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
	}
}
