
#include "stdafx.h"
#include "file_changer.hpp"
#include "file_structs.hpp"

namespace osu_tools
{
	namespace file_changer
	{

		void set_speed_multiplier(float aMulti, osu_file& aOsuFile)
		{
			aOsuFile.sBeatmapID = 0;
			aOsuFile.sCreator = "Bauxe";
			aOsuFile.sVersion = "Test";
			aOsuFile.sFileName = aOsuFile.sArtist + " - " + aOsuFile.sTitle + " (" + aOsuFile.sCreator + ") [" + aOsuFile.sVersion + "].osu";
			aOsuFile.sSliderMultiplier /= aMulti;

			for (auto& lTimingPoint : aOsuFile.sTimingPoints)
			{
				lTimingPoint.sOffset /= aMulti;
				if (lTimingPoint.sInherited)
					lTimingPoint.sMSPerBeat /= aMulti;
			}

			for (auto& lBreakPeriod : aOsuFile.sBreakPeriods)
			{
				lBreakPeriod.first /= aMulti;
				lBreakPeriod.second /= aMulti;
			}

			for (auto& lHitObject : aOsuFile.sHitObjects)
			{
				if (lHitObject->sType & (1 << 0))
				{
					hit_object_circle* lObject = static_cast<hit_object_circle*>(lHitObject);
					lObject->sTime /= aMulti;
				}
				else if (lHitObject->sType & (1 << 1))
				{
					hit_object_slider* lObject = static_cast<hit_object_slider*>(lHitObject);
					lObject->sTime /= aMulti;
				}
				else if (lHitObject->sType & (1 << 3))
				{
					hit_object_spinner* lObject = static_cast<hit_object_spinner*>(lHitObject);
					lObject->sTime /= aMulti;
					lObject->sEndTime /= aMulti;
				}
			}
		}
		void set_speed_percentage(float aMulti, osu_file& aOsuFile);
	}
}
