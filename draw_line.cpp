#include "DxLib.h"
#include "time.h"
#include "stdlib.h"
#define LX 1300//�X�N���[���T�C�Y�i���j
#define LY 720//�X�N���[���T�C�Y�i�c�j
#define capa 10000//���f�[�^�̗e��
#define point 2//�_�̎��Ԋu
#define gia 30//�g��i�K
#define size_roll 1//���[���̔��蕝
#define select 5//�I����ʂ̑I����
#define select_x1 200//�I����ʃ{�^���{�b�N�X����x���W
#define select_y1 100*LY/480//�I����ʃ{�^���{�b�N�X����y���W
#define select_val 10*LY/480//�I����ʂ̊Ԋu
#define size 50*LY/480//�����T�C�Y
#define lsize 1//���̑����i�����j
#define midx LX/2//���S��x���W
#define midy (LY+40)/2//���S��y���W
#define wait_time 10//�I���{�^���N���b�N��둀��h�~�҂�����

int Judge_Box(int X, int Y, int x1, int y1, int x2, int y2);//���ʓ��Ȃ�1���ʊO�Ȃ�0
void Draw_Boxedge(int x1, int y1, int x2, int y2, int color, int edge);//�g�^
int tell_mouse();//�}�E�X�̉�����Ă���ʒu��Ԃ�
int Judge_Pointx(int r, int rat, int xdisp);//�_�����ۂɁA�i�q�_�����悤�ɋ߂��̓_��T���ix���W�j
int Judge_Pointy(int r, int rat, int ydisp);//�_�����ۂɁA�i�q�_�����悤�ɋ߂��̓_��T���iy���W�j

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE), SetGraphMode(LX, LY, 16), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK), SetWindowSizeChangeEnableFlag(TRUE); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�
    int X, Y;//�}�E�X�̈ʒu
    int mouse;//�}�E�X�̓��� 0=none,1=left,2=right,3=middle
    int roll = 0;//�}�E�X�̃z�C�[����]��
    int tate[capa][4], yoko[capa][4];//���̍��W (x1,y1,y2,0)�A(x1,y1,x2,0)�A�Ō�̃Z���͕\����Ԃ��ǂ���
    int tnow_capa = 0, ynow_capa = 0, tnows = 0, ynows = 0;//���f�[�^�ɂ��Ď��ɏ������ރZ���ԍ�
    int i, j, k,save[2];//�v�Z�p�ϐ�
    int rate = 5;//��ʂ̊g��x
    int mode = 0;//��ƃ��[�h�ϐ��A�ҏW����I���܂ŁA�}�C�i�X�̂͑I�����ꂽ���̃��[�h
    int FontHandle = CreateFontToHandle(NULL, size, 3);//�\�����镶���̃t�H���g
    unsigned int col[5] = { GetColor(255, 255, 255),GetColor(255, 255, 0),GetColor(0,255,0) ,GetColor(100,100,100),GetColor(100, 100, 0) };//�F�̕ϐ��A0=none(��),1=select(��),2=selected(��)
    char* selm0[select] = { "�@�@ �@�ҁ@�W","�@ �@�@���[�h","�@ �@�@�Z�[�u","�@  �����_������","�@ �@�@�I�@��" };//�X�^�[�g��ʂ̑I����
    char* fpmessage[5] = { "","�@�ǂݎ��Ɏ��s","�@�ǂݎ��ɐ���","�@�������݂Ɏ��s","�@�������݂ɐ���" };//�t�@�C������̃��b�Z�[�W
    int fpm = 0;//�t�@�C������̃��b�Z�[�W�Z���ԍ�
    char* selm1[8] = { "�m��","�߂�","����","����","�悱","����","���_","�I��"};//�ҏW��ʂ̑I����
    char* selm2[2] = { "  �{���Ƀ��[�h����","�@ �@�@�߁@��" };//���[�h��ʂ̑I����
    char* selm3[2] = { "  �{���ɃZ�[�u����","�@ �@�@�߁@��" };//�Z�[�u��ʂ̑I����
    char* selm4[2] = { " �@�{���ɐ�������","�@ �@�@�߁@��" };//�����_��������ʂ̑I����
    char* selm5[2] = { " �@�{���ɏI������","�@ �@�@�߁@��" };//�I����ʂ̑I����
    int ch1 = 0;//�I�����ꂽ�{�^���ԍ� 0=none,1=�m��,2=�߂�,3=����,4=����,5=�悱,6=����,7=���_,8=�I��
    int Wait = wait_time;//�ҏW�őI���㏭���҂i����ē�d�ɑ��삷��̂�h���j
    int back = 0;//back=1�̎��A�ҏW�ő����߂�
    int k1[10];//�ҏW�őI���{�^���ɂ��đI������Ă����1�����łȂ����0;
    int Kt[capa], Ky[capa];//���̑I���̐���i�}�E�X�����̏�ɂ��邩�ǂ����j
    int chback;//�Z�[�u��I���Ń{�^�����o��܂ő҂i�Z�[�u��I�����s���ƃf�[�^�������Ă��܂����ꂪ���邽�߁j
    int xm1 = INT_MAX, ym1 = INT_MAX, xm2 = INT_MAX, ym2 = INT_MAX;//���\���O�̓_�̍��W
    int tyjud;//�c���łǂ���̃��C����������3=none,4=yoko,5=tate
    int xydisp[2] = { 0 };//���_����ǂꂾ�����W�ړ�������
    int xysave[2] = { INT_MAX,INT_MAX };//�E�N���b�N�ʒu�̕ۑ�
    int xysavemid[2] = {INT_MAX,INT_MAX};//�����N���b�N�ʒu�̕ۑ�
    int xydispsv[2] = { 0 };//�O���xydisp�̒l��ۑ�
    int midstate=0;//�}�E�X�̃~�h���N���b�N��������ċ�����ڂ����ڂ�
    int conti = 0;//�N���b�N���O���܂Ń��[�v�𔲂������Ȃ����ƂŌ둀���h��
    char read_buf[100];//�ǂݎ��p�̔z��
    time_t tim;//�^�C���֐��ϐ�
    unsigned int random_num = 100;//�����쐬��
    unsigned int random_max = 100;//�����̏���i��Βl�j
    int random[3];//����
    FILE* fp;//�ۑ��ǂݍ��݂ł̃t�@�C���ϐ�

    /*�����f�[�^�ǂݍ���init*/
    //�t�@�C���̏��߂̓�s��ǂ�ŁA��s�ڂ𗐐��̍ő�l�A��s�ڂ𗐐��̌��Ƃ���
    fopen_s(&fp, "line_file.txt", "r");
    if (fp == NULL) {
        fpm = 1;
    }
    else {
        fpm = 2;
        fgets(read_buf, 100, fp);
        sscanf_s(read_buf, "%d", &j);
        if (j<=0) {
            j = 1;
        }
        else if(j > capa) {
            j = capa;
        }
        random_num = j;
        fgets(read_buf, 100, fp);
        sscanf_s(read_buf, "%d", &j);
        if (j <= 0) {
            j = 1;
        }
        else if (j > 20000) {
            j = 20000;
        }
        random_max = j;
        fclose(fp);
    }
    /*�����f�[�^�ǂݍ���end*/


    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        if (tnows < tnow_capa)tnows = tnow_capa;//tnows��tnow_capa�̍ő�l����
        if (ynows < ynow_capa)ynows = ynow_capa;//ynows��ynow_capa�̍ő�l����

        /*����init*/
        //�}�E�X�����ϐ��ɑ��
        GetMousePoint(&X, &Y);//�}�E�X�̈ʒu���

        mouse = tell_mouse();//�}�E�X�̓���

        if (mode == 1) {
            //�}�E�X�̃z�C�[������init
            roll += GetMouseWheelRotVol();
            if (roll < 0) {
                roll = 0;
            }
            else if (roll > gia * size_roll) {
                roll = gia * size_roll;
            }
            rate = roll / size_roll + 5;
            //�}�E�X�̃z�C�[������end

            //�}�E�X�̃~�h���N���b�N�ψ�init
            if (mouse == 3 && Y > size + 10 && midstate == 0) {
                midstate = 1;
                while (mouse == 3) {
                    mouse = tell_mouse();
                }
            }
            if (mouse == 3 && Y > size + 10 && midstate == 1) {
                midstate = 0;
                while (mouse == 3) {
                    mouse = tell_mouse();
                }
            }
            if (midstate == 1) {
                if (xysavemid[0] == INT_MAX) {
                    xysavemid[0] = X;
                    xydispsv[0] = xydisp[0];
                }
                else {
                    xydisp[0] += (X - xysavemid[0]) / rate / 5;
                }
                if (xysavemid[1] == INT_MAX) {
                    xysavemid[1] = Y;
                    xydispsv[1] = xydisp[1];
                }
                else {
                    xydisp[1] += (Y - xysavemid[1]) / rate / 5;
                }
            }
            else {
                xysavemid[0] = INT_MAX;
                xysavemid[1] = INT_MAX;
            }
            //�}�E�X�̃~�h���N���b�N�ψ�end

            //�}�E�X�̉E�N���b�N�ψ�init
            if (mouse == 2 && Y > size + 10 && midstate == 0) {
                if (xysave[0] == INT_MAX) {
                    xysave[0] = X;
                    xydispsv[0] = xydisp[0];
                }
                else {
                    xydisp[0] = (X - xysave[0]) / rate + xydispsv[0];
                }
                if (xysave[1] == INT_MAX) {
                    xysave[1] = Y;
                    xydispsv[1] = xydisp[1];
                }
                else {
                    xydisp[1] = (Y - xysave[1]) / rate + xydispsv[1];
                }
            }
            else {
                xysave[0] = INT_MAX;
                xysave[1] = INT_MAX;
            }
            //�}�E�X�̉E�N���b�N�ψ�end
        }
        
        /*����end*/


        /*���[�h���sinit*/
        //mode=0������ԁAmode=1�ҏW�Amode=�}2���[�h�Amode=�}3�Z�[�u�Amode=�}4�����_�������Amode=�}5�I��

        ////(mode-5)init(�I��)
        if (mode == -5) {
            break;
        }
        ////(mode-5)end

        ////(mode-4)init�i�����_�������j
        if (mode == -4) {
            fopen_s(&fp, "line_file.txt", "w");
            if (fp == NULL) {
                mode = 0;
                fpm = 3;
            }
            else {
                tim = time(NULL);
                srand(tim);
                fpm = 4;
                mode = 0;
                fprintf(fp, "%d\n", random_num);
                fprintf(fp, "%d\n", random_num);
                for (i = 0; i < random_num; i++) {
                    random[0] = point * (rand() % (random_max * 2 / point) - random_max / point) + midx;
                    random[1] = point * (rand() % (random_max * 2 / point) - random_max / point) + midy;
                    random[2] = point * (rand() % (random_max * 2 / point) - random_max / point) + midx;
                    if (random[0] > random[2]) {
                        k = random[0];
                        random[0] = random[2];
                        random[2] = k;
                    }
                    fprintf(fp, "%d %d %d %d\n", random[0], random[1], random[2], 1);
                }
                fprintf(fp, "%d\n", random_num);
                fprintf(fp, "%d\n", random_num);
                for (i = 0; i < random_num; i++) {
                    random[0] = point * (rand() % (random_max * 2 / point) - random_max / point) + midx;
                    random[1] = point * (rand() % (random_max * 2 / point) - random_max / point) + midy;
                    random[2] = point * (rand() % (random_max * 2 / point) - random_max / point) + midy;
                    if (random[1] > random[2]) {
                        k = random[1];
                        random[1] = random[2];
                        random[2] = k;
                    }
                    fprintf(fp, "%d %d %d %d\n", random[0], random[1], random[2], 1);
                }
                fclose(fp);
            }
        }
        ////(mode-4)end


        ////(mode-3)init�i�Z�[�u�j
        if (mode == -3) {
            fopen_s(&fp, "line_file.txt", "w");
            if (fp == NULL) {
                mode = 0;
                fpm = 3;
            }
            else {
                fpm = 4;
                mode = 0;
                fprintf(fp, "%d\n", ynows);
                fprintf(fp, "%d\n", ynow_capa);
                for (i = 0; i < ynows; i++) {
                    fprintf(fp, "%d %d %d %d\n", yoko[i][0], yoko[i][1], yoko[i][2], yoko[i][3]);
                }
                fprintf(fp, "%d\n", tnows);
                fprintf(fp, "%d\n", tnow_capa);
                for (i = 0; i < tnows; i++) {
                    fprintf(fp, "%d %d %d %d\n", tate[i][0], tate[i][1], tate[i][2], tate[i][3]);
                }
                fclose(fp);
            }
        }
        ////(mode-3)end

        ////(mode-2)init�i���[�h�j
        if (mode == -2) {
            fopen_s(&fp, "line_file.txt", "r");
            if (fp == NULL) {
                mode = 0;
                fpm = 1;
            }
            else {
                fpm = 2;
                mode = 0;
                fgets(read_buf, 100, fp);
                sscanf_s(read_buf, "%d", &j);
                ynows = j;
                fgets(read_buf, 100, fp);
                sscanf_s(read_buf, "%d", &j);
                ynow_capa = j;
                for (i = 0; i < ynows; i++) {
                    fgets(read_buf, 100, fp);
                    sscanf_s(read_buf, "%d %d %d %d", &yoko[i][0], &yoko[i][1], &yoko[i][2], &yoko[i][3]);
                }
                fgets(read_buf, 100, fp);
                sscanf_s(read_buf, "%d", &j);
                tnows = j;
                fgets(read_buf, 100, fp);
                sscanf_s(read_buf, "%d", &j);
                tnow_capa = j;
                for (i = 0; i < tnows; i++) {
                    fgets(read_buf, 100, fp);
                    sscanf_s(read_buf, "%d %d %d %d", &tate[i][0], &tate[i][1], &tate[i][2], &tate[i][3]);
                }
                fclose(fp);
            }
        }
        ////(mode-2)end

        

        ////(mode0)init�i������ԁj
        if (mode == 0) {
            DrawStringToHandle(100, 30, "���[�h�I��", GetColor(0, 255, 255), FontHandle);
            //�{�^��init
            for (i = 0; i < select; i++) {
                k1[i] = Judge_Box(X, Y, select_x1, select_y1 + select_val * i + size * i, LX - 300, select_y1 + select_val * i + size * (i + 1));
                DrawStringToHandle(select_x1 + k1[i] * 30, select_y1 + select_val * i + size * i, selm0[i], col[k1[i]], FontHandle);
                Draw_Boxedge(select_x1 + k1[i] * 30, select_y1 + select_val * i + size * i, LX - 300 + k1[i] * 30, select_y1 + select_val * i + size * (i + 1), col[k1[i]], 2);
            }
            for (i = 0; i < select; i++) {

                if (k1[i] && mouse == 1) {
                    mode = i + 1;
                    while (mouse == 1) {
                        mouse = tell_mouse();
                    }
                    break;
                }
            }
            if (mode == 2 || mode == 3 || mode == 4 || mode == 5)chback = 1000000;
            k1[i] = 0;
            i = 0;
            //�{�^��end
            DrawStringToHandle(select_x1, select_y1 + select_val * 5 + size * 5, fpmessage[fpm], col[0], FontHandle);
        }
        ////(mode0)end

        ////(mode1)init�i�ҏW�j
        if (mode == 1) {
            if (ch1 == 4) {
                for (i = 0; i < tnows; i++) {
                    if (tate[i][3] == 0) {//�\����Ԃ̎�
                        if ((tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate > 0 && (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX) {//��ʃT�C�Y����̏o�͐���
                            if ((tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY && (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate >0) {//��ʃT�C�Y����̏o�͐���

                                if (Y > size + 10 && Judge_Box(X, Y, (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate, (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                    Kt[i] = 4;//�I�����Ă�����
                                }
                                else {
                                    Kt[i] = 3;
                                }

                                DrawBox((tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate, (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate, col[Kt[i]], 1);
                            }
                        }
                    }
                }
                for (i = 0; i < ynows; i++) {
                    if (yoko[i][3] == 0) {//�\����Ԃ̎�
                        if ((yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate > 0&& (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY) {//��ʃT�C�Y����̏o�͐���
                            if ((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX && (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate>0) {//��ʃT�C�Y����̏o�͐���

                                if (Y > size + 10 && Judge_Box(X, Y, (yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                    Ky[i] = 4;//�I�����Ă�����

                                }
                                else {
                                    Ky[i] = 3;
                                }

                                DrawBox((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate, col[Ky[i]], 1);
                            }
                        }
                    }
                }
            }
            //���̉�ʏo��init
            for (i = 0; i < tnows; i++) {
                if (tate[i][3] == 1) {//�\����Ԃ̎�
                    if ((tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate > 0 && (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX) {//��ʃT�C�Y����̏o�͐���
                        if ((tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY && (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate >0) {//��ʃT�C�Y����̏o�͐���

                            if (Y > size + 10 && Judge_Box(X, Y, (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate, (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                Kt[i] = 1;//�I�����Ă�����
                            }
                            else {
                                Kt[i] = 0;
                            }

                            DrawBox((tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate, (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate, col[Kt[i]], 1);
                        }
                    }
                }

            }
            
            for (i = 0; i < ynows; i++) {
                if (yoko[i][3] == 1) {//�\����Ԃ̎�
                    if ((yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate > 0 && (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY) {//��ʃT�C�Y����̏o�͐���
                        if ((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX && (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate>0) {//��ʃT�C�Y����̏o�͐���

                            if (Y > size + 10 && Judge_Box(X, Y, (yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                Ky[i] = 1;//�I�����Ă�����

                            }
                            else {
                                Ky[i] = 0;
                            }

                            DrawBox((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate, col[Ky[i]], 1);
                        }
                    }
                }

            }
            
        
            DrawCircle((xm1 + xydisp[0] - midx) * rate + midx, (ym1 + xydisp[1] - midy) * rate + midy, lsize * rate * 2, col[0], 1, 1);//��1�_
            DrawCircle((xm2 + xydisp[0] - midx) * rate + midx, (ym2 + xydisp[1] - midy) * rate + midy, lsize * rate * 2, col[0], 1, 1);//��2�_
            if(midstate)DrawCircle(xysavemid[0], xysavemid[1], 5, col[2], 1, 1);//�ړ����S�_
            //���̉�ʏo��end

            DrawBox(0, 0, LX, size + 10, 0, 1); //�{�^������폜

            

            //�{�^��init

            for (i = 0; i < 8; i++) {
                k1[i] = Judge_Box(X, Y, 5 + i * size * 2 + 10 * i, 5, 5 + size * 2 * (i + 1) + 10 * i, 5 + size);
                if (i != ch1 - 1) {//�N���b�N����Ă��邩�̔���
                    DrawStringToHandle(5 + i * size * 2 + 10 * i, 5, selm1[i], col[k1[i]], FontHandle);
                    Draw_Boxedge(5 + i * size * 2 + 10 * i, 5, 5 + size * 2 * (i + 1) + 10 * i, 5 + size, col[k1[i]], 2);
                }
                else {//�N���b�N����Ă��鎞
                    DrawStringToHandle(5 + i * size * 2 + 10 * i, 5, selm1[i], col[2], FontHandle);
                    Draw_Boxedge(5 + i * size * 2 + 10 * i, 5, 5 + size * 2 * (i + 1) + 10 * i, 5 + size, col[2], 2);
                }

            }
            if (ch1 == 0) {
                for (i = 0; i < 8; i++) {
                    if (i == 7) {
                        if (k1[i] && mouse == 1) {
                            mode = 0;
                            while (mouse == 1) {
                                mouse = tell_mouse();
                            }
                            break;
                        }
                    }
                    else {
                        if (k1[i] && mouse == 1) {
                            Wait = wait_time;
                            ch1 = i + 1;
                            if (i == 0 && xm2 == INT_MAX) {
                                ch1 = 0;
                            }
                            if (i == 2 && (tnow_capa + ynow_capa) == 0) {//�������̂�����������������[�h�ɓ���Ȃ�
                                ch1 = 0;
                            }
                            break;
                        }
                    }
                }
            }
            //�{�^��end
            
            //��_�ڂƓ�_�ڂ���؂�init
            while (conti == 1 && mouse == 1) {
                mouse = tell_mouse();
            }
            conti = 0;
            //��_�ڂƓ�_�ڂ���؂�init
            
            //�{�^���̘A�����薳��init
            if (Wait) {
                Wait *= 0.9;
            }
            //�{�^���̘A�����薳��end
            else {
                //���_init
                if (ch1 == 0 || ch1 == 5 || ch1 == 6) {
                    if (mouse == 1 && xm1 == INT_MAX) {
                        if (Y > size + 10) {
                            save[0] = Judge_Pointx(X - xydisp[0], rate, xydisp[0]);
                            save[1] = Judge_Pointy(Y - xydisp[1], rate, xydisp[1]);
                            if (save[0] > INT_MAX / gia && save[0] < INT_MIN / gia)continue;
                            if (save[0] > INT_MAX / gia && save[0] < INT_MIN / gia)continue;
                            xm1 = save[0];
                            ym1 = save[1];
                            DrawCircle((xm1 + xydisp[0] - midx) * rate + midx, (ym1 + xydisp[1] - midy) * rate + midy, lsize * rate * 2, col[0], 1, 1);//��1�_
                            conti = 1;
                            continue;
                        }
                    }
                
                }
                //���_end

                //�m��init
                if (ch1 == 1) {
                    if (tyjud == 4 && ynow_capa < capa) {
                        for (i = 0; i < ynow_capa; i++) {
                            if (yoko[i][3] == 0) {
                                yoko[i][3] = 1;
                                if (xm1 > xm2) {
                                    j = xm1;
                                    xm1 = xm2;
                                    xm2 = j;
                                }
                                yoko[i][0] = xm1;
                                yoko[i][1] = ym1;
                                yoko[i][2] = xm2;
                                ynow_capa++;
                                break;
                            }
                        }
                        if (i == ynow_capa) {
                            yoko[i][3] = 1;
                            if (xm1 > xm2) {
                                j = xm1;
                                xm1 = xm2;
                                xm2 = j;
                            }
                            yoko[i][0] = xm1;
                            yoko[i][1] = ym1;
                            yoko[i][2] = xm2;
                            ynow_capa++;
                        }
                        xm1 = INT_MAX, ym1 = INT_MAX, xm2 = INT_MAX, ym2 = INT_MAX;
                        tyjud = 3;
                        ch1 = 0;
                        Wait = wait_time;
                    }
                    else if (tyjud == 5 && tnow_capa < capa) {
                        for (i = 0; i < tnow_capa; i++) {
                            if (tate[i][3] == 0) {
                                tate[i][3] = 1;
                                if (ym1 > ym2) {
                                    j = ym1;
                                    ym1 = ym2;
                                    ym2 = j;
                                }
                                tate[i][0] = xm1;
                                tate[i][1] = ym1;
                                tate[i][2] = ym2;
                                tnow_capa++;
                                break;
                            }
                        }
                        if (i == tnow_capa) {
                            tate[i][3] = 1;
                            if (ym1 > ym2) {
                                j = ym1;
                                ym1 = ym2;
                                ym2 = j;
                            }
                            tate[i][0] = xm1;
                            tate[i][1] = ym1;
                            tate[i][2] = ym2;
                            tnow_capa++;
                        }
                        xm1 = INT_MAX, ym1 = INT_MAX, xm2 = INT_MAX, ym2 = INT_MAX;
                        tyjud = 3;
                        ch1 = 0;
                        Wait = wait_time;
                    }
                }
                //�m��end


                //�߂�init
                if (ch1 == 2) {
                    if (back) {
                        back = 0;
                    }
                    else if (xm2 != INT_MAX) {
                        xm2 = INT_MAX;
                        ym2 = INT_MAX;
                        tyjud = 3;
                    }
                    else if (xm2 == INT_MAX && ym2 == INT_MAX) {
                        if (xm1 != INT_MAX) {
                            xm1 = INT_MAX;
                            ym1 = INT_MAX;
                        }
                    }
                    ch1 = 0;
                    Wait = wait_time;
                }
                //�߂�end

                //����init
                if (ch1 == 3) {
                    if (k1[1] && mouse == 1) {
                        ch1 = 0;
                        Wait = wait_time;
                        back = 1;
                    }
                    for (i = 0; i < ynows; i++) {
                        if (Ky[i] == 1 && mouse == 1) {
                            yoko[i][3] = 0;
                            ynow_capa--;
                            Ky[i] = 0;
                        }
                    }
                    for (i = 0; i < tnows; i++) {
                        if (Kt[i] == 1 && mouse == 1) {
                            tate[i][3] = 0;
                            tnow_capa--;
                            Kt[i] = 0;
                        }
                    }
                }
                //����end

                //����init
                if (ch1 == 4) {
                    if (k1[1] && mouse == 1) {
                        ch1 = 0;
                        Wait = wait_time;
                        back = 1;
                    }
                    for (i = 0; i < ynows; i++) {
                        if (Ky[i] == 4 && mouse == 1) {
                            yoko[i][3] = 1;
                            ynow_capa++;
                            Ky[i] = 1;
                        }
                    }
                    for (i = 0; i < tnows; i++) {
                        if (Kt[i] == 4 && mouse == 1) {
                            tate[i][3] = 1;
                            tnow_capa++;
                            Kt[i] = 1;
                        }
                    }
                }
                //����end

                //�悱init
                if (ch1 == 5) {
                    if (k1[1] && mouse == 1) {
                        ch1 = 0;
                        Wait = wait_time;
                        back = 1;
                    }
                    if (mouse == 1 && xm1 != INT_MAX && xm2 == INT_MAX) {
                        if (Y > size + 10) {
                            save[0] = Judge_Pointx(X - xydisp[0], rate, xydisp[0]);
                            if (save[0] > INT_MAX / gia && save[0] < INT_MIN / gia)continue;
                            xm2 = save[0];
                            ym2 = ym1;
                            ch1 = 0;
                            Wait = wait_time;
                            tyjud = 4;

                        }
                    }
                }
                //�悱end

                //����init
                if (ch1 == 6) {
                    if (k1[1] && mouse == 1) {
                        ch1 = 0;
                        Wait = wait_time;
                        back = 1;
                    }
                    if (mouse == 1 && xm1 != INT_MAX && ym2 == INT_MAX) {
                        if (Y > size + 10) {
                            save[1] = Judge_Pointy(Y - xydisp[1], rate, xydisp[1]);
                            if (save[1] > INT_MAX / gia && save[1] < INT_MIN / gia)continue;
                            ym2 = save[1];
                            xm2 = xm1;
                            ch1 = 0;
                            Wait = wait_time;
                            tyjud = 5;

                        }
                    }
                }
                //����end

                //���_int
                if (ch1 == 7) {
                    xydisp[0] = 0;
                    xydisp[1] = 0;
                    ch1 = 0;
                    Wait = wait_time;
                }
                //���_end

                
            }


        }
        ////(mode1)end

        ////(mode2)init
        if (mode == 2) {
            DrawStringToHandle(100, 30, "���݂̕ҏW�͍폜����܂�", GetColor(255, 0, 0), FontHandle);
            if (chback) chback *= 0.9;
            for (i = 0; i < 2; i++) {
                if (chback && i == 0)continue;
                k = Judge_Box(X, Y, 200, LY / 2 - select_val + select_val * i * 5 + size * (i - 1), LX - 300, LY / 2 - select_val + select_val * 5 * i + size * i);
                DrawStringToHandle(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), selm2[i], col[k], FontHandle);
                Draw_Boxedge(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300 + 30 * k, LY / 2 - select_val + select_val * 5 * i + size * i, col[k], 2);
                if (k && mouse == 1) {
                    mode = i * 2 - 2;
                    while (mouse == 1) {
                        mouse = tell_mouse();
                    }
                    break;
                }
            }
        }
        ////(mode2)end

        ////(mode3)init

        if (mode == 3) {
            DrawStringToHandle(100, 30, "�O��̃f�[�^�͍폜����܂�", GetColor(255, 0, 0), FontHandle);
            if (chback) chback *= 0.9;
            for (i = 0; i < 2; i++) {
                if (chback && i == 0)continue;
                k = Judge_Box(X, Y, 200, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300, LY / 2 - select_val + select_val * 5 * i + size * i);
                DrawStringToHandle(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), selm3[i], col[k], FontHandle);
                Draw_Boxedge(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300 + 30 * k, LY / 2 - select_val + select_val * 5 * i + size * i, col[k], 2);
                if (k && mouse == 1) {
                    mode = i * 3 - 3;
                    while (mouse == 1) {
                        mouse = tell_mouse();
                    }
                    break;
                }
            }

        }
        ////(mode3)end

        ////(mode4)init
        if (mode == 4) {
            DrawStringToHandle(100, 30, "�O��̃f�[�^�͍폜����܂�", GetColor(255, 0, 0), FontHandle);
            if (chback) chback *= 0.9;
            for (i = 0; i < 2; i++) {
                if (chback && i == 0)continue;
                k = Judge_Box(X, Y, 200, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300, LY / 2 - select_val + select_val * 5 * i + size * i);
                DrawStringToHandle(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), selm4[i], col[k], FontHandle);
                Draw_Boxedge(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300 + 30 * k, LY / 2 - select_val + select_val * 5 * i + size * i, col[k], 2);
                if (k && mouse == 1) {
                    mode = i * 4 - 4;
                    while (mouse == 1) {
                        mouse = tell_mouse();
                    }
                    break;
                }
            }
        }
        ////(mode4)end

        ////(mode5)init
        if (mode == 5) {
            DrawStringToHandle(100, 30, "�����l�ł���", GetColor(0, 255, 0), FontHandle);
            if (chback) chback *= 0.9;
            for (i = 0; i < 2; i++) {
                if (chback && i == 0)continue;
                k = Judge_Box(X, Y, 200, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300, LY / 2 - select_val + select_val * 5 * i + size * i);
                DrawStringToHandle(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), selm5[i], col[k], FontHandle);
                Draw_Boxedge(200 + k * 30, LY / 2 - select_val + select_val * 5 * i + size * (i - 1), LX - 300 + 30 * k, LY / 2 - select_val + select_val * 5 * i + size * i, col[k], 2);
                if (k && mouse == 1) {
                    mode = i - 5;
                    while (mouse == 1) {
                        mouse = tell_mouse();
                    }
                    break;
                }
            }
        }
        ////(mode5)end
        /*���[�h���send*/




    }

    DxLib_End(); // DX���C�u�����I������
    return 0;
}


//�֐���`
int Judge_Box(int X, int Y, int x1, int y1, int x2, int y2) {
    int a = 0, b = 0;
    if (x1 > x2) {
        if (x2 <= X && X <= x1) {
            a = 1;
        }
    }
    else {
        if (x2 >= X && X >= x1) {
            a = 1;
        }
    }
    if (y1 > y2) {
        if (y2 <= Y && Y <= y1) {
            b = 1;
        }
    }
    else {
        if (y2 >= Y && Y >= y1) {
            b = 1;
        }
    }
    return a * b;
}

void Draw_Boxedge(int x1, int y1, int x2, int y2, int color, int edge) {
    DrawBox(x1, y1, x2, y1 + edge, color, 1);
    DrawBox(x1, y2 - edge, x2, y2, color, 1);
    DrawBox(x1, y1, x1 + edge, y2, color, 1);
    DrawBox(x2 - edge, y1, x2, y2, color, 1);
}

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


int Judge_Pointx(int r, int rat, int xdisp) {
    int x_ = r - midx + xdisp - xdisp * rat;
    int pointx = point * rat;
    if (x_ > 0) {
        return point * ((x_ + pointx / 2) / pointx) + midx;
    }
    else {
        return point * ((x_ - pointx / 2) / pointx) + midx;
    }
}

int Judge_Pointy(int r, int rat, int ydisp) {
    int y_ = r - midy + ydisp - ydisp * rat;
    int pointx = point * rat;
    if (y_ > 0) {
        return point * ((y_ + pointx / 2) / pointx) + midy;
    }
    else {
        return point * ((y_ - pointx / 2) / pointx) + midy;
    }
}
