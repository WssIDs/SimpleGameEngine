#include "StreamingContent.h"

//static FStreamingManagerCollection* StreamingManagerCollection = nullptr;

IStreamingManager::~IStreamingManager()
{
	//delete StreamingManagerCollection;
	//StreamingManagerCollection = nullptr;
}

//struct FStreamingManagerCollection& IStreamingManager::Get()
//{
//	if(StreamingManagerCollection == nullptr)
//	{
//		StreamingManagerCollection = new StreamingManagerCollection();
//	}
//
//	return *StreamingManagerCollection;
//}

void IStreamingManager::Tick(float DeltaTime)
{
	UpdateResourceStreaming(DeltaTime);
}
