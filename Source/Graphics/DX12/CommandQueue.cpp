#include "CommandQueue.h"
#include "GraphicsEngine.h"

bool CommandQueue::init()
{
	ID3D12Device* device = GraphicsEngine::get()->device;

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue)));


	return true;
}

bool CommandQueue::release()
{

	if(commandQueue)
	{
		commandQueue->Release();
	}
	
	//delete this;
	return true;
}

ID3D12CommandQueue* CommandQueue::getCommandQueue()
{
	return commandQueue;
}
