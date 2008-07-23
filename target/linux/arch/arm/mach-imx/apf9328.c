/*
 * linux/arch/arm/mach-imx/apf9328.c
 *
 * Copyright (c) 2005 Armadeus
 * This work is based on scb9328.c from Sascha Hauer
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/device.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/serial_8250.h>
#include <linux/spi/spi.h>
#include <linux/spi/tsc2102.h>
#include "../../../drivers/net/can/mcp251x.h"

#include <asm/system.h>
#include <asm/hardware.h>
#include <linux/irq.h>
#include <asm/pgtable.h>
#include <asm/page.h>

#include <asm/mach/map.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/arch/imx-uart.h>
#include <asm/arch/mmc.h>
#include <asm/arch/spi_imx.h>
#include <asm/arch/imx-regs.h> // imx_gpio_mode()
#include <asm/arch/imx-alsa.h>

#ifdef CONFIG_DM9000
#include <linux/dm9000.h>
#endif // CONFIG_DM9000

#include "generic.h"

#ifdef CONFIG_SENSORS_MAX1027_MODULE
#define CONFIG_SENSORS_MAX1027 1
#endif
#ifdef CONFIG_SPI_IMX_MODULE
#define CONFIG_SPI_IMX 1
#endif
#ifdef CONFIG_SPI_TSC2102_MODULE
#define CONFIG_SPI_TSC2102 1
#endif
#ifdef CONFIG_CAN_MCP251X_MODULE
#define CONFIG_CAN_MCP251X 1
#endif
#ifdef CONFIG_IMX_BACKLIGHT_MODULE
#define CONFIG_IMX_BACKLIGHT 1
#endif

// --- ETHERNET RESSOURCE ---
#ifdef CONFIG_DM9000
static struct resource dm9000_resources[] = {
    [0] = {
        .start  = (APF9328_ETH_PHYS + 0),
        .end    = (APF9328_ETH_PHYS + 1),
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = (APF9328_ETH_PHYS + 2),
        .end    = (APF9328_ETH_PHYS + 3),
        .flags  = IORESOURCE_MEM,
    },
    [2] = {
        .start  = (APF9328_ETH_IRQ),
        .end    = (APF9328_ETH_IRQ),
        .flags  = IORESOURCE_IRQ,
    },
};

static struct dm9000_plat_data dm9000_setup = {
    .flags          = DM9000_PLATF_16BITONLY
};

static struct platform_device dm9000x_device = {
    .name           = "dm9000",
    .id             = 0,
    .num_resources  = ARRAY_SIZE(dm9000_resources),
    .resource       = dm9000_resources,
    .dev            = { .platform_data = &dm9000_setup, },
};
#endif // CONFIG_DM9000

// --- PWM RESSOURCE ---
// I don't think it needs an ifdef - JB
static struct resource pwm_resources[] = {
    [0] = {
        .start  = (IMX_PWM_BASE),
        .end    = (IMX_PWM_BASE + 0x0C),
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = (PWM_INT),
        .end    = (PWM_INT),
        .flags  = IORESOURCE_IRQ,
    },
};

static struct platform_device imx_pwm_device = {
    .name           = "imx-pwm", // Should be the same as the driver name
    .id             = 0,
    .num_resources  = ARRAY_SIZE(pwm_resources),
    .resource       = pwm_resources,
};

#if defined(CONFIG_SND_IMX_TSC2102) || defined(CONFIG_SND_IMX_TSC2102_MODULE)
static struct imx_alsa_codec_config tsc2102_alsa_pdata;

static struct resource ssi_resources[] = {
    [0] = {
        .start  = (IMX_SSI_BASE),
        .end    = (IMX_SSI_BASE + 0x28),
        .flags  = IORESOURCE_MEM,
    },
};

static struct platform_device tsc2102_alsa_device = {
    .name           = "tsc2102-alsa", // Should be the same as the driver name
    .id             = 0,
    .num_resources  = ARRAY_SIZE(ssi_resources),
    .resource       = ssi_resources,
    .dev            = {
        .platform_data = &tsc2102_alsa_pdata,
    }
};
#endif // defined(CONFIG_SND_IMX_TSC2102) || defined(CONFIG_SND_IMX_TSC2102_MODULE)


// --- SERIAL RESSOURCE ---
static struct imxuart_platform_data uart_pdata = {
    .flags = IMXUART_HAVE_RTSCTS,
};

static struct resource imx_uart1_resources[] = {
    [0] = {
        .start  = 0x00206000,
        .end    = 0x002060FF,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = (UART1_MINT_RX),
        .end    = (UART1_MINT_RX),
        .flags  = IORESOURCE_IRQ,
    },
    [2] = {
        .start  = (UART1_MINT_TX),
        .end    = (UART1_MINT_TX),
        .flags  = IORESOURCE_IRQ,
},
};

static struct platform_device imx_uart1_device = {
    .name           = "imx-uart",
    .id             = 0,
    .num_resources  = ARRAY_SIZE(imx_uart1_resources),
    .resource       = imx_uart1_resources,
    .dev            = {
        .platform_data = &uart_pdata,
    }
};

static struct resource imx_uart2_resources[] = {
    [0] = {
        .start  = 0x00207000,
        .end    = 0x002070FF,
        .flags  = IORESOURCE_MEM,
    },
    [1] = {
        .start  = (UART2_MINT_RX),
        .end    = (UART2_MINT_RX),
        .flags  = IORESOURCE_IRQ,
    },
    [2] = {
        .start  = (UART2_MINT_TX),
        .end    = (UART2_MINT_TX),
        .flags  = IORESOURCE_IRQ,
    },
};

static struct platform_device imx_uart2_device = {
    .name           = "imx-uart",
    .id             = 1,
    .num_resources  = ARRAY_SIZE(imx_uart2_resources),
    .resource       = imx_uart2_resources,
    .dev            = {
        .platform_data = &uart_pdata,
    }
};

#ifdef CONFIG_SPI_IMX
static struct resource imx_spi1_resource[] = {
	[0] = {
		.start = 0x00213000,
		.end   = 0x00213fff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = CSPI_INT,
		.end   = CSPI_INT,
		.flags = IORESOURCE_IRQ,
	}
};

static struct spi_imx_master imx_spi1_master_info = {
	.num_chipselect	= 1,
	.enable_dma     = 0,
};

static struct platform_device imx_spi1 = {
	.name				= "spi_imx",
	.id					= 1,
	.resource			= imx_spi1_resource,
	.num_resources		= ARRAY_SIZE(imx_spi1_resource),
	.dev = {
		.platform_data	= &imx_spi1_master_info,
	},
};

static struct resource imx_spi2_resource[] = {
	[0] = {
		.start = 0x00219000,
		.end   = 0x00219fff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = SPI2_INT,
		.end   = SPI2_INT,
		.flags = IORESOURCE_IRQ,
	}
};

static struct spi_imx_master imx_spi2_master_info = {
	.num_chipselect	= 1,
	.enable_dma     = 0,
};

static struct platform_device imx_spi2 = {
	.name				= "spi_imx",
	.id					= 2,
	.resource			= imx_spi2_resource,
	.num_resources		= ARRAY_SIZE(imx_spi2_resource),
	.dev = {
		.platform_data	= &imx_spi2_master_info,
	},
};

#endif //CONFIG_SPI_IMX

#ifdef CONFIG_SENSORS_MAX1027
#define IMX_SPI1_SS_PIN 15 
#define IMX_SPI1_SS (1<<15)
static void max1027_init_cs(void)
{
    // SPI CSn is used as chip select but in GPIO mode
    OCR1(2) |= 3<<(IMX_SPI1_SS_PIN*2);
    DDIR(2) |= IMX_SPI1_SS;
    DR(2) |= IMX_SPI1_SS;
    GIUS(2) |= IMX_SPI1_SS;
    GIUS(2) &= ~(1<<14 | 1<<16 | 1<< 17); // Set SPI primary functions
}
static void max1027_cs(u32 command)
{
    if( command == SPI_CS_DEASSERT )
        DR(2) |= IMX_SPI1_SS;
    else
        DR(2) &= ~IMX_SPI1_SS;
}

static struct spi_imx_chip max1027_hw = {
	.cs_control		= max1027_cs,
};

#endif // CONFIG_SENSORS_MAX1027

#ifdef CONFIG_SPI_TSC2102
#define TSC2102_CS_GPIOB 17
#define TSC2102_INT_GPIOD 8 

static void tsc2102_init_gpio(void)
{
    // Activate SPI2 port ----
    // PortB 17 is used as chip select (in GPIO mode)
    DR(1) |= 1 << TSC2102_CS_GPIOB; // Initializes it High
    imx_gpio_mode( GPIO_PORTB | TSC2102_CS_GPIOB | GPIO_OUT | GPIO_GIUS | GPIO_DR );
    // Configure SPI2 outputs
    imx_gpio_mode( PD7_AIN_SPI2_SCLK );
    imx_gpio_mode( PD10_AIN_SPI2_TXD );
    imx_gpio_mode( PD9_AOUT_SPI2_RXD );
    FMCR |= SPI2_RXD_SEL; // RX is AOUT on PORTD

    // PortD 8 is used as PINTDAV interrupt
    imx_gpio_mode( GPIO_PORTD | TSC2102_INT_GPIOD | GPIO_IN | GPIO_GIUS );
    //imx_gpio_irq_type( IRQ_GPIOD(8), __IRQT_FALEDGE );

    // Activate SSI on PortC ----
    imx_gpio_mode( PC3_PF_SSI_RXFS  );
    FMCR &= ~(SSI_RXFS_SEL);
    imx_gpio_mode( PC4_PF_SSI_RXCLK );
    FMCR &= ~(SSI_RXCLK_SEL);
    imx_gpio_mode( PC5_PF_SSI_RXDAT );
    FMCR &= ~(SSI_RXDAT_SEL);
    imx_gpio_mode( PC6_PF_SSI_TXDAT );
    imx_gpio_mode( PC7_PF_SSI_TXFS  );
    FMCR &= ~(SSI_TXFS_SEL);
    imx_gpio_mode( PC8_PF_SSI_TXCLK );
    FMCR &= ~(SSI_TXCLK_SEL);

    printk("TSC2102 GPIOs initialized\n");
}

// Chip select command for TSC2102
static void tsc2102_cs(u32 command)
{
    // PortB 19 is used as chip select
    if( command == SPI_CS_DEASSERT )
        DR(1) |= 1<< TSC2102_CS_GPIOB;
    else
        DR(1) &= ~(1<< TSC2102_CS_GPIOB);
}

static struct spi_imx_chip tsc2102_hw = {
    .cs_control		= tsc2102_cs,
};

static struct tsc2102_config apf9328_tsc2102_config = {
    .use_internal = 1, // -> use internal voltage reference
    .monitor = TSC_BAT1 | TSC_AUX | TSC_TEMP,
    .temp_at25c = 2200,
/*    .apm_report = palmte_get_power_status,
    .alsa_config = &palmte_alsa_config,*/
};

