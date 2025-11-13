// Copyright By UmiKami


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

namespace AuraGameplayTags
{
	// region Primary Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Primary_Strength,
		"Attributes.Primary.Strength",
		"Increases physical damage."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Primary_Intelligence,
		"Attributes.Primary.Intelligence",
		"Increases magical damage."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Primary_Resilience,
		"Attributes.Primary.Resilience",
		"Increases Armor and Armor Penetration."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Primary_Vigor,
		"Attributes.Primary.Vigor",
		"Increases Max Health"
	);
	// endregion 

	// region Secondary Attributes
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_Armor,
		"Attributes.Secondary.Armor",
		"Decreases damage taken and increases chance of hit being blocked."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_ArmorPenetration,
		"Attributes.Secondary.ArmorPenetration",
		"Ignores percentage of enemy Armor, increases Crit Hit Chance."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_BlockChance,
		"Attributes.Secondary.BlockChance",
		"Chance to cut incoming damage in half."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_CriticalHitChance,
		"Attributes.Secondary.CriticalHitChance",
		"Chance to double damage plus critical hit bonus."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_CriticalHitDamage,
		"Attributes.Secondary.CriticalHitDamage",
		"Bonus damage added when a critical hit is scored."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_CriticalHitResistance,
		"Attributes.Secondary.CriticalHitResistance",
		"Reduces critical hit chance of attacking enemies."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_HealthRegeneration,
		"Attributes.Secondary.HealthRegeneration",
		"Amount of Health regenerated every 1 second."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_ManaRegeneration,
		"Attributes.Secondary.ManaRegeneration",
		"Amount of Mana regenerated every 1 second."
	);

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_MaxHealth,
		"Attributes.Secondary.MaxHealth",
		"Maximum amount of Health obtainable."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Attributes_Secondary_MaxMana,
		"Attributes.Secondary.MaxMana",
		"Maximum amount of Health obtainable."
	);
	// endregion

	// region Input Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		InputTag_LMB,
		"InputTag.LMB",
		"Left mouse button."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		InputTag_RMB,
		"InputTag.RMB",
		"Right mouse button."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		InputTag_1,
		"InputTag.1",
		"Input tag for 1 key."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		InputTag_2,
		"InputTag.2",
		"Input tag for 2 key."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		InputTag_3,
		"InputTag.3",
		"Input tag for 3 key."
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		InputTag_4,
		"InputTag.4",
		"Input tag for 4 key."
	);
	// endregion

	// region Ability Event Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Event_Montage_FireBolt,
		"Event.Montage.FireBolt",
		"Event to launch a firebolt from GA"
	);
	// endregion
}
