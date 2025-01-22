// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KillingFloorLikeGameMode.generated.h"

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
	int MaxWaveIndex = 10;
	int WaveIndex = 0;

	float MaxWaveTime = 10;
	float WaveDelayTime = 0;

	int MaxMonsterSpawnCount = 1;
	//몬스터 스폰 횟수.
	int MonsterSpawnCount = 0;

	float MaxBreakTime = 10;
	float BreakTime = 0;

	EModeType CurrentModeType = EModeType::None;

	UPROPERTY(EditDefaultsOnly)
	AUnitManager* UnitManager;

	UFUNCTION(BlueprintCallable)
	void StartWave();
	void SpawnMonster();
	void EndWave(bool IsWin);

	void StartBreak();
	void EndBreak();

	void EndMatch(bool IsWin);

	void ChangeModeType(EModeType NewModeType);
};
