// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "BaseWeapon.h"
#include "KillingFloorLikeCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/DamageEvents.h"
#include "Runtime/Experimental/Chaos/Private/Chaos/PhysicsObjectInternal.h"

// Sets default values
AMonster::AMonster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentUnitType = EUnitType::Enemy;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	CurrentHeadHP = MaxHeadHP;
}


// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                           AActor* DamageCauser)
{
	float realDamageAmount = AActor::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		UE_LOG(LogTemp,Display,TEXT("Hited bone name : %s"), *PointDamageEvent->HitInfo.BoneName.ToString());
		
		if (CurrentHeadHP > 0 && PointDamageEvent->HitInfo.BoneName.ToString().Contains(TEXT("Head")))
		{
			realDamageAmount *= Cast<ABaseWeapon>(DamageCauser)->GetHeadShotValue(); // 맞은 부위가 Head면, 데미지 1.1배.
			CurrentHeadHP -= realDamageAmount;
			UE_LOG(LogTemp, Display, TEXT("CurrentHeadHP : %f"), CurrentHeadHP);
			
			if (CurrentHeadHP <= 0)
			{
				GetMesh()->HideBoneByName(PointDamageEvent->HitInfo.BoneName, PBO_None);
				realDamageAmount = realDamageAmount + 0.25f * MaxHp; //머리가 터지면 추가 데미지
				HeadOff(EventInstigator->GetCharacter());
			}
		}
	}
	// RadialDamage 받기.
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		const FRadialDamageEvent* RadialDamageEvent = static_cast<const FRadialDamageEvent*>(&DamageEvent);
	}

	CurrentHp -= realDamageAmount;
	UE_LOG(LogTemp, Display, TEXT("CurrentHp : %f"), CurrentHp);
	if (CurrentHp <= 0)
	{
		Dead(DamageCauser);
	}

	return realDamageAmount;
}

void AMonster::Dead(AActor* DamageCauser)
{
	if(GetIsAlive() == false)
	{
		return;
	}
	
	Super::Dead(DamageCauser);
	AKillingFloorLikeCharacter* Attacker = Cast<AKillingFloorLikeCharacter>(DamageCauser->GetOwner());
	if(Attacker)
	{
		Attacker->CalcMoney(Bounty);
	}
}

UAnimMontage* AMonster::GetAnimationMontage(EMonsterAnimationType animType, int32 index)
{
	return AnimationMap[animType].Montages[index];
}

int32 AMonster::GetAnimationMaxIndex(EMonsterAnimationType animType)
{
	return AnimationMap[animType].Montages.Num();
}

UAnimMontage* AMonster::GetHitAnimMontage(AActor* DamageCauser)
{
	FVector NormVec1 = GetActorForwardVector().GetSafeNormal2D();
	FVector NormVec2 = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();

	float Dot = FVector::DotProduct(NormVec1, NormVec2);
	float Cross = NormVec1.X * NormVec2.Y - NormVec1.Y * NormVec2.X; // 2D에서의 외적
	float AngleRad = atan2f(Cross, Dot);

	float degree = FMath::RadiansToDegrees(AngleRad);

	UAnimMontage* Montage;

	if (AnimationMap[EMonsterAnimationType::Hit].Montages.Num() <= 1)
	{
		return AnimationMap[EMonsterAnimationType::Hit].Montages[0];
	}

	if (degree < 45 && degree > -45) //앞
	{
		Montage = AnimationMap[EMonsterAnimationType::Hit].Montages[0];
	}
	else if (degree <= -45 && degree > -135) //좌
	{
		Montage = AnimationMap[EMonsterAnimationType::Hit].Montages[1];
	}
	else if (degree <= -135 || degree > 135) //후
	{
		Montage = AnimationMap[EMonsterAnimationType::Hit].Montages[2];
	}
	else //우
	{
		Montage = AnimationMap[EMonsterAnimationType::Hit].Montages[3];
	}

	UE_LOG(LogTemp, Display, TEXT("degree : %f"), degree);

	return Montage;
}

USoundBase* AMonster::GetSoundBase(EMonsterSoundType SoundType)
{
	return SoundMap[SoundType];
}

EMonsterType AMonster::GetMonsterType()
{
	return MonsterType;
}

/*void AMonster::DoSkill()
{
	//IMonsterSkill::DoSkill();
}*/

/*void AMonster::SetMonsterType(EMonsterType NewMonsterType)
{
	MonsterType = NewMonsterType;
}*/
