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
    // float *ptr = new float(5.3);
    std::cout << CoreAsync::IsInstanceVariable<decltype(&MetaTest::productMM)>::value << std::endl;
    std::cout << typeid(CoreAsync::VariableTypeInfo<decltype(&MetaTest::productMM)>::RetType).name() << std::endl;
    M3Test t, p;
    {
        CoreAsync::TA_Serialization output("./test.afw");
        t.setVec({2,3,4,5});
    //     t.setRawPtr(ptr);
    //     t.setArray({2,3,4,5,6});
    //     t.setList({9,9,9});
    //     t.setForwardList({9,9,9});
    //     t.setDeque({8,7,6,5,4});
    //     t.setStack({t.getDeque().begin(), t.getDeque().end()});
    //     t.setQueue({t.getDeque().begin(), t.getDeque().end()});
    //     t.setPrioritQueue({t.getDeque().begin(), t.getDeque().end()});
        output << t;
    }
    {
        CoreAsync::TA_Serialization<CoreAsync::OperationType::Input> input("./test.afw");
        input >> p;
    }
    EXPECT_EQ(t.getVec(), p.getVec());
    // EXPECT_EQ(*t.getRawPtr(), *p.getRawPtr());
    // EXPECT_EQ(t.getArray(), p.getArray());
    // EXPECT_EQ(t.getList(), p.getList());
    // EXPECT_EQ(t.getForwardList(), p.getForwardList());
    // EXPECT_EQ(t.getDeque(), p.getDeque());
    // EXPECT_EQ(t.getStack(), p.getStack());
    // EXPECT_EQ(t.getQueue(), p.getQueue());

    // auto pq1 {t.getPriorityQueue()};
    // auto pq2 {p.getPriorityQueue()};
    // EXPECT_TRUE(arePriorityQueueEqual(pq1, pq2));

    // delete ptr;
}
