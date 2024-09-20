/*
 * BadUSB_Conf.h
 *
 *  Created on: May 13, 2024
 *      Author: 19839
 */

/**Full key conflict free descriptor:
 *-----------------------------------------------------------------------------
 * Descriptor length:73Bytes
 * ____________________________________________________________________________
 *| Send Bytes Num:15Bytes                                                     |
 *| buffer[0] - bit0: Left CTRL                                                |
 *|           - bit1: Left SHIFT                                               |
 *|           - bit2: Left ALT                                                 |
 *|           - bit3: Left GUI                                                 |
 *|           - bit4: Right CTRL                                               |
 *|           - bit5: Right SHIFT                                              |
 *|           - bit6: Right ALT                                                |
 *|           - bit7: Right GUI                                                |
 *| buffer[1] - Padding = Always 0x00                                          |
 *| buffer[2] - (A & a) ~ (H & h)                                              |
 *| buffer[3] - (I & i) ~ (P & p)                                              |
 *| buffer[4] - (Q & q) ~ (X & x)                                              |
 *| buffer[5] - (Y & y) ~ (Z & z) | 1 ~ 6                                      |
 *| buffer[6] - 7 ~ 0 | Enter | Esc | Backspace | Tab                          |
 *|** This is the number key 1 ~ 0 in the main keyboard area                   |
 *| buffer[7] - Space | - | = | [ | ] | \ | \ | ;                              |
 *| buffer[8] - ' | ` | , | . | / | CapsLock | F1 ~ F2                         |
 *| buffer[9] - F3 ~ F10                                                       |
 *| buffer[A] - F11 ~ F12 | PRTSRC | ScrollLock | Pause | Insert | Home | PgUp |
 *| buffer[B] - Delete | End | PgDn | Right | Left | Down | Up | NumLock       |
 *| buffer[C] - / | * | - | + | Enter | 1 ~ 3                                  |
 *| buffer[D] - 4 ~ 0 | .                                                      |
 *|** This is the number key 1 ~ 0 in the numeric keypad area                  |
 *| buffer[E] - (Keypad 6) ~ (Keyboard Application)                            |
 *|____________________________________________________________________________|
 *| Recv Bytes Num:1Bytes                                                      |
 *| buffer[0] - bit0: Num Lock       //States of Num Lock LED                  |
 *|           - bit1: Caps Lock      //States of Caps Lock LED                 |
 *|           - bit2: Scroll Lock    //States of Scroll Lock LED               |
 *|           - bit3: Compose        //States of Compose LED                   |
 *|           - bit4: Kana           //States of Kana LED                      |
 *|           - bit5-7: Additional LED                                         |                                              |
 *|____________________________________________________________________________|
 *-----------------------------------------------------------------------------
 **Following is the mapping from:
 *| [ASCII code characters] to [corresponding keyboard descriptors]
 * ____________________________________________________________________________
 *|        From ASCII to Keyboard descriptors Mapping Table                    |
 *| 1. This is a mapping in [half-width] + [lowercase] mode                    |
 *| 2. The upper four bits are the (buffer index:Ah[from 0 to 15]),            |
 *| 3. the highest bit of the lower four bits is                               |
 *|    whether to use the control key shift(Bh+8h/Bh+0h),                      |
 *| 4. and the remaining three bits are the                                    |
 *|    (byte numbers:Bh[from 0h to 7h])                                        |
 *|    corresponding to                                                        |
 *|    the descriptor of the ASCII character in the buffer array index         |
 *| 5. map[ASCII] = 0x(A)(B/B+8)                                               |
 *| 6. map[0]-[127] is standard mapping from ASCII to its descriptor           |
 *|    map[128]-[]  is Customize keyboard shortcuts to its descriptor:         |
 *|        (1).map[128]: Capslock's descriptor                                 |
 *|        (2).map[129]: Backspace's descriptor                                |
 *|        (3).map[130]: Ctrl's descriptor                                     |
 *|        (4).map[131]: Alt's descriptor                                      |
 *|        Linux start Terminal(Ctrl + Alt + T)                                |
 *|        (5).map[132]: GUI's descriptor (Win)                                |
 *|        Windows start Terminal(Win + R)                                     |
 *|        (6).map[133]: NumLock's descriptor                                  |
 *|____________________________________________________________________________|
  uint8_t map[134]={
  		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  		0x00, 0x00, 0x64, 0x00, 0x00, 0x64, 0x00, 0x00,
  		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  		0x70, 0x5a, 0x88, 0x5c, 0x5d, 0x5e, 0x68, 0x80,
  		0x6a, 0x6b, 0x69, 0x7a, 0x82, 0x71, 0x83, 0x84,
  		0x63, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x60,
  		0x61, 0x62, 0x7f, 0x77, 0x8a, 0x72, 0x8b, 0x8c,
  		0x5b, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
  		0x2f, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
  		0x3f, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
  		0x4f, 0x58, 0x59, 0x73, 0x75, 0x74, 0x5f, 0x79,
  		0x81, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
  		0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
  		0x37, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
  		0x47, 0x50, 0x51, 0x7b, 0x7d, 0x7c, 0x89, 0xb0,
  		0x85,     //ASCII[128]:Capslock
  		0x66,     //ASCII[129]:Backspace
  		0x00,     //ASCII[130]:Ctrl
  		0x02,     //ASCII[131]:Alt
  		0x03,     //ASCII[132]:GUI
  		0xb7,     //ASCII[133]:NumLock
  };
 *-----------------------------------------------------------------------------
 **Following are the descriptor set in Function:
 *                CUSTOM_HID_ReportDesc_FS in usbd_custom_hid_if.c
 //  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)//73U
 //  0x09, 0x06,                    // USAGE (Keyboard)
 //  0xa1, 0x01,                    // COLLECTION (Application)
 //  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
 //  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
 //  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
 //  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
 //  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
 //  0x95, 0x08,                    //   REPORT_COUNT (8)
 //  0x75, 0x01,                    //   REPORT_SIZE (1)
 //  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
 //  0x95, 0x01,                    //   REPORT_COUNT (1)
 //  0x75, 0x08,                    //   REPORT_SIZE (8)
 //  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
 //  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
 //  0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
 //  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
 //  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
 //  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
 //  0x95, 0x62,                    //   REPORT_COUNT (98)
 //  0x75, 0x01,                    //   REPORT_SIZE (1)
 //  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
 //  0x95, 0x01,                    //   REPORT_COUNT (1)
 //  0x75, 0x06,                    //   REPORT_SIZE (6)
 //  0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
 //  0x05, 0x08,                    //   USAGE_PAGE (LEDs)
 //  0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
 //  0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
 //  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
 //  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
 //  0x95, 0x05,                    //   REPORT_COUNT (5)
 //  0x75, 0x01,                    //   REPORT_SIZE (1)
 //  0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
 //  0x95, 0x01,                    //   REPORT_COUNT (1)
 //  0x75, 0x03,                    //   REPORT_SIZE (3)
 //  0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
 //  0xc0                           // END_COLLECTION
 *----------------------------------------------------------------------------
 **Following are init steps about this Project:
 * 1. usbd_conf.h: //can modify to USER CODE position
 *    #define USBD_CUSTOM_HID_REPORT_DESC_SIZE     73U
 *    #define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE    15U
 *    #define USBD_CUSTOMHID_INREPORT_BUF_SIZE     01U
 * 2. usbd_customhid.h:
 *    #ifndef USBD_CUSTOMHID_OUTREPORT_BUF_SIZE
 *    #define USBD_CUSTOMHID_OUTREPORT_BUF_SIZE  0x0fU //15Bytes
 *    #endif  //USBD_CUSTOMHID_OUTREPORT_BUF_SIZE
 *    #ifndef USBD_CUSTOMHID_INREPORT_BUF_SIZE
 *    #define USBD_CUSTOMHID_INREPORT_BUF_SIZE   01U   //1Bytes
 *    #endif  //USBD_CUSTOMHID_INREPORT_BUF_SIZE
 *    #ifndef USBD_CUSTOM_HID_REPORT_DESC_SIZE
 *    #define USBD_CUSTOM_HID_REPORT_DESC_SIZE   73U   //73Bytes
 *    #endif  //USBD_CUSTOM_HID_REPORT_DESC_SIZE
 * 3. usbd_customhid.c:
 *    USB CUSTOM_HID device FS Configuration Descriptor:
 *        Descriptor of CUSTOM HID interface:
 *            0x01,  //nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse
 * 4. usbd_custom_hid_if.c:
 *    Modify your code to USER CODE position:
 *    (1).Referencing external variables:
 *        (1).if stm32f0xx:
 *            extern uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];
 *            extern int InterruptFlag;
 *            extern USBD_HandleTypeDef hUsbDeviceFS;
 *        (2).if stm32l4xx:
 *            Skip this step;
 *    (2).Fill in custom keyboard device descriptors
 *    (3).Customize Callback function:[CUSTOM_HID_OutEvent_FS]
 *        Which deals with received data from host.
 *        (1).stm32f0xx:Add following codes
 *        (2).stm32l4xx:Add following codes in file:[usbd_customhid.c]
 *            In function USBD_CUSTOM_HID_ReceivePacket:
 *        //definition:
 *          extern uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];
 *          extern int InterruptFlag;
 *        //After func USBD_LL_PrepareReceive in USBD_CUSTOM_HID_ReceivePacket
 *        / *USBD_CUSTOMHID_INREPORT_BUF_SIZE is always 1,
 *          * Copy hhid->Report_buf[0] directly to recv_buffer[0]
 *          * By judging conditions,
 *          * Aiming to eliminating loop operations:
 *          * for(int i = 0; i < USBD_CUSTOMHID_INREPORT_BUF_SIZE; i++)
 *          *     recv_buffer[i] = hhid->Report_buf[i];
 *          * /
 *          if(((recv_buffer[0]=(hhid->Report_buf[0]))&0x02) != 0x02){
 *              InterruptFlag = 1;
 *          }
 * 5. stm32l4xx_hal_pcd.c/stm32f0xx_hal_pcd.c:
 *    (1).Add these external declarations at the beginning of this file:
 *        #define USBD_CUSTOMHID_INREPORT_BUF_SIZE 1
 *        extern uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];
 *        extern int InterruptFlag;
 *        extern int NeedRollBack;
 *    (2).In function: HAL_StatusTypeDef HAL_PCD_EP_Transmit();(line 2049)
 *        Add following codes **before** USB_EPStartXfer function:
 *            //Determine whether to generate an interrupt:
 *            if((recv_buffer[0]&0x02) != 0x02)
 *                InterruptFlag = 1;
 *        Add following codes **after** USB_EPStartXfer function:
 *            //Output test to choose whether to RollBack:
 *            if((recv_buffer[0]&0x02) != 0x02)
 *                NeedRollBack = 1;
 *----------------------------------------------------------------------------*/

