MDK531.exe为单片机软件开发环境，安装完成之后使用Keil MDK Crack文件夹的工具进行破解
然后安装GigaDevice.GD32F4xx_DFP.3.0.0，这是GD32单片机的开发包
然后安装Keil.ARM_Compiler.1.6.3
最后安装ARM.CMSIS.5.8.0，这里集成了DSP/MCU 数学计算库
安装完Keil开发环境后，在project文件夹中，打开GD32F407.uvprojx文件即可打开整个软件工程。

Algorithm文件夹内的文件就是Simulink生成的控制算法C代码，被直接移植进工程
DSP文件夹内是MCU/DSP控制器使用到的数学计算库等文件
Firemware是GD32单片机的固件库，用于调动MCU的外部设备、计算资源
Hadware文件夹是电机控制使用到了的相关外设资源，其中GD32F407RET6引脚使用PDF写明了使用到了哪些功能和对应的引脚
Project文件夹是代码工程文件所在文件夹，点击GD32F407.uvprojx文件即可打开整个软件工程
User文件夹有三个C文件：
主函数main.c ：执行外设初始化和OLED显示屏初始化
中断函数gd32f4xx_it.c ：接受Simulink上位机信息，上传实时运行代码变量数值，执行控制算法
精准延时函数systick.c： 几乎用不到这个C文件。