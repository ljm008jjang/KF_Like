// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponFleshComponent.h"

#include "Components/SpotLightComponent.h"

// Sets default values for this component's properties
UWeaponFleshComponent::UWeaponFleshComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// SpotLight 컴포넌트 생성
	FleshLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flesh Light"));

	if (FleshLight)
	{
		FleshLight->SetIntensity(5000.0f); // 밝기 설정
		FleshLight->SetLightColor(FLinearColor::White); // 빛 색상
		FleshLight->SetInnerConeAngle(0); // 내부 콘 각도
		FleshLight->SetOuterConeAngle(25); // 외부 콘 각도
		FleshLight->SetAttenuationRadius(1000.0f); // 감쇠 반경
		FleshLight->SetVisibility(false); // 처음엔 꺼진 상태
	}
}


// Called when the game starts
void UWeaponFleshComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// 소유 액터에 SpotLight 부착
	/*if (AActor* Owner = GetOwner())
	{
		FleshLight->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}*/
}


// 플래시라이트 켜고 끄는 기능
void UWeaponFleshComponent::ToggleFlashlight(bool IsFleshOn)
{
	if (GetIsFleshOn() == IsFleshOn)
	{
		return;
	}


	if (FleshLight)
	{
		FleshLight->SetVisibility(IsFleshOn);

		if (AActor* Owner = GetOwner())
		{
			if (Owner->GetRootComponent())
			{
				FleshLight->AttachToComponent(Owner->GetRootComponent(),
				                              FAttachmentTransformRules::KeepRelativeTransform);
			}


			// 카메라의 위치와 방향을 가져오기 위해 캐릭터를 찾음
			APawn* PlayerPawn = Cast<APawn>(Owner->GetOwner());
			if (PlayerPawn)
			{
				APlayerController* PlayerController = Cast<APlayerController>(PlayerPawn->GetController());
				if (PlayerController)
				{
					FVector CameraLocation;
					FRotator CameraRotation;

					// 현재 플레이어의 카메라 위치와 회전 값 가져오기
					PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

					// SpotLight 위치를 카메라 정면으로 설정
					FleshLight->SetWorldLocation(CameraLocation + CameraRotation.Vector() * 100.0f); // 100cm 앞
					FleshLight->SetWorldRotation(CameraRotation);
				}
			}
		}
	}
}

bool UWeaponFleshComponent::GetIsFleshOn()
{
	return FleshLight->IsVisible();
}

UAnimMontage* UWeaponFleshComponent::GetFleshAnimMontage()
{
	return FleshAnimMontage;
}
