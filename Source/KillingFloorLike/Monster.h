// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MonsterSkill.h"
#include "Monster.generated.h"

/*UENUM(BlueprintType)
enum class EHitBoxType : uint8
{
	None,
	Head,
	L
};*/

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	None,
	Clot,
	Gorefast,
	Stalker,
	Crawler,
	Husk,
	Scrake,
	Fleshpound,
	Siren,
	Bloat
};


UENUM(BlueprintType)
enum class EMonsterAnimationType : uint8
{
	None,
	Idle,
	Walk,
	Run,
	Attack,
	Hit,
	Skill
};

USTRUCT(BlueprintType)
struct FMonsterMontages
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> Montages;
};

UENUM(BlueprintType)
enum class EMonsterSoundType : uint8
{
	None,
	Idle,
	Walk,
	Run,
	Attack,
	Hit,
	Dead
};


UCLASS()
class KILLINGFLOORLIKE_API AMonster : public ABaseCharacter//, public IMonsterSkill
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere)
	TMap<EMonsterAnimationType, FMonsterMontages> AnimationMap;

	UPROPERTY(EditAnywhere)
	TMap<EMonsterSoundType, USoundBase*> SoundMap;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMonsterType)
	EMonsterType MonsterType;

	UPROPERTY(EditAnywhere, Category="Stat")
	float MaxHeadHP;
	UPROPERTY(EditAnywhere, Category="Stat")
	float CurrentHeadHP;

protected:
	virtual void Dead(AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAnimationMontage(EMonsterAnimationType animType, int32 index);

	UFUNCTION(BlueprintCallable)
	int32 GetAnimationMaxIndex(EMonsterAnimationType animType);

		
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetHitAnimMontage(AActor* DamageCauser);

	UFUNCTION(BlueprintCallable)
	USoundBase* GetSoundBase(EMonsterSoundType SoundType);


	UFUNCTION(BlueprintGetter)
	EMonsterType GetMonsterType();

	/*
	UFUNCTION(BlueprintSetter)
	void SetMonsterType(EMonsterType NewMonsterType);*/

	/*UFUNCTION(BlueprintCallable)
	virtual void DoSkill() override;*/
};
