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

	struct TIMER STAGE1 = {5, 2, 20} ;		//ステージ1のLIMIT, STEP, TIME
	struct TIMER STAGE2 = {2, 1, 10} ;		//ステージ2のLIMIT, STEP, TIME
	struct TIMER ENGAGE = {1, 1, 5};		//MCOM起動のLIMIT, STEP, TIME
	struct TIMER DISENGAGE = {10, 1, 5};	//MCOM解除のLIMIT, STEP, TIME


	struct LED_CYCLE{
		const int NORM = 500;			//LED点滅(通常)
		const int FRICK = NORM/2;			//LED点滅(高速)

	}LED_CYCLE;


	struct PIN{
		const int SW =  2;	 			//プッシュスイッチを接続するピン(起動・解除用。入力)
		const int BUZZER = 7;			//ブザーを接続するピン(出力)
		const int LED =  6;				//LEDを接続するピン(出力)
	} PIN ;


void setup( ) {
	pinMode(PIN.SW, INPUT);
	pinMode(PIN.BUZZER, OUTPUT);
	pinMode(PIN.LED, OUTPUT);

	Serial.begin(9600) ;					//シリアル通信の準備を行う

	  }


void mcom_disengage( int &mcom_mode) {
	int cnt = 0  ; 					//カウンタ
	mcom_mode = 0;

	//200msで2回点滅
	for (cnt = 0; cnt <= 2; cnt++ ){
		digitalWrite(PIN.BUZZER, HIGH);	//解除音を鳴らす
		digitalWrite(PIN.LED,LOW);
		delay(100);
		digitalWrite(PIN.LED,HIGH);
		digitalWrite(PIN.BUZZER, LOW);
		delay(100);
		}


	digitalWrite(PIN.BUZZER, LOW);		//ブザ停止
	digitalWrite( PIN.LED, LOW );	//LED消灯

	delay(2000); //長押ししすぎ防止


	if (ENGAGE.TIME >	ENGAGE.LIMIT){
		ENGAGE.TIME -= ENGAGE.STEP ;	//起動に必要な長押し時間をENGAGE.STEP秒短くする
	}

	if (STAGE1.TIME > STAGE1.LIMIT){
		STAGE1.TIME -= STAGE1.STEP ;		//stage1の時間をSTAGE1.STEP秒短くする
	}

	if (STAGE2.TIME > STAGE2.LIMIT){
		STAGE2.TIME -= STAGE2.STEP;		//stage2の時間をSTAGE2.STEP秒短くする
	}

	if (DISENGAGE.TIME < DISENGAGE.LIMIT){
		DISENGAGE.TIME += DISENGAGE.STEP;		//解除に必要な長押し時間をDISENGAGE.STEP秒長くする
	}

	return ;
}

