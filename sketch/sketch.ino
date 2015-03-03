/*
Copyright 2014 Kiyohito AOKI (sambar.fgfs@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 

*/

//構造体・グローバル定数&変数定義
	struct TIMER {						//各ステージの動作及び起動/解除に関する定数・変数をまとめた構造体
		const long LIMIT ;				//各ステージの最低時間・長押し時間の上限or下限
		const long STEP ;				//1回のMCOM解除時に、どれだけTIMEを加減するか
		long TIME  ;					//各ステージの動作時間or起動・解除での長押し時間
	}  ; 

	struct TIMER STAGE1_DEFAULT = {5, 2, 20} ;		//ステージ1のLIMIT, STEP, TIME
	struct TIMER STAGE2_DEFAULT = {2, 1, 10} ;		//ステージ2のLIMIT, STEP, TIME
	struct TIMER FUSE_DEFAULT = {1, 1, 5};		//MCOM起動のLIMIT, STEP, TIME
	struct TIMER DEFUSE_DEFAULT = {10, 1, 5};	//MCOM解除のLIMIT, STEP, TIME

	struct TIMER STAGE1 = STAGE1_DEFAULT;
	struct TIMER STAGE2 = STAGE2_DEFAULT;
	struct TIMER FUSE = FUSE_DEFAULT;
	struct TIMER DEFUSE = DEFUSE_DEFAULT;


	struct LED_CYCLE{
		const int NORM = 500;			//LED点滅(通常)
		const int FRICK = NORM/2;			//LED点滅(高速)

	}LED_CYCLE;


	struct PIN{
		const int PUSH_SW =  2;	 			//プッシュスイッチを接続するピン(起動・解除用。入力)
		const int KEY_SW1 = 8;				//キースイッチ1
		const int KEY_SW2 = 9;				//キースイッチ2
		const int DIP_SW = 10;				//モード切り替え用ディップスイッチ
		const int BUZZER = 7;			//ブザーを接続するピン(出力)
		const int LED =  6;				//LEDを接続するピン(出力)
	} PIN ;




void setup( ) {
	pinMode(PIN.PUSH_SW, INPUT);
	pinMode(PIN.KEY_SW1, INPUT);
	pinMode(PIN.KEY_SW2, INPUT);
	pinMode(PIN.DIP_SW, INPUT);

	pinMode(PIN.BUZZER, OUTPUT);
	pinMode(PIN.LED, OUTPUT);

	Serial.begin(9600) ;					//シリアル通信の準備を行う
	Serial.flush();
	  }



void reset(int &mcom_mode){
	Serial.flush();
	STAGE1.TIME = STAGE1_DEFAULT.TIME ;
	STAGE2.TIME = STAGE2_DEFAULT.TIME ;
	FUSE.TIME = FUSE_DEFAULT.TIME ;
	DEFUSE.TIME = DEFUSE_DEFAULT.TIME ;

	mcom_mode = 0;

	digitalWrite(PIN.LED,HIGH);
	delay (100);

	digitalWrite(PIN.BUZZER, LOW);		//ブザ停止
	digitalWrite(PIN.LED, LOW);

	return;

}

void mcom_defuse(int &obliteration_mode, int &mcom_mode) {
	int cnt_frick = 0  ; 					//点滅カウンタ
	int cnt_disable_button = 0 ;			//送信タイミング維持したままの長押し過剰対策用カウンタ
	mcom_mode = 0;

	check_obliteration_mode (obliteration_mode);


	//200msで2回点滅
	for (cnt_frick = 0; cnt_frick <= 2; cnt_frick++ ){
		digitalWrite(PIN.BUZZER, HIGH);	//解除音を鳴らす
		digitalWrite(PIN.LED,LOW);
		delay(100);
		digitalWrite(PIN.LED,HIGH);
		digitalWrite(PIN.BUZZER, LOW);
		delay(100);
	}

	digitalWrite(PIN.BUZZER, LOW);		//ブザ停止
	digitalWrite( PIN.LED, LOW );	//LED消灯

	if (FUSE.TIME >	FUSE.LIMIT){
		FUSE.TIME -= FUSE.STEP ;	//起動に必要な長押し時間をFUSE.STEP秒短くする
	}

	if (STAGE1.TIME > STAGE1.LIMIT){
		STAGE1.TIME -= STAGE1.STEP ;		//stage1の時間をSTAGE1.STEP秒短くする
	}

	if (STAGE2.TIME > STAGE2.LIMIT){
		STAGE2.TIME -= STAGE2.STEP;		//stage2の時間をSTAGE2.STEP秒短くする
	}

	if (DEFUSE.TIME < DEFUSE.LIMIT){
		DEFUSE.TIME += DEFUSE.STEP;		//解除に必要な長押し時間をDEFUSE.STEP秒長くする
	}

//過剰な長押し対策兼送出タイミング調整で1.5秒間はデータ送信「しか」しない
	for (cnt_disable_button =0; cnt_disable_button <=2; cnt_disable_button++) {
		send_data(obliteration_mode, mcom_mode, true , 0 , (FUSE.TIME *1000 ) );
		delay(500); 
	}
	send_data(obliteration_mode, mcom_mode, false , 0 , (FUSE.TIME *1000 ) );
	delay(500); 

	return ;
}

