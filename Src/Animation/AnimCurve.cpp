#include "Animation/AnimCurve.h"
#include "Animation/AnimKeyFrame.h"
#include <algorithm>

namespace cxc
{
	AnimCurve::AnimCurve(float Start, float End):
		StartTime(Start), EndTime(End)
	{

	}

	AnimCurve::~AnimCurve()
	{

	}

	void AnimCurve::AddKeyFrame(std::shared_ptr<AnimKeyFrame> pKeyFrame)
	{
		KeyFrames.push_back(pKeyFrame);

		// Re-sorting the key frames after inserting
		SortKeyFramesByStartTime();
	}

	void AnimCurve::DeleteKeyFrame(std::shared_ptr<AnimKeyFrame> pKeyFrame)
	{
		for (auto iter = KeyFrames.begin(); iter != KeyFrames.end(); ++iter)
		{
			if (*iter == pKeyFrame)
			{
				KeyFrames.erase(iter);
			}
		}

		// Shrink to avoid holes 
		KeyFrames.shrink_to_fit();
	}

	std::shared_ptr<AnimKeyFrame> AnimCurve::GetKeyFrameByIndex(size_t Index)
	{
		if (Index >= KeyFrames.size())
			return nullptr;
		else
			return KeyFrames[Index];
	}

	void AnimCurve::SortKeyFramesByStartTime()
	{
		// Sort the key frames by ascending order of the start time
		std::sort(KeyFrames.begin(), KeyFrames.end(),
			[](std::shared_ptr<AnimKeyFrame> lhs, std::shared_ptr<AnimKeyFrame> rhs) 
			{
				return lhs->GetStartTime() < rhs->GetStartTime();
			}
		);
	}

	std::shared_ptr<AnimKeyFrame> AnimCurve::FindCurrentKeyFrame(float CurrentTime)
	{
		if (KeyFrames.empty())
			return nullptr;

		if (KeyFrames.size() == 1)
			return KeyFrames[0]->GetStartTime() <= CurrentTime ? KeyFrames[0] : nullptr;

		size_t KeyFrameCount = KeyFrames.size();
		for (size_t KeyFrameIndex = 1; KeyFrameIndex < KeyFrameCount; ++KeyFrameIndex)
		{
			if (KeyFrames[KeyFrameIndex - 1]->GetStartTime() <= CurrentTime
				&& KeyFrames[KeyFrameIndex]->GetStartTime() > CurrentTime)
			{
				return KeyFrames[KeyFrameIndex - 1];
			}
		}

		return KeyFrames.back();
	}

	void AnimCurve::Evaluate(float CurrentTime, eAnimPlayMode PlayMode, const std::vector<glm::vec3>& SrcVertices, std::vector<glm::vec3>& DstVertices)
	{
		
	}
}