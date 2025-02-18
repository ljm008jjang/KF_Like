// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "KillingFloorLikeGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHp = MaxHp;
	CurrentUnitState = EUnitState::Alive;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Attack()
{
	// 월드에서 현재 위치를 기준으로 검색할 위치와 반경 정의
	FVector SearchOrigin = GetActorLocation(); // 검색 중심점
	float SearchRadius = 50.0f; // 검색 반경

	// 검색 조건 설정
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 검색할 객체 타입

	TArray<AActor*> IgnoredActors; // 검색에서 제외할 액터 리스트
	TArray<AActor*> OutActors; // 검색 결과를 담을 배열

	// SphereOverlapActors 호출
	bool bFound = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		SearchOrigin,
		SearchRadius,
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
				if (FoundBaseCharacter)
				{
					FDamageEvent DamageEvent;
					FoundBaseCharacter->TakeDamage(BaseAttackDamage, DamageEvent, GetInstigatorController(), this);
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

void ABaseCharacter::Dead(AActor* DamageCauser)
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	GetMesh()->SetCollisionProfileName(TEXT("DeadActor"));
	GetMesh()->SetSimulatePhysics(true);
	FVector Impulse = (GetActorLocation() - DamageCauser->GetActorLocation()) * 100;
	GetMesh()->AddImpulseToAllBodiesBelow(Impulse);

	if (Controller)
	{
		Controller->StopMovement();
		Controller->UnPossess(); // Possess된 Pawn에서 Controller 분리
	}

	CurrentUnitState = EUnitState::Dead;
	/*Cast<AKillingFloorLikeGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->;*/
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	float realDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= realDamageAmount;
	UE_LOG(LogTemp, Display, TEXT("%f"), CurrentHp);
	if (CurrentHp <= 0)
	{
		Dead(DamageCauser);
	}
	return realDamageAmount;
}


float ABaseCharacter::GetMoveSpeed()
{
	return MoveSpeed;
}

EUnitState ABaseCharacter::GetCurrentUnitState()
{
	return CurrentUnitState;
}
