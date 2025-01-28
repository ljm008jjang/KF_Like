// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Monster.generated.h"

/*UENUM(BlueprintType)
enum class EHitBoxType : uint8
{
	None,
	Head,
	L
};*/


UENUM(BlueprintType)
enum class EMonsterAnimationType : uint8
{
	None,
	Idle,
	Walk,
	Run,
	Attack
};

USTRUCT(BlueprintType)
struct FMonsterMontages
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> Montages;
};


UCLASS()
class KILLINGFLOORLIKE_API AMonster : public ABaseCharacter
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

private:
	UPROPERTY(EditAnywhere)
	TMap<EMonsterAnimationType, FMonsterMontages> AnimationMap;

protected:
	virtual void Dead(AActor* DamageCauser) override;
};
