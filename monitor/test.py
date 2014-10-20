#!/usr/bin/python
# -*- coding: utf-8 -*-

import time
import serial
from struct import *

#Arduinoのリセット待ち
time.sleep(2)

#シリアルポート、ボーレート、タイムアウトを指定
port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)


def checkdata(indata):

	while len(indata) != 4:		#要素数不足の場合、要素数が合うまでやり直す
		# inputdataをコンマ毎に切り分ける
		indata = indata.strip("\n")	#末尾改行を削除
		indata = indata.split(",")

	return indata



while port.readline() != "3,0,0,0\n":
	indata = checkdata(port.readline() )

	mcom_mode = indata[0]		# mcom_mode	;	mcomの状態。0=待機中、 1=ステージ1, 2=ステージ2 , 3=破壊済み
	button_pushing = indata[1]	# button_pushing	;	ボタン押下中か否か
	left = indata[2]				# left	;	破壊までの残り時間
	disengage = indata[3]		# disengage	;	起動・解除までの残り時間

	print mcom_mode
	print disengage

#使用済みのデータを破棄
	port.flushInput()


	
else:
	print "M-COM was destruct."
	time.sleep(10)
	port.close()				#ポートを一旦閉じる

port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)		#再度シリアル接続を開始する
print port.readline()
exit
