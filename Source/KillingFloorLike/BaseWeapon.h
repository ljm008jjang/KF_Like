// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Main,
	Sub,
	Knife
};

UENUM(BlueprintType)
enum class EAnimationType : uint8
{
	None,
	Fire,
	Select,
	PutDown
};

UCLASS()
class KILLINGFLOORLIKE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(class AKillingFloorLikeCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void DropWeapon(class AKillingFloorLikeCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	TSubclassOf<class UAnimInstance> GetAnimInstance();

	USkeletalMesh* GetSkeletalMesh();

	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAnimation(EAnimationType AnimationType);

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AKillingFloorLikeProjectile> ProjectileClass;

	/** The Character holding this weapon*/
	class AKillingFloorLikeCharacter* Character;

	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::Main;

	UPROPERTY(EditAnywhere, Category="Combat")
	float GunDamage = 10;

	UPROPERTY(EditAnywhere)
	TMap<EAnimationType, class UAnimMontage*> AnimationMap;
};
