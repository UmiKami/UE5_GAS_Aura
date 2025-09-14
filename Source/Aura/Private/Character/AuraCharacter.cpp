// Copyright By UmiKami


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");

	SpringArm->SetupAttachment(GetRootComponent());
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}


void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// init ability actor info for the Server

	SetupAbilitySystemAndAttributeSet();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// init ability actor info for the Client

	SetupAbilitySystemAndAttributeSet();
}

void AAuraCharacter::SetupAbilitySystemAndAttributeSet()
{
	const TObjectPtr<AAuraPlayerState> CurrentPlayerState = GetPlayerState<AAuraPlayerState>();

	checkf(CurrentPlayerState, TEXT("CurrentPlayerState is NULL on AAuraCharacter"));

	CurrentPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CurrentPlayerState, this);
	AbilitySystemComponent = CurrentPlayerState->GetAbilitySystemComponent();
	AttributeSet = CurrentPlayerState->GetAttributeSet();
}