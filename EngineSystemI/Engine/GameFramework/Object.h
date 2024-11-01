#pragma once
namespace eae6320
{
	namespace GameFramework
	{
		class UObject
		{
		public:
			UObject() = default;
			~UObject() = default;

			virtual void CleanUp() {}
		};
	}
}