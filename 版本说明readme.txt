2024-04-12 17:54:00
ZGP331 V5.24（双驱泵通用监控程序）更新说明：
1、串口1也可以下载配置文件了，功能与串口2相同；
2、串口1也可以查询数据了，功能与串口2相同；
3、注册网络命令由AT+CREG?改为AT+CEREG?（CREG为2G网络，电信已经把2G网络移除了，只能注册4G网络，所以统一用CEREG）
4、通讯错误的重启时间由1800秒改为80秒；
5、配合ZGP331(V5.3)电路板，增加4G模块断电功能，将所有的重启方式都改为4G模块断电重启；

2024-01-01 23:10:21
ZGP331 V5.22（双驱泵通用监控程序）改进
1、ZGP331硬件升级为V5.2，增加了控制4G模块电源上断电的功能和1路485口（与RS232共用串口2）
2、注册网络由AT+CREG?改为AT+CEREG?（CREG为2G网络，电信已经把2G网络移除了，只能注册4G网络，所以统一用CEREG）

1. CREG是判断CS域 也就是语音短信域;
2. 数据域用AT+CEREG或者AT+CGREG;
注：只要数据业务判断下PS数据域就好

用AT+CGATT?查询是否附着网络时，电信卡就返回+CGATT: 0，而联通卡返回+CGATT: 1
但是这2个卡都能收发数据
难道电信卡没有附着网络，也能收发数据吗？（这是SIM7600-L的一个bug，下一步升级为SIM7600-L1S）


2023-12-25 17:37:24
ZGP331 V5.21（双驱泵通用监控程序）改进
1、串口1也可以下载配置文件了，功能与串口2相同；
2、串口2也可以查询数据了，功能与串口2相同；


2023-10-25 22:19:45
通用ZGP331监控
1、放宽重启条件
原来是S_M35==0x1B时才重启，现在是只要GPRS使能，30分钟收不到数据就重启
	// if (S_M35 == 0x1B && T_Com3NoRcv != SClkSecond)
	if (F_GprsEn && T_Com3NoRcv != SClkSecond) // 放宽重启的条件，不管任何状态，只要30分钟收不到数据就重启
	{
		T_Com3NoRcv = SClkSecond;
		C_Com3NoRcv++;
		if (C_Com3NoRcv > 1800)
		{
			C_Com3NoRcv = 0;
			//
			S_M35 = 0x01;	  // 模块关机，重启
			LCD_DLY_ms(3000); // 此时间内没有喂狗，设备复位！
		}
	}



2023-10-13 23:06:09
通用ZGP331监控，
1、去掉了COM2收不到数，30分钟后重启的功能
2、解决一个BUG，原来TCP模式下，查询数据返回长度只取了低字节，导致TCP模式下，不能查询(255-21)=234字节，也就是不能查询超过117个字
3、更新时间：2023.10.13
4、hex最新版本：ZGP331_SCR_E_V517-20231013.hex

23:49 2023/3/10
ZGP331_4G(20230310)
1、增加域名解析
2、解决了printf串口问题，自定义了u1_printf函数（轮询发送）
3、解决了DTU参数查询的bug
4、解决了设置DTU参数不能保存的问题




8:01 2023/3/9
ZGP331_4G(20230309)
1、改进了透明传输，透明模式传输时GPRS可以返回数据；
2、加上了指示灯；


17:07 2023/3/7
ZGP331_4G(20230307)
1、初步测试通过，可以正常查询；



9:37 2023/2/27
ZGP331_4G(20230227)
1、在周工的程序基础上实现4G模块的连接和收发数据；
2、去掉了dowith中的COM2和COM3超过一定时间收不到数据就重启的程序；
3、串口3的波特率默认为115200；



还需要改进：
2、测试实现XMODEM传输；



			