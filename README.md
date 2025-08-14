# NodeMCU-WI-FI-CAR
# Code for engines and wifi mode

# The analog function is used for the PWM differential speed control of the motor. 
# analogWrite(pin,value)

pin: Specify the pin from which the PWM signal is to be output. Not all pins support PWM output. On the Arduino Uno, the pins that support PWM are 3, 5, 6, 9, 10, and 11. On the Arduino Mega, the pins that support PWM are 2 - 13 and 44 - 46.

value: Specify the duty cycle of the PWM signal, with a value range of 0 - 255. Among them, 0 indicates a duty cycle of 0% (equivalent to outputting a low level), and 255 indicates a duty cycle of 100% (equivalent to outputting a high level). 

---

pin：指定要输出 PWM 信号的引脚。不是所有的引脚都支持 PWM 输出

value：指定 PWM 信号的占空比，取值范围是 0 - 255。其中，0 表示占空比为 0%（相当于输出低电平），255 表示占空比为 100%（相当于输出高电平）。

---


