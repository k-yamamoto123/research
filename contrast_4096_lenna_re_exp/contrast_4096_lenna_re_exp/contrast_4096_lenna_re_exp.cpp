// contrast_4096_lenna_64.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <math.h>
#include <iostream>
#include "wfl.h"
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <random>
#include "MT.h"

using namespace std;
using namespace wfl;

//グローバル変数として定義
//(x方向サンプリング数，ｙ方向サンプリング数，x方向サンプリング間隔，ｙ方向サンプリング間隔，波長）
//SLMのサイズは3.7umで4000x2000
WaveField g_in_no(512, 512, 0.00002, 0.00002, 633e-9);//入力画像
WaveField g_rim_no(512, 512, 0.00002, 0.00002, 633e-9);//シフト角スペクトル伝搬(再構成画像)（nomask）

WaveField g_in_ma_2(4096, 2048, 0.00001, 0.000004, 633e-9);//入力画像
WaveField g_rim_ma_2(4096, 2048, 0.000002, 0.000004, 633e-9);//シフト角スペクトル伝搬(再構成画像)（1/2mask）
WaveField p_2(4096, 2048, 0.000002, 0.000004, 633e-9);//mask

WaveField g_in_ma_4(8192, 2048, 0.000001, 0.000004, 633e-9);//入力画像
WaveField g_rim_ma_4(8192, 2048, 0.000001, 0.000004, 633e-9);//シフト角スペクトル伝搬(再構成画像)（3/4mask）
WaveField p_4(8192, 2048, 0.000001, 0.000004, 633e-9);//mask


//-------------関数定義-------------------------
//int→string
string itos(int i) {
	ostringstream s;
	s << i;
	return s.str();
}


//double→string
string dtos(double i) {
	ostringstream s;
	s << i;
	return s.str();
}

//伝搬計算。入力画像inをx[mm]シフト、z[mm]伝搬した再生像分布を返す。
WaveField propCalc(WaveField in, double x, double y, int z, string name = "");

//-----------int main-----------------------
int main(void) {
	Start();

	//---------------設定------------------------
		//シフト
		//開始
	double start = 0;
	//終了
	double end = 0;

	//シミュレーション切り替え（0:実行しない、1;実行）
	//tr1:nomask,tr2:1/2mask,tr4:3/4mask,tr8:7/8mask,tr16:15/16mask,sine:sine pattern
	int tr1 = 1;
	int tr2 = 0;
	int tr4 = 0;

	//伝搬距離の設定
	int z = 100;


	//再生画像（0:保存しない、1:保存）
	int image = 0;

	//読み込み画像名
	string tr1_n = "centerB256mini";

	//-------------設定ここまで---------------------------

	string bmp = ".bmp";
	string tr1_name = tr1_n + bmp;
	//-------------変数名定義-------------------------	string sdelta_x;
	string sdelta_y;
	string x_or_y = "x";
	double delta_x, delta_y;                       //mm単位

	//------------文字列への変換-----------------
	string sz;
	sz = itos(z);
	//-------------------------------------------

	//出力画像ファイル名
	string output_filename;

	for (int n = 4; n <= 4; n += 4) {
		//----------------------画像読み込み--------------------------------------
		if (tr1) {
			g_in_no.Clear();
			g_rim_no.Clear();
			g_in_no.LoadBmp((tr1_n /*+ itos(n)*/ + bmp).c_str(), AMPLITUDE);
		}
		//************************************************************************


		//---------------------シフト,再生像生成-----------------------------

		for (double l = start; l <= end; l += 0.1) {
			if (x_or_y == "x") {
				delta_x = z * tan(l*PI / 180);
			}
			else if (x_or_y == "y") {
				delta_y = z * tan(l*PI / 180);
			}

			std::cout << "*- - - - - - - - - - - - - - - - - - - - - - -*" << '\n';
			std::cout << "k=" << l << '\n';

			//************************************************************************
			if (tr1) {
				if (x_or_y == "x") {
					g_rim_no = propCalc(g_in_no, delta_x, 0, z, tr1_n /*+ itos(n)*/);
					//再生画像のファイル名の決定
					output_filename = "C:\\Users\\cs13\\source\\repos\\contrast_4096_lenna_re_exp\\result\\" + tr1_n + "_z" + sz + "_x" + dtos(l) + ".bmp";
				}
				else if (x_or_y == "y") {
					g_rim_no = propCalc(g_in_no, 0, delta_y, z, tr1_n /*+ itos(n)*/);
					//再生画像のファイル名の決定
					output_filename = "C:\\Users\\cs13\\source\\repos\\contrast_4096_lenna_re_exp\\result\\" + tr1_n + "_z" + sz + "_y" + dtos(l) + ".bmp";
				}
				
				//正規化
				g_rim_no.Normalize();

				//再生画像保存
				if (image)
					g_rim_no.SaveAsBmp(output_filename.c_str(), AMPLITUDE);
			}
		}
	}
}


WaveField propCalc(WaveField in, double x, double y, int z, string name) {
	WaveField ho, rim;
	double bx, by, bz;
	bx = static_cast<double>(x) * 0.001;
	by = static_cast<double>(y) * 0.001;
	bz = static_cast<double>(z) * 0.001;
	ho.Clear();
	in.ModRandomPhase(1);
	ho.CopyParam(in);
	ho.SetNx(in.GetNx());
	ho.SetNy(in.GetNy());
	ho.Init();
	in.SetOrigin(Vector(0, 0, 0));
	ho.SetOrigin(Vector(-bx, -by, bz));
	ho.ShiftedFresnelProp(in);
	ho.ConvToConjugate();
	if (name.length() != 0) {
		string sz = itos(z);
		string file_name = "C:\\Users\\cs13\\source\\repos\\contrast_4096_lenna_re_exp\\result\\" + name + "_z" + sz + "_x" + dtos(x) + "_phase" + ".bmp";
		double phase;
		for (int i = 0; i < ho.GetNx(); i++) {
			for (int j = 0; j < ho.GetNy(); j++) {
				phase = ho.GetPhase(i, j);
				if (phase < 0) {
					ho.SetPhase(i, j, -PI + 2 * PI * 201 / 256 + phase * 201 / 256);
				}
				else if (phase >= 0) {
					ho.SetPhase(i, j, -PI + phase * 201 / 256);
				}
			}
		}
		ho.SaveAsBmp(file_name.c_str(), PHASE);
	}
	for (int j = 0; j < ho.GetNy(); j++) {
		for (int i = 0; i < ho.GetNx(); i++) {
			ho.SetAmplitude(i, j, 1);
		}
	}

	//再生像
	rim.Clear();
	rim.CopyParam(ho);
	rim.SetNx(ho.GetNx());
	rim.SetNy(ho.GetNy());
	rim.Init();
	rim.SetOrigin(Vector(0, 0, 2 * bz));
	rim.ShiftedAsmProp(ho);
	return rim;
}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
