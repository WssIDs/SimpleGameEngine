#pragma once
#include <vector>



struct IStreamingManager
{
	IStreamingManager()
	{
	}

	virtual ~IStreamingManager();

	//static struct FStreamingManagerCollection& Get();

	virtual void Tick(float DeltaTime);

	/**
 * Updates streaming, taking into account all current view infos. Can be called multiple times per frame.
 *
 * @param DeltaTime				Time since last call in seconds
 * @param bProcessEverything	[opt] If true, process all resources with no throttling limits
 */
	virtual void UpdateResourceStreaming(float DeltaTime) = 0;
};


//struct IRenderAssetStreamingManager : public IStreamingManager
//{
//	/**
//	* Updates streaming for an individual texture/mesh, taking into account all view infos.
//	*
//	* @param RenderAsset		Texture or mesh to update
//	*/
//	//virtual void UpdateIndividualRenderAsset(UStreamableRenderAsset* RenderAsset) = 0;
//
//	/** Adds a new texture/mesh to the streaming manager. */
//	//virtual void AddStreamingRenderAsset(UTexture2D* Texture) = 0;
//	//virtual void AddStreamingRenderAsset(UStaticMesh* StaticMesh) = 0;
//	//virtual void AddStreamingRenderAsset(USkeletalMesh* SkeletalMesh) = 0;
//
//	/** Removes a texture/mesh from the streaming manager. */
//	//virtual void RemoveStreamingRenderAsset(UStreamableRenderAsset* RenderAsset) = 0;
//};
//
//
//struct FStreamingManagerCollection : public IStreamingManager
//{
//	FStreamingManagerCollection();
//
//	/**
//	 * Calls UpdateResourceStreaming(), and does per-frame cleaning. Call once per frame.
//	 *
//	 * @param DeltaTime				Time since last call in seconds
//	 * @param bProcessEverything	[opt] If true, process all resources with no throttling limits
//	 */
//	virtual void Tick(float DeltaTime) override;
//
//	/**
//		* Gets a reference to the Texture Streaming Manager interface
//	*/
//	IRenderAssetStreamingManager& GetTextureStreamingManager() const;
//
//	/**
//	 * Get the streaming manager for textures and meshes
//	 */
//	IRenderAssetStreamingManager& GetRenderAssetStreamingManager() const;
//
//	/**
//	 * Adds a streaming manager to the array of managers to route function calls to.
//	 *
//	 * @param StreamingManager	Streaming manager to add
//	 */
//	void AddStreamingManager(IStreamingManager* StreamingManager);
//
//	/**
//	 * Removes a streaming manager from the array of managers to route function calls to.
//	 *
//	 * @param StreamingManager	Streaming manager to remove
//	 */
//	void RemoveStreamingManager(IStreamingManager* StreamingManager);
//
//protected:
//
//	/** Array of streaming managers to route function calls to */
//	std::vector<IStreamingManager*> StreamingManagers;
//
//	/** The currently added texture streaming manager. Can be NULL*/
//	FRenderAssetStreamingManager* TextureStreamingManager;
//};