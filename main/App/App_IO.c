#include "App_IO.h"

//* 定义用户输入缓冲区
uint8_t first_buffers[50];
uint8_t second_buffers[50];

//* 用户输入的次数,用于判断是第一次还是第二次输入密码
uint8_t send_flag = 1;

//* 输入的是谁的密码,用于区分管理员和普通用户
uint8_t user_type = 0; // 0-管理员 1-普通用户

//* 添加管理员用户
static void App_IO_AddAdmin(void);

//* 删除管理员用户
static void App_IO_DelAdmin(void);

//* 验证管理员身份
static Com_Status App_IO_CheckAdmin(void);

//* 添加普通用户
static void App_IO_AddUser(void);

//* 删除普通用户
static void App_IO_DelUser(void);

//* 重置缓冲数组
static void App_IO_ResetBuffers(void);

//* 用户输入密码
static void App_IO_UserInputPassword(void);

//* 密码开门
static void App_IO_OpenDoor(uint8_t openInfo[]);

//* 模块初始化
void App_IO_Init(void)
{
    Int_BDR6120_Init();
    Int_SC12B_Init();
    Int_WS2812_Init();
    Int_WTN6170_Init();
    Dri_NVS_Init();
}

Com_Status App_IO_GetUserInputContent(uint8_t buffers[])
{
    uint8_t timeout = 0; // 存储超时时间
    uint8_t index = 0;   // 输入数组索引
    while (1)
    {
        //* 获取用户输入数据
        Touch_Key key_num = Int_SC12B_GetKeyNum();
        if (key_num == KEY_NO)
        {
            //* 没有按下按键
            timeout++;
            if (timeout >= 60)
            {
                //* 超过3s即为超时
                return Com_TIMEOUT;
            }
        }
        else
        {
            //* 超时之前按下了按键
            timeout = 0;

            //* 添加按键声音
            sayWithoutInt();
            sayWaterDrop();
            //* 点亮对应的led
            Int_WS2818_LED_Off_All();
            Int_WS2812_LED_On(key_num, cyan);

            if (key_num == KEY_SHARP)
            {
                //* 用户按下了#,视为输入完成
                return Com_OK;
            }

            if (key_num == KEY_M)
            {
                //* 用户按下了M视为非法操作
                return Com_ERROR;
            }

            buffers[index++] = key_num + 48; // 转换为字符存储到缓冲数组中
        }
        vTaskDelay(50); // 50ms执行一次
    }
}

//* 处理用户输入数据
void App_IO_Handler(uint8_t receive_buffers[])
{
    uint8_t length = strlen((char *)receive_buffers);

    if (length < 2)
    {
        //* 非法操作
        sayWithoutInt();
        sayIllegalOperation();
    }
    else if (length == 2)
    {
        //* 判断命令
        if (receive_buffers[0] == '0' && receive_buffers[1] == '0')
        {
            //* 00 -- 添加管理员
            user_type = 0;
            App_IO_AddAdmin();
        }
        else if (receive_buffers[0] == '0' && receive_buffers[1] == '1')
        {
            //* 01 -- 删除管理员
            App_IO_DelAdmin();
        }
        else if (receive_buffers[0] == '1' && receive_buffers[1] == '0')
        {
            //* 10 -- 添加普通用户
            user_type = 1;
            App_IO_AddUser();
        }
        else if (receive_buffers[0] == '1' && receive_buffers[1] == '1')
        {
            //* 11 -- 删除普通用户
            App_IO_DelUser();
        }
        else if (receive_buffers[0] == '2' && receive_buffers[1] == '2')
        {
            //* 22 -- 删除所有用户
            esp_err_t result = Dri_NVS_DelAll();
            if (result == ESP_OK)
            {
                //* 删除成功
                sayWithoutInt();
                sayDelSucc();
            }
            else
            {
                //* 删除失败
                sayWithoutInt();
                sayDelFail();
            }
        }
    }
    else
    {
        //* 剩余情况判断密码是否输入正确,决定是否开门
        App_IO_OpenDoor(receive_buffers);
    }
}

