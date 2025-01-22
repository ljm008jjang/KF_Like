// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Monster.generated.h"

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

protected:
	virtual void Dead(AActor* DamageCauser) override;
};