#endif // CONFIG_SPI_TSC2102

#ifdef CONFIG_CAN_MCP251X
#define CAN_MPC251X_CS_GPIOB 19
#define CAN_MPC251X_INT_GPIOC 13 

// Chip select command for MCP251X
static void mcp251X_cs(u32 command)
{
    // PortB 19 is used as chip select
    if( command == SPI_CS_DEASSERT )
        DR(1) |= 1<< CAN_MPC251X_CS_GPIOB;
    else
        DR(1) &= ~(1<< CAN_MPC251X_CS_GPIOB);
}

static struct spi_imx_chip mcp251X_hw = {
    .cs_control		= mcp251X_cs,
};

static void mcp251X_init_gpio(void)
{
    // Activate SPI2 port ----
    // PortB 19 is used as chip select (in GPIO mode)
    DR(1) |= 1 << CAN_MPC251X_CS_GPIOB; // Initializes it High
    imx_gpio_mode( GPIO_PORTB | CAN_MPC251X_CS_GPIOB | GPIO_OUT | GPIO_GIUS | GPIO_DR );

    // PortC 13 is used as CAN interrupt
    imx_gpio_mode( GPIO_PORTC | CAN_MPC251X_INT_GPIOC | GPIO_IN | GPIO_GIUS );

    // Configure SPI2 outputs
    imx_gpio_mode( PD7_AIN_SPI2_SCLK );
    imx_gpio_mode( PD10_AIN_SPI2_TXD );
    imx_gpio_mode( PD9_AOUT_SPI2_RXD );
    FMCR |= SPI2_RXD_SEL; // RX is AOUT on PORTD
}

