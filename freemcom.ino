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

//グローバル定数&変数定義
	const long MIN_ENGAGE_TIME = 1 ;			//タイマー起動のSW長押し時間下限(秒)
	const long MIN_STAGE1_TIME= 5 ;			//タイマー起動後ブザー断続(低速)で警告音鳴らす時間下限(秒)
	const long MIN_STAGE2_TIME = 2 ;			//ブザー断続(早く)で鳴らす時間下限(秒)
	const long MAX_DISENGAGE_TIME = 30 ;		//タイマー解除のSW長押し時間上限(秒)


	long ENGAGE_TIME = 5 ;			//タイマー起動のSW長押し時間(秒)
	long STAGE1_TIME = 20 ;			//タイマー起動後ブザー断続(低速)で警告音鳴らす時間(秒)
	long STAGE2_TIME = 10 ;			//ブザー断続(早く)で鳴らす時間(秒)
	long DISENGAGE_TIME = 5 ;		//タイマー解除のSW長押し時間(秒)


	const int LED_CYCLE_FRICK = 250;			//LED点滅(高速)
	const int LED_CYCLE_NORM = 500;			//LED点滅(通常)

	const int SW_PIN =  2;	 			//プッシュスイッチを接続するピン(起動・解除用。入力)
	const int BUZZER_PIN = 7;			//ブザーを接続するピン(出力)
	const int LED_PIN =  6;			//LEDを接続するピン(出力)

	int mcom_mode = 0 ;	 //mcomの作動状態。 0で待機中、1でブザー断続中、2でブザー連続、3で破壊済み



void setup( ) {
	pinMode(SW_PIN, INPUT);
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);

	  }


void mcom_disengage() {
	int cnt = 0  ; 					//カウンタ

	//200msで2回点滅
	for (cnt = 0; cnt <= 2; cnt++ ){
		digitalWrite(BUZZER_PIN, HIGH);	//解除音を鳴らす
		digitalWrite(LED_PIN,LOW);
		delay(100);
		digitalWrite(LED_PIN,HIGH);
		digitalWrite(BUZZER_PIN, LOW);
		delay(100);
		}

	mcom_mode = 0;
	digitalWrite(BUZZER_PIN, LOW);		//ブザ停止
	digitalWrite( LED_PIN, LOW );	//LED消灯
	delay(2000); //長押ししすぎ防止

	if (ENGAGE_TIME >	MIN_ENGAGE_TIME){
		--ENGAGE_TIME ;	//起動に必要な長押し時間を1秒短くする
	}

	if (STAGE1_TIME > MIN_STAGE1_TIME){
		STAGE1_TIME -= 5 ;	//stage1の時間を5秒短くする
	}

	if (STAGE2_TIME > MIN_STAGE2_TIME){
		STAGE2_TIME -= 2;		//stage2の時間を2秒短くする
	}

	if (DISENGAGE_TIME < MAX_DISENGAGE_TIME){
		DISENGAGE_TIME += 5 ;		//解除に必要な長押し時間を5秒長くする
	}


	return ;

}

void stage1_blink(){
		//LEDオン、ブザーオフ
		digitalWrite(BUZZER_PIN, LOW);	   
		digitalWrite(LED_PIN,HIGH);
		delay(LED_CYCLE_NORM);

		//LEDオフ、ブザーオン
		digitalWrite(LED_PIN,LOW);
		digitalWrite(BUZZER_PIN, HIGH);
		delay(LED_CYCLE_NORM);
}


void stage2_blink(){
		//LEDオン、ブザーオフ
		digitalWrite(BUZZER_PIN, LOW);
		digitalWrite(LED_PIN,HIGH);
		delay(LED_CYCLE_FRICK);

		//LEDオフ、ブザーオン
		digitalWrite(BUZZER_PIN, HIGH);
		digitalWrite(LED_PIN,LOW);
		delay(LED_CYCLE_FRICK);
}



long mcom_stage1(long boot_time){	//長断続音モード。　boot_time : mcomを起動した時間。
	//mcomカウントダウン
	long push_time;		//ボタン押下開始した時間
	long release_time;		//ボタンを離した時間
	long pushing_time;		//release-time - push_time


	mcom_mode=1;

	while(millis() <=  boot_time + STAGE1_TIME * 1000){
		stage1_blink();

		if (digitalRead(SW_PIN) == HIGH){
			push_time = millis();

			while (digitalRead(SW_PIN) == HIGH){
				release_time = millis();
				stage1_blink();

				if (release_time - push_time >= DISENGAGE_TIME * 1000 ){
					mcom_disengage();
					return 0 ;
				} 
				if (millis() >= boot_time + STAGE1_TIME * 1000 ){
					pushing_time = release_time - push_time ; //押していた時間を返す
					return(pushing_time);
				}
			}
		}
	}
	return 0 ;
}

void mcom_stage2(long boot_time , long pushing_time){	//短断続音モード。 boot_time : mcomが短断続音モードになった時間
	//mcomカウントダウン
	long push_time;		//ボタン押下開始した時間
	long release_time;		//ボタンを離した時間

  	mcom_mode=2;

	while(millis() <=  boot_time + STAGE2_TIME * 1000 ){
		stage2_blink();

		if (digitalRead(SW_PIN) == HIGH){
			push_time = millis();

			while (digitalRead(SW_PIN) ==HIGH ){
				release_time = millis();
				stage2_blink();

				if (release_time - push_time >= DISENGAGE_TIME * 1000 - pushing_time ){
					mcom_disengage();
					return ;
				}
				if (millis() >= (boot_time + STAGE2_TIME * 1000)) {
						return;
				}
			  }
		}
	}
	return ;
}

void mcom_stage3(){
                mcom_mode =3;
		digitalWrite(LED_PIN, HIGH);
		digitalWrite(BUZZER_PIN, HIGH);
		delay(5000);

		frozen:		//無限ループ用
			digitalWrite(BUZZER_PIN, LOW);		//ブザ停止
			digitalWrite(LED_PIN, HIGH);
		goto frozen;	
}


void loop( ) {

	long push_time = 0 ;		//ボタン押下開始した時間
	long release_time = 0 ;		//ボタンを離した時間

	long pushing_time ;				//ボタンを押していた時間

	if (digitalRead(SW_PIN) == HIGH){
		push_time = millis();
		while (digitalRead(SW_PIN) ==HIGH ){
			release_time = millis();
			if (release_time - push_time >= ENGAGE_TIME * 1000 ){
				pushing_time = mcom_stage1( millis() );
			} else if (digitalRead(SW_PIN) == LOW) {
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
			mcom_stage2 (millis() , pushing_time );
			pushing_time = 0;
		}

		if (mcom_mode == 2){
			mcom_stage3();
		}
	} 
}
