#include "TA_SerializationTest.h"
#include "Components/TA_Serialization.h"
#include "Components/TA_TypeFilter.h"

TA_SerializationTest::TA_SerializationTest()
{

}

TA_SerializationTest::~TA_SerializationTest()
{

}

void TA_SerializationTest::SetUp()
{

}

void TA_SerializationTest::TearDown()
{

}

TEST_F(TA_SerializationTest, CustomTypeTest)
{
    M3Test t, p;
    {
        CoreAsync::TA_Serialization output("./test.afw");
        t.setVec({2,3,4,5});
        output << t;
    }
    {
        CoreAsync::TA_Serialization<CoreAsync::OperationType::Input> input("./test.afw");
        input >> p;
    }
    EXPECT_EQ(t.getVec(), p.getVec());
}
