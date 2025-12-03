// Copyright By UmiKami


#include "ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemFunctionLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

struct AuraDamageStatics
{
	// DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)

	AuraDamageStatics()
	{
		// DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;

	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceAsc = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAsc = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceAsc ? SourceAsc->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetAsc ? TargetAsc->GetAvatarActor() : nullptr;
	
	TScriptInterface<ICombatInterface> SourceCombatInterface = SourceAvatar;
	TScriptInterface<ICombatInterface> TargetCombatInterface = TargetAvatar;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;

	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Get Damage Set By Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(AuraGameplayTags::Damage);

	const UAuraAttributeSet* TargetAttributes = Cast<UAuraAttributeSet>(
		TargetAsc->GetAttributeSet(UAuraAttributeSet::StaticClass()));
	const UAuraAttributeSet* SourceAttributes = Cast<UAuraAttributeSet>(
		SourceAsc->GetAttributeSet(UAuraAttributeSet::StaticClass()));

	// Capture block chance on target and determine if there was a successful block
	const float BlockChance = FMath::Max<float>(TargetAttributes->GetBlockChance(), 0.f) / 100.f;
	const bool bBlockSuccessful = UKismetMathLibrary::RandomBoolWithWeight(BlockChance);

	// DECREASES damage by half IF successful
	Damage /= bBlockSuccessful ? 2.f : 1.f;

	// Do all calculations after block chance
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemFunctionLibrary::GetCharacterClassInfo(SourceAvatar);
	
	// region Armor & Armor Penetration Based Damage Calc
	const float TargetArmor = TargetAttributes->GetArmor();
	
	// Armor penetration ignores a percentage of the target's armor
	const float SourceArmorPenetration = SourceAttributes->GetArmorPenetration();
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	// INCREASES damage based on Source ArmorPenetration AND TargetArmor; Assumes absurd ArmorPenetration values as players levels up and trys to balance it by making it less effective the higher the level
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	// DECREASES damage based on Coefficient that scales based on level; EffectiveArmorCoefficient lowers the effectiveness of the armor the higher the level to mitigate absurdly high numbers 
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	// endregion
	
	// region Critical Hit Damage calculation
	const float SourceCriticalHitChance = FMath::Max(0.f, SourceAttributes->GetCriticalHitChance()) / 100.f;
	const float SourceCriticalHitDamage = SourceAttributes->GetCriticalHitDamage();
	const float TargetCriticalHitResistance =  FMath::Max(0.f,TargetAttributes->GetCriticalHitResistance()) / 100.f;
	const float EffectiveCriticalHitChance = SourceCriticalHitChance * (1 - TargetCriticalHitResistance);
	
	const bool bCriticalHitSuccessful = UKismetMathLibrary::RandomBoolWithWeight(EffectiveCriticalHitChance);
	
	Damage = bCriticalHitSuccessful ? Damage * 2 + SourceCriticalHitDamage : Damage;
	// endregion
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
