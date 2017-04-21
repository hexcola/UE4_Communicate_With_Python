// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_Network_Socket.h"
#include "ActorTCPClient.h"


// Sets default values
AActorTCPClient::AActorTCPClient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorTCPClient::BeginPlay()
{
	Super::BeginPlay();
	
	Launch();
}

// Called every frame
void AActorTCPClient::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AActorTCPClient::Launch()
{
	NodeSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	FString address = TEXT("127.0.0.1");
	uint32 port = 8891;
	FIPv4Address ip;

	if (FIPv4Address::Parse(address, ip))
	{
		UE_LOG(LogTemp, Log, TEXT("Parsed IP Address: %d"), ip.Value);
		TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(ip.Value, port);

		UE_LOG(LogTemp, Log, TEXT("Remote Host: %s"), *addr->ToString(true));

		bool connected = NodeSocket->Connect(*addr);

		if (connected)
		{
			UE_LOG(LogTemp, Log, TEXT("Connect successfully!! "));

			UE_LOG(LogTemp, Log, TEXT("Start to waiting for server respond "));

			GetWorldTimerManager().SetTimer(ServerRespondHandle, this, &AActorTCPClient::ServerRespond, 0.01, true);

			FString message = TEXT("Hello, Master!");
			SendToServer(message);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Connect to %s: %d "), *address, port);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Convert IP Error "));
	}
}

void AActorTCPClient::SendToServer(FString message)
{
	if (!NodeSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("Node Socket is NULL "));
		return;
	}

	TArray<uint8> SendData = TArray<uint8>();
	int32 sent = 0;
	SendData.Append(message.GetCharArray());
	bool successful = NodeSocket->Send(SendData.GetData(), SendData.Num(), sent);

	if (successful)
	{
		UE_LOG(LogTemp, Log, TEXT("Sent successfully!! "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sent Failed! "));
	}
}

void AActorTCPClient::ServerRespond() {

	//can thread this too

	if (!NodeSocket)
	{
		UE_LOG(LogTemp, Error, TEXT("Node Socket is NULL "));
		return;
	}

	TArray<uint8> ReceivedData;
	uint8 default_data = 0;
	uint32 Size;

	while (NodeSocket->HasPendingData(Size))
	{
		UE_LOG(LogTemp, Log, TEXT("Got Data! "));

		ReceivedData.Init(default_data, FMath::Min(Size, 65507u));

		int32 Read = 0;

		NodeSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		UE_LOG(LogTemp, Log, TEXT("Data Bytes Read! %d"), ReceivedData.Num());
	}

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Data Bytes Read! %d"), ReceivedData.Num());


	//	Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);

	UE_LOG(LogTemp, Log, TEXT("I got your message: %s"), *ReceivedUE4String);

	SendToServer(TEXT("Cooool!"));
}


FString AActorTCPClient::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	BinaryArray.Add(0); // Add 0 termination. Even if the string is already 0-terminated, it doesn't change the results.
						// Create a string from a byte array. The string is expected to be 0 terminated (i.e. a byte set to 0).
						// Use UTF8_TO_TCHAR if needed.
						// If you happen to know the data is UTF-16 (USC2) formatted, you do not need any conversion to begin with.
						// Otherwise you might have to write your own conversion algorithm to convert between multilingual UTF-16 planes.
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

void AActorTCPClient::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Log, TEXT("Actor Begin Destroy "));

	if (NodeSocket)
	{
		NodeSocket->Close();
	}
}
