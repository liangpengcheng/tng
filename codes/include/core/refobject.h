#ifndef __TNG_REFOBJ_H__
#define __TNG_REFOBJ_H__

#include "core/memobj.h"
#include "core/types.h"
#include "core/core_exports.h"
namespace tng
{
	//from poco
	class CORE_API RefCountedObject :public MemObj
	{
	public:
		RefCountedObject() :ref_counter_(1) {}
		/// Creates the RefCountedObject.
		/// The initial reference count is one.

		void Duplicate() const;
		/// Increments the object's reference count.

		void Release() const;
		/// Decrements the object's reference count
		/// and deletes the object if the count
		/// reaches zero.

		int ReferenceCount() const;
		/// Returns the reference count.

	protected:
		virtual ~RefCountedObject() {}
		/// Destroys the RefCountedObject.

	private:
		RefCountedObject(const RefCountedObject&) {}
		RefCountedObject& operator = (const RefCountedObject&) { return *this; }

		mutable u32 ref_counter_;
	};
	//
	// inlines
	//
	inline int RefCountedObject::ReferenceCount() const
	{
		return ref_counter_;
	}


	inline void RefCountedObject::Duplicate() const
	{
		++ref_counter_;
	}


	inline void RefCountedObject::Release() const
	{
		if (--ref_counter_ == 0) delete this;
	}

}
#endif