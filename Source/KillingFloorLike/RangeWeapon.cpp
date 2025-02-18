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

bool ARangeWeapon::FireWeapon(float AttackDamage)
{
	if (CurrentLoadedAmmo <= 0)
	{
		return false;
	}
	
	if(Super::FireWeapon(AttackDamage))
	{
		CurrentLoadedAmmo--;

		return true;
	}
	return false;

	/*if (Character == nullptr || Character->GetController() == nullptr)
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
				projectile->Initialize(AttackDamage);
				CurrentLoadedAmmo--;
			}
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

void ARangeWeapon::ReloadAmmoAdd(int ReloadedAmmo)
{
	if (SavedAmmo <= 0)
	{
		return;
	}

	CurrentLoadedAmmo += ReloadedAmmo;
	SavedAmmo -= ReloadedAmmo;
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

bool ARangeWeapon::IsAttackable()
{
	return Super::IsAttackable() && CurrentLoadedAmmo > 0;
}

int32 ARangeWeapon::GetMaxLoadedAmmon()
{
	return MaxLoadedAmmo;
}
