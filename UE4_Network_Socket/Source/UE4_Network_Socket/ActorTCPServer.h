// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Networking.h"
#include "GameFramework/Actor.h"
#include "ActorTCPServer.generated.h"

UCLASS()
class UE4_NETWORK_SOCKET_API AActorTCPServer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorTCPServer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void BeginDestroy() override;


	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;

	bool StartTCPReceiver(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort,
		const int32 ReceiveBufferSize = 2 * 1024 * 1024
	);

	//Timer functions, could be threads
	void TCPConnectionListener(); 	//can thread this eventually
	void TCPSocketListener();		//can thread this eventually

	FString StringFromBinaryArray(TArray<uint8> BinaryArray);

	void Launch();

	FTimerHandle MyTimerHandler;
	FTimerHandle MyTimerHandler2;
	
};
