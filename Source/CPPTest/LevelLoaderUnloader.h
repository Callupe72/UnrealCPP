// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPPTestCharacter.h"
#include "GameFramework/Actor.h"
#include "LevelLoaderUnloader.generated.h"

UENUM()
enum ActionToDoEnum
{
	LoadOnEnter     UMETA(DisplayName = "LoadOnEnter"),
	UnloadOnEnter      UMETA(DisplayName = "UnloadOnEnter"),
	LoadWhileIn   UMETA(DisplayName = "LoadWhileIn"),
  };

UCLASS()
class CPPTEST_API ALevelLoaderUnloader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelLoaderUnloader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ActionToDoEnum> ActionToDo;
	
	UPROPERTY(EditAnywhere)
	FName LevelToLoad = "";
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor) ;

	UFUNCTION()
	void LoadUnload(bool load);
	
};