void stage1_blink1(){
		//LEDオン、ブザーオフ。シリアル送出サイクルを0.5秒に統一するため、点灯と点滅を分割。
		digitalWrite(PIN.BUZZER, LOW);	   
		digitalWrite(PIN.LED,HIGH);
		delay(LED_CYCLE.NORM);
}


void stage1_blink2(){
		//LEDオフ、ブザーオン
		digitalWrite(PIN.LED,LOW);
		digitalWrite(PIN.BUZZER, HIGH);
		delay(LED_CYCLE.NORM);
}


void stage2_blink(){
		//LEDオン、ブザーオフ
		digitalWrite(PIN.BUZZER, LOW);
		digitalWrite(PIN.LED,HIGH);
		delay(LED_CYCLE.FRICK);

		//LEDオフ、ブザーオン
		digitalWrite(PIN.BUZZER, HIGH);
		digitalWrite(PIN.LED,LOW);
		delay(LED_CYCLE.FRICK);
}


void send_data(int obliteration_mode, int &mcom_mode, boolean button_pushing, long left, long defuse){
//	引数
//		obliteration_mode	;オブリタレーションモードの状態
//		mcom_mode		;		//mcomの状態。0=待機中、 1=ステージ1(低速で点滅) , 2=ステージ2(早い点滅) , 3=破壊済み(無限ループになり解除不能になる)
//		button_pushing	;		//ボタン押下中か否か
//		left				;		//残り時間
//		defuse		;		//解除までの残り時間

	Serial.print (obliteration_mode);
	Serial.print (",");

	Serial.print (mcom_mode);
	Serial.print (",");

	Serial.print (button_pushing);
	Serial.print (",");

	Serial.print (left);
	Serial.print (",");

	Serial.print (defuse);
	Serial.print ("\n");

}


long mcom_stage1(int &obliteration_mode, int &mcom_mode , long boot_time ){	//長断続音モード。　boot_time : mcomを起動した時間。 &mcom_mode :mcomの状態(参照)
	//mcomカウントダウン
	long push_time ;								//ボタン押下開始した時間
	long release_time ;								//ボタンを離した時間
	long pushing_time = release_time - push_time ;		//release_time - push_time

	mcom_mode=1;

	while(millis() <=  boot_time + STAGE1.TIME * 1000){
		if (digitalRead(PIN.PUSH_SW) == LOW) {
			push_time = 0 ;
			release_time =0 ;
			pushing_time = release_time - push_time ;

			if (Serial.read() != -1 ){			//シリアル入力が空でなければリセット
				reset (mcom_mode);
				return 0;
			}
			send_data(obliteration_mode, mcom_mode,  false , ((STAGE1.TIME + STAGE2.TIME) * 1000 + boot_time - millis() ) , DEFUSE.TIME * 1000 );
			stage1_blink1();
			send_data(obliteration_mode, mcom_mode,  false , ((STAGE1.TIME + STAGE2.TIME) * 1000 + boot_time - millis() ) , DEFUSE.TIME * 1000 );
			stage1_blink2();


		}else {
			push_time = millis();

			while (digitalRead(PIN.PUSH_SW) == HIGH){

  				release_time = millis();
				pushing_time = release_time - push_time ; //押していた時間を計算
				send_data(obliteration_mode, mcom_mode,  true , ((STAGE1.TIME + STAGE2.TIME) * 1000 + boot_time - millis() ) , DEFUSE.TIME*1000 - pushing_time   );
				stage1_blink1();

				release_time = millis();
				pushing_time = release_time - push_time ; //押していた時間を再計算
				send_data(obliteration_mode, mcom_mode,  true , ((STAGE1.TIME + STAGE2.TIME) * 1000 + boot_time - millis() ) , DEFUSE.TIME * 1000 - pushing_time );
				stage1_blink2();

				if (release_time - push_time >= DEFUSE.TIME * 1000 ){
					mcom_defuse(obliteration_mode, mcom_mode );
					return 0 ;
				} 
				if (millis() >= boot_time + STAGE1.TIME * 1000 ){
					return(pushing_time);
				}
			}
		}
	}
	return 0 ;
}

