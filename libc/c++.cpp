//http://wiki.osdev.org/Visual_C++_Runtime
#include "c++.h"
#include "liballoc.h"
#include "stdio.h"

// Constructor prototypes
typedef void(__cdecl *_PVFV)(void);
typedef int(__cdecl *_PIFV)(void);

// Linker puts constructors between these sections, and we use them to locate constructor pointers.
#pragma section(".CRT$XIA",long,read)
#pragma section(".CRT$XIZ",long,read)
#pragma section(".CRT$XCA",long,read)
#pragma section(".CRT$XCZ",long,read)

// Put .CRT data into .rdata section
#pragma comment(linker, "/merge:.CRT=.rdata")

// Pointers surrounding constructors
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[] = { 0 };
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[] = { 0 };
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = { 0 };
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = { 0 };

extern __declspec(allocate(".CRT$XIA")) _PIFV __xi_a[];
extern __declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[];    // C initializers
extern __declspec(allocate(".CRT$XCA")) _PVFV __xc_a[];
extern __declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[];    // C++ initializers

// Call C constructors
static int _initterm_e(_PIFV * pfbegin, _PIFV * pfend)
{
	int ret = 0;

	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while (pfbegin < pfend  && ret == 0)
	{
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			ret = (**pfbegin)();
		++pfbegin;
	}

	return ret;
}

// Call C++ constructors
static void _initterm(_PVFV * pfbegin, _PVFV * pfend)
{
	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while (pfbegin < pfend)
	{
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			(**pfbegin)();
		++pfbegin;
	}
}

// Call this function as soon as possible. Basically should be at the moment you
// jump into your C/C++ kernel. But keep in mind that kernel is not yet initialized,
// and you can't use a lot of stuff in your constructors!
bool CppInit()
{
	// Do C initialization
	int initret = _initterm_e(__xi_a, __xi_z);
	if (initret != 0)
	{
		return false;
	}
	// Do C++ initialization
	_initterm(__xc_a, __xc_z);
	return true;
}

void* __cdecl operator new(size_t size)
{
	// Allocate memory
	void *obj = malloc(size);
	dprint("new (obj=%08X) size=%08X\n", obj, size);
	return obj;
}

void* __cdecl operator new[](size_t size)
{
	// Allocate memory
	void *obj = malloc(size);
	dprint("new[](obj=%08X) size=%08X\n", obj, size);
	return obj;
}

void __cdecl operator delete(void *p)
{
	dprint("delete(p=%08X)\n", p);
	if (p)
	{
		free(p);
	}

	// Release allocated memory
}

void __cdecl operator delete[](void *p)
{
	dprint("delete[](obj=%08X)\n", p);
	if (p)
	{
		free(p);
	}

	// Release allocated memory
}

void __cdecl  operator delete(void *p, unsigned int size)
{
	dprint("delete(obj=%08X, size=%08X)\n", p, size);
	if (p)
	{
		free(p);
	}
}

int    __cdecl atexit(void(__cdecl *)(void))
{
	return 0;
}

