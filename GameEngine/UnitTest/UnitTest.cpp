#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:		
        TEST_METHOD(TestRemoveEntity)
        {
            Pool<int> pool;
            Entity entity = 3;
            pool.AdjustPool(entity + 1);
            pool.AddNewEntity(entity);
            pool.RemoveEntity(entity);

            Assert::IsFalse(pool.HasComponent(entity), L"Entity should not have a component after being removed.");
        }

        TEST_METHOD(TestAddComponent)
        {
            Pool<int> pool;
            Entity entity = 10;
            pool.AdjustPool(entity + 1);
            int component = 42;
            pool.AddComponent(entity, component);

            Assert::IsTrue(pool.HasComponent(entity), L"Component should be added to entity.");
            Assert::AreEqual(component, pool.GetComponent(entity), L"Component value should match the added value.");
        }

        TEST_METHOD(TestRemoveComponent)
        {
            Pool<int> pool;
            Entity entity = 6;
            int component = 55;
            pool.AddComponent(entity, component);
            pool.RemoveComponent(entity);

            Assert::IsFalse(pool.HasComponent(entity), L"Entity should no longer have the component after removal.");
        }

        TEST_METHOD(TestGetComponent_Default)
        {
            Pool<int> pool;
            Entity entity = 7;
            pool.AdjustPool(entity + 1);
            int defaultComponent = 0;

            Assert::AreEqual(defaultComponent, pool.GetComponent(entity), L"Non-existent entity should return default component.");
        }

        TEST_METHOD(TestSetFlag)
        {
            Pool<int> pool;
            Entity entity = 12;
            pool.AdjustPool(entity + 1);
            pool.AddComponent(entity, 100);
            pool.SetFlag(entity, UPDATE_FLAG);

            Assert::IsTrue(pool.IsFlagSet(entity, UPDATE_FLAG), L"UPDATE_FLAG should be set for the entity.");
        }

        TEST_METHOD(TestResetFlag)
        {
            Pool<int> pool;
            Entity entity = 13;
            pool.AdjustPool(entity + 1);
            pool.AddComponent(entity, 200);
            pool.SetFlag(entity, REGENERATE_FLAG);
            pool.ResFlag(entity, REGENERATE_FLAG);

            Assert::IsFalse(pool.IsFlagSet(entity, REGENERATE_FLAG), L"REGENERATE_FLAG should be cleared for the entity.");
        }

        TEST_METHOD(TestAdjustPool)
        {
            Pool<int> pool;
            unsigned int newSize = 20;
            pool.AdjustPool(newSize + 1);

            Assert::AreEqual(newSize, static_cast<unsigned int>(pool.GetSparseEntitiesArray().size()), L"Pool size should be adjusted.");
        }

        TEST_METHOD(TestAddMultipleEntities)
        {
            Pool<unsigned int> pool;
            for (Entity i = 0; i < 5; ++i)
            {
                pool.AddNewEntity(i);
                pool.AddComponent(i, i * 10);
            }

            for (Entity i = 0; i < 5; ++i)
            {
                Assert::IsTrue(pool.HasComponent(i), L"Each entity should have its component.");
                Assert::AreEqual(i * 10, pool.GetComponent(i), L"Component values should match the added values.");
            }
        }

        TEST_METHOD(TestGetIndex)
        {
            Pool<int> pool;
            Entity entity = 8;
            pool.AdjustPool(entity + 1);
            pool.AddComponent(entity, 400);

            unsigned int index = pool.GetIndex(entity);
            Assert::AreEqual(index, 0u, L"The index should be correct after adding one component.");
        }

        TEST_METHOD(TestFlagsArraySizeConsistency)
        {
            Pool<int> pool;
            Entity entity1 = 9;
            Entity entity2 = 15;
            pool.AdjustPool(entity2 + 1);

            pool.AddComponent(entity1, 500);
            pool.AddComponent(entity2, 600);

            Assert::AreEqual(pool.GetDenseFlagsArray().size(), pool.GetDenseComponentsArray().size(), L"Dense arrays should be consistent in size.");
            Assert::AreEqual(pool.GetDenseEntitiesArray().size(), pool.GetDenseFlagsArray().size(), L"Dense arrays should be consistent in size.");
        }

        TEST_METHOD(TestRemoveLastEntity)
        {
            Pool<int> pool;
            Entity entity1 = 16;
            Entity entity2 = 17;
            pool.AdjustPool(entity2 + 1);
            pool.AddComponent(entity1, 700);
            pool.AddComponent(entity2, 800);

            pool.RemoveComponent(entity2);

            Assert::IsFalse(pool.HasComponent(entity2), L"Entity2 should be removed.");
            Assert::IsTrue(pool.HasComponent(entity1), L"Entity1 should still be present.");
            Assert::AreEqual(pool.GetComponent(entity1), 700, L"Entity1's component value should remain intact.");
        }

        TEST_METHOD(TestRemoveEntityAndReorder)
        {
            Pool<int> pool;
            Entity entity1 = 18;
            Entity entity2 = 19;
            pool.AdjustPool(entity2 + 1);
            pool.AddComponent(entity1, 900);
            pool.AddComponent(entity2, 1000);

            pool.RemoveComponent(entity1);

            Assert::IsTrue(pool.HasComponent(entity2), L"Entity2 should remain in the pool.");
            Assert::AreEqual(pool.GetComponent(entity2), 1000, L"Entity2's component value should be intact.");
        }
	};
}
