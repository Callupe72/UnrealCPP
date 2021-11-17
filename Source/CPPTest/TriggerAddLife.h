// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPPTestCharacter.h"
#include "TriggerAddLife.generated.h"

UCLASS()
class CPPTEST_API ATriggerAddLife : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerAddLife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		int lifeToAdd = 10;

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	UFUNCTION()
		void TimerStayInCollision();

	UPROPERTY(EditAnywhere)
		float cooldownTime = 0.2f;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;;

	UPROPERTY(VisibleAnywhere)
		ACPPTestCharacter* player;

};
