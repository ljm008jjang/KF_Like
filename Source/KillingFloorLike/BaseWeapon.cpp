// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "KillingFloorLikeCharacter.h"
#include "TP_PickUpComponent.h"
#include "WeaponShootingInterface.h"

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
	SetAttackCooltime(GetAnimation(EWeaponAnimationType::Fire, false, 0));
	
	ShootingComponent = FindComponentByInterface<IWeaponShootingInterface>();
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

bool ABaseWeapon::FireWeapon(float AttackDamage)
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return false;
	}
	
	if (ShootingComponent)
	{
		return ShootingComponent->Fire(Character, AttackDamage);  // 현재 장착된 발사 방식을 실행
	}

	return false;
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

UAnimMontage* ABaseWeapon::GetAnimation(EWeaponAnimationType AnimationType, bool IsIron, int index)
{
	if (IsIron && AnimationType == EWeaponAnimationType::Fire && AnimationMap[AnimationType].IronMontages.Num() > 0)
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

int32 ABaseWeapon::GetAnimationMaxIndex(EWeaponAnimationType type)
{
	if (CurrentAimType == EAimType::Iron && type == EWeaponAnimationType::Fire)
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
	if (AnimationMap[EWeaponAnimationType::Fire].IronMontages.Num() > 0)
	{
		return true;
	}
	return false;
}

void ABaseWeapon::SetCurrentAimType(EAimType NewAimType)
{
	CurrentAimType = NewAimType;
}

float ABaseWeapon::GetFireDamage()
{
	return FireDamage;
}

float ABaseWeapon::GetSkillFireDamage()
{
	return SkillFireDamage;
}

/*USoundBase* ABaseWeapon::GetSoundBase(EWeaponSoundType SoundType, int32 index)
{
	return SoundMap[SoundType].SoundBases[index];
}*/

float ABaseWeapon::GetHeadShotValue()
{
	return HeadShotValue;
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

	SetOwner(TargetCharacter);
}

void ABaseWeapon::DropWeapon(AKillingFloorLikeCharacter* TargetCharacter)
{
	if (Character == nullptr)
	{
		return;
	}

	if (UTP_PickUpComponent* PickUpComponent = FindComponentByClass<UTP_PickUpComponent>())
	{
		PickUpComponent->OnDrop.Broadcast(TargetCharacter);
		PickUpComponent->AfterDrop();
	}
	
	Character = nullptr;
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorLocation(GetOwner()->GetActorLocation() + TargetCharacter->GetActorForwardVector() * 300, true,
	                             nullptr, ETeleportType::TeleportPhysics);
	
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->AddImpulse(TargetCharacter->GetActorForwardVector() * 300);

	SetOwner(nullptr);
}
