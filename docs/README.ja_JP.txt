�E����͉��H
	�T�o�Q�E�y�C���g�{�[���A���邢�͐ፇ�퓙�ŁABattlefield(R)�V���[�Y��"���b�V��"�ɏ��������[���ŗV�Ԃ��߂́AArduino���g�p�����A"M-COM"�̃t���[�����ł��B

�E�K�v�Ȃ���
	Arduino (�����[�V�[���h�Ƃ̌��ˍ�����AArduino Uno/Leonard���A�W���T�C�Y�����B)
	Arduino IDE(http://www.arduino.cc/)
	�剹�ʂ̃u�U�[
	LED�ԐF��
	���{�^���X�C�b�`(���[�����^��)�@�~1 (�N��/�����p)
	�^�N�g�X�C�b�` �~1 (���Z�b�g�{�^���p)
	�����[�V�[���h (SLD01101P����)
	�d�� (Arduino�p�ƁALED�ԐF���E�u�U�[�p��2�n�����K�v)

	�ȉ��̕��͉��u�Ď����s���ꍇ�ɕK�v�ƂȂ�܂��B
		PC
		UTF-8��LF�̉��s�R�[�h�Ή��̃e�L�X�g�G�f�B�^

		python 2.7�n��
		python2.7�p��wxPython
		pyserial2.7



�E�Z�b�g�A�b�v
	0:(���D�݂�)26�s�`29�s�ӂ�̃O���[�o���ȕϐ��E�萔��ύX���܂��B

	����́Astruct TIMER { const long LIMIT , const long STEP , long TIME  }  �Ƃ����\���̂ɂȂ��Ă��܂��B

		struct TIMER STAGE1 = {5, 2, 20} ;		//�X�e�[�W1(�������_��)��LIMIT, STEP, TIME
		struct TIMER STAGE2 = {2, 1, 10} ;		//�X�e�[�W2(�����_��)��LIMIT, STEP, TIME
		struct TIMER ENGAGE = {1, 1, 5};		//MCOM�N����LIMIT, STEP, TIME
		struct TIMER DISENGAGE = {10, 1, 5};	//MCOM������LIMIT, STEP, TIME

		LIMIT�͒��������ԁE�e�X�e�[�W�̍Œ᎞�Ԃ��w���Ă���܂��B
		STEP��MCOM�������ɂǂꂾ��TIME(��q)�𑝌����邩��\���Ă���܂��B
			�Ȃ��A���̒l���Ɗe�X�e�[�W�y�ыN���������͉�������STEP�b�Z���Ȃ�A�����������͒����Ȃ�悤�ɂȂ��Ă��܂��B		
		TIME�͂��ꂼ��̒��������ԁE�e�X�e�[�W�̌p�����Ԃł��B


	1:freemcom.ino��arduino�ɏĂ��܂�
	2:SW_PIN(2�ԃs��)���N��/�����p���{�^���ɐڑ����܂��B(���̃X�C�b�`�̓P�[�X�\�ʂ��牟���鏊�ɐݒu)
	3:�����[���g�p���āABUZZER_PIN(4�ԃs��)��剹�ʃu�U�[�ɐڑ����܂��B
	4:�����[���g�p���āALED_PIN(5�ԃs��)��ԐF���ɐڑ����܂��B
	5:Arudino UNO�̏ꍇ���Z�b�g�s����GND�ɗ��Ƃ��ƃ��Z�b�g�����̂ŁA���{�^���X�C�b�`�����GND�ɗ��Ƃ���悤�ɂ���B���̃X�C�b�`�̓P�[�X�����A�e�Ղɉ����Ȃ��ꏊ�ɐݒu���Ă��������B

	Fritzing�`���̉�H�}��freemcom.fzz , png�`���̉�H�}��freemcom_circuit.png�ł��B


�E���u�Ď��̍s����
	(USB�P�[�u���ł̉��u�Ď��͎��p�I�ł͂Ȃ��̂ŁA�{���ڂ͖������������㏑���\��ł��B)


�E�Q�[���ł̎g����
	0:�Q�[���J�n�O�Ƀ��Z�b�g�{�^��������
	1:�N��/�����{�^����ENGAGE_TIME�Ŏw�肵�����ԉ���������ƁA�f���I�Ƀu�U�[����ALED���_�ł����߂܂��B
	2:WAIT_TIME���o�߂����WARN_TIME�ɓ���ALED�_�ŃT�C�N���ƃu�U�[�̒f���Ԋu���Z���Ȃ�܂�
	3:1�`2�̊ԂɋN��/�����{�^����DISENGAGE_TIME�Ŏw�肳�ꂽ���ԉ���������ƁA�u�U�[���Z����&LED��2��_�ł��A�J�E���g�J�n�O�̏�Ԃɖ߂�܂��B
	3:WARN_TIME���o�߂��Ă��܂��ƁALED���_�����A������5�b�ԃu�U�[���葱���āu�t���[�Y�v���܂��B(����ȍ~�A�N��/�����{�^���������Ă�������Ԃɂ͖߂��܂���)
	4:�Q�[���̏��s��������A���邢�͎��Ԑ؂�ŃQ�[�����I��������A���Z�b�g�{�^����������0�̏�Ԃɖ߂��Ă��������B


�E���[�U�[������Ď���
	�E�N��/�����p�X�C�b�`���A�����{�^���X�C�b�`2����ɐڑ����āu���艟���X�C�b�`�v�ɕύX����ƁA�e��Ў�Ŏ������܂܋N��/�������삪�ł��Ȃ��Ȃ�܂��̂ŁA�`�[�����[�N�����d�v�ɂȂ�Q�[������傫���ς��鎖���ł���ł��傤�B


�E���C�Z���X
	���C�Z���X��GNU GPL v3(�ȍ~�̔C�Ӕ�)�Ƃ��܂��B���C�Z���X�̑S����COPYING�����ǂ݂��������B
		(���C�Z���X�S�����Z���R�[�h�ł����A�u���蕨�v�ɂ���鎖���l��������ŁA���[�U�[�ɂ����ǂ�W���Ȃ������Ӑ}��GPLv3�Ƃ��Ă��܂��B�����Ƃ��A�u���蕨�v�ɂ���ɂ́A�n�[�h�E�F�A�̃R�X�g�_�E�����K�{�ł����E�E�E)

�E���ҏ��
	Kiyohito AOKI(sambar.fgfs@gmail.com)
