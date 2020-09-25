// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ACM_AttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ARKDECM_API UACM_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	//Functions
	UACM_AttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAtribute, const FGameplayAttributeData& MaxAtribute, float NewMaxValue, const FGameplayAttribute& AffectedAtributeProperty);

public:

	//Attributes

	//Health
	UPROPERTY(BlueprintReadOnly, Category = "Health")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, HealthRegen);

	//Mana
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
		FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, ManaRegen);

	//Mana
	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UACM_AttributeSet, StaminaRegen);


	
};
