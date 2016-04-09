/*
*  SmartPoint.h
*  Created by LiangPengcheng on 08-5-26.
*  Copyright 2008 Quartz Game Studio. All rights reserved.
*
*/
#ifndef _KERNELRESMGR_H_
#define _KERNELRESMGR_H_
#include "types.h"
namespace tng
{
template<typename T>
class refCounter
{
public:

    refCounter():ptr_(NULL),ref_counter_(0) {}
    refCounter(T* p)
    {
        ptr_ = p;
        ref_counter_ = 1;
    }
    ~refCounter()
    {
        delete (ptr_);
    }

    __inline refCounter<T>& operator = (const refCounter<T>& rc)
    {
        ptr_ = rc.ptr;
        ref_counter_ = rc.m_refCounter;
    }
    template<typename T2>
    __inline refCounter<T>& operator = (const refCounter<T2>& rc)
    {
        ptr_ = static_cast<T*>(rc.ptr);
        ref_counter_ = rc.m_refCounter;
    }

    __inline void	addRef()const {
        ref_counter_++;
    }
    __inline void	decRef()const {
        ref_counter_--;
    }

    mutable T*		ptr_;
    mutable u32		ref_counter_;
};


template<typename T>
class SmartPtr
{
public:

    SmartPtr():ref_ptr_(NULL) {}
    ~SmartPtr()
    {
        if (ref_ptr_)
        {
            decRef();
            if (type_ref_ptr_->ref_counter_ == 0)
            {
                delete(type_ref_ptr_);
                ref_ptr_ = 0;
            }
        }

    }
    template<typename T2>
    __inline SmartPtr<T>& operator = (SmartPtr<T2>& t2)
    {
        if (ref_ptr_)
        {
            decRef();
            if (type_ref_ptr_->ref_counter_ == 0)
            {
                delete(type_ref_ptr_);
                ref_ptr_ = 0;
            }
        }
		if (t2.ref_ptr_)
        {
            t2.addRef();
			ref_ptr_ = t2.ref_ptr_;

        }
        return *this;
    }
    SmartPtr(const SmartPtr<T>& dat):ref_ptr_(NULL)
    {
		if (dat.ref_ptr_)
        {
            dat.addRef();
        }
		ref_ptr_ = dat.ref_ptr_;
    }
    template<typename T2>
    SmartPtr(SmartPtr<T2>& dat):ref_ptr_(NULL)
    {
        if (dat.ref_ptr_)
        {
            dat.addRef();
        }
        ref_ptr_ = dat.ref_ptr_;
    }
    explicit SmartPtr(T* dat)
    {
        if (dat)
        {
            ref_ptr_ = new refCounter<T>;
            CastRef<T>()->ptr_ = dat;
            addRef();
        }
        else
        {
            ref_ptr_ = NULL;
        }


    }
    __inline SmartPtr<T>&		operator = (const SmartPtr<T>& dat)
    {
        if (ref_ptr_)
        {
            decRef();
            if (type_ref_ptr_->ref_counter_ == 0)
            {
                delete(type_ref_ptr_);
                ref_ptr_ = 0;
            }
        }

        if (dat.ref_ptr_)
        {
            dat.addRef();
        }
        ref_ptr_ = dat.ref_ptr_;
        return *this;
    }

    __inline friend bool operator >(const SmartPtr<T>& dat1,const SmartPtr<T>& dat) {
        return dat1.type_ref_ptr_&&dat.type_ref_ptr_?
               upointer(dat1.type_ref_ptr_->ptr_)>upointer(dat.type_ref_ptr_->ptr_):0;
    }
    __inline friend bool operator <(const SmartPtr<T>& dat1,const SmartPtr<T>& dat) {
        return dat1.type_ref_ptr_&&dat.type_ref_ptr_?
               upointer(dat1.type_ref_ptr_->ptr_)<upointer(dat.type_ref_ptr_->ptr_):0;
    }

    __inline T*			getPtr()const {
        return (T*)ref_ptr_?(type_ref_ptr_->ptr_?type_ref_ptr_->ptr_:0):0;
    }
    __inline T*			operator->()const {
        return (T*)(CastRef()->ptr_);
    }
    operator bool()const
    {
        return getPtr()?true:false;
    }
    __inline friend T&	operator*(SmartPtr<T>& dat) {
        return *(dat.type_ref_ptr_->ptr_);
    }

    __inline friend bool		operator == (const SmartPtr<T>& dat,const SmartPtr<T>& dat1) {
        return dat1.type_ref_ptr_==0&&dat.type_ref_ptr_==0?1:(dat1.type_ref_ptr_==dat.type_ref_ptr_)&&(dat.type_ref_ptr_->ptr_ == dat1.type_ref_ptr_->ptr_);
    }
    __inline friend bool		operator != (const SmartPtr<T>& dat,SmartPtr<T>& dat1) {
        return !(dat==dat1);
    }

public:
    __inline void		addRef()const {
        type_ref_ptr_->addRef();
    }
    __inline void		decRef()const {
        type_ref_ptr_->decRef();
    }
    template<typename T2>
    refCounter<T2>*		CastRef() {
        return (refCounter<T2>*)(type_ref_ptr_);
    }

    refCounter<T>*		CastRef() {
        return type_ref_ptr_;
    }

    template<typename T2>
    const refCounter<T2>*		CastRef()const {
        return (refCounter<T2>*)(type_ref_ptr_);
    }

    const refCounter<T>*		CastRef()const {
        return type_ref_ptr_;
    }
    union
    {
        refCounter<T>*	type_ref_ptr_;
        void*			ref_ptr_;
    };

};
}

#endif



