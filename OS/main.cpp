#include <Windows.h>
#include "stdio.h"
#include "intrin.h"
#include "typedef.h"
#include "ioport.h"
#include "vga.h"
#include "C++.h"
#include "page_frame.h"
#include "gdt.h"
#include "idt.h"
#include "pci.h"
#include "cpu.h"
#include "tss.h"
#include "mmu.h"
#include "acpi.h"
#include "trap.h"
#include "8253.h"
#include "8259.h"
#include "keyboard.h"
#include "rtc.h"

//PAGE_FRAME_DB  page_frame_db;
CPU			  cpu;
GDT			  gdt;
IDT			  idt;
TSS           tss;
MMU			  mmu;
TRAP		  trap;

void main(uint32 kernel_size, uint32 page_frame_min, uint32 page_frame_max)
{
	init_vga((void*)PMODE_VIDEO_BASE);
	puts("\nHello world\n", 30);
	puts("Shellcode OS is starting...\n", 30);

	CppInit();
	PAGE_FRAME_DB::Init(page_frame_min, page_frame_max);
	
	mmu.Init();

	//ACPI acpi;
	//acpi.Init(&mmu);

	uint32 CR3 = __readcr3();
	printf("PageDir=%08X\n", CR3);

	gdt.Init();
	idt.Init();
	tss.Init(CR3,&gdt);
	trap.Init(&idt);
	PIC::Init(&idt);
	PIT::Init();
	RTC::Init();
	Keyboard::Init();
	_enable();
	__asm jmp $
}

