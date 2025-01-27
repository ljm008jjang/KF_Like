// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "KillingFloorLikeCharacter.h"
#include "KillingFloorLikeProjectile.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	SetAttackCooltime(GetAnimation(EAnimationType::Fire, false, 0));
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentAttackCooltime > 0)
	{
		CurrentAttackCooltime -= DeltaTime;
	}
}

void ABaseWeapon::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	/*// Try and fire a projectile
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
				projectile->Initialize(GunDamage);
			}
		}
	}*/

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

TSubclassOf<class UAnimInstance> ABaseWeapon::GetAnimInstance()
{
	return AnimInstance;
}

USkeletalMesh* ABaseWeapon::GetSkeletalMesh()
{
	return SkeletalMesh;
}

EWeaponType ABaseWeapon::GetWeaponType()
{
	return WeaponType;
}

UAnimMontage* ABaseWeapon::GetAnimation(EAnimationType AnimationType, bool IsIron, int index)
{
	if (IsIron && AnimationType == EAnimationType::Fire && AnimationMap[AnimationType].IronMontages.Num() > 0)
	{
		return AnimationMap[AnimationType].IronMontages[index];
	}
	else
	{
		return AnimationMap[AnimationType].Montages[index];
	}
}

bool ABaseWeapon::IsAttackable()
{
	return CurrentAttackCooltime <= 0;
}

void ABaseWeapon::SetCurrentAttackCooltime()
{
	CurrentAttackCooltime = MaxAttackCooltime;
}

int32 ABaseWeapon::GetAnimationMaxIndex(EAnimationType type)
{
	if (CurrentAimType == EAimType::Iron && type == EAnimationType::Fire)
	{
		return AnimationMap[type].IronMontages.Num();
	}
	else
	{
		return AnimationMap[type].Montages.Num();
	}
}

EAimType ABaseWeapon::GetCurrentAimType()
{
	return CurrentAimType;
}

bool ABaseWeapon::IsAimTypeChangeable()
{
	if (AnimationMap[EAnimationType::Fire].IronMontages.Num() > 0)
	{
		return true;
	}
	return false;
}

void ABaseWeapon::SetCurrentAimType(EAimType NewAimType)
{
	CurrentAimType = NewAimType;
}

void ABaseWeapon::SetAttackCooltime(UAnimMontage* PlayedAnimMontage)
{
	if (IsAsyncAttackCooltimeWithAnimation)
	{
		//MaxAttackCooltime = attack 애니메이션의 길이
		MaxAttackCooltime = PlayedAnimMontage->GetPlayLength();
	}
	CurrentAttackCooltime = 0;
}

void ABaseWeapon::AttachWeapon(AKillingFloorLikeCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);
	Character->PickUpWeapon(this);
}

void ABaseWeapon::DropWeapon(AKillingFloorLikeCharacter* TargetCharacter)
{
	if (Character == nullptr)
	{
		return;
	}
	Character = nullptr;
	StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + TargetCharacter->GetActorForwardVector() * 300, true,
	                             nullptr, ETeleportType::TeleportPhysics);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->AddImpulse(TargetCharacter->GetActorForwardVector() * 3000);
}
