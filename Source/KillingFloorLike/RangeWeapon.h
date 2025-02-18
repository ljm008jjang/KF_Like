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
	virtual bool FireWeapon(float AttackDamage) override;

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void ReloadAmmoAdd(int ReloadedAmmo);

	UFUNCTION(BlueprintCallable)
	bool IsReloadable();

	UFUNCTION(BlueprintCallable)
	FString GetAmmoText();

	void GetAmmo(int GetAmmoAmount);

	virtual bool IsAttackable() override;

	UFUNCTION(BlueprintCallable, BlueprintGetter)
	int32 GetMaxLoadedAmmon();

private:
	int32 CurrentLoadedAmmo;
	int32 SavedAmmo;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetMaxLoadedAmmon, Category="Stat")
	int32 MaxLoadedAmmo;
	UPROPERTY(EditAnywhere, Category="Stat")
	int32 MaxSavedAmmo;
};
