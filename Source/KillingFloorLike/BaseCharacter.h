// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	None,
	Idle,
	Walk,
	Run,
	Dead
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
	None,
	Ally,
	Enemy,
	Neutral
};

UCLASS()
class KILLINGFLOORLIKE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere)
	float BaseAttackDamage = 10;
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetCurrentUnitState)
	EUnitState CurrentUnitState = EUnitState::None;

	UFUNCTION(BlueprintCallable)
	void Attack();

	const float HeadshotScale = 1.1f;

protected:
	float MaxHp = 100;
	UPROPERTY(BlueprintGetter = GetCurrentHP)
	float CurrentHp;
	
	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float RunSpeed;

	UPROPERTY(VisibleAnywhere)
	EUnitType CurrentUnitType = EUnitType::None;

	virtual void Dead(AActor* DamageCauser);

public:
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintGetter)
	float GetCurrentHP();

	UFUNCTION(BlueprintCallable)
	void ChangeUnitState(EUnitState NewUnitState);

	UFUNCTION(BlueprintGetter)
	EUnitState GetCurrentUnitState();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsAlive();
	
	EUnitType GetCurrentUnitType();
	bool IsAttackableUnitType(ABaseCharacter* AttackedUnit);
};
