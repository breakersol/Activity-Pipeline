/*
 * Copyright [2024] [Shuang Zhu / Sol]
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

#include "TA_ThreadPoolTest.h"
#include "ITA_ActivityCreator.h"

TA_ThreadPoolTest::TA_ThreadPoolTest()
{

}

TA_ThreadPoolTest::~TA_ThreadPoolTest()
{

}

void TA_ThreadPoolTest::SetUp()
{
    std::function<int(int)> func = [](int a) {
//        std::printf("%d\n", a);
//        std::printf("%d has completed an activity!\n", std::this_thread::get_id());
        return a;
    };
    for(int i = 0;i < activities.size();++i)
    {
        auto activity = CoreAsync::ITA_ActivityCreator::create(func,std::move(i));
        activities[i] = new CoreAsync::TA_ActivityProxy(activity);
    }
}

void TA_ThreadPoolTest::TearDown()
{
    for(auto &activity : activities)
    {
        if(activity)
        {
            delete activity;
            activity = nullptr;
        }
    }
}

TEST_F(TA_ThreadPoolTest, postActivityTest)
{
    CoreAsync::TA_ThreadPool threadPool;
    auto ft = threadPool.postActivity(activities[0]);
    EXPECT_EQ(0, ft().get<int>());
}

TEST_F(TA_ThreadPoolTest, notifyResultTest)
{
    CoreAsync::TA_ThreadPool threadPool;
    std::vector<CoreAsync::ActivityResultFetcher> testVec;
    std::vector<int> validVec(1024);
    for(int i = 0;i < activities.size();++i)
    {
        testVec.emplace_back(threadPool.postActivity(activities[i]));
        validVec[i] = i;
    }
    EXPECT_EQ(testVec.size(), validVec.size());
    for(int i = 0;i < testVec.size();++i)
    {
        EXPECT_EQ(testVec[i]().get<int>(), validVec[i]);
    }
}

TEST_F(TA_ThreadPoolTest, threadSizeTest)
{
    CoreAsync::TA_ThreadPool threadPool;
    EXPECT_EQ(std::thread::hardware_concurrency(), threadPool.size());
}
