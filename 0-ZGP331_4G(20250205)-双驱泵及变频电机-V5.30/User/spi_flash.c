/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : spi_flash.c
* Author             : MCD Application Team
* Version            : V2.0.3
* Date               : 09/22/2008
* Description        : This file provides a set of functions needed to manage the
*                      communication between SPI peripheral and SPI M25P64 FLASH.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "spi_flash.h"
#include "GlobalV_Extern.h"			// ȫ�ֱ�������
#include "GlobalConst.h"

/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize    0x100

/* Private define ------------------------------------------------------------*/
#define WRITE      0x02  /* Write to Memory instruction */			//дָ��
#define WRSR       0x01  /* Write Status Register instruction */		//д״̬�Ĵ���ָ�� 
#define WREN       0x06  /* Write enable instruction */					//дʹ��ָ��

#define READ       0x03  /* Read from Memory instruction */			//��ָ��
#define RDSR       0x05  /* Read Status Register instruction  */		//��״̬�Ĵ���ָ��
#define RDID       0x9F  /* Read identification */							//��ID��ʾ��
#define SE         0xD8  /* Sector Erase instruction */					//��������ָ��
#define BE         0xC7  /* Bulk Erase instruction */						//ȫ������ָ��

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */			//�ڱ��д��־

#define Dummy_Byte 0xA5		//α�ֽ�

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Init(void)
{
	//ZCL 2018.12.10

}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase			SPI FALSH��������
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* Send write enable instruction */			//1. дʹ��ָ��		
  SPI_FLASH_WriteEnable();			//WREN

  /* Sector Erase */ 
  /* Select the FLASH: Chip Select low */		//2. ʹ��Ƭѡ
  SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */				//3. ����ȫ������ָ��
  SPI_FLASH_SendByte(SE);
  /* Send SectorAddr high nibble address byte */			//4. ���͸�λ�ֽڵ�ַ
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */		//5. �����м�λ�ֽڵ�ַ
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */				//6. ���͵�λ�ֽڵ�ַ
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */					//7.��ֹFLASHƬѡ		
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */				//8. �ȴ�FLASHд����
  SPI_FLASH_WaitForWriteEnd();				//�ܳ��ڼǺţ�д����д����
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase  ȫ������
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */		//1. дʹ��ָ��
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */ 
  /* Select the FLASH: Chip Select low */		//2. ʹ��Ƭѡ
  SPI_FLASH_CS_LOW();
  /* Send Bulk Erase instruction  */				//3. ����ȫ������ָ��
  SPI_FLASH_SendByte(BE);
  /* Deselect the FLASH: Chip Select high */	//4. ��ֹƬѡ
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */				//5. �ȴ�FLASHд����
  SPI_FLASH_WaitForWriteEnd();				//�ܳ��ڼǺţ�д����д����
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite		ҳд
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* Enable the write access to the FLASH */	//0. ʹ��FLASHдʹ��	
  SPI_FLASH_WriteEnable();
  
  /* Select the FLASH: Chip Select low */			//1. ʹ��FLASHƬѡ
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */		//2. ����дָ��
  SPI_FLASH_SendByte(WRITE);
  /* Send WriteAddr high nibble address byte to write to */			//3.���͸��ֽڵ�ַ
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */		//4.�����м��ֽڵ�ַ
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);  
  /* Send WriteAddr low nibble address byte to write to */			//5.���͵��ֽڵ�ַ
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
  
  /* while there is data to be written on the FLASH */
  while(NumByteToWrite--) 
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);								//6. �����ֽ�
    /* Point on the next byte to be written */
    pBuffer++; 
  }
  
  /* Deselect the FLASH: Chip Select high */		//7.��ֹFLASHƬѡ
  SPI_FLASH_CS_HIGH();
  
  /* Wait the end of Flash writing */				//8. �ȴ�FLASHд����
  SPI_FLASH_WaitForWriteEnd();				//�ܳ��ڼǺţ�д����д����
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite  ����д���Ը���д�������д��Ҳ����
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.  ʹ��ҳд˳��
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
* ��AAAA��Ӧ������ȷ�ģ�I2C����������ط����񲻶ԡ�
*******************************************************************************/
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
 u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % SPI_FLASH_PageSize;			//���࣬��:5%4=1
  count = SPI_FLASH_PageSize - Addr;					//4-1=3;����ҳ���пռ���
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;		//26/4=6  ����ҳ
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;	//26%4=2	����һҳ���ֽ���
  
  if(Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */		//һ��������ҳ����ʼ��
  {
    if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */ 
    {
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);		//д����ҳ
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;  
      }    
     
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);			//д����һҳ��ʣ�ಿ��
    }
  }
  else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
  {
    if(NumOfPage== 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if(NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */		//�˴���I2C��һ��
      {
        temp = NumOfSingle - count;							//count��1.��ҳ�����ֽ���
      
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);		//2.д��ҳָ����ַ��ҳβ
        WriteAddr +=  count;
        pBuffer += count; 
        
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);		//3.������һҳдʣ�ಿ��
      }
      else
      {			
				//�˴���I2C��һ��������һ�� ������ AAAA
				//Ҫд���ֽ���С�ڱ�ҳʣ���ֽڣ�����ֱ��д			
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);								
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;						//count��1.��ҳ��Ҫд��ʣ����
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;			//2.��ҳ��
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;		//3.����Ҫд�Ĳ���һҳ��ʣ����
      
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);				//1.д��ҳʣ���ֽڣ��ֽ�����COUNT��
      WriteAddr +=  count;
      pBuffer += count;  
     
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);	//2.����д��ҳ
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
      
      if(NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);			//3.д����һҳ��ʣ�ಿ��
      }
    }
  }
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead		����� �ܼ�
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID  ��FLASH��ʾ��ID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */			//1. ʹ��FLASHƬѡ
  SPI_FLASH_CS_LOW();
  
  /* Send "RDID " instruction */			//2. ���Ͷ�IDָ��
  SPI_FLASH_SendByte(0x9F);

  /* Read a byte from the FLASH */					//3. ��ID���ֽ�
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */					//4. ��ID�м��ֽ�
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Read a byte from the FLASH */					//5. ��ID���ֽ�
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */	//6. ��ֹFLASHƬѡ
  SPI_FLASH_CS_HIGH();
																							//7. �����ֽںϳ�24λ��ID
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;		//���Կ���ID��һ��24λ����

  return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence	������˳��
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */			//1. ʹ��FLASHƬѡ
  SPI_FLASH_CS_LOW();
  
  /* Send "Read from Memory " instruction */		//2. ���Ͷ�ָ��
  SPI_FLASH_SendByte(READ);

	/* Send the 24-bit address of the address to read from */  		//3. ����24λ��ַ
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte		���ֽ�
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));		// �÷���α�ֽں�ķ���ֵ����
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte		SPI�����ֽ�
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_SendByte(u8 byte)
{

#ifdef GD32 
	//u8 retry=0; 								//ZCL 2021.5.4
  SPI_Cmd(SPI3, DISABLE);	
  SPI_Cmd(SPI3, ENABLE); 			//ZCL 2021.5.4
#else
	;
#endif
	
  /* Loop while DR register in not emplty */		//1. ��⣺DR�Ĵ����ǿգ����Է���
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI3 peripheral */		//2. ����
  SPI_I2S_SendData(SPI3, byte);

  /* Wait to receive a byte */				//3. �ȴ������ֽ����
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */			//4. ���ؽ��յ��ֽ�
  return SPI_I2S_ReceiveData(SPI3);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord		SPI���Ͱ��֣������һ����32λ��
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI3 peripheral */
  SPI_I2S_SendData(SPI3, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI3);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable		FLASHдʹ��ָ��
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */		//1. ʹ��FLASHƬѡ
  SPI_FLASH_CS_LOW();
  
  /* Send "Write Enable" instruction */			//2. ����WRENָ��
  SPI_FLASH_SendByte(WREN);
  
  /* Deselect the FLASH: Chip Select high */	//3. ��ֹFLASHƬѡ
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd		�ȴ�д����
* Description    : Polls the status of the Write In Progress (WIP) flag in the  
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;
  
  /* Select the FLASH: Chip Select low */		//1. ʹ��FLASHƬѡ
  SPI_FLASH_CS_LOW();
  
  /* Send "Read Status Register" instruction */		//2. ����RDSRָ��
  SPI_FLASH_SendByte(RDSR);
  
  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH 
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);		//3. ����α�ֽڣ���״̬�Ĵ���ֵ��FLASH_Status

  } while((FLASH_Status & WIP_Flag) == SET); /* Write in progress */		//FLASH_Status=0,˵��д��

  /* Deselect the FLASH: Chip Select high */		//4. ��ֹFLASHƬѡ
  SPI_FLASH_CS_HIGH();
}


