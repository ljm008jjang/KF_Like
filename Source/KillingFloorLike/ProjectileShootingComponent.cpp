// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileShootingComponent.h"

#include "KillingFloorLikeCharacter.h"
#include "KillingFloorLikeProjectile.h"

class AKillingFloorLikeCharacter;
// Sets default values for this component's properties
UProjectileShootingComponent::UProjectileShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UProjectileShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UProjectileShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UProjectileShootingComponent::Fire(AKillingFloorLikeCharacter* Character, float AttackDamage)
{
	/*if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}*/

	/*if (CurrentLoadedAmmo <= 0)
	{
		return;
	}*/

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			FRotator SpawnRotation; // = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			FVector SpawnLocation;
			// = Character->GetController()->GetPlayerViewPoint() + SpawnRotation.RotateVector(MuzzleOffset);

			PlayerController->GetPlayerViewPoint(SpawnLocation, SpawnRotation);
			SpawnLocation += SpawnRotation.RotateVector(FVector(100, 0, 0));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Owner = GetOwner(); // 현재 액터의 Owner를 설정
			ActorSpawnParams.Instigator = Character->GetInstigator(); // 공격자를 설정 (옵션)

			// Spawn the projectile at the muzzle
			AKillingFloorLikeProjectile* projectile = World->SpawnActor<AKillingFloorLikeProjectile>(
				ProjectileClass, SpawnLocation, SpawnRotation,
				ActorSpawnParams);
			if (projectile)
			{
				projectile->Initialize(AttackDamage);
				//CurrentLoadedAmmo--;
				return true;
			}
		}
	}

	return false;
}

