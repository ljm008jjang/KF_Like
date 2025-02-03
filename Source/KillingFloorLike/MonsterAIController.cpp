// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

/*AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	// Sight Config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 800.f;
	SightConfig->LoseSightRadius = 1200.f; 
	SightConfig->PeripheralVisionAngleDegrees = 60.f; 
	SightConfig->SetMaxAge(5.f); 
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

	// Detect only specific actors 
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
    
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 500.f;
	HearingConfig->SetMaxAge(3.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerception->ConfigureSense(*HearingConfig);
	// ...
    
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::PerceptionUpdated);
	출처: https://dlaiml.tistory.com/entry/UE5-AI-Perception-C [Deeper Learning:티스토리]
}*/

void AMonsterAIController::OnDead()
{
	Destroy();
}
