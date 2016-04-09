

#if TNG_OS == TNG_OS_WINDOWS_NT
#define HASATOMIC	1
#elif TNG_OS == TNG_OS_MAC_OS_X
#define HASATOMIC	1
#elif ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1) || __GNUC__ > 4 || defined(__clang__)) && (defined(__x86_64__) || defined(__i386__))
#define HASATOMIC	1
#define HAVE_GCC_ATOMICS
#elif (TNG_OS_FAMILY_UNIX&&(defined(TNG_PPC)||defined(TNG_PPC64)))
#ifdef PPC405_ERRATA
#   define PPC405_ERR77_SYNC   "	sync\n"
#else
#   define PPC405_ERR77_SYNC
#endif
#define HAVE_PPC_ATOMICS
#define HASATOMIC	1
#else
#define HASATOMIC	0
#endif


class AtomicCounter :public MemObj
{
public:
#if TNG_OS_FAMILY_WINDOWS
	typedef long ValueType;
#elif TNG_OS== TNG_OS_MAC_OS_X
	typedef int32_t ValueType; 
#elif defined(HAVE_PPC_ATOMICS)
	typedef u32	ValueType;
#else
	typedef s32	ValueType;
#endif
	__inline AtomicCounter() :internal_val_(0){}


	__inline explicit AtomicCounter(ValueType initialValue) :internal_val_(initialValue){}


	__inline AtomicCounter(const AtomicCounter& counter){
		*this = counter;
	}


	__inline ~AtomicCounter(){}


	__inline AtomicCounter& operator = (const AtomicCounter& counter)
	{
#if TNG_OS_FAMILY_WINDOWS
		InterlockedExchange(&internal_val_, counter.value());
#elif TNG_OS== TNG_OS_MAC_OS_X
		internal_val_ = counter.value();
#elif TNG_OS==TNG_OS_SOLARIS
		atomic_swap_32(&internal_val_, counter.value());
#elif defined(HAVE_GCC_ATOMICS)
		__sync_lock_test_and_set(&internal_val_, counter.value());

#elif defined(HAVE_PPC_ATOMICS)
		__asm__ __volatile__("stw%U0%X0 %1,%0" : "=m"(internal_val_) : "r"(counter.value()));
#else
		AutoLock<Mutex> scopelock(&lock_);
		internal_val_ = counter.value();
#endif

		return *this;
	}


	__inline AtomicCounter& operator = (ValueType value)
	{
#if TNG_OS_FAMILY_WINDOWS
		InterlockedExchange(&internal_val_, value);
#elif TNG_OS== TNG_OS_MAC_OS_X
		internal_val_ = value;
#elif defined(HAVE_GCC_ATOMICS)
		__sync_lock_test_and_set(&internal_val_, value);
#elif TNG_OS==TNG_OS_SOLARIS
		atomic_swap_32(&internal_val_, value);
#elif defined(HAVE_PPC_ATOMICS)
		__asm__ __volatile__("stw%U0%X0 %1,%0" : "=m"(internal_val_) : "r"(value));
#else
		AutoLock<Mutex> scopelock(&lock_);
		internal_val_ = value;
#endif
		return *this;
	}


	__inline operator ValueType () const{
#if HASATOMIC ==0
		AutoLock<Mutex> scopelock(&lock_);
#elif defined(HAVE_PPC_ATOMICS)
		int t;
		__asm__ __volatile__("lwz%U1%X1 %0,%1" : "=r"(t) : "m"(internal_val_));		
		return t;
#endif
		return internal_val_;
	}


	__inline ValueType value() const{
#if HASATOMIC ==0
		AutoLock<Mutex> scopelock(&lock_);
#elif defined(HAVE_PPC_ATOMICS)
		int t;
		__asm__ __volatile__("lwz%U1%X1 %0,%1" : "=r"(t) : "m"(internal_val_));
#endif
		return internal_val_;
	}


