// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"

#include <string>

#include "KillingFloorLikeCharacter.h"
#include "KillingFloorLikeProjectile.h"

void ARangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentLoadedAmmo = MaxLoadedAmmo;
	SavedAmmo = MaxLoadedAmmo;
}

void ARangeWeapon::Fire()
{
	Super::Fire();

	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (CurrentLoadedAmmo <= 0)
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
			ActorSpawnParams.Owner = this; // 현재 액터의 Owner를 설정
			ActorSpawnParams.Instigator = GetOwner()->GetInstigator(); // 공격자를 설정 (옵션)

			// Spawn the projectile at the muzzle
			AKillingFloorLikeProjectile* projectile = World->SpawnActor<AKillingFloorLikeProjectile>(
				ProjectileClass, SpawnLocation, SpawnRotation,
				ActorSpawnParams);
			if (projectile)
			{
				projectile->Initialize(FireDamage);
				CurrentLoadedAmmo--;
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

void ARangeWeapon::Reload()
{
	if (SavedAmmo <= 0)
	{
		return;
	}
	int32 AllAmmonCount = CurrentLoadedAmmo + SavedAmmo;
	int32 ReloadAmmoCount = FMath::Min(MaxLoadedAmmo, AllAmmonCount);
	CurrentLoadedAmmo = ReloadAmmoCount;
	SavedAmmo = AllAmmonCount - ReloadAmmoCount;
}

bool ARangeWeapon::IsReloadable()
{
	if (CurrentLoadedAmmo >= MaxLoadedAmmo)
	{
		return false;
	}

	if (SavedAmmo <= 0)
	{
		return false;
	}

	return true;
}

FString ARangeWeapon::GetAmmoText()
{
	return FString::FromInt(CurrentLoadedAmmo) + " / " + FString::FromInt(SavedAmmo);
}

void ARangeWeapon::GetAmmo(int GetAmmoAmount)
{
	CurrentLoadedAmmo += GetAmmoAmount;
	if (CurrentLoadedAmmo > MaxLoadedAmmo)
	{
		CurrentLoadedAmmo = MaxLoadedAmmo;
	}
}
