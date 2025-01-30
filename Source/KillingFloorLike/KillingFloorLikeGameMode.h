// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KillingFloorLikeGameMode.generated.h"

enum class EMonsterType : uint8;
class AUnitManager;

UENUM(BlueprintType)
enum class EModeType : uint8
{
	None,
	Wave,
	Break,
	End
};

UCLASS(minimalapi)
class AKillingFloorLikeGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

public:
	AKillingFloorLikeGameMode();

	/*UPROPERTY(EditDefaultsOnly)
	TScriptInterface<class UBaseGameConditionInterface> Wave;
	class UBaseGameConditionInterface* CurrentGameCondition;*/

	virtual void Tick(float DeltaSeconds) override;

private:
	int32 MaxWaveIndex = 10;
	int32 CurrentWave = 0;

	float MaxWaveTime = 3;
	float WaveDelayTime = 0;

	int32 MaxSpawnedMonsters;

	float MaxBreakTime = 10;
	float BreakTime = 0;

	EModeType CurrentModeType = EModeType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetUnitManager)
	AUnitManager* UnitManager;

	UPROPERTY(EditAnywhere)
	TMap<EMonsterType, int32> MonsterPool;

	UFUNCTION(BlueprintCallable)
	void StartWave();
	void SpawnMonster();
	void EndWave(bool IsWin);

	void StartBreak();
	void EndBreak();

	void EndMatch(bool IsWin);

	void ChangeModeType(EModeType NewModeType);

	// Check and Refill Pool Function
	void RefillMonsterPool();

public:
	UFUNCTION(BlueprintGetter)
	AUnitManager* GetUnitManager();

	UFUNCTION(BlueprintCallable)
	int32 GetWaveRemainMonsterCount();
};