	__inline ValueType operator ++ ()
	{
#if TNG_OS_FAMILY_WINDOWS
		return InterlockedIncrement(&internal_val_);
#elif TNG_OS== TNG_OS_MAC_OS_X
		return OSAtomicIncrement32(static_cast<int32_t*>(&internal_val_));
#elif defined(HAVE_GCC_ATOMICS)
		return __sync_add_and_fetch(&internal_val_, 1);
#elif TNG_OS==TNG_OS_SOLARIS
		return atomic_inc_32_nv(&internal_val_);
#elif defined(HAVE_PPC_ATOMICS)
		int t;
		__asm__ __volatile__("\n\
							 								 1:	lwarx	%0,0,%2\n\
															 								 addic	%0,%0,1\n\
																							 								 stwcx.	%0,0,%2\n\
																															 								 bne-	1b"
																																							 : "=&r" (t), "=m" (internal_val_)
																																							 : "r" (&internal_val_), "m" (internal_val_)
																																							 : "cc");	
		return t;
#else
		AutoLock<Mutex> scopelock(&lock_);
		return ++internal_val_;
#endif

	}


	__inline ValueType operator ++ (int)
	{
#if TNG_OS_FAMILY_WINDOWS
		ValueType result = InterlockedIncrement(&internal_val_);
		return --result;

#elif TNG_OS== TNG_OS_MAC_OS_X
		ValueType result = OSAtomicIncrement32(static_cast<int32_t*>(&internal_val_));
		return --result;
#elif defined(HAVE_GCC_ATOMICS)
		return __sync_fetch_and_add(&internal_val_, 1);
#elif TNG_OS==TNG_OS_SOLARIS
		return atomic_inc_32_nv(&internal_val_) - 1;
#elif defined(HAVE_PPC_ATOMICS)
		int t;
		__asm__ __volatile__("\n\
							 							 1:	lwarx	%0,0,%2\n\
														 							 addic	%0,%0,1\n\
																					 							 stwcx.	%0,0,%2\n\
																												 							 bne-	1b"
																																			 : "=&r" (t), "=m" (internal_val_)
																																			 : "r" (&internal_val_), "m" (internal_val_)
																																			 : "cc");	
		return t-1;
#else
		AutoLock<Mutex> scopelock(&lock_);
		return internal_val_++;
#endif
	}



	__inline ValueType operator -- ()
	{
#if TNG_OS_FAMILY_WINDOWS
		return InterlockedDecrement(&internal_val_);
#elif TNG_OS== TNG_OS_MAC_OS_X
		return OSAtomicDecrement32(static_cast<int32_t*>(&internal_val_));
#elif defined(HAVE_GCC_ATOMICS)
		return __sync_sub_and_fetch(&internal_val_, 1);
#elif TNG_OS==TNG_OS_SOLARIS
		return atomic_dec_32_nv(&internal_val_);
#elif defined(HAVE_PPC_ATOMICS)
		int t;
		__asm__ __volatile__("\n\
							 							 1:	lwarx	%0,0,%2\n\
														 							 addic	%0,%0,-1\n\
																					 							 stwcx.	%0,0,%2\n\
																												 							 bne	1b"
																																			 : "=&r" (t), "=m" (internal_val_)
																																			 : "r" (&internal_val_), "m" (internal_val_)
																																			 : "cc");
		return t;
#else
		AutoLock<Mutex> scopelock(&lock_);
		return --internal_val_;
#endif
	}



	__inline ValueType operator -- (int)
	{
#if TNG_OS_FAMILY_WINDOWS
		ValueType result = InterlockedDecrement(&internal_val_);
		return ++result;
#elif TNG_OS== TNG_OS_MAC_OS_X
		ValueType result = OSAtomicDecrement32(static_cast<int32_t*>(&internal_val_));
		return ++result;
#elif TNG_OS==TNG_OS_SOLARIS
		return atomic_dec_32_nv(&internal_val_)+1;
#elif defined(HAVE_GCC_ATOMICS)
		return __sync_fetch_and_sub(&internal_val_, 1);
#elif defined(HAVE_PPC_ATOMICS)
		int t;
		__asm__ __volatile__("\n\
							 							 1:	lwarx	%0,0,%2\n\
														 							 addic	%0,%0,-1\n\
																					 							 stwcx.	%0,0,%2\n\
																												 							 bne	1b"
																																			 : "=&r" (t), "=m" (internal_val_)
																																			 : "r" (&internal_val_), "m" (internal_val_)
																																			 : "cc");
		return t+1;
#else
		AutoLock<Mutex> scopelock(&lock_);
		return internal_val_--;
#endif
	}


	__inline bool operator ! () const{
		return internal_val_ == 0;
	}


private:
	ValueType	internal_val_;
#if HASATOMIC==0
	mutable Mutex		lock_;
#endif
};