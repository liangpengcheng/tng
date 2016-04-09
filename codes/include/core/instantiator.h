#pragma once
#include "core/memobj.h"
namespace tng
{
	//from poco
	template <class Base>
	class AbstractInstantiator:public MemObj
		/// The common base class for all Instantiator instantiations.
		/// Used by DynamicFactory.
	{
	public:
		AbstractInstantiator()
			/// Creates the AbstractInstantiator.
		{
		}
		
		virtual ~AbstractInstantiator()
			/// Destroys the AbstractInstantiator.
		{
		}
		
		virtual Base* createInstance() const = 0;
			/// Creates an instance of a concrete subclass of Base.	

	private:
		AbstractInstantiator(const AbstractInstantiator&);
		AbstractInstantiator& operator = (const AbstractInstantiator&);
	};


	template <class C, class Base>
	class Instantiator: public AbstractInstantiator<Base>
		/// A template class for the easy instantiation of 
		/// instantiators. 
		///
		/// For the Instantiator to work, the class of which
		/// instances are to be instantiated must have a no-argument
		/// constructor.
	{
	public:
		Instantiator()
			/// Creates the Instantiator.
		{
		}
		
		virtual ~Instantiator()
			/// Destroys the Instantiator.
		{
		}

		Base* createInstance() const
		{
			return new C;
		}
	};
}