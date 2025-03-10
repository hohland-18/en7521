/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#include <linux/config.h>
#include <asm/bootinfo.h>
#include <asm/jazz.h>

/*
 * This assumes you have a 1.8432 MHz clock for your UART.
 *
 * It'd be nice if someone built a serial card with a 24.576 MHz
 * clock, since the 16550A is capable of handling a top speed of 1.5
 * megabits/second; but this requires the faster clock.
 */
#define BASE_BAUD ( 1843200 / 16 )

#ifndef CONFIG_OLIVETTI_M700
   /* Some Jazz machines seem to have an 8MHz crystal clock but I don't know
      exactly which ones ... XXX */
#define JAZZ_BASE_BAUD ( 8000000 / 16 ) /* ( 3072000 / 16) */
#else
/* but the M700 isn't such a strange beast */
#define JAZZ_BASE_BAUD BASE_BAUD
#endif

/* Standard COM flags (except for COM4, because of the 8514 problem) */
#ifdef CONFIG_SERIAL_DETECT_IRQ
#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST | ASYNC_AUTO_IRQ)
#define STD_COM4_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_AUTO_IRQ)
#else
#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST)
#define STD_COM4_FLAGS ASYNC_BOOT_AUTOCONF
#endif

#ifdef CONFIG_SERIAL_MANY_PORTS
#define FOURPORT_FLAGS ASYNC_FOURPORT
#define ACCENT_FLAGS 0
#define BOCA_FLAGS 0
#define HUB6_FLAGS 0
#define RS_TABLE_SIZE	64
#else
#define RS_TABLE_SIZE
#endif

/*
 * The following define the access methods for the HUB6 card. All
 * access is through two ports for all 24 possible chips. The card is
 * selected through the high 2 bits, the port on that card with the
 * "middle" 3 bits, and the register on that port with the bottom
 * 3 bits.
 *
 * While the access port and interrupt is configurable, the default
 * port locations are 0x302 for the port control register, and 0x303
 * for the data read/write register. Normally, the interrupt is at irq3
 * but can be anything from 3 to 7 inclusive. Note that using 3 will
 * require disabling com2.
 */

#define C_P(card,port) (((card)<<6|(port)<<3) + 1)

#ifdef CONFIG_MIPS_JAZZ
#define _JAZZ_SERIAL_INIT(int, base)					\
	{ baud_base: JAZZ_BASE_BAUD, irq: int, flags: STD_COM_FLAGS,	\
	  iomem_base: (u8 *) base, iomem_reg_shift: 0,			\
	  io_type: SERIAL_IO_MEM }
#define JAZZ_SERIAL_PORT_DEFNS						\
	_JAZZ_SERIAL_INIT(JAZZ_SERIAL1_IRQ, JAZZ_SERIAL1_BASE),		\
	_JAZZ_SERIAL_INIT(JAZZ_SERIAL2_IRQ, JAZZ_SERIAL2_BASE),
#else
#define JAZZ_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_MIPS_ATLAS
#include <asm/mips-boards/atlas.h>
#include <asm/mips-boards/atlasint.h>
#define ATLAS_SERIAL_PORT_DEFNS			\
	/* UART CLK   PORT IRQ     FLAGS        */			\
	{ 0, ATLAS_BASE_BAUD, ATLAS_UART_REGS_BASE, ATLASINT_UART, STD_COM_FLAGS },     /* ttyS0 */
#else
#define ATLAS_SERIAL_PORT_DEFNS
#endif

/*
 * Both Galileo boards have the same UART mappings.
 */
#if defined (CONFIG_MIPS_EV96100) || defined (CONFIG_MIPS_EV64120)
#include <asm/galileo-boards/ev96100.h>
#include <asm/galileo-boards/ev96100int.h>
#define EV96100_SERIAL_PORT_DEFNS                                  \
    { baud_base: EV96100_BASE_BAUD, port: EV96100_UART0_REGS_BASE, \
      irq: EV96100INT_UART_0, flags: STD_COM_FLAGS, type: 0x3,   \
      iomem_base: EV96100_UART0_REGS_BASE },                       \
    { baud_base: EV96100_BASE_BAUD, port: EV96100_UART1_REGS_BASE, \
      irq: EV96100INT_UART_0, flags: STD_COM_FLAGS, type: 0x3,   \
      iomem_base: EV96100_UART1_REGS_BASE },
