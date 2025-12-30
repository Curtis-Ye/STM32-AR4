#include "bsp_CAN.h"
#include "string.h"

uint8_t CAN1_Config(void)
{
	uint8_t CAN1_Status;
	
    // 1-声明配置CAN所需的结构体
    GPIO_InitTypeDef GPIO_InitStructure;                          // GPIO配置结构体
    CAN_InitTypeDef  CAN_InitStructure;                           // CAN配置结构体
    CAN_FilterInitTypeDef CAN_FilterInitTypeStruct;               // 筛选器配置结构体
    // 2-使能时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);          // 使能CAN1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);         // 使能GPIOA时钟
    // 3-初始化CAN所需的GPIO:CAN_TX_PA12
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 ;                 // CAN_TX_PA12
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;              // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             // 引脚速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);                        // 初始化引脚
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 ;                 // CAN_RX_PA11
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                // 浮空输入或上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;             // 引脚速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);                        // 初始化引脚
    // 4-初始化CAN外设
    CAN_InitStructure.CAN_ABOM = ENABLE;                          // 退出离线管理; DISABLE-由软件控制; ENABLE-硬件自动控制
    CAN_InitStructure.CAN_AWUM = ENABLE;                          // 自动唤醒管理; DISABLE-由软件唤醒; ENABLE-硬件自动唤醒
    CAN_InitStructure.CAN_NART = ENABLE;                          // 禁止自动重传; DISABLE-发送报文失败时一直自动重传直到发送成功; ENABLE-报文只被发送1次，不管发送的结果如何
    CAN_InitStructure.CAN_RFLM = DISABLE;                         // 接收FIFO锁定; DISABLE-当接收溢出时，FIFO的报文未被读出，新到的报文将覆盖旧报文; ENABLE-新报文不能覆盖旧报文，直接舍弃
    CAN_InitStructure.CAN_TTCM = DISABLE;                         // 时间触发模式：DISABLE-禁止时间触发通信模式; ENABLE-允许时间触发通信模式
    CAN_InitStructure.CAN_TXFP = ENABLE;                          // 发送的优先级; DISABLE-发送优先级由报文的标识符来决定; ENABLE-发送优先级按报文存入邮箱的先后顺序发送
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal ;                // 工作模式，　Normal=正常, LoopBack=回环, Silent=静默, Silent_LoopBack=静默回环
    // 波特率、位时间配置_500K
    CAN_InitStructure.CAN_Prescaler = 4;                          // 分频系数, 直接填写要的分频值，函数会自动减1
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;                      // 时间段1
    CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;                     // 时间段2
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;                      // 时间段3
    CAN1_Status = CAN_Init(CAN1, &CAN_InitStructure);                           // 初始化CAN控制器，即把上述参数写入寄存器中
    // 5-配置过滤器，规则：接收所有数据帧
    CAN_FilterInitTypeStruct.CAN_FilterNumber = 1;                                                                       // 指定使用哪个过滤器; F103系列可选:0~13, F105和F107系列可选:0~27
    CAN_FilterInitTypeStruct.CAN_FilterMode   = CAN_FilterMode_IdMask ;                                                  // 过滤模式；IDMask=0=屏蔽位模式；IdList=1=列表模式
    CAN_FilterInitTypeStruct.CAN_FilterScale  = CAN_FilterScale_32bit ;                                                  // 过滤器位宽
    CAN_FilterInitTypeStruct.CAN_FilterIdHigh = (((uint32_t)0x000 << 3) & 0xFFFF0000) >> 16;                             // 掩码模式时，验证码的高16位值
    CAN_FilterInitTypeStruct.CAN_FilterIdLow  = (((uint32_t)0x000 << 3 | CAN_Id_Standard | CAN_RTR_Data) & 0xFFFF);      // 掩码模式时，验证码的低16位值
    CAN_FilterInitTypeStruct.CAN_FilterMaskIdHigh = ((uint32_t)0x000 << 3) & 0xFFFF0000 >> 16;                           // 过滤器屏蔽标识符的高16位; 重点建议，用二进制来看待此值，否则很难理解; 当设置相关位为0时，表示不用管总线上的帧ID这个位是啥值，都通过; 当位为1时，帧ID的这个位，必须与验证码此位的值相同，才能通过
    CAN_FilterInitTypeStruct.CAN_FilterMaskIdLow  = (((uint32_t)0x000 << 3 | CAN_Id_Standard | CAN_RTR_Data) & 0xFFFF);  // 过滤器屏蔽标识符的低16位; 接上，如果高低位寄存器，都设置为0x0000，表示，不用与验证码对比，所有帧ID都通过，接收所有报文数据; 如果都设置为0xFFFF, 表示帧ID值，每一位都与验证码相同，才接收报文;
    CAN_FilterInitTypeStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;                                                // FIFO关联；上面这个过滤器，接收到的数据，存放到FIFO0; CAN共有两个接收FIFO，每个FIFO包含3个接收邮箱，都是完全由硬件管理; 接收到的报文，会自动存放到邮箱中,读取时，自动读出最先收到的报文,不用管是哪个邮箱的)
    CAN_FilterInitTypeStruct.CAN_FilterActivation = ENABLE;                                                              // 使能过滤器
    CAN_FilterInit(&CAN_FilterInitTypeStruct);
	
	return CAN1_Status;
}
    
uint8_t myCANSendData(uint8_t *cmd, uint8_t len)
{
    CanTxMsg xCAN_TX;
    
	uint8_t i=0,j=0,k=0,l=0,packNum = 0;
	j=len-2;
	while(i<j)
	{
		xCAN_TX.IDE    = CAN_Id_Extended;                                           // 帧格式
		xCAN_TX.StdId  = 0x00;                                                      // 标识符
		xCAN_TX.ExtId  = ((uint32_t)cmd[0]) << 8 | (uint32_t)packNum;
		xCAN_TX.RTR    = CAN_RTR_Data;                                              // 帧类型
		xCAN_TX.Data[0] = cmd[1];
		k=j-i;
		if(k<8)
		{
			for(l=0;l<k;i++,l++)
			{
				xCAN_TX.Data[l+1]=cmd[i+2];
			}
			xCAN_TX.DLC=k+1;
		}
		else
		{
			for(l=0;l<7;l++,i++)
			{
				xCAN_TX.Data[l+1]=cmd[i+2];
			}
			xCAN_TX.DLC=8;
		}
		CAN_Transmit(CAN1,&xCAN_TX);
		++packNum;
	}	
}

uint8_t myCANReceiveFlag(void)
{
	if(CAN_MessagePending(CAN1,CAN_FilterFIFO0)>0)
	{
		return 1;
	}
	return 0;
}

void myCANReceiveData(uint32_t *ID, uint8_t *len, uint8_t* Data)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN1,CAN_FilterFIFO0,&RxMessage);
	*len = RxMessage.DLC;
	if(RxMessage.IDE==CAN_Id_Standard)
	{
	  *ID = RxMessage.StdId;
	}
	else
	{
	  *ID = RxMessage.ExtId;
	}
	if(RxMessage.RTR==CAN_RTR_Data)
	{
		for(uint8_t i=0;i<*len;i++)
		{
			Data[i]=RxMessage.Data[i];
		}
	}	
}

