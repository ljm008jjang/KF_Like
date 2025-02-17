// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "KillingFloorLikeCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AMeleeWeapon::Fire(float AttackDamage)
{
	Super::Fire(AttackDamage);


	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	//Shoot();

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
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
		FVector RotationAxis = GetActorUpVector().GetSafeNormal();
		FQuat RotationQuat = FQuat(RotationAxis, FMath::DegreesToRadians(thisAngle));
		End = RotationQuat.RotateVector(ForwardVec);

		End.Normalize();

		End = End * Length;
		End += Start;

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);

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
				GetActorLocation(),
				HitPair.Value,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
			);
			IsHit = true;
			UE_LOG(LogTemp, Log, TEXT("%s"), *HitPair.Key -> GetName());
		}
	}
	if(IsHit)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSoundBase(EWeaponSoundType::Hit, 2),
													  GetActorLocation());
	}else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSoundBase(EWeaponSoundType::Hit, 0), GetActorLocation());
	}

	/*// 월드에서 현재 위치를 기준으로 검색할 위치와 반경 정의
	FVector SearchOrigin = GetActorLocation(); // 검색 중심점

	// 검색 조건 설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 검색할 객체 타입

	TArray<AActor*> IgnoredActors; // 검색에서 제외할 액터 리스트
	TArray<AActor*> OutActors; // 검색 결과를 담을 배열

	// SphereOverlapActors 호출
	bool bFound = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		SearchOrigin,
		AttackRange,
		ObjectTypes,
		ABaseCharacter::StaticClass(),
		IgnoredActors,
		OutActors
	);

	// 결과 출력
	if (bFound)
	{
		for (AActor* FoundActor : OutActors)
		{
			if (FoundActor && FoundActor != this)
			{
				ABaseCharacter* FoundBaseCharacter = Cast<ABaseCharacter>(FoundActor);
				if (FoundBaseCharacter && Character->IsAttackableUnitType(FoundBaseCharacter))
				{
					UGameplayStatics::ApplyDamage(
						FoundBaseCharacter,
						FireDamage,
						GetInstigatorController(),
						this,
						UDamageType::StaticClass()
					);
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSoundBase(EWeaponSoundType::Hit, 2),
					                                      FoundBaseCharacter->GetActorLocation());
					UE_LOG(LogTemp, Log, TEXT("%s"), *FoundBaseCharacter -> GetName());
					return;
				}
			}
		}
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSoundBase(EWeaponSoundType::Hit, 0), GetActorLocation());
	*/
}

void AMeleeWeapon::SkillFire(float SkillAttackDamage)
{
	Fire(SkillAttackDamage);
}
