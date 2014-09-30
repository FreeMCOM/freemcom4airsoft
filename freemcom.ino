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
	const unsigned long ENGAGE_TIME = 5000 ;			//タイマー起動のSW長押し時間(ミリ秒)
	const unsigned long STAGE1_TIME = 20000 ;			//タイマー起動後ブザー断続(低速)で警告音鳴らす時間(ミリ秒)
	const unsigned long STAGE2_TIME = 10000 ;			//ブザー断続(早く)で鳴らす時間(ミリ秒)
	const unsigned long DISENGAGE_TIME = 5000 ;		//タイマー解除のSW長押し時間(ミリ秒)


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



void mcom_stage1(unsigned long boot_time){	//長断続音モード。　boot_time : mcomを起動した時間
	//mcomカウントダウン
	unsigned long push_time;		//ボタン押下開始した時間
	unsigned long release_time;		//ボタンを離した時間
	mcom_mode=1;

	while(millis() <  boot_time + STAGE1_TIME){
		stage1_blink();

		if (digitalRead(SW_PIN) == HIGH){
			push_time = millis();

			while (digitalRead(SW_PIN) == HIGH){
				release_time = millis();
				stage1_blink();

				if (release_time - push_time >= DISENGAGE_TIME ){
					mcom_disengage();
					return ;
				} 
//				if (millis() <= (boot_time + STAGE1_TIME) ){
//						goto rtb1;
//				}
			}
		}
	}
rtb1:
	return ;
}

void mcom_stage2(unsigned long boot_time){	//短断続音モード。 boot_time : mcomが短断続音モードになった時間
	//mcomカウントダウン
	unsigned long push_time;		//ボタン押下開始した時間
	unsigned long release_time;		//ボタンを離した時間

  	mcom_mode=2;

	while(millis() <  boot_time + STAGE2_TIME){
		stage2_blink();

		if (digitalRead(SW_PIN) == HIGH){
			push_time = millis();

			while (digitalRead(SW_PIN) ==HIGH ){
				release_time = millis();
				stage2_blink();

				if (release_time - push_time >= DISENGAGE_TIME ){
					mcom_disengage();
					return ;
				}
//				if (millis() <= (boot_time + STAGE2_TIME)) {
//						goto rtb2;
//				}
			  }
		}
	}
rtb2:
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

	unsigned long push_time = 0 ;		//ボタン押下開始した時間
	unsigned long release_time = 0 ;		//ボタンを離した時間

	if (digitalRead(SW_PIN) == HIGH){
		push_time = millis();
		while (digitalRead(SW_PIN) ==HIGH ){
			release_time = millis();
			if (release_time - push_time >= ENGAGE_TIME ){
				mcom_stage1( millis() );
			}
			if (digitalRead(SW_PIN) == LOW) {
				push_time = 0;
				release_time = 0;
			}
		}

		if(mcom_mode == 1){
			push_time = 0;
			release_time = 0;			
			mcom_stage2 (millis() );
		}

		if (mcom_mode == 2){
			mcom_stage3();
		}
	} 
}
