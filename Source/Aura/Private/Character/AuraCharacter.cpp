// Copyright By UmiKami


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

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
	SetupOverlayWidget();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// init ability actor info for the Client

	SetupAbilitySystemAndAttributeSet();
	SetupOverlayWidget();
}

void AAuraCharacter::SetupAbilitySystemAndAttributeSet()
{
	const TObjectPtr<AAuraPlayerState> CurrentPlayerState = GetPlayerState<AAuraPlayerState>();

	checkf(CurrentPlayerState, TEXT("CurrentPlayerState is NULL on AAuraCharacter"));

	CurrentPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CurrentPlayerState, this);
	AbilitySystemComponent = CurrentPlayerState->GetAbilitySystemComponent();
	AttributeSet = CurrentPlayerState->GetAttributeSet();
}

void AAuraCharacter::SetupOverlayWidget()
{
	const TObjectPtr<AAuraPlayerState> CurrentPlayerState = GetPlayerState<AAuraPlayerState>();

	checkf(CurrentPlayerState, TEXT("CurrentPlayerState is NULL on AAuraCharacter"));

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		/*
		 * The reason why we are checking nullptr with if statement is because AuraHUD is only valid for local player.
		 * This function will also be run on the server side, in which the HUD of other players will be NULL for the local player.
		 */
		if (const TObjectPtr<AAuraHUD> AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(
				AuraPlayerController,
				CurrentPlayerState,
				CurrentPlayerState->GetAbilitySystemComponent(),
				CurrentPlayerState->GetAttributeSet()
			);
		}
	}
}
