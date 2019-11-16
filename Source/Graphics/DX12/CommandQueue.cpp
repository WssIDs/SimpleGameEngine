#include "CommandQueue.h"
#include "GraphicsEngine.h"

bool CommandQueue::init()
{
	ID3D12Device* device = GraphicsEngine::get()->m_device;

	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));


	return true;
}

bool CommandQueue::release()
{

	if(m_commandQueue)
	{
		m_commandQueue->Release();
	}
	
	//delete this;
	return true;
}

ID3D12CommandQueue* CommandQueue::getCommandQueue()
{
	return m_commandQueue;
}
