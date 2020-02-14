//****************************************************************************
//
// FlashW25Q128.h
//
// Header file for the W25Q128 128Mbit Winbond SPI Flash
//
// Copyright (c) Microsoft. All rights reserved.
//
//****************************************************************************
#pragma once

//****************************************************************************
//
//                               INCLUDES
//
//****************************************************************************

//****************************************************************************
//
//                                  DEFINES
//
//****************************************************************************

//****************************************************************************
//
//                                  TYPEDEFS
//
//****************************************************************************
typedef enum
{
    W25Q16FW_FLASH_COMMAND_READ_DATA                = 0x03,
    W25Q16FW_FLASH_COMMAND_QUAD_READ                = 0x6B,
    W25Q16FW_FLASH_COMMAND_READ_STATUS_REGISTER     = 0x05,
    W25Q16FW_FLASH_COMMAND_WRITE_STATUS_REGISTER_2  = 0x31,
    W25Q16FW_FLASH_COMMAND_WRITE_ENABLE             = 0x06,
    W25Q16FW_FLASH_COMMAND_SECTOR_ERASE             = 0x20,
    W25Q16FW_FLASH_COMMAND_BLOCK_ERASE              = 0xD8,
    W25Q16FW_FLASH_COMMAND_PAGE_PROGRAM             = 0x02,
    W25Q16FW_FLASH_COMMAND_CHIP_ERASE               = 0x60,
    W25Q16FW_FLASH_COMMAND_READ_JEDEC               = 0x9F,
} W25Q16FW_FLASH_COMMANDS;
//****************************************************************************
//
//                          GLOBAL VARIABLE EXTERNS
///
//****************************************************************************

//****************************************************************************
//
//                          GLOBAL FUNCTION EXTERNS
//
//****************************************************************************
