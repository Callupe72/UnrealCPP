// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalsPaint.h"
#include "Components/DecalComponent.h"

// Sets default values
ADecalsPaint::ADecalsPaint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick =true;

	Decals = CreateDefaultSubobject<UDecalComponent>(TEXT("Decals"));
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMat (TEXT("Material'/Game/Projectile/DecalsMat.DecalsMat'"));

	if(DecalMat.Succeeded())
	{
		GLog->Log("I Found mat");
		Decals->SetDecalMaterial(DecalMat.Object);
	}
	RootComponent = Decals;

	Decals->DecalSize = FVector(50,50,50);
}

// Called when the game starts or when spawned
void ADecalsPaint::BeginPlay()
{
	
	GLog->Log("FadeIn");
	Decals->SetFadeIn(0.001f,0.001f);
	GLog->Log("FadeOut");
	Decals->SetFadeOut(2,2,true);
	Super::BeginPlay();
}

// Called every frame
void ADecalsPaint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

