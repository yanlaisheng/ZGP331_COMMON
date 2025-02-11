#include "stm32f10x.h"
#include "malloc.h"   
//  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 开发板
//内存管理 代码     
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2011/7/5 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//没有更新信息
//   

//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
 mem_init, //内存初始化
 mem_perused,//内存使用率
 0,   //内存池
 0,   //内存管理状态表
 0,     //内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void memcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
 u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void memset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}    
//内存管理初始化  
void mem_init(void)  
{  
    memset(mallco_dev.membase, 0, sizeof(mallco_dev.membase));//内存池素有数据清零  
    mallco_dev.memrdy=1;//内存管理初始化OK  
}  
//获取内存使用率
//返回值:使用率(0~100)
u8 mem_perused(void)  
{  
    u16 used=0;  
    u32 i;  
    for(i=0;i<MEM_ALLOC_TABLE_SIZE;i++)  
    {  
        if(mallco_dev.memmap[i])used++; 
    }  
    return used*100/MEM_ALLOC_TABLE_SIZE;  
}  
//内存分配(内部调用)
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 mem_malloc(u32 size)  
{  
    signed long offset=0;  
    u16 nmemb; //需要的内存块数  
 u16 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy)mallco_dev.init();//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配

    nmemb=size/MEM_BLOCK_SIZE;   //获取需要分配的连续内存块数
    if(size%MEM_BLOCK_SIZE)nmemb++;  
    for(offset=MEM_ALLOC_TABLE_SIZE-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
  if(!mallco_dev.memmap[offset])cmemb++; //连续空内存块数增加
  else cmemb=0;       //连续内存块清零
  if(cmemb==nmemb)      //找到了连续nmemb个空内存块
  {
            for(i=0;i<nmemb;i++)      //标注内存块非空 
            {  
                mallco_dev.memmap[offset+i]=nmemb;  
            }  
            return (offset*MEM_BLOCK_SIZE);//返回偏移地址  
  }
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用) 
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 mem_free(u32 offset)  
{  
    int i;  
    if(!mallco_dev.memrdy)//未初始化,先执行初始化
 {
  mallco_dev.init();    
        return 1;//未初始化  
    }  
    if(offset<MAX_MEM_SIZE)//偏移在内存池内. 
    {  
        int index=offset/MEM_BLOCK_SIZE;//偏移所在内存块号码  
        int nmemb=mallco_dev.memmap[index];   //内存块数量
        for(i=0;i<nmemb;i++)     //内存块清零
        {  
            mallco_dev.memmap[index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//ptr:内存首地址 
void myfree(void *ptr)  
{  
 u32 offset;  
    if(ptr==NULL)return;//地址为0.  
    offset=(u32)ptr-(u32)&mallco_dev.membase;  
    mem_free(offset);//释放内存     
}  
//分配内存(外部调用)
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u32 size)  
{  
    u32 offset;  
    offset=mem_malloc(size);  
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)&mallco_dev.membase+offset);  
}  
//重新分配内存(外部调用)
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(void *ptr,u32 size)  
{  
    u32 offset;  
    offset=mem_malloc(size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  
        memcpy((void*)((u32)&mallco_dev.membase+offset),ptr,size);//拷贝旧内存内容到新内存   
        myfree(ptr);               //释放旧内存
        return (void*)((u32)&mallco_dev.membase+offset);          //返回新内存首地址
    }  
}

 

//最后测试代码如下:
//int main(void)
//{    
// u8 *ptr;
// u16 *ptr1;
// u32 *ptr2;
//  u32 *ptr3;


// u8 i;
//   Stm32_Clock_Init(9);//系统时钟设置
// delay_init(72);  //延时初始化
// uart_init(72,9600); //串口1初始化  
// LED_Init();
//  //LCD_Init();  

// ptr=(u8*)mymalloc(100);
// if(*ptr)i=0;
// i=mallco_dev.perused();//查看使用率
// ptr1=(u16*)mymalloc(2*100);
// i=mallco_dev.perused();//查看使用率
// ptr2=(u32*)mymalloc(4*100);
// i=mallco_dev.perused();//查看使用率

// myfree(ptr); 
// i=mallco_dev.perused();//查看使用率
// ptr3=(u32*)mymalloc(4*20);
// i=mallco_dev.perused();//查看使用率

// myfree(ptr1);
// i=mallco_dev.perused();//查看使用率

// ptr=(u8*)mymalloc(8*32);
// 
// myfree(ptr2);
// i=mallco_dev.perused();//查看使用率
// myfree(ptr3);
// i=mallco_dev.perused();//查看使用率

// if(i)i=0;

// usmart_dev.init();
// POINT_COLOR=RED;      
//   while(1) 
// {     
//  LED0=!LED0;      
//  delay_ms(500); 
// }               
//}

