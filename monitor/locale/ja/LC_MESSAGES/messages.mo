Þ    !      $  /   ,      è     é  /   û     +     I  F   g      ®  ?   Ï            [   $                     ¸     Ä     Ñ     ã     ø       %   %     K     \     _  #   o           ´     º     Õ     Ý     í  Q   þ     P  @  a     ¢  <   ·  4   ô  ,   )  k   V  0   Â  d   ó     X	     _	     n	     ÷	  3   þ	     2
     J
  
   X
     c
     s
     
     
  ;   ´
      ð
       "        7     V     u               ¤     ¿  d   Ð     5                           
             !                                                                                                                	        %s  <port name>  - Correctly connect between Arduino and XBee ?
 - Designate correctly port ?
 - MCOM power are turned on? 
 - SERIAL SELECT switch (on wireless proto shield) is choose MICRO ? 

 Can not receive data from MCOM!! Communication error occurred. Please do following checklist :

 Config Connecting... Error occurred if when above are good, possibility PC cannot recive radio wave from MCOM. 
 Exit Input port name correctly. Linux - /dev/ttyACMx 

 MCOM CONFIG MCOM Monitor MCOM Monitor - %s MCOM was Destroyed!! MCOM was Disengaging... MCOM was Engaging... Normally, port name is like below. 

 Not found %s . 
 OK Port not found! Press %d sec. until MCOM disengage. Press %d sec. until MCOM engage. Reset Until destruction %d sec.  Usage : Use port %s . 
 Windows - COMx 
 You have to remove interrupt object, move MCOM to more near from PC, and etc... 
 default are %s . Project-Id-Version: PACKAGE VERSION
POT-Creation-Date: 2014-11-29 20:22+JST
PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE
Last-Translator: FULL NAME <EMAIL@ADDRESS>
Language-Team: LANGUAGE <LL@li.org>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: ENCODING
Generated-By: pygettext.py 1.5
  %s  <ãã¼ãå>  ã»Arduinoã¨XBeeã¯æ­£ããæ¥ç¶ããã¦ãã¾ããï¼
 ã»æ­£ãããã¼ããæå®ãã¦ãã¾ããï¼
 ã»MCOMã®é»æºã¯å¥ã£ã¦ãã¾ããï¼
 ã»ã¯ã¤ã¤ã¬ã¹ãã­ãã·ã¼ã«ãä¸ã®SERIAL SELECTã¹ã¤ããã¯MICROãé¸æãã¦ãããï¼

 MCOMããã®ãã¼ã¿ãåä¿¡ã§ãã¾ãã!! éä¿¡ã¨ã©ã¼ãçºçãã¾ãããä»¥ä¸ã®ãã§ãã¯ãªã¹ããå®æ½ãã¦ãã ãã :

 è¨­å®   æ¥ç¶å¾ã¡ ããããå¨ã¦æ­£å¸¸ã§ããã®ã¨ã©ã¼ãçºçããå ´åãPCãMCOMããã®é»æ³¢ãåä¿¡ã§ãã¦ããªãããã§ãã
 çµäº ãã¼ãåãæ­£ç¢ºã«å¥åãã¦ãã ããã Linux - /dev/ttyACMx 

 MCOMã®è¨­å® MCOMç£è¦ MCOMç£è¦ - %s   ç ´å£æ¸ã¿   MCOMè§£é¤ä½æ¥­ä¸­...   MCOMèµ·åä½æ¥­ä¸­... éå¸¸ããã¼ãåã¯ä»¥ä¸ã®ããã«ãªãã¾ãã

  %s ãè¦ã¤ããã¾ããã
 OK ãã¼ããè¦ã¤ããã¾ãã!   è§£é¤ã¾ã§ %d ç§é·æ¼ã   èµ·åã¾ã§ %d ç§é·æ¼ã ãªã»ãã   ç ´å£ã¾ã§ %d ç§ ä½¿ç¨æ³ :  %s ãä½¿ç¨ãã¾ãã
 Windows - COMx 
 ãã£ã¨PCã®è¿ãã«MCOMãç§»åãããé®è½ç©ãåãé¤ãç­ã®æªç½®ãå¿è¦ã§ãã
 ããã©ã«ãã¯ %s ã§ãã 