#!/usr/bin/python
# -*- coding: utf-8 -*-

import time
import serial
from struct import *

#Arduinoのリセット待ち
time.sleep(2)

#シリアルポート、ボーレート、タイムアウトを指定
port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)

while port.readline() != "3,0,0,0\n":
#とりあえず、読んだデータを1行表示し、すぐに破棄(暫定。そのうちきちんと処理するコードが並ぶ)。
	inputdata = port.readline(),split	# inputdataをコンマ毎に切り分ける
		while inputdata == 4:
			inputdata.append("0")

	mcom_mode = inputdata[0]		# mcom_mode	;	mcomの状態。0=待機中、 1=ステージ1, 2=ステージ2 , 3=破壊済み
	button_pushing = inputdata[1]	# button_pushing	;	ボタン押下中か否か
	left = inputdata[2]				# left	;	破壊までの残り時間
	disengage = inputdata[3]			# disengage	;	起動・解除までの残り時間
	print disengage

#使用済みのデータを破棄
	port.flushInput()


	
else:
	print "M-COM was destruct."
	exit
