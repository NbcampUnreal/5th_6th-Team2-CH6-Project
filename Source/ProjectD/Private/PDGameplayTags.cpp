#include "PDGameplayTags.h"

namespace PDGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Slot1, "InputTag.Equip.Slot1");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Slot2, "InputTag.Equip.Slot2");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Slot3, "InputTag.Equip.Slot3");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Equip_Slot4, "InputTag.Equip.Slot4");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip, "InputTag.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Fire, "InputTag.Weapon.Fire");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_Reload, "InputTag.Weapon.Reload");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Weapon_ADS, "InputTag.Weapon.ADS");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_Skill1, "InputTag.Ability.Skill1");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Ability_Skill2, "InputTag.Ability.Skill2");

	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip, "Player.Ability.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip, "Player.Ability.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Fire, "Player.Ability.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Reload, "Player.Ability.Reload");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_ADS, "Player.Ability.ADS");

	/** Event Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Event_Weapon_EquipRequest, "Event.Weapon.EquipRequest");
	UE_DEFINE_GAMEPLAY_TAG(Event_Weapon_EquipConfirm, "Event.Weapon.EquipConfirm");

	/** SetByCaller Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Data_Weapon_Damage, "Data.Weapon.Damage");
}