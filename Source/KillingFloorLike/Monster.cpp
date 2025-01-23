// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
AMonster::AMonster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentUnitType = EUnitType::Enemy;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonster::Dead(AActor* DamageCauser)
{
	Super::Dead(DamageCauser);

	InitialLifeSpan = 3.0f;
}
