# 视频&模拟量监控主机

***东南大学自动化学院2020年嵌入式系统大设计***

## 组员

*08017101 李俊廷*

*08017202 王天瑶*

## 完成情况（TODO List）

1. 数据采集模块
   - [ ] 基本要求：每1秒采集1次数据：AD读取阻值、摄像头获取图片（图片）
   - [ ] 拔高要求: 采集时间动态可设置；
   - [ ] 进阶要求：动态监测，监测到有移动物体，则上传图片
2. 数据通讯模块
    - [ ] 基本功能：能够通过以太网、串口，将数据传输到上位机
    - [ ] 拔高功能：实现Modbus 协议
    - [ ] 拔高功能：实现MQTT 协议
3. 本地监控界面
    - [ ] 基本要求：能够在触摸屏上显示阻值、图片及报警信息
    - [ ] 拔高要求：历史数据，能显示阻值历史曲线；图片列表
    - [ ] 进阶要求：UI设计美观，登录界面，等其他附加功能
4. 上位机监控界面（使用WPF）
    - [ ] 基本要求：能够在上位机上实时显示阻值、图片及报警信息
    - [ ] 拔高要求：历史数据，能显示阻值历史曲线；图片列表
    - [ ] 进阶要求：UI设计美观，登录界面，等其他附加功能