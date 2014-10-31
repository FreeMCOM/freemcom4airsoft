#!/usr/bin/python
# -*- coding: utf-8 -*-

#Copyright 2014 Kiyohito AOKI (sambar.fgfs@gmail.com)
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#

import time
import serial
from struct import *



class Mcom:
	def __init__(self, port):
		self.mcom_mode = ""
		self.button_pushing = ""
		self.left = ""
		self.disengage = "" 

		time.sleep(2)							#Arduinoの起動待ち
		self.port = serial.Serial (port, 9600, timeout=10)

	def reset(self, port):
		self.port.close()						#ポートを一旦閉じる
		time.sleep(2)							#Arduinoの起動待ち
		self.port = serial.Serial (port, 9600, timeout=10)	#再度開ける
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
		self.button_pushing = int(indata[1])		# button_pushing	;	ボタン押下中か否か

		if  int(indata[2]) % 1000 >= 1 : 				# left	;	破壊までの残り時間
			self.left = str( (int(indata[2]) +1000) /1000)			#余りがあれば、表示は + 1000ミリ秒
		else:
			self.left  = str( int(indata[2]) /1000)


		if int(indata[3]) <= 1000 :					# disengage	;	起動・解除までの残り時間	
			self.disengage = str(1)					#1000ミリ秒以下なら1秒を表示
		elif  int(indata[3]) % 1000 >= 1 : 				
			self.disengage = str (( int(indata[3]) +1000) /1000 )		#余りがあれば、表示は + 1000ミリ秒
		else:
			self.disengage = str( int(indata[3]) /1000)
		return

