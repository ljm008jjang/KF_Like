// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponShootingInterface.h"
#include "Components/SceneComponent.h"
#include "MeleeShootingComponent.generated.h"



UENUM(BlueprintType)
enum class EWeaponSoundType : uint8
{
	None,
	Fire,
	Hit,
	Select,
	PutDown,
	Reload
};

USTRUCT(BlueprintType)
struct FWeaponSoundBases
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> SoundBases;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KILLINGFLOORLIKE_API UMeleeShootingComponent : public USceneComponent, public IWeaponShootingInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeleeShootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere)
	TMap<EWeaponSoundType, FWeaponSoundBases> SoundMap;
	
	UPROPERTY(EditAnywhere)
	float AttackRange = 200;
	
public:
	virtual bool Fire(AKillingFloorLikeCharacter* Character, float AttackDamage) override;
	
	//normal -> index = 0
	//Hit 사운드는 0 = 안맞음
	// 1 = 무기체에 맞음
	// 2 = 몬스터에 맞음
	UFUNCTION(BlueprintCallable)
	USoundBase* GetSoundBase(EWeaponSoundType SoundType, int32 index);
		
};