// �ܳ������Ӻ��� 
void SPI_FLASH_CS_LOW(void)						//Ƭѡ��
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOA, GPIO_Pin_0);		//LORA			ZCL 2018.12.10	
}

void SPI_FLASH_CS_HIGH(void)					//Ƭѡ��
{
	GPIO_SetBits(GPIOB, GPIO_Pin_6);		//FM25L16		ZCL 2018.12.10
	GPIO_SetBits(GPIOA, GPIO_Pin_0);		//LORA			ZCL 2018.12.10
}

void SPI_LORA_CS_LOW(void)						//Ƭѡ��
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);	//LORA			ZCL 2018.12.10
	GPIO_SetBits(GPIOB, GPIO_Pin_6);			
}

//����Ϊ����RAM FM25L16 ����-------------------------------------------------------------------------
/*******************************************************************************
* Function Name  : SPI_FMRAM_BufferWrite  ���绺��д-��������д
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FMRAM_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
  /* Enable the write access to the FLASH */	//0. ʹ��FLASHдʹ��	
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */			//1. ʹ��FLASHƬѡ
  SPI_FLASH_CS_LOW();
  /* Send "Write to Memory " instruction */		//2. ����дָ��
  SPI_FLASH_SendByte(WRITE);
  /* Send WriteAddr medium nibble address byte to write to */		//3.�����м��ֽڵ�ַ(û�и��ֽ�)
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */			//4.���͵��ֽڵ�ַ
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)      // 5. ѭ��д
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);								//6. �����ֽ�
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */		//7.��ֹFLASHƬѡ
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */				//8. �ȴ�FLASHд����
  SPI_FLASH_WaitForWriteEnd();				//�ܳ��ڼǺţ�д����д����
}

/*******************************************************************************
* Function Name  : SPI_FMRAM_BufferRead     ���绺���-�Ƚϼ�
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FMRAM_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */      //���Ͷ�ָ��
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
