#ifndef __INT_WTN6170_H__
#define __INT_WTN6170_H__
#include "driver/gpio.h"
#include "esp_task.h"
#include "sys/unistd.h"

#define WTN_DATA GPIO_NUM_9

#define sayWithoutInt() Int_WTN6170_SendByte(0xf3)

#define sayNum(x) Int_WTN6170_SendByte(x + 1)
#define sayWaterDrop() Int_WTN6170_SendByte(11)
#define sayBuzzer() Int_WTN6170_SendByte(12)
#define sayAlarm() Int_WTN6170_SendByte(13)
#define sayDoorBell() Int_WTN6170_SendByte(14)
#define sayFail() Int_WTN6170_SendByte(16)
#define sayPassword() Int_WTN6170_SendByte(19)
#define sayDoorOpen() Int_WTN6170_SendByte(25)
#define sayDoorClose() Int_WTN6170_SendByte(26)
#define sayIllegalOperation() Int_WTN6170_SendByte(28)
#define sayVerify() Int_WTN6170_SendByte(31)
#define sayFinish() Int_WTN6170_SendByte(33)
#define sayPressSharp() Int_WTN6170_SendByte(34)
#define sayDelSucc() Int_WTN6170_SendByte(36)
#define sayDelFail() Int_WTN6170_SendByte(37)
#define sayVerifySucc() Int_WTN6170_SendByte(41)
#define sayVerifyFail() Int_WTN6170_SendByte(42)
#define saySetSucc() Int_WTN6170_SendByte(43)
#define saySetFail() Int_WTN6170_SendByte(44)
#define sayOperateSucc() Int_WTN6170_SendByte(46)
#define sayOperateFail() Int_WTN6170_SendByte(47)
#define sayInvalid() Int_WTN6170_SendByte(48)
#define sayAddSucc() Int_WTN6170_SendByte(49)
#define sayAddFail() Int_WTN6170_SendByte(50)
#define sayAddUser() Int_WTN6170_SendByte(51)
#define sayDelUser() Int_WTN6170_SendByte(52)
#define sayUserFull() Int_WTN6170_SendByte(53)
#define sayAddAdmin() Int_WTN6170_SendByte(54)
#define sayDelAdmin() Int_WTN6170_SendByte(55)
#define sayAdminFull() Int_WTN6170_SendByte(56)
#define sayRetry() Int_WTN6170_SendByte(58)
#define sayInputUserPassword() Int_WTN6170_SendByte(64)
#define sayInputUserPasswordAgain() Int_WTN6170_SendByte(65)
#define sayPasswordAddSucc() Int_WTN6170_SendByte(66)
#define sayPasswordAddFail() Int_WTN6170_SendByte(67)
#define sayPasswordVerifySucc() Int_WTN6170_SendByte(68)
#define sayPasswordVerifyFail() Int_WTN6170_SendByte(69)
#define sayInputAdminPassword() Int_WTN6170_SendByte(70)
#define sayInputAdminPasswordAgain() Int_WTN6170_SendByte(71)
#define sayAddUserFingerprint() Int_WTN6170_SendByte(76)
#define sayDelUserFingerprint() Int_WTN6170_SendByte(77)
#define sayPlaceFinger() Int_WTN6170_SendByte(80)
#define sayPlaceFingerAgain() Int_WTN6170_SendByte(81)
#define sayTakeAwayFinger() Int_WTN6170_SendByte(82)
#define sayFingerprintAddSucc() Int_WTN6170_SendByte(83)
#define sayFingerprintAddFail() Int_WTN6170_SendByte(84)
#define sayFingerprintVerifySucc() Int_WTN6170_SendByte(85)
#define sayFingerprintVerifyFail() Int_WTN6170_SendByte(86)

// P9引脚拉高、拉低
#define WTN_DATA_H (gpio_set_level(WTN_DATA, 1))
#define WTN_DATA_L (gpio_set_level(WTN_DATA, 0))

void Int_WTN6170_Init(void);

void Int_WTN6170_SendByte(uint8_t byte);

#endif /* __INT_WTN6170_H__ */
