// Fill out your copyright notice in the Description page of Project Settings.


#include "TestJumpPad.h"
#include "GameFramework/Character.h"

// Sets default values
ATestJumpPad::ATestJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	//Donner un mesh / attribut la 1ere fois que l'objet est glissé dans la scene
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonMesh"));

	if (CubeMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(CubeMesh.Object);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, (TEXT("Hit")));

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->OnComponentHit.AddDynamic(this, &ATestJumpPad::OnHit);
}

// Called when the game starts or when spawned
void ATestJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
	//GLog->Log("Bonjour");
}

// Called every frame
void ATestJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestJumpPad::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character ==nullptr)
	{
		return;
	}

	character->LaunchCharacter(FVector(0, 0, 1000), false, true);
}

