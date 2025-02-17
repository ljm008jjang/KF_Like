// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class KILLINGFLOORLIKE_API ARangeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Fire(float AttackDamage) override;

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	bool IsReloadable();

	UFUNCTION(BlueprintCallable)
	FString GetAmmoText();

	void GetAmmo(int GetAmmoAmount);
	
	virtual bool IsAttackable() override;

private:
	int32 CurrentLoadedAmmo;
	int32 SavedAmmo;

	UPROPERTY(EditAnywhere, Category="Stat")
	int32 MaxLoadedAmmo;
	UPROPERTY(EditAnywhere, Category="Stat")
	int32 MaxSavedAmmo;
};
