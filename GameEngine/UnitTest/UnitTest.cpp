#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <algorithm>

class TestComponentA
{
public:
	int x = -1;
};

class TestComponentB
{
public:
	int y = -1;
};

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:		
        TEST_METHOD(Registry_CreateEntity)
        {
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			Entity entity1 = registry.CreateEntity();
			Entity entity2 = registry.CreateEntity();

			Assert::AreEqual(entity0, (Entity)0);
			Assert::AreEqual(entity1, (Entity)1);
			Assert::AreEqual(entity2, (Entity)2);
        }

		TEST_METHOD(Registry_CheckActiveEntities)
		{
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			Entity entity1 = registry.CreateEntity();

			Assert::IsTrue(std::find(registry.GetActiveEntities().begin(), registry.GetActiveEntities().end(), entity0) != registry.GetActiveEntities().end());
			Assert::IsTrue(std::find(registry.GetActiveEntities().begin(), registry.GetActiveEntities().end(), entity1) != registry.GetActiveEntities().end());
		}

		TEST_METHOD(Registry_CheckInactiveEntities)
		{
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			Entity entity1 = registry.CreateEntity();
			registry.DestroyEntity(entity0);

			Assert::IsTrue(std::find(registry.GetActiveEntities().begin(), registry.GetActiveEntities().end(), entity1) != registry.GetActiveEntities().end());
			Assert::IsFalse(std::find(registry.GetActiveEntities().begin(), registry.GetActiveEntities().end(), entity0) != registry.GetActiveEntities().end());
			Assert::IsTrue(std::find(registry.GetInactiveEntities().begin(), registry.GetInactiveEntities().end(), entity0) != registry.GetInactiveEntities().end());
		}
		TEST_METHOD(Registry_CreateChildParentRelation)
		{
			Registry registry;
			Entity parent = registry.CreateEntity();
			Entity child = registry.CreateEntity();
			registry.SetParent(child, parent);

			Assert::IsTrue(registry.GetParents()[child] == parent);
			Assert::IsTrue(std::find(registry.GetChildren()[parent].begin(), registry.GetChildren()[parent].end(), child) != registry.GetChildren()[parent].end());
			Assert::IsTrue(registry.HasParent(child));
			Assert::IsTrue(!registry.HasParent(parent));
		}

		TEST_METHOD(Registry_DestroyChildParentRelation)
		{
			Registry registry;
			Entity parent = registry.CreateEntity();
			Entity child = registry.CreateEntity();
			registry.SetParent(child, parent);
			registry.SetParent(child, null);

			Assert::AreNotEqual(registry.GetParents()[child], parent);
			Assert::AreEqual(registry.GetParents()[child], null);
			Assert::IsFalse(std::find(registry.GetChildren()[parent].begin(), registry.GetChildren()[parent].end(), child) != registry.GetChildren()[parent].end());
			Assert::IsTrue(!registry.HasParent(child));
			Assert::IsTrue(!registry.HasParent(parent));
		}

		TEST_METHOD(Registry_IsDeepConnectedChildParentRelation)
		{
			Registry registry;
			Entity grand = registry.CreateEntity();
			Entity parent = registry.CreateEntity();
			Entity child = registry.CreateEntity();
			registry.SetParent(parent, grand);
			registry.SetParent(child, parent);

			Assert::IsTrue(registry.IsDeepConnected(grand, child));
		}

		TEST_METHOD(Registry_RegisterComponent)
		{
			Registry registry;
			Entity entity = registry.CreateEntity();
			registry.AddComponent<TestComponentA>(entity);

			Assert::IsTrue(registry.GetComponentPool<TestComponentA>() != nullptr);
			Assert::IsTrue(registry.GetComponentPool<TestComponentA>()->GetSize() == 1);
			Assert::IsTrue(registry.HasComponent<TestComponentA>(entity));
		}

		TEST_METHOD(Registry_RegisterMultipleComponent)
		{
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			Entity entity1 = registry.CreateEntity();
			Entity entity2 = registry.CreateEntity();
			registry.AddComponent<TestComponentA>(entity0);
			registry.AddComponent<TestComponentA>(entity1);
			registry.AddComponent<TestComponentA>(entity2);

			Assert::IsTrue(registry.GetComponentPool<TestComponentA>() != nullptr);
			Assert::IsTrue(registry.GetComponentPool<TestComponentA>()->GetSize() == 3);
			Assert::IsTrue(registry.HasComponent<TestComponentA>(entity0));
			Assert::IsTrue(registry.HasComponent<TestComponentA>(entity1));
			Assert::IsTrue(registry.HasComponent<TestComponentA>(entity2));
		}

		TEST_METHOD(Registry_RegisterMultipleDifferentComponent)
		{
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			registry.AddComponent<TestComponentA>(entity0);
			registry.AddComponent<TestComponentB>(entity0);

			Assert::IsTrue(registry.GetComponentPool<TestComponentA>() != nullptr);
			Assert::IsTrue(registry.GetComponentPool<TestComponentB>() != nullptr);
			Assert::IsTrue(registry.HasComponent<TestComponentA>(entity0));
			Assert::IsTrue(registry.HasComponent<TestComponentB>(entity0));
		}

		TEST_METHOD(Registry_RegisterExistingComponent)
		{
			TestComponentA testComponent;
			testComponent.x = 10;

			Registry registry;
			Entity entity0 = registry.CreateEntity();
			registry.AddComponent<TestComponentA>(entity0, testComponent);

			Assert::IsTrue(registry.GetComponent<TestComponentA>(entity0).x == 10);
		}
		TEST_METHOD(Registry_DeleteComponent)
		{
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			registry.AddComponent<TestComponentA>(entity0);
			registry.RemoveComponent<TestComponentA>(entity0);

			Assert::IsTrue(registry.GetComponentPool<TestComponentA>()->GetSize() == 0);
			Assert::IsFalse(registry.HasComponent<TestComponentA>(entity0));
		}

		TEST_METHOD(Registry_DeleteEntityAndComponents)
		{
			Registry registry;
			Entity entity0 = registry.CreateEntity();
			registry.AddComponent<TestComponentA>(entity0);
			registry.AddComponent<TestComponentB>(entity0);

			Assert::IsTrue(registry.GetComponentPool<TestComponentA>()->GetSize() == 1);
			Assert::IsTrue(registry.GetComponentPool<TestComponentB>()->GetSize() == 1);

			registry.DestroyEntity(entity0);

			Assert::IsTrue(registry.GetComponentPool<TestComponentA>()->GetSize() == 0);
			Assert::IsTrue(registry.GetComponentPool<TestComponentB>()->GetSize() == 0);
		}

		TEST_METHOD(Pool_AddNewEntity)
		{
			Entity entity0 = 0;
			Entity entity1 = 1;
			Entity entity2 = 2;
			Pool<TestComponentA> pool;
			pool.AddNewEntity(entity0);
			pool.AddNewEntity(entity1);
			pool.AddNewEntity(entity2);
			
			Assert::IsTrue(pool.GetSparseEntitiesArray().size() == 3);
		}

		TEST_METHOD(Pool_CheckSparseArrayContent)
		{
			Entity entity0 = 0;
			Entity entity1 = 1;
			Entity entity2 = 2;
			Pool<TestComponentA> pool;
			pool.AddNewEntity(entity0);
			pool.AddNewEntity(entity1);
			pool.AddNewEntity(entity2);

			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity0] == null);
			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity1] == null);
			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity2] == null);
		}

		TEST_METHOD(Pool_AddComponentAndCheckSparseDenseArrays)
		{
			Entity entity0 = 0;
			Entity entity1 = 1;
			Pool<TestComponentA> pool;
			pool.AddNewEntity(entity0);
			pool.AddNewEntity(entity1);

			pool.AddComponent(entity1);
			pool.AddComponent(entity0);

			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity1] == 0);
			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity0] == 1);
			
			Assert::IsTrue(pool.GetDenseEntitiesArray()[0] == entity1);
			Assert::IsTrue(pool.GetDenseEntitiesArray()[1] == entity0);
		}

		TEST_METHOD(Pool_RemoveComponentAndCheckSparseDenseArrays)
		{
			Entity entity0 = 0;
			Entity entity1 = 1;
			Pool<TestComponentA> pool;
			pool.AddNewEntity(entity0);
			pool.AddNewEntity(entity1);

			pool.AddComponent(entity0);
			pool.AddComponent(entity1);

			pool.RemoveComponent(entity0);

			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity0] == null);
			Assert::IsTrue(pool.GetSparseEntitiesArray()[entity1] == 0);

			Assert::IsTrue(pool.GetDenseEntitiesArray().size() == 1);
			Assert::IsTrue(pool.GetDenseEntitiesArray()[0] == entity1);
		}

		TEST_METHOD(Pool_FlagMethodes)
		{
			Entity entity0 = 0;
			Pool<TestComponentA> pool;
			pool.AddNewEntity(entity0);
			pool.AddComponent(entity0);
			pool.ResFlag(entity0, UPDATE_FLAG);
			pool.ResFlag(entity0, REGENERATE_FLAG);
			
			Assert::IsFalse(pool.IsFlagSet(entity0, UPDATE_FLAG));
			Assert::IsFalse(pool.IsFlagSet(entity0, REGENERATE_FLAG));

			pool.SetFlag(entity0, UPDATE_FLAG);
			pool.SetFlag(entity0, REGENERATE_FLAG);

			Assert::IsTrue(pool.IsFlagSet(entity0, UPDATE_FLAG));
			Assert::IsTrue(pool.IsFlagSet(entity0, REGENERATE_FLAG));
		}

		TEST_METHOD(Pool_GetComponentIndex)
		{
			Entity entity0 = 0;
			Entity entity1 = 1;
			Entity entity2 = 2;

			Pool<TestComponentA> pool;
			pool.AddNewEntity(entity0);
			pool.AddNewEntity(entity1);
			pool.AddNewEntity(entity2);

			pool.AddComponent(entity1);
			pool.AddComponent(entity2);
			pool.AddComponent(entity0);

			//Sparse -> [2, 0, 1]
			//Dense -> [entity1, entity2, entity0]

			Assert::IsTrue(pool.GetIndex(entity0) == 2);
			Assert::IsTrue(pool.GetIndex(entity1) == 0);
			Assert::IsTrue(pool.GetIndex(entity2) == 1);
		}
	};
}
