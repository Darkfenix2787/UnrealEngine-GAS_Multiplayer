// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include <Runtime\Engine\Public\Net\UnrealNetwork.h>

//===========================================================================================================================================================
UACM_AttributeSet::UACM_AttributeSet() 
{
	MaxHealth = 100.f;
	Health = MaxHealth;
	HealthRegen = 0.5f;

	MaxMana = 100.f;
	Mana = MaxMana;
	ManaRegen = 2.f;

	MaxStamina = 100.f;
	Stamina = MaxStamina;
	StaminaRegen = 1.f;
}

//===========================================================================================================================================================
void UACM_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}	
}

//===========================================================================================================================================================
void UACM_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UACM_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UACM_AttributeSet, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));				
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UACM_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UACM_AttributeSet, Mana)))
	{
		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.f, MaxMana.GetCurrentValue()));		
	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UACM_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UACM_AttributeSet, Stamina)))
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.f, MaxStamina.GetCurrentValue()));		
	}
}

//===========================================================================================================================================================
void UACM_AttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAtribute, const FGameplayAttributeData& MaxAtribute, float NewMaxValue, const FGameplayAttribute& AffectedAtributeProperty)
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAtribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && IsValid(AbilityComponent))
	{
		const float CurrentValue = AffectedAtribute.GetCurrentValue();
		float NewDelta = CurrentMaxValue > 0.f ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComponent->ApplyModToAttributeUnsafe(AffectedAtributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UACM_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, Health, OldHealth);
}

void UACM_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, MaxHealth, OldMaxHealth);
}

void UACM_AttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, HealthRegen, OldHealthRegen);
}

void UACM_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, Mana, OldMana);
}

void UACM_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, MaxMana, OldMaxMana);
}

void UACM_AttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, ManaRegen, OldManaRegen);
}

void UACM_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, Stamina, OldStamina);
}

void UACM_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, MaxStamina, OldMaxStamina);
}

void UACM_AttributeSet::OnRep_StaminaRegen(const FGameplayAttributeData& OldStaminaRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UACM_AttributeSet, StaminaRegen, OldStaminaRegen);
}

void UACM_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UACM_AttributeSet, StaminaRegen, COND_None, REPNOTIFY_Always);
}