static struct mcp251x_platform_data apf9328_mcp251x_config = {
 .oscillator_frequency  = 16000000,
 .board_specific_setup  = NULL,
 .device_reset          = NULL,
 .transceiver_enable    = NULL,
};

#endif // CONFIG_CAN_MCP251X


static struct spi_board_info spi_board_info[] __initdata = {
#ifdef CONFIG_SENSORS_MAX1027
{
    .modalias			= "max1027",
    .controller_data 	= &max1027_hw,
    .max_speed_hz		= 8000000,
    .bus_num			= 1, // SPI1
    .chip_select		= 0,
    .mode 				= 0,
},
#endif // CONFIG_SENSORS_MAX1027
#ifdef CONFIG_SPI_TSC2102
{
    .modalias			= "tsc2102",
    .controller_data 	= &tsc2102_hw,
    .max_speed_hz		= 8000000,
    .bus_num			= 2, // SPI2
    .irq                = IRQ_GPIOD(TSC2102_INT_GPIOD),
    .chip_select		= 0,
    .mode 				= 0,
    .platform_data      = &apf9328_tsc2102_config,
},
#endif // CONFIG_SPI_TSC2102
#ifdef CONFIG_CAN_MCP251X
 {
 .modalias          = "mcp251x",
 .max_speed_hz      = 8000000, /* 8MHz */
 .controller_data 	= &mcp251X_hw,
 .bus_num           = 2,
 .mode              = SPI_MODE_0,
 .chip_select       = 0,
 .irq               = IRQ_GPIOC( CAN_MPC251X_INT_GPIOC ),
 .platform_data     = &apf9328_mcp251x_config,
 },
#endif // CONFIG_CAN_MCP251X
};

