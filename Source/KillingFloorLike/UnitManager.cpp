// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitManager.h"

#include "Monster.h"

class AMonster;
// Sets default values
AUnitManager::AUnitManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUnitManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUnitManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitManager::SpawnMonster()
{
	for (AActor* Point : SpawnPoints)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AMonster* SpawnAnimal = GetWorld()->SpawnActor<AMonster>(MonsterClass[EMonsterType::Clot],
		                                                         Point->GetActorLocation(),
		                                                         Point->GetActorRotation(), params);
		SpawnAnimal->SpawnDefaultController();
		Monsters.Add(SpawnAnimal);
	}
}

//TODO 매 프레임 체크해야하는 것 같은데?
int AUnitManager::GetAliveMonsterCount()
{
	int Result = 0;
	for (AMonster* Monster : Monsters)
	{
		if (Monster->GetCurrentUnitState() == EUnitState::Alive)
		{
			Result++;
		}
	}
	return Result;
}

void AUnitManager::ClearUnitDB()
{
	Monsters.Empty();
}
