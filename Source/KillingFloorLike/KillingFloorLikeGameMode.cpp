// Copyright Epic Games, Inc. All Rights Reserved.

#include "KillingFloorLikeGameMode.h"
#include "KillingFloorLikeCharacter.h"
#include "Monster.h"
#include "SpawnPoint.h"
#include "UnitManager.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

void AKillingFloorLikeGameMode::BeginPlay()
{
	Super::BeginPlay();
}

AKillingFloorLikeGameMode::AKillingFloorLikeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	UnitManager = Cast<AUnitManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUnitManager::StaticClass()));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	PrimaryActorTick.bCanEverTick = true;

	CurrentWave = 0;
	MaxSpawnedMonsters = 16;
}

void AKillingFloorLikeGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentModeType == EModeType::None)
	{
		StartWave();
		return;
	}

	if (CurrentModeType == EModeType::Wave)
	{
		if (MonsterPool.Num() <= 0)
		{
			if (UnitManager->GetAliveMonsterCount() <= 0)
			{
				EndWave(true);
			}
			return;
		}

		WaveDelayTime -= DeltaSeconds;
		if (UnitManager->GetAliveMonsterCount() <= 0 || WaveDelayTime <= 0)
		{
			SpawnMonster();
			WaveDelayTime = MaxWaveTime;
		}
	}

	if (CurrentModeType == EModeType::Break)
	{
		BreakTime -= DeltaSeconds;
		if (BreakTime <= 0)
		{
			EndBreak();
			return;
		}
	}
}

void AKillingFloorLikeGameMode::StartWave()
{
	CurrentWave++;
	WaveDelayTime = 0;
	UnitManager->ClearUnitDB();
	RefillMonsterPool();
	ChangeModeType(EModeType::Wave);
	UE_LOG(LogTemp, Warning, TEXT("Start Wave"))
}

void AKillingFloorLikeGameMode::SpawnMonster()
{
	if (MonsterPool.Num() == 0)
	{
		return;
	}

	// Select Monster Type from Pool
	TArray<EMonsterType> Keys;
	MonsterPool.GetKeys(Keys);
	EMonsterType SelectedMonster = Keys[FMath::RandHelper(Keys.Num())];

	if (MonsterPool[SelectedMonster] <= 0)
	{
		MonsterPool.Remove(SelectedMonster);
		return;
	}

	// Find Spawn Point
	AActor* SpawnPoint = UnitManager->GetSpawnPoints()[FMath::RandHelper(UnitManager->GetSpawnPoints().Num())];


	// Spawn the Monster
	//GetWorld()->SpawnActor<AActor>(MonsterClasses[SelectedMonster], SpawnPoint->GetActorLocation(), SpawnPoint->GetActorRotation());
	UnitManager->SpawnMonster(SelectedMonster);
	MonsterPool[SelectedMonster]--;
}

void AKillingFloorLikeGameMode::EndWave(bool IsWin)
{
	if (IsWin)
	{
		if (CurrentWave > MaxWaveIndex)
		{
			EndMatch(true);
			return;
		}

		StartBreak();
	}
	else
	{
		EndMatch(false);
	}
}

void AKillingFloorLikeGameMode::StartBreak()
{
	BreakTime = MaxBreakTime;
	UE_LOG(LogTemp, Warning, TEXT("Start Break"))
	ChangeModeType(EModeType::Break);
}

void AKillingFloorLikeGameMode::EndBreak()
{
	StartWave();
}

void AKillingFloorLikeGameMode::EndMatch(bool IsWin)
{
	UE_LOG(LogTemp, Warning, TEXT("End Match : %hhd"), IsWin);
	ChangeModeType(EModeType::End);
}

void AKillingFloorLikeGameMode::ChangeModeType(EModeType NewModeType)
{
	CurrentModeType = NewModeType;
}

void AKillingFloorLikeGameMode::RefillMonsterPool()
{
	// Adjust Monster Counts Based on Current Wave
	MonsterPool.Add(EMonsterType::Clot, 0);
	MonsterPool.Add(EMonsterType::Gorefast, 0);
	MonsterPool[EMonsterType::Clot] = FMath::RoundToInt(CurrentWave * 2.0f);
	MonsterPool[EMonsterType::Gorefast] = FMath::RoundToInt(CurrentWave * 1.5f);

	if (CurrentWave >= 5)
	{
		MonsterPool.Add(EMonsterType::Scrake, FMath::RoundToInt(CurrentWave * 0.5f));
	}
	if (CurrentWave >= 7)
	{
		MonsterPool.Add(EMonsterType::Fleshpound, FMath::RoundToInt(CurrentWave * 0.3f));
	}
}