// --- LCD RESSOURCE ---
#ifdef CONFIG_FB_IMX
// all custom LCD configuration is deported to this file for clarity purpose:
#include "apf9328_lcd_config.h"
#define IMX_FB_PHYS     (0x0C000000 - 0x40000)
#endif // CONFIG_FB_IMX

#ifdef CONFIG_IMX_BACKLIGHT
static struct imxbl_machinfo imx_bl_machinfo = {
    .max_intensity      = 0xff,
    .default_intensity  = 0x90,
    .limit_mask         = 0x7f, // When battery is low
//    set_bl_intensity = put a function here if you want to overload default one,
};
//extern struct platform_device imxfb_device; // filled in generic.c
static struct platform_device imxbl_device = {
    .name       = "imxl-bl",
    .dev        = {
//        .parent = &imxfb_device.dev, crash kernel even if EXPORT_SYMBOL() is done in generic.c
        .platform_data	= &imx_bl_machinfo,
    },
    .id        = 0,
};
#endif // CONFIG_IMX_BACKLIGHT

static struct platform_device *devices[] __initdata = {
    &imx_uart1_device,
    &imx_uart2_device,
#ifdef CONFIG_DM9000    
    &dm9000x_device,
#endif // CONFIG_DM9000
#ifdef CONFIG_SPI_IMX
    &imx_spi1,
    &imx_spi2,
#endif //CONFIG_SPI_IMX
    &imx_pwm_device,
#ifdef CONFIG_IMX_BACKLIGHT
    &imxbl_device,
#endif // CONFIG_IMX_BACKLIGHT
#if defined(CONFIG_SND_IMX_TSC2102) || defined(CONFIG_SND_IMX_TSC2102_MODULE)
	&tsc2102_alsa_device,
#endif // CONFIG_SND_IMX_TSC2102 || CONFIG_SND_IMX_TSC2102_MODULE
};

