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
	
public:
	virtual void Fire() override;
	
};
