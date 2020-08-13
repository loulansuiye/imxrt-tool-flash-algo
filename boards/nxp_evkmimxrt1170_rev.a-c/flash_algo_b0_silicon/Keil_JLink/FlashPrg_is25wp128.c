/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for New Device 256kB Flash                            */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.H" // FlashOS Structures
#include "fsl_romapi.h"
#include "clock_config.h"

#define FLEXSPI_NOR_INSTANCE 1
#define SECTOR_SIZE (0x10000)
#define BASE_ADDRESS (0x30000000)
#define CONFIG_OPTION (0xc0000007)

/* Init this global variable to workaround of the issue to running this flash algo in Segger */
flexspi_nor_config_t config = {1};

static void restore_clock()
{
        unsigned int i = 0;
        for (i = 0; i < 79; i++)
        {
                CCM->CLOCK_ROOT[i].CONTROL = 0;
        }
}

/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{
    status_t status;
    serial_nor_config_option_t option;

    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_DisableDCache();
    }
    
    restore_clock();
    CCM->CLOCK_ROOT[kCLOCK_Root_M7].CONTROL = 0x200;
    CCM->CLOCK_ROOT[kCLOCK_Root_M4].CONTROL = 0x201;
    /* PLL LDO shall be enabled first before enable PLLs */
    CLOCK_EnableOsc24M();
    /* SYS PLL2 528MHz. */
    const clock_sys_pll_config_t sysPllConfig = {
        .loopDivider = 1,
        /* Using 24Mhz OSC */
        .mfn = 0,
        .mfi = 22,
    };
    CLOCK_InitSysPll2(&sysPllConfig);
    const clock_sys_pll3_config_t sysPll3Config = {
        .divSelect = 3,
    };
    CLOCK_InitSysPll3(&sysPll3Config);

    CCM->CLOCK_ROOT[kCLOCK_Root_Flexspi1].CONTROL_SET = 0x503;
    option.option0.U = CONFIG_OPTION;
    SRC->GPR[9] = 0;

    ROM_API_Init();
    option.option0.U = 0xc0000007;

    status = ROM_FLEXSPI_NorFlash_GetConfig(FLEXSPI_NOR_INSTANCE, &config, &option);
    if (status != kStatus_Success)
    {
        return (1);
    }

    status = ROM_FLEXSPI_NorFlash_Init(FLEXSPI_NOR_INSTANCE, &config);
    if (status != kStatus_Success)
    {
        return (1);
    }
    else
    {
        return (0); // Finished without Errors
    }
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit(unsigned long fnc)
{
    /* Add your Code */
    return (0); // Finished without Errors
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseChip(void)
{
    status_t status;
    status = ROM_FLEXSPI_NorFlash_EraseAll(FLEXSPI_NOR_INSTANCE, &config); // Erase all
    if (status != kStatus_Success)
    {
        return (1);
    }
    else
    {
        return (0); // Finished without Errors
    }
}

/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector(unsigned long adr)
{
    status_t status;
    adr    = adr - BASE_ADDRESS;
    status = ROM_FLEXSPI_NorFlash_Erase(FLEXSPI_NOR_INSTANCE, &config, adr, SECTOR_SIZE); // Erase 1 sector
    if (status != kStatus_Success)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf)
{
    status_t status;
    adr = adr - BASE_ADDRESS;
    // Program data to destination
    status = ROM_FLEXSPI_NorFlash_ProgramPage(FLEXSPI_NOR_INSTANCE, &config, adr, (const uint32_t *)buf); // program 1 page
    if (status != kStatus_Success)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}
