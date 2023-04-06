#include "DxLib.h"
#include "time.h"
#include "stdlib.h"
#define LX 1300//スクリーンサイズ（横）
#define LY 720//スクリーンサイズ（縦）
#define capa 10000//線データの容量
#define point 2//点の取る間隔
#define gia 30//拡大段階
#define size_roll 1//ロールの判定幅
#define select 5//選択画面の選択肢
#define select_x1 200//選択画面ボタンボックス初めx座標
#define select_y1 100*LY/480//選択画面ボタンボックス初めy座標
#define select_val 10*LY/480//選択画面の間隔
#define size 50*LY/480//文字サイズ
#define lsize 1//線の太さ（半分）
#define midx LX/2//中心のx座標
#define midy (LY+40)/2//中心のy座標
#define wait_time 10//選択ボタンクリック後誤操作防止待ち時間

int Judge_Box(int X, int Y, int x1, int y1, int x2, int y2);//括弧内なら1括弧外なら0
void Draw_Boxedge(int x1, int y1, int x2, int y2, int color, int edge);//枠型
int tell_mouse();//マウスの押されている位置を返す
int Judge_Pointx(int r, int rat, int xdisp);//点を取る際に、格子点を取るように近くの点を探す（x座標）
int Judge_Pointy(int r, int rat, int ydisp);//点を取る際に、格子点を取るように近くの点を探す（y座標）

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE), SetGraphMode(LX, LY, 16), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK), SetWindowSizeChangeEnableFlag(TRUE); //ウィンドウモード変更と初期化と裏画面設定
    int X, Y;//マウスの位置
    int mouse;//マウスの入力 0=none,1=left,2=right,3=middle
    int roll = 0;//マウスのホイール回転数
    int tate[capa][4], yoko[capa][4];//線の座標 (x1,y1,y2,0)、(x1,y1,x2,0)、最後のセルは表示状態かどうか
    int tnow_capa = 0, ynow_capa = 0, tnows = 0, ynows = 0;//線データについて次に書き込むセル番号
    int i, j, k,save[2];//計算用変数
    int rate = 5;//画面の拡大度
    int mode = 0;//作業モード変数、編集から終了まで、マイナスのは選択された時のモード
    int FontHandle = CreateFontToHandle(NULL, size, 3);//表示する文字のフォント
    unsigned int col[5] = { GetColor(255, 255, 255),GetColor(255, 255, 0),GetColor(0,255,0) ,GetColor(100,100,100),GetColor(100, 100, 0) };//色の変数、0=none(白),1=select(黄),2=selected(緑)
    char* selm0[select] = { "　　 　編　集","　 　　ロード","　 　　セーブ","　  ランダム生成","　 　　終　了" };//スタート画面の選択肢
    char* fpmessage[5] = { "","　読み取りに失敗","　読み取りに成功","　書き込みに失敗","　書き込みに成功" };//ファイル操作のメッセージ
    int fpm = 0;//ファイル操作のメッセージセル番号
    char* selm1[8] = { "確定","戻る","消す","復旧","よこ","たて","原点","終了"};//編集画面の選択肢
    char* selm2[2] = { "  本当にロードする","　 　　戻　る" };//ロード画面の選択肢
    char* selm3[2] = { "  本当にセーブする","　 　　戻　る" };//セーブ画面の選択肢
    char* selm4[2] = { " 　本当に生成する","　 　　戻　る" };//ランダム生成画面の選択肢
    char* selm5[2] = { " 　本当に終了する","　 　　戻　る" };//終了画面の選択肢
    int ch1 = 0;//選択されたボタン番号 0=none,1=確定,2=戻る,3=消す,4=復旧,5=よこ,6=たて,7=原点,8=終了
    int Wait = wait_time;//編集で選択後少し待つ（誤って二重に操作するのを防ぐ）
    int back = 0;//back=1の時、編集で操作を戻す
    int k1[10];//編集で選択ボタンについて選択されていれば1そうでなければ0;
    int Kt[capa], Ky[capa];//線の選択の是非（マウスが線の上にあるかどうか）
    int chback;//セーブや終了でボタンが出るまで待つ（セーブや終了を行うとデータが消えてしまう恐れがあるため）
    int xm1 = INT_MAX, ym1 = INT_MAX, xm2 = INT_MAX, ym2 = INT_MAX;//線表示前の点の座標
    int tyjud;//縦横でどちらのラインを書くか3=none,4=yoko,5=tate
    int xydisp[2] = { 0 };//原点からどれだけ座標移動したか
    int xysave[2] = { INT_MAX,INT_MAX };//右クリック位置の保存
    int xysavemid[2] = {INT_MAX,INT_MAX};//中央クリック位置の保存
    int xydispsv[2] = { 0 };//前回のxydispの値を保存
    int midstate=0;//マウスのミドルクリックが押されて偶数回目か奇数回目か
    int conti = 0;//クリックを外すまでループを抜けさせないことで誤操作を防ぐ
    char read_buf[100];//読み取り用の配列
    time_t tim;//タイム関数変数
    unsigned int random_num = 100;//乱数作成数
    unsigned int random_max = 100;//乱数の上限（絶対値）
    int random[3];//乱数
    FILE* fp;//保存読み込みでのファイル変数

    /*乱数データ読み込みinit*/
    //ファイルの初めの二行を読んで、一行目を乱数の最大値、二行目を乱数の個数とする
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
    /*乱数データ読み込みend*/


    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        if (tnows < tnow_capa)tnows = tnow_capa;//tnowsにtnow_capaの最大値を代入
        if (ynows < ynow_capa)ynows = ynow_capa;//ynowsにynow_capaの最大値を代入

        /*入力init*/
        //マウス操作を変数に代入
        GetMousePoint(&X, &Y);//マウスの位置代入

        mouse = tell_mouse();//マウスの入力

        if (mode == 1) {
            //マウスのホイール入力init
            roll += GetMouseWheelRotVol();
            if (roll < 0) {
                roll = 0;
            }
            else if (roll > gia * size_roll) {
                roll = gia * size_roll;
            }
            rate = roll / size_roll + 5;
            //マウスのホイール入力end

            //マウスのミドルクリック変位init
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
            //マウスのミドルクリック変位end

            //マウスの右クリック変位init
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
            //マウスの右クリック変位end
        }
        
        /*入力end*/


        /*モード実行init*/
        //mode=0初期状態、mode=1編集、mode=±2ロード、mode=±3セーブ、mode=±4ランダム生成、mode=±5終了

        ////(mode-5)init(終了)
        if (mode == -5) {
            break;
        }
        ////(mode-5)end

        ////(mode-4)init（ランダム生成）
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


        ////(mode-3)init（セーブ）
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

        ////(mode-2)init（ロード）
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

        

        ////(mode0)init（初期状態）
        if (mode == 0) {
            DrawStringToHandle(100, 30, "モード選択", GetColor(0, 255, 255), FontHandle);
            //ボタンinit
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
            //ボタンend
            DrawStringToHandle(select_x1, select_y1 + select_val * 5 + size * 5, fpmessage[fpm], col[0], FontHandle);
        }
        ////(mode0)end

        ////(mode1)init（編集）
        if (mode == 1) {
            if (ch1 == 4) {
                for (i = 0; i < tnows; i++) {
                    if (tate[i][3] == 0) {//表示状態の時
                        if ((tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate > 0 && (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX) {//画面サイズからの出力制限
                            if ((tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY && (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate >0) {//画面サイズからの出力制限

                                if (Y > size + 10 && Judge_Box(X, Y, (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate, (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                    Kt[i] = 4;//選択していたら
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
                    if (yoko[i][3] == 0) {//表示状態の時
                        if ((yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate > 0&& (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY) {//画面サイズからの出力制限
                            if ((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX && (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate>0) {//画面サイズからの出力制限

                                if (Y > size + 10 && Judge_Box(X, Y, (yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                    Ky[i] = 4;//選択していたら

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
            //線の画面出力init
            for (i = 0; i < tnows; i++) {
                if (tate[i][3] == 1) {//表示状態の時
                    if ((tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate > 0 && (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX) {//画面サイズからの出力制限
                        if ((tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY && (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate >0) {//画面サイズからの出力制限

                            if (Y > size + 10 && Judge_Box(X, Y, (tate[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (tate[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (tate[i][0] + xydisp[0] - midx) * rate + midx + lsize * rate, (tate[i][2] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                Kt[i] = 1;//選択していたら
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
                if (yoko[i][3] == 1) {//表示状態の時
                    if ((yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate > 0 && (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate < LY) {//画面サイズからの出力制限
                        if ((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate < LX && (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate>0) {//画面サイズからの出力制限

                            if (Y > size + 10 && Judge_Box(X, Y, (yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate)) {
                                Ky[i] = 1;//選択していたら

                            }
                            else {
                                Ky[i] = 0;
                            }

                            DrawBox((yoko[i][0] + xydisp[0] - midx) * rate + midx - lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy - lsize * rate, (yoko[i][2] + xydisp[0] - midx) * rate + midx + lsize * rate, (yoko[i][1] + xydisp[1] - midy) * rate + midy + lsize * rate, col[Ky[i]], 1);
                        }
                    }
                }

            }
            
        
            DrawCircle((xm1 + xydisp[0] - midx) * rate + midx, (ym1 + xydisp[1] - midy) * rate + midy, lsize * rate * 2, col[0], 1, 1);//第1点
            DrawCircle((xm2 + xydisp[0] - midx) * rate + midx, (ym2 + xydisp[1] - midy) * rate + midy, lsize * rate * 2, col[0], 1, 1);//第2点
            if(midstate)DrawCircle(xysavemid[0], xysavemid[1], 5, col[2], 1, 1);//移動中心点
            //線の画面出力end

            DrawBox(0, 0, LX, size + 10, 0, 1); //ボタン上線削除

            

            //ボタンinit

            for (i = 0; i < 8; i++) {
                k1[i] = Judge_Box(X, Y, 5 + i * size * 2 + 10 * i, 5, 5 + size * 2 * (i + 1) + 10 * i, 5 + size);
                if (i != ch1 - 1) {//クリックされているかの判定
                    DrawStringToHandle(5 + i * size * 2 + 10 * i, 5, selm1[i], col[k1[i]], FontHandle);
                    Draw_Boxedge(5 + i * size * 2 + 10 * i, 5, 5 + size * 2 * (i + 1) + 10 * i, 5 + size, col[k1[i]], 2);
                }
                else {//クリックされている時
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
                            if (i == 2 && (tnow_capa + ynow_capa) == 0) {//消すものが無かったら消去モードに入らない
                                ch1 = 0;
                            }
                            break;
                        }
                    }
                }
            }
            //ボタンend
            
            //一点目と二点目を区切るinit
            while (conti == 1 && mouse == 1) {
                mouse = tell_mouse();
            }
            conti = 0;
            //一点目と二点目を区切るinit
            
            //ボタンの連続判定無効init
            if (Wait) {
                Wait *= 0.9;
            }
            //ボタンの連続判定無効end
            else {
                //第一点init
                if (ch1 == 0 || ch1 == 5 || ch1 == 6) {
                    if (mouse == 1 && xm1 == INT_MAX) {
                        if (Y > size + 10) {
                            save[0] = Judge_Pointx(X - xydisp[0], rate, xydisp[0]);
                            save[1] = Judge_Pointy(Y - xydisp[1], rate, xydisp[1]);
                            if (save[0] > INT_MAX / gia && save[0] < INT_MIN / gia)continue;
                            if (save[0] > INT_MAX / gia && save[0] < INT_MIN / gia)continue;
                            xm1 = save[0];
                            ym1 = save[1];
                            DrawCircle((xm1 + xydisp[0] - midx) * rate + midx, (ym1 + xydisp[1] - midy) * rate + midy, lsize * rate * 2, col[0], 1, 1);//第1点
                            conti = 1;
                            continue;
                        }
                    }
                
                }
                //第一点end

                //確定init
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
                //確定end


                //戻るinit
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
                //戻るend

                //消去init
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
                //消去end

                //復旧init
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
                //復旧end

                //よこinit
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
                //よこend

                //たてinit
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
                //たてend

                //原点int
                if (ch1 == 7) {
                    xydisp[0] = 0;
                    xydisp[1] = 0;
                    ch1 = 0;
                    Wait = wait_time;
                }
                //原点end

                
            }


        }
        ////(mode1)end

        ////(mode2)init
        if (mode == 2) {
            DrawStringToHandle(100, 30, "現在の編集は削除されます", GetColor(255, 0, 0), FontHandle);
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
            DrawStringToHandle(100, 30, "前回のデータは削除されます", GetColor(255, 0, 0), FontHandle);
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
            DrawStringToHandle(100, 30, "前回のデータは削除されます", GetColor(255, 0, 0), FontHandle);
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
            DrawStringToHandle(100, 30, "お疲れ様でした", GetColor(0, 255, 0), FontHandle);
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
        /*モード実行end*/




    }

    DxLib_End(); // DXライブラリ終了処理
    return 0;
}


//関数定義
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
