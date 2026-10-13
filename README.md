# PMSM_shiyan

永磁同步电机（PMSM）矢量控制实验工程：在 MATLAB/Simulink 中建模和生成 C 代码，再移植到 GD32F407 单片机，通过 USB 虚拟串口与 Simulink 上位机交互。

本仓库保存原始文件夹的完整快照，包括模型、数据字典、控制算法、硬件驱动、第三方库、编译缓存和调试日志。

## 目录

| 路径 | 内容 |
| --- | --- |
| `1_PMSM_PI/1_PMSM_PI/PMSM_PI.slx` | 电机、平均值逆变器和控制器闭环仿真模型 |
| `1_PMSM_PI/1_PMSM_PI/data_PMSM_PI.sldd` | 电机参数、采样周期和控制器参数 |
| `1_PMSM_PI/1_PMSM_PI/GD32__HOST.slx` | USB 虚拟串口上位机，发送指令并显示反馈 |
| `1_PMSM_PI/1_PMSM_PI/Algorithm_ert_rtw/` | Simulink 生成的控制算法 C 代码 |
| `2_CODE_PMSM_PI/2_CODE_PMSM_PI/Project/GD32F407.uvprojx` | Keil 工程入口 |
| `2_CODE_PMSM_PI/2_CODE_PMSM_PI/Algorithm/` | 移植到单片机的控制算法与保护状态机 |
| `2_CODE_PMSM_PI/2_CODE_PMSM_PI/Hardware/` | PWM、ADC、编码器、GPIO 和串口配置 |
| `2_CODE_PMSM_PI/2_CODE_PMSM_PI/Firmware/` | GD32 固件和 USB 驱动 |
| `2_CODE_PMSM_PI/2_CODE_PMSM_PI/User/` | 主程序、中断调度、USB 通信和显示任务 |
| `2_CODE_PMSM_PI/2_CODE_PMSM_PI/DSP/` | 随工程保存的 ARM DSP 库、示例和测试代码 |

## 控制流程

当前选用速度 PI 外环、d/q 电流 PI 内环和 SVPWM：

1. 编码器反馈转子角度和实际转速。
2. 速度 PI 根据转速误差计算 q 轴目标电流；d 轴目标电流设为 0。
3. Clarke/Park 变换将采样电流转换到 d/q 坐标系。
4. 电流 PI 计算 d/q 电压，经逆变换和 SVPWM 得到三相 PWM 比较值。
5. PWM 定时器触发 ADC，ADC 转换结束中断执行控制计算并更新 PWM。

工程还保留 SMC、LADRC、LQR 等实验分支。当前速度选择开关为 1，对应 PI；导出的 SMC 计算结果没有参与当前 PWM 输出。

`main.c` 初始化外设和算法，主循环执行显示任务；实时控制位于 `User/gd32f4xx_it.c` 的 `ADC_IRQHandler()`，USB 通信位于 `User/Scope.c`。保护状态机包含母线欠压和过压判断。

## 主要参数

| 参数 | 当前设置 |
| --- | --- |
| 仿真母线电压 | 24 V |
| 极对数 | 5 |
| 相电阻 | 0.07 Ω |
| d/q 轴电感 | 0.00017 H |
| 永磁磁链 | 0.0042 Wb |
| 转动惯量 | 0.000058 kg·m² |
| 控制采样周期 `Ts` | 100 μs |
| 电机仿真步长 `TsModel` | 25 μs |

## 打开与复现

### MATLAB/Simulink

1. 将 MATLAB 当前目录切换至 `1_PMSM_PI/1_PMSM_PI/`。
2. 打开 `PMSM_PI.slx`，确认关联的数据字典 `data_PMSM_PI.sldd` 可访问。
3. 检查电机和控制参数后进行仿真。模型配置使用 `ert.tlc` 和 C 语言生成代码。
4. 实机上位机使用 `GD32__HOST.slx`，需将串口配置由当前保存的 `COM3` 改为实际设备端口。

当前模型保存版本为 R2024a，读取检查环境为 R2024b。模型与工作流涉及 Simulink、Stateflow、Motor Control Blockset、PMSM 电机库、串口组件，以及用于生成代码的 Simulink Coder/Embedded Coder；`LISAN_LIANXU.m` 使用 Control System Toolbox。

### GD32/Keil

1. 打开 `2_CODE_PMSM_PI/2_CODE_PMSM_PI/Project/GD32F407.uvprojx`。
2. 工程配置目标为 GD32F407RE，设备包为 `GigaDevice.GD32F4xx_DFP.3.4.0`，编译器为 ARM Compiler 5.06 update 7。
3. 校核 PWM、采样、编码器引脚，以及电流换算系数和编码器零位标定。
4. 处理下面的已知复现问题后，再编译和下载。

## 快照中的已知问题

- Keil 工程仍通过旧机器的绝对路径引用 `Algorithm_data.c`，该路径不在本仓库中。
- 本地保留的同名 `Algorithm_data.c` 使用 `ConstB_Algorithm_T`，但当前头文件未定义该类型；需要核对生成代码版本，不能仅替换路径就认定问题解决。
- 编码器 Z 相偏置 `845`、采样换算系数及硬件引脚配置依赖具体实验装置。
- 本快照进行了模型、参数和源码读取检查，未完成完整仿真、Keil 编译或实机验证。

## 完整下载

可使用 GitHub 的 **Code → Download ZIP** 下载仓库文件，或在 **Releases** 下载上传时生成的完整工程压缩包。

第三方库及原有文件中的版权和许可说明保留在对应文件内。
