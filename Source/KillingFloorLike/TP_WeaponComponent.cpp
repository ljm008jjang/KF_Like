// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "KillingFloorLikeCharacter.h"
#include "KillingFloorLikeProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "TP_PickUpComponent.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire()
{
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
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

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
				projectile->Initialize(GunDamage);
			}
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
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
	}
}

EWeaponType UTP_WeaponComponent::GetWeaponType()
{
	return WeaponType;
}

void UTP_WeaponComponent::AttachWeapon(AKillingFloorLikeCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	SetSimulatePhysics(false);

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);
	Character->PickUpWeapon(this);
}

void UTP_WeaponComponent::DropWeapon(AKillingFloorLikeCharacter* TargetCharacter)
{
	if (Character == nullptr)
	{
		return;
	}
	Character = nullptr;
	DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + TargetCharacter->GetActorForwardVector() * 300, true,
	                             nullptr, ETeleportType::TeleportPhysics);
	SetSimulatePhysics(true);
	AddImpulse(TargetCharacter->GetActorForwardVector() * 3000);
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
