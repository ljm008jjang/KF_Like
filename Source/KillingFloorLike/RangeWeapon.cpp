// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"

#include "KillingFloorLikeCharacter.h"
#include "KillingFloorLikeProjectile.h"

void ARangeWeapon::Fire()
{
	Super::Fire();

	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

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

			// Spawn the projectile at the muzzle
			AKillingFloorLikeProjectile* projectile = World->SpawnActor<AKillingFloorLikeProjectile>(
				ProjectileClass, SpawnLocation, SpawnRotation,
				ActorSpawnParams);
			if (projectile)
			{
				projectile->Initialize(FireDamage);
			}
		}
	}

	// Try and play the sound if specified
	/*if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}*/
}
