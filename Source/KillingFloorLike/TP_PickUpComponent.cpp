// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}


void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AKillingFloorLikeCharacter* Character = Cast<AKillingFloorLikeCharacter>(OtherActor);
	if (Character != nullptr && (Character->GetWeapon(
		GetOwner()->GetComponentByClass<UTP_WeaponComponent>()->GetWeaponType()) == nullptr))
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}

void UTP_PickUpComponent::AfterDrop()
{
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}
