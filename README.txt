This program is working 'Driving Module' consisted of 4 Motors.

Computer : Raspberry pi 3
Library : WiringPi		#      http://wiringpi.com/

You can see the debuging messages to know where program is now.
Just removre '__DEBUG__' when you don't need these.

#########################
Pin Connection
#########################

Motor1						           Motor3
   └───┐				    	  ┌───┘
	left_pwm[0]	   ─┐		   ┌─ 	right_pwm[3]
	left_dir   [1]	   ─┼ [RaspberryPi] ┼─	right_dir[4]
	left_gnd [14]	   ─┘		   └─	right_gnd[20]
   ┌───┘				    	  └───┐
Motor2						  	Motor4 

#########################
Program Flow
#########################

Main
└─SelfDrivingMode	:is not developed.

└─RemoteMode		:Joystick control mode
	└Waiting serial flag -> GetChar 
	    ^			└─> DoOrder : change pwm&dir
	    └───────────────┘ 

└─DevelopMode		:Develope mode
	 └Scanf -> Change duty
	     ^		│
	     └──────┘ 



