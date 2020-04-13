#pragma once
#include "AetPlugin.h"
#include "Graphics/Auth2D/Aet/AetSet.h"

namespace AetPlugin
{
	using namespace Comfy;
	using namespace Comfy::Graphics;

	class AetExporter : NonCopyable
	{
	public:
		AetExporter() = default;
		~AetExporter() = default;

		std::string GetAetSetNameFromProjectName() const;
		UniquePtr<Aet::AetSet> ExportAetSet(std::wstring_view workingDirectory);

	protected:
		SuitesData suites;

	protected:
		struct AEItemData
		{
			std::string Name;
			std::string Comment;
			CommentUtil::Property CommentProperty;

			AEGP_ItemH ItemHandle;
			AEGP_CompH CompHandle;

			AEGP_ItemFlags Flags;
			AEGP_ItemType Type;
			std::pair<A_long, A_long> Dimensions;
			AEItemData* Parent;

			inline bool IsParentOf(const AEItemData& parent) const
			{
				if (Parent == nullptr)
					return false;
				else if (Parent == &parent)
					return true;
				return Parent->IsParentOf(parent);
			}
		};

		struct WorkingProjectData
		{
			A_long Index;
			AEGP_ProjectH Handle;
			A_char Name[AEGP_MAX_PROJ_NAME_SIZE];
			std::wstring Path;
			AEGP_ItemH RootFolder;

			std::vector<AEItemData> Items;

		} workingProject = {};

		void SetupWorkingProjectData();

	protected:
		struct WorkingDirectoryData
		{
			std::wstring ImportDirectory;
		} workingDirectory;

		struct WorkingAetData
		{
			Aet::AetSet* Set = nullptr;
			AEItemData* Folder;
			std::vector<AEItemData*> SceneComps;

			std::string SprPrefix, SprHashPrefix;

		} workingSet;

		void SetupWorkingSetData(Aet::AetSet& set);

	protected:
		struct WorkingSceneData
		{
			Aet::Scene* Scene;
			AEItemData* AESceneComp;
		} workingScene = {};

		void SetupWorkingSceneData(AEItemData* sceneComp);

	protected:
		void ExportScene();
		void ExportAllCompositions();

		void ExportComp(Aet::Composition& comp);

		void ExportLayer(Aet::Layer& layer);
		void ExportLayerName(Aet::Layer& layer);
		void ExportLayerTime(Aet::Layer& layer);
		void ExportLayerQuality(Aet::Layer& layer);
		void ExportLayerMarkers(Aet::Layer& layer);
		void ExportLayerFlags(Aet::Layer& layer);
		void ExportLayerSourceItem(Aet::Layer& layer);

		void ExportLayerVideo(Aet::Layer& layer);
		void ExportLayerTransferMode(Aet::Layer& layer, Aet::LayerTransferMode& transferMode);
		void ExportLayerVideoStream(Aet::Layer& layer, Aet::LayerVideo& layerVideo);
		void SetLayerVideoPropertyLinear(Aet::Property1D& property);

		void ExportNewCompSource(Aet::Layer& layer, AEGP_ItemH sourceItem);
		void ExportNewVideoSource(Aet::Layer& layer, AEGP_ItemH sourceItem);
		void ExportVideo(Aet::Video& video);

		RefPtr<Aet::Composition> FindExistingCompSourceItem(AEGP_ItemH sourceItem);
		RefPtr<Aet::Video> FindExistingVideoSourceItem(AEGP_ItemH sourceItem);

		void ScanCheckSetLayerRefParents(Aet::Layer& layer);
		RefPtr<Aet::Layer> FindLayerRefParent(Aet::Layer& layer, AEGP_LayerH parentHandle);

		void FixInvalidSceneData();
		void FixInvalidCompositionData(Aet::Composition& comp);
		void FixInvalidCompositionTrackMatteDurations(Aet::Composition& comp);
		void FixInvalidLayerTrackMatteDurations(Aet::Layer& layer, Aet::Layer& trackMatteLayer);
	};
}
