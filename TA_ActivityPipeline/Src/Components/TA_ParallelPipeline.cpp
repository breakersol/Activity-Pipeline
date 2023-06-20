﻿/*
 * Copyright [2023] [Shuang Zhu / Sol]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Components/TA_ParallelPipeline.h"
#include "Components/TA_CommonTools.h"
#include "Components/TA_ThreadPool.h"

#include <future>

namespace CoreAsync {
    TA_ParallelPipeline::TA_ParallelPipeline() : TA_BasicPipeline()
    {

    }

    void TA_ParallelPipeline::run()
    {
        unsigned int sIndex(std::move(startIndex()));
        std::size_t activitySize = m_pActivityList.size();
        if(activitySize > 0)
        {
            std::future<TA_Variant> *pFArray = nullptr;
            pFArray = new std::future<TA_Variant> [activitySize];
            for(std::size_t i = sIndex;i < activitySize;++i)
            {
                pFArray[i] = TA_ThreadHolder::get().postActivity(TA_CommonTools::at<TA_BasicActivity *>(m_pActivityList, i));
            }
            for(int index = sIndex;index < activitySize;++index)
            {
                TA_Variant var = pFArray[index].get();
                TA_CommonTools::replace(m_resultList, index, std::forward<TA_Variant>(var));
                TA_Connection::active(this, &TA_ParallelPipeline::activityCompleted, index, var);;
            }
            delete [] pFArray;
            setState(State::Ready);
        }
    }
}
