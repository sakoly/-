#include "DxLib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define readnum 27//���̒�`��
int tell_mouse();//�}�E�X�̉�����Ă���ʒu��Ԃ�
int absol(int);
int zeropra(int);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	int stickx = 80;//�X�e�B�b�N�̂����W
	int size_x = 2; //�X�e�B�b�N��1/2����
	int size_y = 50; //�X�e�B�b�N��1/2��
	int edge = 5;//�X�e�B�b�N�̒[�̕�(������Ƃ��Ă�������ɋ����Ȃ���)
	int middle=5;//�X�e�B�b�N�̒���1/2���i������ƒx���Ȃ�j
	int vedge = 1;//�[�ɓ����������̋Ȃ����i�������قǋ����Ȃ���j
	int alpha = 10;//���������x
	int vo = 10;//�����x��
	int min = -1;//���x����
	int mac = 30;//���x�����̗������i�傫���قǑ��x����ɂ�����ɂ����Ȃ�j
	int vmax = 30;//���x����W���i����ɉ����X�R�A�ƒ��˕Ԃ萔��p���ď���l���v�Z����j
	int vscorerate = 7;	//���x����X�R�A���Z�䗦(�������قǏ�����オ��)
	int goalsize = 50;//�S�[����1/2��
	int goalx = 1080;//�S�[���̈ʒu
	int goalmv = 0;//�㉺�̕ǂŒ��˕Ԃ������̃S�[���̈ړ�����
	int boundmax = 2000;//�o�O�Ή��p���˕Ԃ����i������Ə�O������s���j
	int R = 30;//���̔��a
	int balsx = 100;//�{�[���̃X�^�[�g�ʒu�����i�S�[����[�Ƃ��āj
	int balsy = 100;//�{�[���̃X�^�[�g�ʒu���c�i�����𒆐S���Ƃ��āj
	int Screenx = 1300;//�X�N���[���T�C�Y��
	int Screeny = 300;//�X�N���[���T�C�Y�c
	int boxedge = 20;//�O�ǂ̕�
	int scoreo = 100;//�����X�R�A
	int scorepn = 5;//��O�̎��̃X�R�A���_���}�C�i�X�ɂ��ĉ������y����ł��ǂ�
	int boundrate = -10;//���˕Ԃ����񐔂�����Ŋ������l���X�R�A�ɉ��Z�����
	int scorepra = 5;//�S�[�������Ƃ��ɃX�R�A���Z�l
	int speed = 4;//�{�[���\�����x��i�{�[�����x�����̒l�Ŋ����Ĉړ������Ƃ��Ă���j

	//�t�@�C���ǂݍ���init
	FILE* fp;
	char read_buf[200]; 
	int intbuf[readnum], read;
	fopen_s(&fp, "ballgame.txt", "r");
	if (fp != NULL) {
		for (read = 0; read < readnum; read++) {
			fgets(read_buf, 200, fp);
			fgets(read_buf, 200, fp);
			sscanf_s(read_buf, "%d", &intbuf[read]);
		}
		stickx = absol(intbuf[0]);
		size_x = absol(intbuf[1]);
		size_y = absol(intbuf[2]);
		edge = absol(intbuf[3]);
		middle = absol(intbuf[4]);
		vedge = zeropra(absol(intbuf[5]));
		alpha = absol(intbuf[6]);
		vo = absol(intbuf[7]);
		min = intbuf[8];
		mac = absol(intbuf[9]);
		vmax = absol(intbuf[10]);
		vscorerate = zeropra(absol(intbuf[11]));
		goalsize = absol(intbuf[12]);
		goalx = absol(intbuf[13]);
		goalmv = intbuf[14];
		boundmax = absol(intbuf[15]);
		R = absol(intbuf[16]);
		balsx = absol(intbuf[17]);
		balsy = absol(intbuf[18]);
		Screenx = absol(intbuf[19]);
		Screeny = absol(intbuf[20]);
		boxedge = absol(intbuf[21]);
		scoreo = absol(intbuf[22]);
		scorepn = intbuf[23];
		boundrate = zeropra(intbuf[24]);
		scorepra = intbuf[25];
		speed= zeropra(absol(intbuf[26]));
		fclose(fp);
	}
	//�t�@�C���ǂݍ���end

	time_t tim;
	tim = time(NULL);
	srand(tim);

	ChangeWindowMode(TRUE);
	SetGraphMode(Screenx, Screeny, 16);
	SetWindowSizeChangeEnableFlag(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	if (R + boxedge+1 > Screeny / 2) {
		R = Screeny / 2 - boxedge-1;
	}
	if (balsy + R + boxedge > Screeny / 2) {
		balsy = Screeny / 2 - R - boxedge;
	}
	int X, Y, size = 0, x1, y1, x2, y2, bx= goalx+ rand() % balsx - balsx -R, by= Screeny/2 + rand() % balsy*2-balsy ,i=0,j=0,k,l=0;
	int color;
	int score = scoreo, bound = 0, hscore = 0;
	int vx = -rand() % (vo + (vmax + score / vscorerate)/10) *2 - (vo + (vmax + score / vscorerate) / 10), vy = rand() % vo*2-vo,fin=0;
	
	
	while (ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0) {
		fin = 0;
		DrawBox(0, 0, Screenx-boxedge, Screeny, GetColor(200, 200, 200), 1);

		//�Œᑬ�xinit
		if (vx <= min && vx >= 0) {
			vx = min;
		}
		else if (vx <0 && vx >= -min) {
			vx = -min;
		}
		if (vy <= min && vy > 0) {
			vy = min;
		}
		else if (vy < 0 && vy >= -min) {
			vy = -min;
		}
		else if (vy == 0 && min > 0) {
			vy = pow(-1, rand() % 2)*min;
		}
		//�Œᑬ�xinit

		GetMousePoint(&X, &Y);
		
		//���_��d��init
		if (l == 1) {
			Sleep(1000);
			l = 0;
		}
		//���_��d��end


		//�|�[�Yinit
		if (tell_mouse() == 1) {//�R�}����p
			while (tell_mouse() != 1);
			Sleep(100);
		}
		if (tell_mouse() == 2) {//��~�p
			while (tell_mouse()!=1);
			Sleep(150);
		}
		//�|�[�Yend


		//�X�e�B�b�Ninit
		
		//�A�V�X�ginit
		if (bx <= stickx + Screenx /4&&(X> -100 &&X< 0)) {
			Y = ((- stickx+ bx) * Y + (Screenx/4+stickx-bx) * by) / (Screenx/4);
		}
		//�A�V�X�gend

		if (Y - size_y < boxedge) {
			Y = boxedge + size_y;
		}
		if (Y + size_y > Screeny -boxedge) {
			Y = Screeny -boxedge - size_y;
		}
		
		X = stickx;
		
		x1 = X - size_x, y1 = Y - size_y, x2 = X + size_x, y2 = Y + size_y;
		
		DrawBox(x1, y1, x2, y2, GetColor(0, 0, 0), 1);
		DrawBox(x1, y1+edge, x2, y2-edge, GetColor(0, 255, 0), 1);
		DrawBox(x1 , Y-middle, x2, Y+middle, GetColor(255, 255, 0), 1);
		//�X�e�B�b�Nend

		
		//���˕Ԃ�init

		//�㉺���˕Ԃ�init
		if (by + R > Screeny -boxedge && bx + R <= goalx) {
			if (vy > 0) {
				vy += rand() % (alpha +score/100+score/30) / 3;
				vy *= -1;
				j += goalmv;
				bound++;
			}
			
		}
		if (by - R < boxedge && bx + R <= goalx) {
			if (vy < 0) {
				vy -= rand() % (alpha + score / 100+score/30) / 3;
				vy *= -1;
				j -= goalmv;
				bound++;
			}
		}
		if (j < -(Screeny / 2 - boxedge) + goalsize)j = -(Screeny / 2 - boxedge) + goalsize;
		if (j > (Screeny / 2 - boxedge) - goalsize)j = (Screeny / 2 - boxedge) - goalsize;
		//�㉺���˕Ԃ�end

		//�O��init
		color = GetColor(0, 120, 0);
		DrawBox(goalx, 0, Screenx, Screeny , color, 1);
		DrawBox(goalx, Screeny / 2 - goalsize + j, Screenx, Screeny / 2 + goalsize + j, GetColor(200, 200, 200), 1);
		DrawBox(0, 0, Screenx, boxedge, color, 1);
		DrawBox(0, Screeny -boxedge, Screenx, Screeny, color, 1);
		//�O��end

		
		//�X�e�B�b�N�ł��Ԃ�init
		if (bx - R * 5 / 7 >= X - size_x - absol(vx) / speed&&bx - R < X &&( by+R*5/7 >= Y - size_y && by-R*5/7 <= Y + size_y||by<boxedge||by>Screeny-boxedge)) {
			if (Y + middle > by && Y - middle<by) {
				i = 2;
				k = score/100+1;
			}
			else {
				i = 1;
				k = 0;
			}
			
			if (rand() % mac + 30 + score/100  < absol(vx)) {
				if(i==2)i++;
			}

			if (vx < 0) {
				vx = -vx / i + rand() % (alpha + score / 100 + score / 30);
				bound++;
				if (k > 0)k++;
				if (rand() % mac + 30 + score/100  < absol(vy)) {
					i = 2 + k;
				}
				else {
					i = 1 + k;
				}
				if (min <= 0||vy>i) {
					vy = vy / i;
				}
				else {
					if (vy > 0) {
						vy = 1;
					}
					else {
						vy = -1;
					}
				}
				if (Y + size_y - edge<=by || Y - size_y + edge>=by) {
					vy += (by - Y)*5/ vedge;
				}
			}
		}
		//�X�e�B�b�N�ł��Ԃ�end

		//�S�[���̘g�ɓ���Ȃ�init
		if ((bx <= goalx && by <= Screeny / 2 + goalsize + j && by > Screeny / 2 + goalsize + j - sqrt(R * R - pow(goalx - bx, 2))) &&
			(bx <= goalx && by >= Screeny / 2 - goalsize + j && by < Screeny / 2 - goalsize + j + sqrt(R * R - pow(goalx - bx, 2)))) {
			fin = 1;
			if (vx > 0) {
				vx += rand() % (alpha + score / 100 + score / 30) * 3 / 2;
				vx *= -1;
				bound++;
			}
		}
		//��̊pinit
		if ((bx <= goalx && by >= Screeny / 2 - goalsize + j - bx + goalx && by < Screeny / 2 - goalsize + j + sqrt(R * R - pow(goalx - bx, 2))) ||
			(goalx - sqrt(R * R - pow(by - Screeny / 2 + goalsize - j, 2)) <= bx && bx <= goalx - (by - Screeny / 2 + goalsize - j) && by >= Screeny / 2 - goalsize + j)) {
			fin = 1;
			if (vx > 0) {
				vx += rand() % (alpha + score / 100 + score / 30) * 3 / 2;
				vx *= -1;
				bound++;
				vy = vy + 8;
			}
		}
		if (bx <= goalx && by >= Screeny / 2 - goalsize + j - bx + goalx && by < Screeny / 2 - goalsize + j + sqrt(R * R - pow(goalx - bx, 2))) {
			if (vy < 0) {
				vy -= rand() % (alpha/2 + score / 100 + score / 50) / 3;
				vy *= -1;
				bound++;
			}
		}
		//��̊pend

		//���̊pinit
		if ((bx <= goalx && by <= Screeny / 2 + goalsize + j + bx - goalx && by > Screeny / 2 + goalsize + j - sqrt(R * R - pow(goalx - bx, 2))) ||
			(goalx - sqrt(R * R - pow(Screeny / 2 + goalsize + j - by, 2)) <= bx && bx <= goalx - (Screeny / 2 + goalsize + j - by) && by <= Screeny / 2 + goalsize + j)) {
			fin = 1;
			if (vx > 0) {
				vx += rand() % (alpha + score / 100 + score / 30) * 3 / 2;
				vx *= -1;
				bound++;
				vy = vy - 8;
			}
		}
		if (bx <= goalx && by <= Screeny / 2 + goalsize + j + bx - goalx && by > Screeny / 2 + goalsize + j - sqrt(R * R - pow(goalx - bx, 2))) {
			if (vy > 0) {
				vy += rand() % (alpha/2 + score / 100 + score / 50) / 3;
				vy *= -1;
				bound++;
			}
		}
		//���̊pend
		
		//�S�[���̘g�ɓ���Ȃ�end

		//�S�[���O���ǒ��˕Ԃ�init
		if ((vx > 0 && fin == 0 && bx + R >= goalx && (Screeny / 2 - goalsize + j >= by || by >= Screeny / 2 + goalsize + j)) && 
			(bx > goalx + vx / speed || (vy < 0 && Screeny / 2 - goalsize + j<by - vy / vx * (bx - goalx) && Screeny / 2 + goalsize + j>by - vy / vx * (bx - goalx) ||
			vy>0 && Screeny / 2 + goalsize + j > by - vy / vx * (bx - goalx) && by - vy / vx * (bx - goalx) > Screeny / 2 - goalsize + j)) == 0 ) {
			fin = 1;
			if (vx > 0) {
				vx += rand() % (alpha + score / 100 + score / 30) * 3 / 2;
				vx *= -1;
				bound++;
			}
		}
		//�S�[���O���ǒ��˕Ԃ�end

		//�S�[���㉺���˕Ԃ�init
		if ((vx > 0 && fin == 0 && bx >= goalx && (vy<0&&Screeny / 2 - goalsize + j > by - R || by + R > Screeny / 2 + goalsize + j&&vy>0) &&
			(bx>goalx+vx/speed||(vy<0&&Screeny / 2 - goalsize + j<by - vy / vx * (bx - goalx)&& Screeny / 2 + goalsize + j>by - vy / vx * (bx - goalx) || 
				vy>0&&Screeny / 2 + goalsize + j>by - vy / vx * (bx - goalx)&& by - vy / vx * (bx - goalx)> Screeny / 2 - goalsize + j)))) {
			fin = 1;
			if (by + R >= Screeny / 2 + j + goalsize) {
				if (vy > 0) {
					vy = -vy;
					bound ++;
				}
			}
			if (by - R <= Screeny / 2 + j - goalsize) {
				if (vy < 0) {
					vy = -vy;
					bound ++;
				}
			}

		}
		//�S�[���㉺���˕Ԃ�end

		//���˕Ԃ�end

		//�{�[���\��init
		if (vy > vmax + score / vscorerate/2 + bound / 50 + 3 * (bound / 300))vy = vmax + score / vscorerate/2 + bound / 50 + 3 * (bound / 300);
		if (vy < -vmax - score / vscorerate/2 - bound / 50 - 30 * (bound / 300))vy = -vmax - score / vscorerate/2 - bound / 50 - 3 * (bound / 300);
		if (vx > vmax + score / vscorerate + bound / 50 + 3*(bound / 300))vx = vmax + score / vscorerate + bound / 50 + 3 * (bound / 300);
		if (vx < -vmax - score / vscorerate- bound / 50 - 3*(bound / 300))vx = -vmax - score / vscorerate - bound / 50 - 3 * (bound / 300);
		bx = bx + vx/speed;
		by = by + vy/speed;
		DrawCircle(bx, by, R, GetColor(255, 0, 0), 1, 1);
		//�{�[���\��end
		
		//�{�[����Oinit
		if (vx<0&&bx - R * 5 / 7 < X - size_x - absol(vx) / speed || bound>boundmax) {//�X�e�B�b�N�O���A���˕Ԃ�������
			l = 1;
			j = 0;
			vx = -rand() % (vo + (vmax + score / vscorerate) / 10) *2 - (vo + (vmax + score / vscorerate) / 10), vy = rand() % vo*2 - vo;
			bx = goalx + rand() % balsx - balsx - R, by = Screeny / 2 + rand() % balsy * 2 - balsy;
			if (score >= scorepn) {
				DrawFormatString(Screenx / 2, Screeny / 2, GetColor(0, 0, 255), "%d%+d", score, -scorepn);
				score = score- scorepn;
			}
			else {
				DrawFormatString(Screenx / 2, Screeny / 2, GetColor(0, 0, 255), "%d%+d", score, -score);
				score = 0;
			}
			bound = 0;
		}
		if (vx>0&&bx-R > Screenx+vx/speed && by <= Screeny / 2 +goalsize+j && by >= Screeny / 2 -goalsize+j) {//�S�[������
			DrawFormatString(Screenx / 2, Screeny / 2, GetColor(0, 0, 255), "%d%+d", score,bound / boundrate+scorepra);
			l = 1;
			j = 0;
			vx = -rand() % (vo + (vmax + score / vscorerate) / 10) *2 - (vo + (vmax + score / vscorerate) / 10), vy = rand() % vo*2 - vo;
			bx = goalx + rand() % balsx - balsx - R, by = Screeny / 2 + rand() % balsy * 2 - balsy;
			score=score+scorepra;
			score =score+ bound / boundrate;
			bound = 0;
		}
		//�{�[����Oend

		//�S�[����X�R�A�\��init
		if (score < 0)score = 0;
		if (score > hscore)hscore = score;
		DrawFormatString(30, 30, GetColor(0, 0, 255), "now score:%3d  high score:%3d  bound:%3d", score, hscore, bound);
		//�S�[����X�R�A�\��end
		
	}
	DxLib_End();
	return 0;
}



//�֐���`
int tell_mouse() {
	int x = 0;
	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		x = 1;
	}
	else if (GetMouseInput() & MOUSE_INPUT_RIGHT) {
		x = 2;
	}
	else if (GetMouseInput() & MOUSE_INPUT_MIDDLE) {
		x = 3;
	}
	return x;
}
int zeropra(int t) {
	if (t == 0) {
		return 1;
	}
	else {
		return t;
	}
}
int absol(int t) {
	if (t < 0) {
		return -t;
	}
	else {
		return t;
	}
}