void mcom_stage2(int &obliteration_mode, int &mcom_mode ,long boot_time , long pushing_time ){	//短断続音モード。 boot_time : mcomが短断続音モードになった時間  &mcom_mode :mcomの状態(参照)
	//mcomカウントダウン
	long push_time ;							//ボタン押下開始した時間
	long release_time  ;							//ボタンを離した時間

  	mcom_mode=2;


	while(millis() <=  boot_time + STAGE2.TIME * 1000 ){
		if (digitalRead(PIN.PUSH_SW) == LOW) {
			push_time = 0 ;
			release_time =0 ;
			pushing_time = 0 ;

			if (Serial.read() != -1 ){			//シリアル入力が空でなければリセット
				reset (mcom_mode);
				return;
			}
			send_data(obliteration_mode, mcom_mode,  false , (STAGE2.TIME * 1000 + boot_time - millis() ) , (DEFUSE.TIME * 1000) );
			stage2_blink();

		} else {
			push_time = millis();

			while (digitalRead(PIN.PUSH_SW) ==HIGH ){
				release_time = millis();

				send_data(obliteration_mode, mcom_mode,  true , (STAGE2.TIME * 1000 + boot_time - millis() ) , (DEFUSE.TIME*1000 - pushing_time -(release_time-push_time) )  );
				stage2_blink();

				if (release_time - push_time >= DEFUSE.TIME * 1000 - pushing_time ){
					mcom_defuse(obliteration_mode, mcom_mode );
					return ;
				}
				if (millis() >= (boot_time + STAGE2.TIME * 1000)) {
						return;
				}
			  }
		}
	}
	return ;
}

void mcom_stage3(int obliteration_mode, int &mcom_mode){
	int cnt = 0;

        mcom_mode =3;
	digitalWrite(PIN.LED, HIGH);
	digitalWrite(PIN.BUZZER, HIGH);

	for (cnt = 0; cnt <= 10; cnt++){
		send_data(obliteration_mode, mcom_mode, false , 0 , 0);
		delay(500);
		if (Serial.read() != -1 ){
			reset (mcom_mode);
			return;
		}
	}

	digitalWrite(PIN.BUZZER, LOW);		//ブザ停止
	digitalWrite(PIN.LED, HIGH);

	frozen:		//無限ループ用
		send_data(obliteration_mode, mcom_mode,  false , 0 , 0);	
		delay(500);

		if (Serial.read() != -1 ){
			reset (mcom_mode);
			return;
		}
	goto frozen;	
}


void check_obliteration_mode (int &obliteration_mode){
//オブリタレーションモードか否かの判定及びキースイッチの状態をチェック

	if (digitalRead(PIN.DIP_SW) == LOW){
		obliteration_mode = 0;
	}else if (digitalRead(PIN.KEY_SW1) == digitalRead(PIN.KEY_SW2) ){
		obliteration_mode = 1;
	}else if ( digitalRead(PIN.KEY_SW1) == HIGH && digitalRead(PIN.KEY_SW2) == LOW ){
			obliteration_mode = 2;
	}else if ( digitalRead(PIN.KEY_SW1) == LOW && digitalRead(PIN.KEY_SW2) == HIGH ) {
			obliteration_mode = 3;
	}
}

void loop( ) {
	
  
	long push_time	 ;		//ボタン押下開始した時間
	long release_time 	 ;		//ボタンを離した時間
	long pushing_time	 ;		//ボタンを押していた時間
	int mcom_mode ;			//0=待機中、 1=ステージ1(低速で点滅) , 2=ステージ2(早い点滅) , 3=破壊済み(無限ループになり解除不能になる)
	int obliteration_mode ; 		//0=ラッシュ、1=オブリタレーション(この場合ボタン操作を無視) 2=キー1がオン 3=キー2がオン

//MCOM初期化
        if (mcom_mode <= -1 || mcom_mode >= 4 ){
		push_time = 0;
		release_time = 0;
		pushing_time = 0;
		mcom_mode = 0;
		
        }

	check_obliteration_mode (obliteration_mode);


	send_data(obliteration_mode, mcom_mode, false , 0 , FUSE.TIME * 1000 );
	if (Serial.read() != -1 ){			//シリアル入力が空でなければリセット
		reset (mcom_mode);
	}

	delay (500);


	if (digitalRead(PIN.PUSH_SW) == HIGH ){

  		push_time = millis();
		while (digitalRead(PIN.PUSH_SW) ==HIGH && obliteration_mode != 1 ){
			release_time = millis();

			send_data( obliteration_mode,mcom_mode, true , 0 , (FUSE.TIME *1000 - (release_time -push_time) ) );

			delay (500);

			if (release_time - push_time >= FUSE.TIME * 1000 ){
				pushing_time = mcom_stage1(obliteration_mode, mcom_mode , millis() );
			} else if (digitalRead(PIN.PUSH_SW) == LOW) {
				push_time = 0;
				release_time = 0;
				pushing_time = 0;
			}

                        if (mcom_mode == 1){
                                break;
                        }
		}

		if(mcom_mode == 1){
			push_time = 0;
			release_time = 0;			
			mcom_stage2 (obliteration_mode,mcom_mode , millis() , pushing_time );
			pushing_time = 0;
		}

		if (mcom_mode == 2){
			mcom_stage3(obliteration_mode,mcom_mode);
		}
	} 
}