#else
#define EV96100_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_MIPS_ITE8172
#include <asm/it8172/it8172.h>
#include <asm/it8172/it8172_int.h>
#include <asm/it8712.h>
#define ITE_SERIAL_PORT_DEFNS                                  \
    { baud_base: BASE_BAUD, port: (IT8172_PCI_IO_BASE + IT_UART_BASE), \
      irq: IT8172_UART_IRQ, flags: STD_COM_FLAGS, type: 0x3 }, \
    { baud_base: (24000000/(16*13)), port: (IT8172_PCI_IO_BASE + IT8712_UART1_PORT), \
      irq: IT8172_SERIRQ_4, flags: STD_COM_FLAGS, type: 0x3 }, \
    /* Smart Card Reader 0 */ \
    { baud_base: BASE_BAUD, port: (IT8172_PCI_IO_BASE + IT_SCR0_BASE), \
      irq: IT8172_SCR0_IRQ, flags: STD_COM_FLAGS, type: 0x3 }, \
    /* Smart Card Reader 1 */ \
    { baud_base: BASE_BAUD, port: (IT8172_PCI_IO_BASE + IT_SCR1_BASE), \
      irq: IT8172_SCR1_IRQ, flags: STD_COM_FLAGS, type: 0x3 },
#else
#define ITE_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_MIPS_IVR
#include <asm/it8172/it8172.h>
#include <asm/it8172/it8172_int.h>
#define IVR_SERIAL_PORT_DEFNS                                  \
    { baud_base: BASE_BAUD, port: (IT8172_PCI_IO_BASE + IT_UART_BASE), \
      irq: IT8172_UART_IRQ, flags: STD_COM_FLAGS, type: 0x3 },         \
    /* Smart Card Reader 1 */ \
    { baud_base: BASE_BAUD, port: (IT8172_PCI_IO_BASE + IT_SCR1_BASE), \
      irq: IT8172_SCR1_IRQ, flags: STD_COM_FLAGS, type: 0x3 },
#else
#define IVR_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_AU1000_UART
#include <asm/au1000.h>
#define AU1000_SERIAL_PORT_DEFNS                              \
    { baud_base: 0, port: UART0_ADDR, irq: AU1000_UART0_INT,  \
      flags: STD_COM_FLAGS, type: 1 },                        \
    { baud_base: 0, port: UART1_ADDR, irq: AU1000_UART1_INT,  \
      flags: STD_COM_FLAGS, type: 1 },     \
    { baud_base: 0, port: UART2_ADDR, irq: AU1000_UART2_INT,  \
      flags: STD_COM_FLAGS, type: 1 },    \
    { baud_base: 0, port: UART3_ADDR, irq: AU1000_UART3_INT,  \
      flags: STD_COM_FLAGS, type: 1 },
#else
#define AU1000_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_HAVE_STD_PC_SERIAL_PORT
#define STD_SERIAL_PORT_DEFNS			\
	/* UART CLK   PORT IRQ     FLAGS        */			\
	{ 0, BASE_BAUD, 0x3F8, 4, STD_COM_FLAGS },	/* ttyS0 */	\
	{ 0, BASE_BAUD, 0x2F8, 3, STD_COM_FLAGS },	/* ttyS1 */	\
	{ 0, BASE_BAUD, 0x3E8, 4, STD_COM_FLAGS },	/* ttyS2 */	\
	{ 0, BASE_BAUD, 0x2E8, 3, STD_COM4_FLAGS },	/* ttyS3 */

