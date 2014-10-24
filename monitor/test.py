#!/usr/bin/python
# -*- coding: utf-8 -*-

import time
import serial
from struct import *


time.sleep(2)		#Arduinoの起動待ち
port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)	#シリアルポート、ボーレート、タイムアウトを指定



class Mcom:
	def __init__(self):
		self.mcom_mode = ""
		self.button_pushing = ""
		self.left = ""
		self.disengage = "" 

	def reset(self):
		port.close()				#ポートを一旦閉じる
		port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)	#再度開ける
		return

	def checkdata(self):
		indata = ''
		port.flushInput()			#使用済みのデータを破棄
		while len(indata) != 4:			#要素数不足の場合、要素数が合うまでやり直す
			indata = port.readline()
			indata = indata.strip("\n")	#末尾改行を削除
			indata = indata.split(",")		# inputdataをコンマ毎に切り分ける
		return indata

	def getdata(self):
		indata = self.checkdata()
		self.mcom_mode = int(indata[0])		# mcom_mode	;	mcomの状態。0=待機中、 1=ステージ1, 2=ステージ2 , 3=破壊済み
		self.button_pushing = int(indata[1])		# button_pushing	;	ボタン押下中か否か
		self.left = int(indata[2])/1000				# left	;	破壊までの残り時間
		self.disengage = int(indata[3])/1000			# disengage	;	起動・解除までの残り時間
		return



g = Mcom()

while port.readline() != "3,0,0,0":
	g.getdata()

	print g.mcom_mode
	print g.button_pushing
	print g.left
	print g.disengage
