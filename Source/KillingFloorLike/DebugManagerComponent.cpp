// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugManagerComponent.h"

// Sets default values for this component's properties
UDebugManagerComponent::UDebugManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDebugManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDebugManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDebugManagerComponent::DrawDebugLineManager(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd,
	FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	if(IsDebugActive == false)
	{
		return;
	}
	
	DrawDebugLine(InWorld,LineStart,LineEnd,Color,bPersistentLines,LifeTime,DepthPriority,Thickness);
}

