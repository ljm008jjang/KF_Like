// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeShootingComponent.h"

#include "KillingFloorLikeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMeleeShootingComponent::UMeleeShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UMeleeShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeleeShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UMeleeShootingComponent::Fire(AKillingFloorLikeCharacter* Character, float AttackDamage)
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return false;
	}
	//Shoot();

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetInstigatorController());
	FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
	float Length = AttackRange;

	int32 segments = 11;
	float Angle = 180;


	float SegAngle = Angle / (segments - 1);

	float HalfAngle = Angle * 0.5f;
	//결과 저장
	TMap<ABaseCharacter*, FHitResult> HitMap;

	for (int i = 0; i < segments; i++)
	{
		//90, 45, 0, -45...
		float thisAngle = HalfAngle - SegAngle * i;

		FVector End;

		FVector ForwardVec = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();

		// 회전 변환 적용
		//x = xCosA - ySinA
		//y = xSinA + yCosA
		FVector RotationAxis = GetOwner()->GetActorUpVector().GetSafeNormal();
		FQuat RotationQuat = FQuat(RotationAxis, FMath::DegreesToRadians(thisAngle));
		End = RotationQuat.RotateVector(ForwardVec);

		End.Normalize();

		End = End * Length;
		End += Start;

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

		TArray<FHitResult> HitResult; // 충돌 결과를 저장할 변수
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

		// 레이캐스트 실행
		bool bHit = GetWorld()->LineTraceMultiByObjectType(
			HitResult, Start, End, ObjectQueryParams
		);

		if (bHit == false)
		{
			continue;
			// 충돌한 오브젝트 정보 출력
		}

		for (FHitResult result : HitResult)
		{
			ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(result.GetActor());
			if (HitCharacter == nullptr)
			{
				continue;
			}

			if (HitMap.Contains(HitCharacter) == false)
			{
				HitMap.Add(HitCharacter, result);
			}
			//이전에 맞았지만 헤드샷이 아니고, 헤드샷이 새롭게 체크됨
			else if (result.BoneName.ToString().Contains("Head") && HitMap[HitCharacter].BoneName.ToString().
				Contains("Head") == false)
			{
				HitMap[HitCharacter] = result;
			}
		}
	}

	bool IsHit = false;
	for (auto HitPair : HitMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : %s"), *HitPair.Key->GetName(),
		       *HitPair.Value.BoneName.ToString());

		if (Character->IsAttackableUnitType(HitPair.Key))
		{
			UGameplayStatics::ApplyPointDamage(
				HitPair.Key,
				AttackDamage,
				Character->GetActorLocation(),
				HitPair.Value,
				Character->GetInstigatorController(),
				GetOwner(),
				UDamageType::StaticClass()
			);
			IsHit = true;
			UE_LOG(LogTemp, Log, TEXT("%s"), *HitPair.Key -> GetName());
		}
	}
	if(IsHit)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSoundBase(EWeaponSoundType::Hit, 2),
													  Character->GetActorLocation());
	}else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSoundBase(EWeaponSoundType::Hit, 0), Character->GetActorLocation());
	}

	return true;
}

USoundBase* UMeleeShootingComponent::GetSoundBase(EWeaponSoundType SoundType, int32 index)
{
	return SoundMap[SoundType].SoundBases[index];
}

