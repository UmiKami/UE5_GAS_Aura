// Copyright By UmiKami


#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"


UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}

	return OverlayWidgetController;
}


/**
 * @brief Initializes Overlay Widget with required parameters. All four parameters are required to extract the data needed to display on overlay. The overlay controller is attached to the overlay widget before it is displayed.
 * @param PC The Player Controller.
 * @param PS The Player State.
 * @param ASC The Ability System Component.
 * @param AS The Attribute Set.
 * @return
 */
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class Uninitialized, please fill out BP_AuraHUD"))
	checkf(OverlayWidgetControllerClass,
	       TEXT("Overlay Widget CONTROLLER Class Uninitialized, please fill out BP_AuraHUD"))

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(
		PC,
		PS,
		ASC,
		AS
	);

	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	// SetWidgetController triggers a blueprint event, only after that we can trigger the BroadcastInitiailValues
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}