#ifdef CONFIG_SERIAL_MANY_PORTS
#define EXTRA_SERIAL_PORT_DEFNS			\
	{ 0, BASE_BAUD, 0x1A0, 9, FOURPORT_FLAGS }, 	/* ttyS4 */	\
	{ 0, BASE_BAUD, 0x1A8, 9, FOURPORT_FLAGS },	/* ttyS5 */	\
	{ 0, BASE_BAUD, 0x1B0, 9, FOURPORT_FLAGS },	/* ttyS6 */	\
	{ 0, BASE_BAUD, 0x1B8, 9, FOURPORT_FLAGS },	/* ttyS7 */	\
	{ 0, BASE_BAUD, 0x2A0, 5, FOURPORT_FLAGS },	/* ttyS8 */	\
	{ 0, BASE_BAUD, 0x2A8, 5, FOURPORT_FLAGS },	/* ttyS9 */	\
	{ 0, BASE_BAUD, 0x2B0, 5, FOURPORT_FLAGS },	/* ttyS10 */	\
	{ 0, BASE_BAUD, 0x2B8, 5, FOURPORT_FLAGS },	/* ttyS11 */	\
	{ 0, BASE_BAUD, 0x330, 4, ACCENT_FLAGS },	/* ttyS12 */	\
	{ 0, BASE_BAUD, 0x338, 4, ACCENT_FLAGS },	/* ttyS13 */	\
	{ 0, BASE_BAUD, 0x000, 0, 0 },			/* ttyS14 (spare) */ \
	{ 0, BASE_BAUD, 0x000, 0, 0 },			/* ttyS15 (spare) */ \
	{ 0, BASE_BAUD, 0x100, 12, BOCA_FLAGS },	/* ttyS16 */	\
	{ 0, BASE_BAUD, 0x108, 12, BOCA_FLAGS },	/* ttyS17 */	\
	{ 0, BASE_BAUD, 0x110, 12, BOCA_FLAGS },	/* ttyS18 */	\
	{ 0, BASE_BAUD, 0x118, 12, BOCA_FLAGS },	/* ttyS19 */	\
	{ 0, BASE_BAUD, 0x120, 12, BOCA_FLAGS },	/* ttyS20 */	\
	{ 0, BASE_BAUD, 0x128, 12, BOCA_FLAGS },	/* ttyS21 */	\
	{ 0, BASE_BAUD, 0x130, 12, BOCA_FLAGS },	/* ttyS22 */	\
	{ 0, BASE_BAUD, 0x138, 12, BOCA_FLAGS },	/* ttyS23 */	\
	{ 0, BASE_BAUD, 0x140, 12, BOCA_FLAGS },	/* ttyS24 */	\
	{ 0, BASE_BAUD, 0x148, 12, BOCA_FLAGS },	/* ttyS25 */	\
	{ 0, BASE_BAUD, 0x150, 12, BOCA_FLAGS },	/* ttyS26 */	\
	{ 0, BASE_BAUD, 0x158, 12, BOCA_FLAGS },	/* ttyS27 */	\
	{ 0, BASE_BAUD, 0x160, 12, BOCA_FLAGS },	/* ttyS28 */	\
	{ 0, BASE_BAUD, 0x168, 12, BOCA_FLAGS },	/* ttyS29 */	\
	{ 0, BASE_BAUD, 0x170, 12, BOCA_FLAGS },	/* ttyS30 */	\
	{ 0, BASE_BAUD, 0x178, 12, BOCA_FLAGS },	/* ttyS31 */
#else /* CONFIG_SERIAL_MANY_PORTS */
#define EXTRA_SERIAL_PORT_DEFNS
#endif /* CONFIG_SERIAL_MANY_PORTS */

#else /* CONFIG_HAVE_STD_PC_SERIAL_PORTS */
#define STD_SERIAL_PORT_DEFNS
#define EXTRA_SERIAL_PORT_DEFNS
#endif /* CONFIG_HAVE_STD_PC_SERIAL_PORTS */

/* You can have up to four HUB6's in the system, but I've only
 * included two cards here for a total of twelve ports.
 */
#if (defined(CONFIG_HUB6) && defined(CONFIG_SERIAL_MANY_PORTS))
#define HUB6_SERIAL_PORT_DFNS		\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(0,0) },  /* ttyS32 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(0,1) },  /* ttyS33 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(0,2) },  /* ttyS34 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(0,3) },  /* ttyS35 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(0,4) },  /* ttyS36 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(0,5) },  /* ttyS37 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(1,0) },  /* ttyS38 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(1,1) },  /* ttyS39 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(1,2) },  /* ttyS40 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(1,3) },  /* ttyS41 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(1,4) },  /* ttyS42 */	\
	{ 0, BASE_BAUD, 0x302, 3, HUB6_FLAGS, C_P(1,5) },  /* ttyS43 */
#else
#define HUB6_SERIAL_PORT_DFNS
#endif

#ifdef CONFIG_MCA
#define MCA_SERIAL_PORT_DFNS			\
	{ 0, BASE_BAUD, 0x3220, 3, STD_COM_FLAGS },	\
	{ 0, BASE_BAUD, 0x3228, 3, STD_COM_FLAGS },	\
	{ 0, BASE_BAUD, 0x4220, 3, STD_COM_FLAGS },	\
	{ 0, BASE_BAUD, 0x4228, 3, STD_COM_FLAGS },	\
	{ 0, BASE_BAUD, 0x5220, 3, STD_COM_FLAGS },	\
	{ 0, BASE_BAUD, 0x5228, 3, STD_COM_FLAGS },
