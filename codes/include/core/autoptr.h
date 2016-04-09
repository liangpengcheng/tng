#pragma once
#include "memobj.h"
#include "exception.h"
#include <algorithm>


namespace tng {

	//template param must be a sub class of Object
	template <class C>
	class AutoPtr :public MemObj
		/// AutoPtr is a "smart" pointer for classes implementing
		/// reference counting based garbage collection.
		/// To be usable with the AutoPtr template, a class must
		/// implement the following behaviour:
		/// A class must maintain a reference count.
		/// The constructors of the object initialize the reference
		/// count to one.
		/// The class must implement a public Duplicate() method:
		///     void Duplicate();
		/// that increments the reference count by one.
		/// The class must implement a public Release() method:
		///     void Release()
		/// that decrements the reference count by one, and,
		/// if the reference count reaches zero, deletes the
		/// object.
		///
		/// AutoPtr works in the following way:
		/// If an AutoPtr is assigned an ordinary pointer to
		/// an object (via the constructor or the assignment operator),
		/// it takes ownership of the object and the object's reference
		/// count remains unchanged.
		/// If the AutoPtr is assigned another AutoPtr, the
		/// object's reference count is incremented by one by
		/// calling Duplicate() on its object.
		/// The destructor of AutoPtr calls Release() on its
		/// object.
		/// AutoPtr supports dereferencing with both the ->
		/// and the * operator. An attempt to dereference a null
		/// AutoPtr results in a NullPointerException being thrown.
		/// AutoPtr also implements all relational operators.
		/// Note that AutoPtr allows casting of its encapsulated data types.
	{
	public:
		AutoPtr() : ptr_(0)
		{
		}

		AutoPtr(C* ptr) : ptr_(ptr)
		{
		}

		AutoPtr(C* ptr, bool shared) : ptr_(ptr)
		{
			if (shared && ptr_) ptr_->Duplicate();
		}

		AutoPtr(const AutoPtr& ptr) : ptr_(ptr.ptr_)
		{
			if (ptr_) ptr_->Duplicate();
		}

		template <class Other>
		AutoPtr(const AutoPtr<Other>& ptr) : ptr_(const_cast<Other*>(ptr.get()))
		{
			if (ptr_) ptr_->Duplicate();
		}

		~AutoPtr()
		{
			if (ptr_) ptr_->Release();
		}

		AutoPtr& assign(C* ptr)
		{
			if (ptr_ != ptr)
			{
				if (ptr_) ptr_->Release();
				ptr_ = ptr;
			}
			return *this;
		}

		AutoPtr& assign(C* ptr, bool shared)
		{
			if (ptr_ != ptr)
			{
				if (ptr_) ptr_->Release();
				ptr_ = ptr;
				if (shared && ptr_) ptr_->Duplicate();
			}
			return *this;
		}

		AutoPtr& assign(const AutoPtr& ptr)
		{
			if (&ptr != this)
			{
				if (ptr_) ptr_->Release();
				ptr_ = ptr.ptr_;
				if (ptr_) ptr_->Duplicate();
			}
			return *this;
		}

		template <class Other>
		AutoPtr& assign(const AutoPtr<Other>& ptr)
		{
			if (ptr.get() != ptr_)
			{
				if (ptr_) ptr_->Release();
				ptr_ = const_cast<Other*>(ptr.get());
				if (ptr_) ptr_->Duplicate();
			}
			return *this;
		}

		AutoPtr& operator = (C* ptr)
		{
			return assign(ptr);
		}

		AutoPtr& operator = (const AutoPtr& ptr)
		{
			return assign(ptr);
		}

		template <class Other>
		AutoPtr& operator = (const AutoPtr<Other>& ptr)
		{
			return assign<Other>(ptr);
		}

		void swap(AutoPtr& ptr)
		{
			std::swap(ptr_, ptr.ptr_);
		}
		// 
		//     template <class Other>
		//     AutoPtr<Other> cast() const
		//     /// Casts the AutoPtr via a dynamic cast to the given type.
		//     /// Returns an AutoPtr containing NULL if the cast fails.
		//     /// Example: (assume class Sub: public Super)
		//     ///    AutoPtr<Super> super(new Sub());
		//     ///    AutoPtr<Sub> sub = super.cast<Sub>();
		//     ///    poco_assert (sub.get());
		//     {
		//         Other* pOther = _ptr->DynamicCast<Other*>();
		//         return AutoPtr<Other>(pOther, true);
		//     }

		template <class Other>
		AutoPtr<Other> unsafeCast() const
			/// Casts the AutoPtr via a static cast to the given type.
			/// Example: (assume class Sub: public Super)
			///    AutoPtr<Super> super(new Sub());
			///    AutoPtr<Sub> sub = super.unsafeCast<Sub>();
			///    poco_assert (sub.get());
		{
			Other* pOther = static_cast<Other*>(ptr_);
			return AutoPtr<Other>(pOther, true);
		}

		C* operator -> ()
		{
			if (ptr_)
				return ptr_;
			else
				throw NullPointerException();
		}

		const C* operator -> () const
		{
			if (ptr_)
				return ptr_;
			else
				throw NullPointerException();
		}

		C& operator * ()
		{
			if (ptr_)
				return *ptr_;
			else
				throw NullPointerException();
		}

		const C& operator * () const
		{
			if (ptr_)
				return *ptr_;
			else
				throw NullPointerException();
		}

		C* get()
		{
			return ptr_;
		}

		const C* get() const
		{
			return ptr_;
		}

		operator C* ()
		{
			return ptr_;
		}

		operator const C* () const
		{
			return ptr_;
		}

		bool operator ! () const
		{
			return ptr_ == 0;
		}

		bool isNull() const
		{
			return ptr_ == 0;
		}

		C* Duplicate()
		{
			if (ptr_) ptr_->Duplicate();
			return ptr_;
		}

		bool operator == (const AutoPtr& ptr) const
		{
			return ptr_ == ptr.ptr_;
		}

		bool operator == (const C* ptr) const
		{
			return ptr_ == ptr;
		}

		bool operator == (C* ptr) const
		{
			return ptr_ == ptr;
		}

		bool operator != (const AutoPtr& ptr) const
		{
			return ptr_ != ptr.ptr_;
		}

		bool operator != (const C* ptr) const
		{
			return ptr_ != ptr;
		}

		bool operator != (C* ptr) const
		{
			return ptr_ != ptr;
		}

		bool operator < (const AutoPtr& ptr) const
		{
			return ptr_ < ptr.ptr_;
		}

		bool operator < (const C* ptr) const
		{
			return ptr_ < ptr;
		}

		bool operator < (C* ptr) const
		{
			return ptr_ < ptr;
		}

		bool operator <= (const AutoPtr& ptr) const
		{
			return ptr_ <= ptr.ptr_;
		}

		bool operator <= (const C* ptr) const
		{
			return ptr_ <= ptr;
		}

		bool operator <= (C* ptr) const
		{
			return ptr_ <= ptr;
		}

		bool operator > (const AutoPtr& ptr) const
		{
			return ptr_ > ptr.ptr_;
		}

		bool operator > (const C* ptr) const
		{
			return ptr_ > ptr;
		}

		bool operator > (C* ptr) const
		{
			return ptr_ > ptr;
		}

		bool operator >= (const AutoPtr& ptr) const
		{
			return ptr_ >= ptr.ptr_;
		}

		bool operator >= (const C* ptr) const
		{
			return ptr_ >= ptr;
		}

		bool operator >= (C* ptr) const
		{
			return ptr_ >= ptr;
		}

	private:
		C* ptr_;
	};


	template <class C>
	inline void swap(AutoPtr<C>& p1, AutoPtr<C>& p2)
	{
		p1.swap(p2);
	}


} //
