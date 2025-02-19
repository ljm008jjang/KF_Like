// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastShootingComponent.h"

#include "DebugManagerComponent.h"
#include "KillingFloorLikeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
URaycastShootingComponent::URaycastShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URaycastShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URaycastShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool URaycastShootingComponent::Fire(AKillingFloorLikeCharacter* Character, float AttackDamage)
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return false;
	}
	
	//레이케스트
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetInstigatorController());
	FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation();
	float Length = 10000;

	//결과 저장
	TMap<ABaseCharacter*, FHitResult> HitMap;
	

	FVector End = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
	End.Normalize();
	End = End * Length;
	End += Start;

	UDebugManagerComponent::DrawDebugLineManager(GetWorld(), Start, End, FColor::Red, true);

	TArray<FHitResult> HitResult; // 충돌 결과를 저장할 변수
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	// 레이캐스트 실행
	bool bHit = GetWorld()->LineTraceMultiByObjectType(
		HitResult, Start, End, ObjectQueryParams
	);

	if (bHit == false)
	{
		return true;
	}

	for (FHitResult result : HitResult)
	{
		ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(result.GetActor());
		//몬스터가 아닌걸 맞추면 stop
		if (HitCharacter == nullptr)
		{
			break;
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
			UE_LOG(LogTemp, Log, TEXT("%s"), *HitPair.Key -> GetName());
		}
	}

	return true;
}