#else
#define MCA_SERIAL_PORT_DFNS
#endif

#ifdef CONFIG_MOMENCO_OCELOT
/* Ordinary NS16552 duart with a 20MHz crystal.  */
#define OCELOT_BASE_BAUD ( 20000000 / 16 )

#define OCELOT_SERIAL1_IRQ	4
#define OCELOT_SERIAL1_BASE	0xe0001020

#define _OCELOT_SERIAL_INIT(int, base)					\
	{ baud_base: OCELOT_BASE_BAUD, irq: int, flags: STD_COM_FLAGS,	\
	  iomem_base: (u8 *) base, iomem_reg_shift: 2,			\
	  io_type: SERIAL_IO_MEM }
#define MOMENCO_OCELOT_SERIAL_PORT_DEFNS				\
	_OCELOT_SERIAL_INIT(OCELOT_SERIAL1_IRQ, OCELOT_SERIAL1_BASE)
#else
#define MOMENCO_OCELOT_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_DDB5477
#define DDB5477_SERIAL_PORT_DEFNS                                       \
        { baud_base: BASE_BAUD, irq: 12, flags: STD_COM_FLAGS,          \
          iomem_base: (u8*)0xbfa04200, iomem_reg_shift: 3,              \
          io_type: SERIAL_IO_MEM},\
        { baud_base: BASE_BAUD, irq: 28, flags: STD_COM_FLAGS,          \
          iomem_base: (u8*)0xbfa04240, iomem_reg_shift: 3,              \
          io_type: SERIAL_IO_MEM},
#else
#define DDB5477_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_SUNPLUS					//lobo add
#define SUNPLUS_SERIAL_PORT_DEFNS                   \
	/* UART CLK   PORT IRQ     FLAGS        */                      \
       { 0, BASE_BAUD, 0xAFFF01A0, 6, STD_COM_FLAGS }, /* ttyS0 */ 
//       { 0, BASE_BAUD, 0xAFFF01A0, 7, STD_COM_FLAGS }, /* ttyS1 */
#else
#define SUNPLUS_SERIAL_PORT_DEFNS
#endif
       
#ifdef CONFIG_TRENDCHIP_TC3160
#include <asm/tc3160/interrupts.h>
#include <asm/tc3160/sio.h>
#define TC3160_SERIAL_PORT_DEFNS                   \
	/* UART CLK   PORT IRQ     FLAGS        */                      \
       { 0, BASE_BAUD, TC3160_UART_BASE, INTR_MUART, STD_COM_FLAGS }, /* ttyS0 */ 
#else
#define TC3160_SERIAL_PORT_DEFNS
#endif

#ifdef CONFIG_TRENDCHIP_TC3162
#include <asm/tc3162/interrupts.h>
#include <asm/tc3162/sio.h>
#define TC3162_SERIAL_PORT_DEFNS                   \
	/* UART CLK   PORT IRQ     FLAGS        */                      \
       { 0, BASE_BAUD, TC3162_UART_BASE, INTR_UART, STD_COM_FLAGS }, /* ttyS0 */ 
#else
#define TC3162_SERIAL_PORT_DEFNS
#endif
       
#define SERIAL_PORT_DFNS		\
	IVR_SERIAL_PORT_DEFNS           \
	ITE_SERIAL_PORT_DEFNS           \
	ATLAS_SERIAL_PORT_DEFNS		\
	EV96100_SERIAL_PORT_DEFNS	\
	JAZZ_SERIAL_PORT_DEFNS		\
	STD_SERIAL_PORT_DEFNS		\
	EXTRA_SERIAL_PORT_DEFNS		\
	HUB6_SERIAL_PORT_DFNS		\
	MOMENCO_OCELOT_SERIAL_PORT_DEFNS\
	AU1000_SERIAL_PORT_DEFNS	\
	DDB5477_SERIAL_PORT_DEFNS	\
	TC3160_SERIAL_PORT_DEFNS	\
	TC3162_SERIAL_PORT_DEFNS	\
	SUNPLUS_SERIAL_PORT_DEFNS		//lobo add