#ifndef INC_BADUSB_CONF_H_
#define INC_BADUSB_CONF_H_

#define FPGA       1
#define Desktop    2
#define Target     FPGA
#define File_Host_Addr "192.168.2.207"

/*You can adjust following configure Slot:
 * to ensure the the correctness and stability of the output*/
#if Target == FPGA
  #define StrokeSlot 35
  #define PlugSlot 50
  #define ShortcutSlot 1000
  #define SwitchDeviceSlot 3000
  #define TestHIDSlot 1000
  #define InjectStringSlot 1000
#elif Target == Desktop
  #define StrokeSlot 35
  #define PlugSlot 50
  #define ShortcutSlot 500
  #define SwitchDeviceSlot 1000
  #define TestHIDSlot 500
  #define InjectStringSlot 1000
#endif

uint8_t sent_buffer[USBD_CUSTOMHID_OUTREPORT_BUF_SIZE];

uint8_t recv_buffer[USBD_CUSTOMHID_INREPORT_BUF_SIZE];

#define MapLen 134
#define Map_Init {\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
0x00, 0x00, 0x64, 0x00, 0x00, 0x64, 0x00, 0x00,\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
0x70, 0x5a, 0x88, 0x5c, 0x5d, 0x5e, 0x68, 0x80,\
0x6a, 0x6b, 0x69, 0x7a, 0x82, 0x71, 0x83, 0x84,\
0x63, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x60,\
0x61, 0x62, 0x7f, 0x77, 0x8a, 0x72, 0x8b, 0x8c,\
0x5b, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,\
0x2f, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,\
0x3f, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,\
0x4f, 0x58, 0x59, 0x73, 0x75, 0x74, 0x5f, 0x79,\
0x81, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,\
0x27, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,\
0x37, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,\
0x47, 0x50, 0x51, 0x7b, 0x7d, 0x7c, 0x89, 0xb0,\
0x85, 0x66, 0x00, 0x02, 0x03, 0xb7, \
}\

#define LinuxTerminal {130, 131, 'T'}
#define WindowsTerminal {132, 'R'}

#endif /* INC_BADUSB_CONF_H_ */
