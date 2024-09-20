/*
 * W25QXX.h
 *
 *  Created on: Mar 21, 2024
 *      Author: 19839
 */

/*
程序是我理解数据手册和学习了大佬写的程序后自己造的轮子，如果觉得好用可以多多移植使用
功能：用于SPI方式读写华邦的W25QXX（W25Q80、W25Q16、W25Q32、W25Q64、W25Q128...）系列FLASH的数据。
开发环境：
	KEIL版本：V5.21a
	CUBEMX版本：V6.2.1
	CUBEMX针对STM32F407VET6的库版本：STM32Cube_FW_F4_V1.26.2
	系统版本：Win10
	FLASH型号：W25Q16
	STM32的SPI口：SPI1（CLK--PB3，MISO--PB4，MOSI--PB5，CS--PB0），如果需要移植，注意这几个IO口。
	W25QXX指令来源：华邦发布于2017年5月9日的数据手册“W25Q16JV”，Revision F版本

	其他flash信息：
	华邦发布于2015年10月2日的数据手册“W25Q80DV/DL”，Revision H版本
	华邦发布于2017年5月9日的数据手册“W25Q16JV”，Revision F版本
	华邦发布于2017年5月11日的数据手册“W25Q32JV”，Revision F版本
	华邦发布于2017年5月11日的数据手册“W25Q64JV”，Revision H版本
	华邦发布于2016年11月16日的数据手册“W25Q128JV”，Revision C版本
	华邦发布于2017年8月3日的数据手册“W25Q256JV”，Revision G版本
	华邦发布于2019年6月25日的数据手册“W25Q512JV”，Revision B版本
*/

#ifndef __W25QXX_H
#define __W25QXX_H

#include "stm32l4xx.h"
#include "spi.h"

/* ???????? ----------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

typedef  uint32_t  u32;
typedef  uint8_t  u8;//
typedef  uint16_t  u16;

#define SPI_FLASH_REBUILD         0    //1:???????????Flash??0??????????????Flash
#define SPI_FLASH_SECTOR_SIZE    4096    // ????Flash????????
#define SPI_FLASH_START_SECTOR   256*4    // ????Flash?????FatFS?????
#define SPI_FLASH_SECTOR_COUNT   256   // ????Flash?????FatFS???????????
//W25X/Q不同容量对应不同ID关系
#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18
//#define  sFLASH_ID     0XEF4017     //W25Q64
#define  sFLASH_ID 0XEF14
extern SPI_HandleTypeDef hspi1;
extern u16 W25QXX_TYPE;					//????W25QXX??????

//#define	W25QXX_CS 		PBout(12)  		//W25QXX片选引脚
#define W25QXX_CS_1		  HAL_GPIO_WritePin(GPIOB,SPI1_CS_Pin,GPIO_PIN_SET)
#define W25QXX_CS_0	      HAL_GPIO_WritePin(GPIOB,SPI1_CS_Pin,GPIO_PIN_RESET)

#define W25X_WriteEnable		0x06
#define W25X_WriteDisable		0x04
#define W25X_ReadStatusReg1		0x05
#define W25X_ReadStatusReg2		0x35
#define W25X_ReadStatusReg3		0x15
#define W25X_WriteStatusReg1    0x01
#define W25X_WriteStatusReg2    0x31
#define W25X_WriteStatusReg3    0x11
#define W25X_ReadData			0x03
#define W25X_FastReadData		0x0B
#define W25X_FastReadDual		0x3B
#define W25X_PageProgram		0x02
#define W25X_BlockErase			0xD8
#define W25X_SectorErase		0x20
#define W25X_ChipErase			0xC7
#define W25X_PowerDown			0xB9
#define W25X_ReleasePowerDown	0xAB
#define W25X_DeviceID			0xAB
#define W25X_ManufactDeviceID	0x90
#define W25X_JedecDeviceID		0x9F
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

void W25QXX_Init(void);
u16  W25QXX_ReadID(void);  	    		//???FLASH ID
void W25QXX_WAKEUP(void);				//????
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

#endif



