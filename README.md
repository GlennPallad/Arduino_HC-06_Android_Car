
* [zh_CN](#zh_cn)
* [en_US](#en_us)


### zh_CN
##### APK 安装包位置
Compiled APK\app-debug.apk
##### 结构图和接线图
* 结构图<br/>
![结构图](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/frame.png)

* 接线图<br/>
![接线图](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/hookup.png)

##### 模块功能描述
* LM2596 直流降压电源板<br/>
将 9V 的直流电源降为 5V 的直流电供 Arduino 使用。

* L298N 电机驱动板模块<br/>
驱动输入电压：9V<br/>
用于控制电机，支持 PWM 调速。

* HC-06<br/>
用于和 Android 进行通信。

* HC-SR04<br/>
超声波测距模块，放在车头，用于防撞。<br/>
通过软件设置当和前方障碍物的距离小于 30cm 时，停车。

##### Android App 界面说明
请确保使用前蓝牙已经成功和 HC-06 模块配对。<br/>
左侧的 <br/>
`1` 按钮<br/>
`2` 按钮<br/>
`3` 按钮<br/>
用于控制速度。<br/>
![界面](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/APP%20UI.png)


### en_US
##### APK path
Compiled APK\app-debug.apk
##### Frame and hookup
* Frame<br/>
![Frame](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/frame.png)

* Hookup<br/>
![Hookup](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/hookup.png)

##### Modules function description
* LM2596 buck DC power convert<br/>
Convert 9V DC to 5V DC for Arduino power input.

* L298N motor drive module<br/>
Input voltage: 9V<br/>
For motor control, PWM supported.

* HC-06<br/>
For communicating with Android.

* HC-SR04<br/>
Supersonic distance measurement, placed in front of car, for anti-collision.<br/>
Configured to stop the car when distance between the car and obstacles less then 30cm via software.

##### Android App UI description
Make sure HC-06 is paired with your phone before using this app.<br/>
Buttons<br/>
`1`<br/>
`2`<br/>
`3`<br/>
on the left side are used for motor speed controlling.<br/>
![App UI](https://raw.githubusercontent.com/GlennPallad/Arduino_HC-06_Android_Car/master/README_pictures/APP%20UI.png)