void stage1_blink(){
		//LEDオン、ブザーオフ
		digitalWrite(PIN.BUZZER, LOW);	   
		digitalWrite(PIN.LED,HIGH);
		delay(LED_CYCLE.NORM);

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


void send_data(int &mcom_mode, boolean button_pushing, long left, long disengage){
//	引数
//		mcom_mode		;		//mcomの状態。0=待機中、 1=ステージ1(低速で点滅) , 2=ステージ2(早い点滅) , 3=破壊済み(無限ループになり解除不能になる)
//		button_pushing	;		//ボタン押下中か否か			
//		left				;		//残り時間
//		disengage		;		//解除までの残り時間

	Serial.print (mcom_mode);
	Serial.print (",");

	Serial.print (button_pushing);
	Serial.print (",");

	Serial.print (left);
	Serial.print (",");

	Serial.print (disengage);
	Serial.print (",\n");

}


long mcom_stage1(int &mcom_mode , long boot_time ){	//長断続音モード。　boot_time : mcomを起動した時間。 &mcom_mode :mcomの状態(参照)
	//mcomカウントダウン
	long push_time ;								//ボタン押下開始した時間
	long release_time ;								//ボタンを離した時間
	long pushing_time = release_time - push_time ;		//release_time - push_time

	mcom_mode=1;

	while(millis() <=  boot_time + STAGE1.TIME * 1000){
		if (digitalRead(PIN.SW) == LOW) {
			push_time = 0 ;
			release_time =0 ;
			pushing_time = release_time - push_time ;

			send_data( mcom_mode, false , ((STAGE1.TIME + STAGE2.TIME) * 1000 + boot_time - millis() ) , DISENGAGE.TIME * 1000 - pushing_time );

		}else {
			push_time = millis();

			while (digitalRead(PIN.SW) == HIGH){
				release_time = millis();
				pushing_time = release_time - push_time ; //押していた時間を返す

				send_data( mcom_mode, true , ((STAGE1.TIME + STAGE2.TIME) * 1000 + boot_time - millis() ) , DISENGAGE.TIME*1000 - pushing_time   );

				stage1_blink();

				if (release_time - push_time >= DISENGAGE.TIME * 1000 ){
					mcom_disengage( mcom_mode );
					return 0 ;
				} 
				if (millis() >= boot_time + STAGE1.TIME * 1000 ){
					return(pushing_time);
				}
			}
		}
		stage1_blink();
	}
	return 0 ;
}

void mcom_stage2(int &mcom_mode ,long boot_time , long pushing_time ){	//短断続音モード。 boot_time : mcomが短断続音モードになった時間  &mcom_mode :mcomの状態(参照)
	//mcomカウントダウン
	long push_time ;							//ボタン押下開始した時間
	long release_time  ;							//ボタンを離した時間

  	mcom_mode=2;


	while(millis() <=  boot_time + STAGE2.TIME * 1000 ){
		if (digitalRead(PIN.SW) == LOW) {
			push_time = 0 ;
			release_time =0 ;
			pushing_time = 0 ;

			send_data( mcom_mode, false , (STAGE2.TIME * 1000 + boot_time - millis() ) , (DISENGAGE.TIME * 1000) );
			stage2_blink();		//データ送信サイクルをステージ1に合わせるためのウエイトを兼ねる

		} else {
			push_time = millis();

			while (digitalRead(PIN.SW) ==HIGH ){
				release_time = millis();

				send_data( mcom_mode, true , ((STAGE2.TIME) * 1000 + boot_time - millis() ) , (DISENGAGE.TIME*1000 - pushing_time -(release_time-push_time) )  );

				stage2_blink();
				stage2_blink();		//2つめの方はデータ送信サイクルをステージ1に合わせるためのウエイトを兼ねる

				if (release_time - push_time >= DISENGAGE.TIME * 1000 - pushing_time ){
					mcom_disengage( mcom_mode );
					return ;
				}
				if (millis() >= (boot_time + STAGE2.TIME * 1000)) {
						return;
				}
			  }
		}
		stage2_blink();
		stage2_blink();		//2つめの方はデータ送信サイクルをステージ1に合わせるためのウエイトを兼ねる
	}
	return ;
}

void mcom_stage3(int &mcom_mode){
                mcom_mode =3;
		digitalWrite(PIN.LED, HIGH);
		digitalWrite(PIN.BUZZER, HIGH);
		delay(5000);

		frozen:		//無限ループ用
			digitalWrite(PIN.BUZZER, LOW);		//ブザ停止
			digitalWrite(PIN.LED, HIGH);
//ココらへんに「シリアル入力があればリセット」を追加したい

		goto frozen;	
}


void loop( ) {

  
	long push_time	 ;		//ボタン押下開始した時間
	long release_time 	 ;		//ボタンを離した時間
	long pushing_time	 ;		//ボタンを押していた時間
	int mcom_mode ;			//0=待機中、 1=ステージ1(低速で点滅) , 2=ステージ2(早い点滅) , 3=破壊済み(無限ループになり解除不能になる)

        if (mcom_mode >= 4 ){
		push_time = 0;
		release_time = 0;
		pushing_time = 0;
		mcom_mode = 0;
		
        } else if (mcom_mode <= -1){
		mcom_mode = 0;
		push_time = 0;
		release_time = 0;
		pushing_time = 0;
	}


	send_data( mcom_mode, false , 0 , 0 );
	delay (500);

	if (digitalRead(PIN.SW) == HIGH){

  		push_time = millis();
		while (digitalRead(PIN.SW) ==HIGH ){
			release_time = millis();

			send_data( mcom_mode, true , 0 , (ENGAGE.TIME *1000 - (release_time -push_time) ) );

			delay (500);

			if (release_time - push_time >= ENGAGE.TIME * 1000 ){
				pushing_time = mcom_stage1( mcom_mode , millis() );
			} else if (digitalRead(PIN.SW) == LOW) {
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
			mcom_stage2 (mcom_mode , millis() , pushing_time );
			pushing_time = 0;
		}

		if (mcom_mode == 2){
			mcom_stage3(mcom_mode);
		}
	} 
}
