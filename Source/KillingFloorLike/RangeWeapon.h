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
	virtual void Fire() override;

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	bool IsReloadable();

	UFUNCTION(BlueprintCallable)
	FString GetAmmoText();

private:
	int32 CurrentLoadedAmmo;
	int32 SavedAmmo;

	UPROPERTY(EditAnywhere)
	int32 MaxLoadedAmmo = 30;
};
