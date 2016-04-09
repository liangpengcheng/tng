#pragma once
#include "instantiator.h"
#include "core/threadfun.h"
#include "core/runtime_check.h"
#include <unordered_map>
namespace tng
{
	template <class Base,class IDType,class LockType = tng::NullLock>
	class DynamicFactory
		/// A factory that creates objects by class name.
	{
	public:
		typedef AbstractInstantiator<Base> AbstractFactory;

		DynamicFactory()
			/// Creates the DynamicFactory.
		{
		}

		~DynamicFactory()
			/// Destroys the DynamicFactory and deletes the instantiators for 
			/// all registered classes.
		{
			for (typename FactoryMap::iterator it = map_factory_.begin(); it != map_factory_.end(); ++it)
			{
				delete it->second;
			}
		}

		Base* CreateObject(const IDType& className) const
			/// Creates a new instance of the class with the given name.
			/// The class must have been registered with registerClass.
			/// If the class name is unknown, a NotFoundException is thrown.
		{
			AutoLock<LockType> lock(&mutex_);

			typename FactoryMap::const_iterator it = map_factory_.find(className);
			if (it != map_factory_.end())
				return it->second->createInstance();
			else
				throw NotFoundException(className);
		}

		template <class C> 
		void RegisterClass(const IDType& className)
			/// Registers the instantiator for the given class with the DynamicFactory.
			/// The DynamicFactory takes ownership of the instantiator and deletes
			/// it when it's no longer used.
			/// If the class has already been registered, an ExistsException is thrown
			/// and the instantiator is deleted.
		{
			RegisterClass(className, new Instantiator<C, Base>);
		}

		void RegisterClass(const IDType& className, AbstractFactory* pAbstractFactory)
			/// Registers the instantiator for the given class with the DynamicFactory.
			/// The DynamicFactory takes ownership of the instantiator and deletes
			/// it when it's no longer used.
			/// If the class has already been registered, an ExistsException is thrown
			/// and the instantiator is deleted.
		{
			CheckPtr (pAbstractFactory);

			AutoLock<LockType> lock(&mutex_);

			typename FactoryMap::iterator it = map_factory_.find(className);
			if (it == map_factory_.end())
				map_factory_[className] = pAbstractFactory;
			else
				throw ExistsException(className);
		}

		void UnregisterClass(const IDType& className)
			/// Unregisters the given class and deletes the instantiator
			/// for the class.
			/// Throws a NotFoundException if the class has not been registered.
		{
			AutoLock<LockType> lock(&mutex_);

			typename FactoryMap::iterator it = map_factory_.find(className);
			if (it != map_factory_.end())
			{
				delete it->second;
				map_factory_.erase(it);
			}
			else throw NotFoundException(className);
		}

		bool IsClass(const IDType& className) const
			/// Returns true if the given class has been registered.
		{
			AutoLock<LockType> lock(&mutex_);

			return map_factory_.find(className) != map_factory_.end();
		}

	private:
		DynamicFactory(const DynamicFactory&);
		DynamicFactory& operator = (const DynamicFactory&);

		typedef unordered_map<IDType, AbstractFactory*> FactoryMap;

		FactoryMap map_factory_;
		mutable LockType mutex_;
	};

}