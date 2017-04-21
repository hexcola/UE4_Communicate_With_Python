// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "GameFramework/Actor.h"
#include "ActorTCPClient.generated.h"

UCLASS()
class UE4_NETWORK_SOCKET_API AActorTCPClient : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorTCPClient();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginDestroy() override;

	FSocket* NodeSocket;

	void Launch();

	FString StringFromBinaryArray(TArray<uint8> BinaryArray);

	FTimerHandle ServerRespondHandle;

	void SendToServer(FString message);

	void ServerRespond();

};