//* 添加管理员
void App_IO_AddAdmin(void)
{
    //* 先判断是否已有管理员
    if (Dri_NVS_KeyIsExist(ADMIN) == ESP_OK)
    {
        //* 提示已有管理员
        sayWithoutInt();
        sayAdminFull();
    }
    else
    {
        //* 没有管理员
        sayWithoutInt();
        sayInputAdminPassword();
        send_flag = 1;
        App_IO_UserInputPassword();
    }
}

//* 删除管理员
void App_IO_DelAdmin(void)
{
    //* 先判断是否有管理员
    if (Dri_NVS_KeyIsExist(ADMIN) == ESP_OK)
    {
        //* 有管理员
        //* 验证管理员身份
        Com_Status status = App_IO_CheckAdmin();
        if (status == Com_OK)
        {
            // 管理员身份校验成功-FLASH干掉
            esp_err_t res = Dri_NVS_DelByKey(ADMIN);
            // flash管理员数据删除成功
            if (res == ESP_OK)
            {
                sayWithoutInt();
                sayDelAdmin();
                sayWithoutInt();
                sayDelSucc();
            }
            else
            {
                sayWithoutInt();
                sayDelAdmin();
                sayWithoutInt();
                sayDelFail();
            }
        }
        else
        {
            sayWithoutInt();
            sayRetry();
        }
    }
    else
    {
        //* 没有管理员
        sayWithoutInt();
        sayIllegalOperation();
    }
}

//* 验证管理员身份
Com_Status App_IO_CheckAdmin(void)
{
    //* 提示输入管理员密码
    sayWithoutInt();
    sayInputAdminPassword();

    //* 获取用户输入的密码
    Com_Status status = App_IO_GetUserInputContent(first_buffers);
    switch (status)
    {
    case Com_OK:
        //* 确定管理员密码是否输入正确
        char admin_password_buffer[50];
        size_t sizes = 50;
        Dri_NVS_GetStr(ADMIN, admin_password_buffer, &sizes);
        if (strcmp((char *)first_buffers, admin_password_buffer) == 0)
        {
            //* 密码输入正确
            App_IO_ResetBuffers();
            return Com_OK;
        }
        else
        {
            //* 输入不正确
            App_IO_ResetBuffers();
            return Com_ERROR;
        }
    case Com_ERROR:
        //* 非法操作
        App_IO_ResetBuffers();
        return Com_ERROR;
    case Com_TIMEOUT:
        //* 输入超时
        App_IO_ResetBuffers();
        return Com_TIMEOUT;
    default:
        break;
    }
    return Com_ERROR;
}

//* 添加普通用户
void App_IO_AddUser(void)
{
    //* 先判断是否有管理员信息
    if (Dri_NVS_KeyIsExist(ADMIN) != ESP_OK)
    {
        //* 管理员不存在
        sayWithoutInt();
        sayIllegalOperation();
    }
    else
    {
        //* 管理员存在
        //* 验证管理员身份
        Com_Status status = App_IO_CheckAdmin();
        if (status == Com_OK)
        {
            //* 验证通过
            //* 提示输入用户密码
            sayWithoutInt();
            sayInputUserPassword();
            send_flag = 1;
            App_IO_UserInputPassword();
        }
        else
        {
            //* 验证不通过,请重试
            sayWithoutInt();
            sayRetry();
        }
    }
}

//* 删除普通用户
void App_IO_DelUser(void)
{
    //* 先判断是否有管理员信息
    if (Dri_NVS_KeyIsExist(ADMIN) != ESP_OK)
    {
        //* 管理员不存在
        sayWithoutInt();
        sayIllegalOperation();
    }
    else
    {
        //* 管理员存在
        //* 验证管理员身份
        Com_Status status = App_IO_CheckAdmin();
        if (status == Com_OK)
        {
            //* 验证通过
            //* 提示输入用户密码
            sayWithoutInt();
            sayInputUserPassword();

            //* 接收用户输入的密码
            status = App_IO_GetUserInputContent(first_buffers);
            if (status == Com_OK)
            {
                //* 删除用户密码
                esp_err_t res = Dri_NVS_DelByKey((char *)first_buffers);
                if (res == ESP_OK)
                {
                    //* 删除成功
                    sayWithoutInt();
                    sayDelUser();
                    sayWithoutInt();
                    sayDelSucc();
                }
                else
                {
                    //* 删除失败
                    sayWithoutInt();
                    sayDelUser();
                    sayWithoutInt();
                    sayDelFail();
                }
            }
            else
            {
                //* 接收用户密码失败,请重试
                sayWithoutInt();
                sayRetry();
            }
        }
        else
        {
            //* 验证管理员失败,请重试
            sayWithoutInt();
            sayRetry();
        }
    }
}

