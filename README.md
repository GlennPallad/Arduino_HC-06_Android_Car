
[TOC]

### zh_CN
##### APK 安装包位置
BlueSerial\app\build\outputs\apk\debug\app-debug.apk
##### 结构图和接线图
* 结构图
![结构图](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/frame.png)

* 接线图
![接线图](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/hookup.png)

##### 模块功能描述
* LM2596 直流降压电源板
将 9V 的直流电源降为 5V 的直流电供 Arduino 使用。

* L298N 电机驱动板模块
驱动输入电压：9V
用于控制电机，支持 PWM 调速。

* HC-06
用于和 Android 进行通信。

* HC-SR04
超声波测距模块，放在车头，用于防撞。
通过软件设置当和前方障碍物的距离小于 30cm 时，停车。

##### Android App 界面说明
请确保使用前蓝牙已经成功和 HC-06 模块配对。
左侧的 
`1` 按钮
`2` 按钮
`3` 按钮
用于控制速度。
![界面](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/APP%20UI.png)


### en_US
##### APK path
BlueSerial\app\build\outputs\apk\debug\app-debug.apk
##### Frame and hookup
* Frame
![Frame](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/frame.png)

* Hookup
![Hookup](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/hookup.png)

##### Modules function description
* LM2596 buck DC power convert
Convert 9V DC to 5V DC for Arduino power input.

* L298N motor drive module
Input voltage: 9V
For motor control, PWM supported.

* HC-06
For communicate with Android.

* HC-SR04
Supersonic distance measurement, placed in head of car, for anti-collision.
Configured to stop the car when distance between the car and obstacles less then 30cm via software.

##### Android App UI description
Make sure HC-06 is paired with your phone before using this app.
Buttons
`1`
`2`
`3`
on the left side are used for motor speed control.
![App UI](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/APP%20UI.png)