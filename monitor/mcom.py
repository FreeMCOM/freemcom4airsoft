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

import sys
import os.path
import string
import gettext
import locale



class Mcom:
    def __init__(self, port):
	self.obliteration_mode = 0	#int型。オブリタレーションモードの状態
        self.mcom_mode = 0		#int型。mcomの状態
        self.button_pushing = 0		#int型。ボタン押下有無。押していれば1、押していなければ0
        self.left = 0			#int型。mcom破壊までの残り時間(ミリ秒)
        self.defuse = 0 		#int型。mcom起動/解除までの残り時間(ミリ秒)

        time.sleep(2)							#Arduinoの起動待ち
        self.port = serial.Serial (port, 9600, timeout=5)


    def reset(self, port):
        self.port.write ("1")
        self.port.flushOutput()
        self.port.close()
        self.port = serial.Serial (port, 9600,timeout = 5)
        return


    def checkdata(self):
        indata = ''
        self.port.flushInput()			#使用済みのデータを破棄
        while len(indata) != 5:			#要素数不足の場合、要素数が合うまでやり直す
            indata = self.port.readline()
            if indata == '' :
                return -1			#エラーなので-1を返す

            indata = indata.strip("\n")		#末尾改行を削除
            indata = indata.split(",")		# inputdataをコンマ毎に切り分ける
        return indata


    def getdata(self):
        indata = self.checkdata()
        if indata == -1 :
            return -1

	self.obliteration_mode = int(indata[0])			# obliteration_mode 	;オブリタレーションモードの状態。 0=ラッシュ, 1=エラー(両方のキーSWが入 or 両方が切), 2=キーSW1が入でSW2が切, 3=SW1が切でSW2が入
        self.mcom_mode = int(indata[1])				# mcom_mode		;	mcomの状態。0=待機中、 1=ステージ1, 2=ステージ2 , 3=破壊済み
        self.button_pushing = int(indata[2])			# button_pushing	;	ボタン押下中か否か

        if  int(indata[3]) % 1000 >= 1 : 			# left			;	破壊までの残り時間
            self.left = ( int(indata[3]) +1000) /1000		#余りがあれば、表示は + 1000ミリ秒
        else:
            self.left  = int(indata[3]) /1000

        if int(indata[4]) <= 1000 :				# defuse		;	起動・解除までの残り時間
            self.defuse = 1					#1000ミリ秒以下なら1秒を表示
        elif  int(indata[4]) % 1000 >= 1 : 				
            self.defuse = ( int(indata[4]) +1000) /1000	#余りがあれば、表示は + 1000ミリ秒
        else:
            self.defuse = int(indata[4]) /1000
        return

        if int(indata[4]) <= 1000 :				# defuse		;	起動・解除までの残り時間
            self.defuse = 1					#1000ミリ秒以下なら1秒を表示
        elif  int(indata[4]) % 1000 >= 1 : 				
            self.defuse = ( int(indata[4]) +1000) /1000	#余りがあれば、表示は + 1000ミリ秒
        else:
            self.defuse = int(indata[4]) /1000
        return


if __name__ == "__main__":
    lang = [""] 
    lang.insert(0, locale.getdefaultlocale()[0] )

    if sys.platform == "win32" :
        localedir = os.path.abspath(os.path.dirname(__file__))+'\\locale'
        _ = gettext.translation("messages", localedir, lang, fallback=True  ).ugettext

    else :
        localedir = os.path.abspath(os.path.dirname(__file__))+'/locale'
        _ = gettext.translation("messages", localedir , lang, fallback=True  ).ugettext

    print _(u"This file is Define a class. aborting.")
    exit()

