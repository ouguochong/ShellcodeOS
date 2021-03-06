#include "8253.h"
#include <string.h>
#include "vga.h"
#include <stdio.h>

uint32 PIT::m_counter = 0;
uint64 PIT::m_timer = 0;
PIT_HANDLER  PIT::m_handlers[MAX_PIT_HANDLERS] = { 0 };

bool PIT::Init()
{
	m_counter = 0;
	m_timer = 0;
	memset(m_handlers, 0, sizeof(m_handlers));

	// 设置8253定时芯片。把计数器通道0设置成每隔50ms向中断控制器发送一个中断请求信号。
	outportb(0x43, 0x36);// 控制字:设置通道0工作在方式3、计数初值采用二进制。
						 // 8253芯片控制字寄存器写端口。
	uint16 count = PIT_CLOCK / TIMER_FREQ; //20HZ
	outportb(0x40, count & 0xff);
	outportb(0x40, (count >> 8) & 0xff);

	PIC::register_irq(IRQ_PIT, PIT::irq_handler);
	return true;
}

void	PIT::irq_handler(PIC_IRQ_CONTEXT* context)
{
	if (++m_counter >= 5)
	{
		m_counter = 0;
		m_timer++;

		int x = 79, y = 0;
		int color = 10;
		switch (m_timer % 4)
		{
		case 0: setchar(x, y, '-', color, 0);break;
		case 1: setchar(x, y, '\\', color, 0);break;
		case 2: setchar(x, y, '|', color, 0);break;
		case 3: setchar(x, y, '/', color, 0);break;
		}
	}
	for (int i = 0; i < MAX_PIT_HANDLERS; i++)
	{
		if (m_handlers[i].handler != NULL && m_handlers[i].period >= 0)
		{
			m_handlers[i].counter += 1000 / TIMER_FREQ;
			if (m_handlers[i].counter >=m_handlers[i].period)
			{
				m_handlers[i].handler(m_handlers[i].id);
				m_handlers[i].counter = 0;
			}
		}
	}
}

bool PIT::set_timer(uint32 id, uint32 period, TIMER_HANDLER handler)
{
	int free_item = -1;
	for (int i = 0; i < MAX_PIT_HANDLERS; i++)
	{
		if (m_handlers[i].id == id)
		{
			m_handlers[i].period = period;
			m_handlers[i].handler = handler;
			m_handlers[i].counter = 0;
			return true;
		}
		if (m_handlers[i].handler == NULL && free_item == -1 ) free_item = i;
	}
	if (free_item == -1) return false;
	m_handlers[free_item].id = id;
	m_handlers[free_item].period = period;
	m_handlers[free_item].handler = handler;
	m_handlers[free_item].counter = 0;
	return true;
}

bool PIT::kill_timer(uint32 id)
{
	for (int i = 0; i < MAX_PIT_HANDLERS; i++)
	{
		if (m_handlers[i].id == id)
		{
			m_handlers[i].id = 0;
			m_handlers[i].period = 0;
			m_handlers[i].handler = NULL;
			m_handlers[i].counter = 0;
			return true;
		}
	}
	return false;
}



