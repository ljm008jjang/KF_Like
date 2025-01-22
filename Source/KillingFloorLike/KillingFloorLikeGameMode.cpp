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

	WaveIndex = 0;
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
		if (MonsterSpawnCount <= 0)
		{
			if (UnitManager->GetAliveMonsterCount() <= 0)
			{
				EndWave(true);
			}
			return;
		}

		WaveDelayTime -= DeltaSeconds;
		if (WaveDelayTime <= 0)
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
	MonsterSpawnCount = MaxMonsterSpawnCount;
	WaveDelayTime = 0;
	UnitManager->ClearUnitDB();
	ChangeModeType(EModeType::Wave);
	UE_LOG(LogTemp, Warning, TEXT("Start Wave"))
}

void AKillingFloorLikeGameMode::SpawnMonster()
{
	MonsterSpawnCount--;
	UnitManager->SpawnMonster();
}

void AKillingFloorLikeGameMode::EndWave(bool IsWin)
{
	if (IsWin)
	{
		WaveIndex++;
		if (WaveIndex > MaxWaveIndex)
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
