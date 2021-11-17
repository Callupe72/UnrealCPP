// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerAddLife.h"

// Sets default values
ATriggerAddLife::ATriggerAddLife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Donner un mesh / attribut la 1ere fois que l'objet est glissé dans la scene

	OnActorBeginOverlap.AddDynamic(this, &ATriggerAddLife::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerAddLife::OnOverlapEnd);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonMesh"));
	if (CubeMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(CubeMesh.Object);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, (TEXT("Hit")));
	StaticMesh->SetSimulatePhysics(false);
	//StaticMesh->OnComponentHit.AddDynamic(this, &ATriggerAddLife::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ATriggerAddLife::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATriggerAddLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ATriggerAddLife::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		GLog->Log("Touch");
		player = Cast<ACPPTestCharacter>(OtherActor);
		if (player == nullptr)
		{
			return;
		}

		GLog->Log("ITouchPlayer");

		if (player->health > 0)
		{
			player->AddLife(lifeToAdd);
			TimerStayInCollision();
		}
	}
}
void ATriggerAddLife::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
		GLog->Log("EndTouch");
		player = nullptr;
}

void ATriggerAddLife::TimerStayInCollision()
{
	if(cooldownTime == 0)
	{
		return;
	}

	FTimerHandle    handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
		{
			if (player == nullptr)
			{
				return;
			}
			player->AddLife(lifeToAdd);
		}, cooldownTime, 1);
}