void App_IO_ResetBuffers(void)
{
    memset(first_buffers, 0, 50);
    memset(second_buffers, 0, 50);
}

void App_IO_UserInputPassword(void)
{
    Com_Status status;
    if (send_flag == 1)
    {
        //* 第一次获取输入密码
        status = App_IO_GetUserInputContent(first_buffers);
        send_flag = 2;
    }
    else
    {
        //* 第二次获取输入密码
        status = App_IO_GetUserInputContent(second_buffers);
        send_flag = 1;
    }

    switch (status)
    {
    case Com_OK:
        if (send_flag == 2)
        {
            //* 第一次输入密码成功
            //* 语音提示再次输入密码
            if (user_type == 0)
            {
                //* 管理员
                sayWithoutInt();
                sayInputAdminPasswordAgain();
            }
            else
            {
                //* 普通用户
                sayWithoutInt();
                sayInputUserPasswordAgain();
            }
            App_IO_UserInputPassword();
        }
        else
        {
            //* 第二次输入密码成功
            //* 比较两次输入密码是否一致
            if (strcmp((char *)first_buffers, (char *)second_buffers) == 0)
            {
                //* 两次密码输入一致,可以写入flash
                if (user_type == 0)
                {
                    //* 写入管理员密码
                    esp_err_t err = Dri_NVS_SetStr(ADMIN, (char *)first_buffers);
                    if (err == ESP_OK)
                    {
                        //* 写入成功
                        sayWithoutInt();
                        sayAddAdmin();
                        sayWithoutInt();
                        sayAddSucc();
                    }
                    else
                    {
                        //* 写入失败
                        sayWithoutInt();
                        sayAddAdmin();
                        sayWithoutInt();
                        sayAddFail();
                    }
                }
                else
                {
                    //* 先判断这个密码是否存在
                    if (Dri_NVS_KeyIsExist((char *)first_buffers) == ESP_OK)
                    {
                        //* 密码已存在,无法再次添加
                        sayWithoutInt();
                        sayAddFail();
                    }
                    else
                    {
                        //* 不存在,可以写入
                        esp_err_t err = Dri_NVS_SetU8((char *)first_buffers, 0);
                        if (err == ESP_OK)
                        {
                            //* 写入成功
                            sayWithoutInt();
                            sayAddUser();
                            sayWithoutInt();
                            sayAddSucc();
                        }
                        else
                        {
                            //* 写入失败
                            sayWithoutInt();
                            sayAddUser();
                            sayWithoutInt();
                            sayAddFail();
                        }
                    }
                }
            }
        }
        break;
    case Com_ERROR:
        //* 非法操作
        sayWithoutInt();
        sayIllegalOperation();
        break;
    case Com_TIMEOUT:
        //* 提示超时,请重试
        sayWithoutInt();
        sayRetry();
        break;
    default:
        break;
    }

    //* 清空缓冲数组
    App_IO_ResetBuffers();
}

//* 密码开门
void App_IO_OpenDoor(uint8_t openInfo[])
{
    //* 获取管理员密码
    char admin_password_buffer[10];
    size_t sizes = 10;
    Dri_NVS_GetStr(ADMIN,admin_password_buffer,&sizes);

    if(Dri_NVS_KeyIsExist((char *)openInfo) == ESP_OK || strcmp(admin_password_buffer,(char *)openInfo) == 0)
    {
        sayWithoutInt();
        sayDoorOpen();

        //* 开门
        Int_BDR6120_OpenDoor();

        sayWithoutInt();
        sayDoorClose();
    }
    else
    {
        //* 报警
        sayWithoutInt();
        sayAlarm();
    }
}
