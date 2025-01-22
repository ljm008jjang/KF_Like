// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void AMonsterAIController::OnDead()
{
	Destroy();
}
