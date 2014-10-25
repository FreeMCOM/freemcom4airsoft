#!/usr/bin/python
# -*- coding: utf-8 -*-

import time
import serial
from struct import *



class Mcom:
	def __init__(self):
		self.mcom_mode = ""
		self.button_pushing = ""
		self.left = ""
		self.disengage = "" 

		self.port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)

	def reset(self):
		self.port.close()				#ポートを一旦閉じる
		self.port = serial.Serial ("/dev/ttyACM0", 9600, timeout=10)	#再度開ける
		return

	def checkdata(self):
		indata = ''
		self.port.flushInput()			#使用済みのデータを破棄
		while len(indata) != 4:			#要素数不足の場合、要素数が合うまでやり直す
			indata = self.port.readline()
			indata = indata.strip("\n")	#末尾改行を削除
			indata = indata.split(",")		# inputdataをコンマ毎に切り分ける
		return indata

	def getdata(self):
		indata = self.checkdata()
		self.mcom_mode = int(indata[0])			# mcom_mode	;	mcomの状態。0=待機中、 1=ステージ1, 2=ステージ2 , 3=破壊済み
		self.button_pushing = int(indata[1])			# button_pushing	;	ボタン押下中か否か
		self.left = str( int(indata[2])/1000 )				# left	;	破壊までの残り時間
		self.disengage = str( int(indata[3])/1000 )			# disengage	;	起動・解除までの残り時間
		return

