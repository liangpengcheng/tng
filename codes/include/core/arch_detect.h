#pragma once
#if defined(__arm__) || defined(__TARGET_ARCH_ARM) ||defined(_M_ARM) || defined(_ARM_)
#ifdef __ARMEB__
#define TNG_BIGENGIAN
#define TNG_BIGENDIAN	true
#endif
#define TNG_ARM
#if defined(__ARM_ARCH_7__) \
	|| defined(__ARM_ARCH_7A__) \
	|| defined(__ARM_ARCH_7R__) \
	|| defined(__ARM_ARCH_7M__) \
	|| (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7)
#define TNG_ARMV7
#elif defined(__ARM_ARCH_6__) \
	|| defined(__ARM_ARCH_6J__) \
	|| defined(__ARM_ARCH_6T2__) \
	|| defined(__ARM_ARCH_6Z__) \
	|| defined(__ARM_ARCH_6K__) \
	|| defined(__ARM_ARCH_6ZK__) \
	|| defined(__ARM_ARCH_6M__) \
	|| (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6)
#define TNG_ARMV6
#elif defined(__ARM_ARCH_5TEJ__) \
	|| (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5)
#define TNG_ARMV5
#else
#define TNG_ARM_unknow
#endif
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#define TNG_X86
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
#define TNG_X86_64
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
#define TNG_IA64
#elif defined(__mips__) || defined(mips)
#define TNG_MIPS
#elif defined(__mips64__) || defined(mips64)
#define TNG_MIPS64
#elif defined(__sparc64__)|| defined(sparc64)
#define TNG_BIGENDIAN	true
#define TNG_SPARC64
#elif defined(__sparc__)|| defined(sparc)
#define TNG_BIGENDIAN	true
#define TNG_SPARC
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \
	|| defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \
	|| defined(_M_MPPC) || defined(_M_PPC)
#define TNG_BIGENGIAN
#define TNG_BIGENDIAN	true
#if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
#define TNG_PPC64
#else
#define TNG_PPC
#endif
#endif

//micros copy from poco
#define TNG_OS_FREE_BSD      0x0001
#define TNG_OS_AIX           0x0002
#define TNG_OS_HPUX          0x0003
#define TNG_OS_TRU64         0x0004
#define TNG_OS_LINUX         0x0005
#define TNG_OS_MAC_OS_X      0x0006
#define TNG_OS_NET_BSD       0x0007
#define TNG_OS_OPEN_BSD      0x0008
#define TNG_OS_IRIX          0x0009
#define TNG_OS_SOLARIS       0x000a
#define TNG_OS_QNX           0x000b
#define TNG_OS_VXWORKS       0x000c
#define TNG_OS_CYGWIN        0x000d
#define TNG_OS_DRAGONFLY     0x000e
#define TNG_OS_UNKNOWN_UNIX  0x00ff
#define TNG_OS_WINDOWS_NT    0x1001
#define TNG_OS_WINDOWS_CE    0x1011
#define TNG_OS_VMS           0x2001
#define USEGLES				 0
#if defined(__FreeBSD__)|| defined(__FreeBSD_kernel__)
#if defined(__AVM2__)
#define TNG_OS_FLASCC				1
#endif
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS_FAMILY_BSD 1
#define TNG_OS TNG_OS_FREE_BSD
#elif defined(_AIX) || defined(__TOS_AIX__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_AIX
#elif defined(hpux) || defined(_hpux)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_HPUX
#elif defined(__digital__) || defined(__osf__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_TRU64
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_LINUX
#if defined(ANDROID)
#define TNG_ANDROID
#undef USEGLES
#define USEGLES 1
#endif
#elif defined(__APPLE__) || defined(__TOS_MACOS__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS_FAMILY_BSD 1
#define TNG_OS TNG_OS_MAC_OS_X
#if TARGET_OS_IPHONE
#define TNG_IPHONE
#undef USEGLES
#define USEGLES 1
#endif
#elif defined(__NetBSD__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS_FAMILY_BSD 1
#define TNG_OS TNG_OS_NET_BSD
#elif defined(__DragonFly__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS_FAMILY_BSD 1
#define TNG_OS TNG_OS_DRAGONFLY
#elif defined(__OpenBSD__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS_FAMILY_BSD 1
#define TNG_OS TNG_OS_OPEN_BSD
#elif defined(sgi) || defined(__sgi)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_IRIX
#elif defined(sun) || defined(__sun)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_SOLARIS
#elif defined(__QNX__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_QNX
#elif defined(unix) || defined(__unix) || defined(__unix__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_UNKNOWN_UNIX
#elif defined(_WIN32_WCE)
#define TNG_OS_FAMILY_WINDOWS 1
#define TNG_OS TNG_OS_WINDOWS_CE
#elif defined(_WIN32) || defined(_WIN64)
#define TNG_OS_FAMILY_WINDOWS 1
#define TNG_OS TNG_OS_WINDOWS_NT
#elif defined(__CYGWIN__)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_CYGWIN
#elif defined(__VMS)
#define TNG_OS_FAMILY_VMS 1
#define TNG_OS TNG_OS_VMS
#elif defined(TNG_VXWORKS)
#define TNG_OS_FAMILY_UNIX 1
#define TNG_OS TNG_OS_VXWORKS
#endif
