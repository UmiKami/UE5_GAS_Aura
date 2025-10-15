// Copyright By UmiKami


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// NOTE: GetVigorAttribute method is provided by the ATTRIBUTE_ACCESSORS macro on UAuraAttributeSet, it is not literally defined on UAuraAttributeSet
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);

	Vigor = FMath::Max<float>(Vigor, 0.f);

	TScriptInterface<ICombatInterface> CombatInterface = Spec.GetContext().GetSourceObject();

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
