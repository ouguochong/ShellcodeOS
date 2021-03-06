#include "keyboard.h"
#include "stdio.h"
#include "vga.h"
#include "system.h"

byte KBD::m_led_status=0;
byte KBD::m_control_keys=0;
int  KBD::m_ext=0;

void KBD::Init()
{
	PIC::register_irq(IRQ_KEYBOARD, KBD::irq_handler);
}

void KBD::irq_handler(PIC_IRQ_CONTEXT* context)
{
	unsigned char status;

	while ((status = __inbyte(KBD_STATUS)) & KBD_STAT_OBF)
	{
		byte scancode = __inbyte(KBD_DATA);	// Get next scancode from KBD
		uint32 key = decode(scancode);// Process scan code
		if (key) putc(key);
	}
}

uint32 KBD::decode(byte scancode)
{
	enum SCANCODE 
	{
		NULL_KEY = 0,
		Q_PRESSED = 0x10,
		Q_RELEASED = 0x90,
		W_PRESSED = 0x11,
		W_RELEASED = 0x91,
		E_PRESSED = 0x12,
		E_RELEASED = 0x92,
		R_PRESSED = 0x13,
		R_RELEASED = 0x93,
		T_PRESSED = 0x14,
		T_RELEASED = 0x94,
		Z_PRESSED = 0x15,
		Z_RELEASED = 0x95,
		U_PRESSED = 0x16,
		U_RELEASED = 0x96,
		I_PRESSED = 0x17,
		I_RELEASED = 0x97,
		O_PRESSED = 0x18,
		O_RELEASED = 0x98,
		P_PRESSED = 0x19,
		P_RELEASED = 0x99,
		A_PRESSED = 0x1E,
		A_RELEASED = 0x9E,
		S_PRESSED = 0x1F,
		S_RELEASED = 0x9F,
		D_PRESSED = 0x20,
		D_RELEASED = 0xA0,
		F_PRESSED = 0x21,
		F_RELEASED = 0xA1,
		G_PRESSED = 0x22,
		G_RELEASED = 0xA2,
		H_PRESSED = 0x23,
		H_RELEASED = 0xA3,
		J_PRESSED = 0x24,
		J_RELEASED = 0xA4,
		K_PRESSED = 0x25,
		K_RELEASED = 0xA5,
		L_PRESSED = 0x26,
		L_RELEASED = 0xA6,
		Y_PRESSED = 0x2C,
		Y_RELEASED = 0xAC,
		X_PRESSED = 0x2D,
		X_RELEASED = 0xAD,
		C_PRESSED = 0x2E,
		C_RELEASED = 0xAE,
		V_PRESSED = 0x2F,
		V_RELEASED = 0xAF,
		B_PRESSED = 0x30,
		B_RELEASED = 0xB0,
		N_PRESSED = 0x31,
		N_RELEASED = 0xB1,
		M_PRESSED = 0x32,
		M_RELEASED = 0xB2,

		ZERO_PRESSED = 0x29,
		ONE_PRESSED = 0x2,
		NINE_PRESSED = 0xA,

		POINT_PRESSED = 0x34,
		POINT_RELEASED = 0xB4,

		SLASH_RELEASED = 0xB5,

		BACKSPACE_PRESSED = 0xE,
		BACKSPACE_RELEASED = 0x8E,
		SPACE_PRESSED = 0x39,
		SPACE_RELEASED = 0xB9,
		ENTER_PRESSED = 0x1C,
		ENTER_RELEASED = 0x9C,
	};
	static char* _qwertzuiop = "qwertzuiop"; // 0x10-0x1c
	static char* _asdfghjkl = "asdfghjkl";
	static char* _yxcvbnm = "yxcvbnm";
	static char* _num = "123456789";
	//kprintf("key=0x%x\n", key);
	if (scancode == 0x1C) return '\n';
	if (scancode == 0x39) return ' ';
	if (scancode == 0x0E) return '\r';
	if (scancode == POINT_RELEASED) return '.';
	if (scancode == SLASH_RELEASED) return '/';
	if (scancode == ZERO_PRESSED) return '0';
	if (scancode >= ONE_PRESSED && scancode <= NINE_PRESSED)
		return _num[scancode - ONE_PRESSED];
	if (scancode >= 0x10 && scancode <= 0x1C)
	{
		return _qwertzuiop[scancode - 0x10];
	}
	else if (scancode >= 0x1E && scancode <= 0x26)
	{
		return _asdfghjkl[scancode - 0x1E];
	}
	else if (scancode >= 0x2C && scancode <= 0x32)
	{
		return _yxcvbnm[scancode - 0x2C];
	}
	return 0;
}