static void __init apf9328_init(void)
{
    printk("--- Registering APF9328 ressources\n");

    // Serial
    /*UFCR(IMX_UART1_BASE) = 0xa81; To be removed
    UFCR(IMX_UART2_BASE) = 0xa81;*/
    // LCD
#ifdef CONFIG_FB_IMX
    set_imx_fb_info(&apf9328_fb_info);
    printk("Set apf9328_fb_info\n");
    // init PORTD ....//
    imx_fb_set_gpios();
#endif // CONFIG_FB_IMX

#ifdef CONFIG_SENSORS_MAX1027
    max1027_init_cs();
#endif //CONFIG_SENSORS_MAX1027
#ifdef CONFIG_SPI_TSC2102
    tsc2102_init_gpio();
#endif // CONFIG_SPI_TSC2102
#ifdef CONFIG_CAN_MCP251X
    mcp251X_init_gpio();
#endif // CONFIG_CAN_MCP251X

    platform_add_devices(devices, ARRAY_SIZE(devices));
#if defined (CONFIG_SENSORS_MAX1027) || defined (CONFIG_SPI_TSC2102) || defined (CONFIG_CAN_MCP251X)
    spi_register_board_info(spi_board_info, ARRAY_SIZE(spi_board_info));
#endif 

#ifdef CONFIG_ARMADEUS_FPGA_DRIVERS
    request_mem_region(IMX_CS1_PHYS,IMX_CS1_SIZE,"FPGA");
#endif // CONFIG_ARMADEUS_FPGA_DRIVERS

    printk("--- APF9328 ressources registered\n");
}

/*
 * Register here the memory addresses we want to access from our drivers and
 * which are not already registerd in generic.c
 */
static struct map_desc apf9328_io_desc[] __initdata = {
#ifdef CONFIG_FB_IMX
    {
    .virtual    = IMX_FB_VIRT,
    .pfn        = __phys_to_pfn(IMX_FB_PHYS),
    .length     = IMX_FB_SIZE,
    .type       = MT_DEVICE
    },
#endif // CONFIG_FB_IMX
    // FPGA Memory mapping
    {.virtual = IMX_CS1_VIRT, .pfn = __phys_to_pfn(IMX_CS1_PHYS), .length = IMX_CS1_SIZE, .type = MT_DEVICE},
};

void __init apf9328_map_io(void)
{
    imx_map_io();
    iotable_init(apf9328_io_desc, ARRAY_SIZE(apf9328_io_desc));
}

static void __init apf9328_init_irq(void)
{
    // Init generic IRQs
    imx_init_irq();

    // Init our custom IRQs
#ifdef CONFIG_DM9000
    set_irq_type(APF9328_ETH_IRQ, IRQF_TRIGGER_FALLING);
#endif
#ifdef CONFIG_SPI_TSC2102
    set_irq_type( IRQ_GPIOD(TSC2102_INT_GPIOD), IRQF_TRIGGER_FALLING );
#endif
#ifdef CONFIG_CAN_MCP251X
    set_irq_type( IRQ_GPIOC(CAN_MPC251X_INT_GPIOC), IRQF_TRIGGER_FALLING );
#endif
#ifdef CONFIG_ARMADEUS_FPGA_DRIVERS
    set_irq_type( APF9328_FPGA_IRQ, IRQF_TRIGGER_RISING );
#endif
}


MACHINE_START(APF9328, "Armadeus APF9328")
    /* Maintainer: Julien Boibessot, Armadeus */
    .phys_io      = 0x00200000,
    .io_pg_offst  = ((0xe0200000) >> 18) & 0xfffc,
    .boot_params  = 0x08000100,
    .map_io       = apf9328_map_io,
    .init_irq     = apf9328_init_irq,
    .timer        = &imx_timer,
    .init_machine = apf9328_init,
MACHINE_END