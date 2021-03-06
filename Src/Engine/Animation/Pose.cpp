#include "Animation/Pose.h"

namespace cxc
{
	CPose::CPose()
	{

	}

	CPose::~CPose()
	{

	}

	std::shared_ptr<PoseInfo> CPose::GetPoseInfo(uint32_t Index) const
	{
		if(Index >= PoseInfos.size())
			return nullptr;
		else
			return PoseInfos[Index];
	}

	void CPose::AddPoseInfo(std::shared_ptr<PoseInfo> pNewInfo)
	{
		PoseInfos.push_back(pNewInfo);
	}

	void CPose::RemovePoseInfo(uint32_t Index)
	{
		PoseInfos.erase(PoseInfos.begin() + Index);
		PoseInfos.shrink_to_fit();
	}

	std::shared_ptr<PoseInfo> CPose::FindPoseInfo(const std::string& NodeName)
	{
		for (auto pInfo : PoseInfos)
		{
			if (pInfo->LinkNodeInfo.NodeName == NodeName)
				return pInfo;
		}

		return nullptr;
	}
}