// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitManager.generated.h"

UCLASS()
class KILLINGFLOORLIKE_API AUnitManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonster> MonsterClass;

	UPROPERTY(VisibleAnywhere)
	TArray<AMonster*> Monsters;

public:
	void SpawnMonster();

	int GetAliveMonsterCount();
	
	void ClearUnitDB();
};
