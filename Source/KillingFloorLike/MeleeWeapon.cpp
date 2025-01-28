// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"

#include "KillingFloorLikeCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AMeleeWeapon::Fire()
{
	Super::Fire();

	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// 월드에서 현재 위치를 기준으로 검색할 위치와 반경 정의
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
					UE_LOG(LogTemp, Log, TEXT("%s"), *FoundBaseCharacter -> GetName());
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No actors found in radius."));
	}
}
