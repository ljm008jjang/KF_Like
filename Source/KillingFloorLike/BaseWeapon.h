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
enum class EWeaponAnimationType : uint8
{
	None,
	Fire,
	Select,
	PutDown,
	Reload
};

UENUM(BlueprintType)
enum class EAimType : uint8
{
	None,
	Normal,
	Iron
};


USTRUCT(BlueprintType)
struct FAttackMontages
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> Montages;
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> IronMontages;
};

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
	virtual void Fire();

	TSubclassOf<class UAnimInstance> GetAnimInstance();

	USkeletalMesh* GetSkeletalMesh();

	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAnimation(EWeaponAnimationType AnimationType, bool IsIron, int index);

	UFUNCTION(BlueprintCallable)
	bool IsAttackable();

	UFUNCTION(BlueprintCallable)
	void SetCurrentAttackCooltime();

	UFUNCTION(BlueprintCallable)
	int32 GetAnimationMaxIndex(EWeaponAnimationType type);

	UFUNCTION(BlueprintCallable)
	EAimType GetCurrentAimType();

	UFUNCTION(BlueprintCallable)
	bool IsAimTypeChangeable();

	UFUNCTION(BlueprintCallable)
	void SetCurrentAimType(EAimType NewAimType);

	//normal -> index = 0
	//Hit 사운드는 0 = 안맞음
	// 1 = 무기체에 맞음
	// 2 = 몬스터에 맞음
	UFUNCTION(BlueprintCallable)
	USoundBase* GetSoundBase(EWeaponSoundType SoundType, int32 index);

protected:
	/** The Character holding this weapon*/
	class AKillingFloorLikeCharacter* Character;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AKillingFloorLikeProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	float FireDamage = 10;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;


	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::Main;

	UPROPERTY(EditDefaultsOnly)
	EAimType CurrentAimType = EAimType::Normal;

	//true 시 공격 쿨타임이 애니메이션과 동기화 ex)Knife
	UPROPERTY(EditDefaultsOnly)
	bool IsAsyncAttackCooltimeWithAnimation = true;
	UPROPERTY(EditAnywhere)
	float MaxAttackCooltime;
	UPROPERTY(EditAnywhere)
	float CurrentAttackCooltime;

	UPROPERTY(EditAnywhere)
	TMap<EWeaponAnimationType, FAttackMontages> AnimationMap;

	UPROPERTY(EditAnywhere)
	TMap<EWeaponSoundType, FWeaponSoundBases> SoundMap;

	void SetAttackCooltime(UAnimMontage* PlayedAnimMontage);
};
