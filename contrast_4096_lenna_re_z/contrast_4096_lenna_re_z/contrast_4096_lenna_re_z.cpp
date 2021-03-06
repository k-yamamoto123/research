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
WaveField g_rim_no_out(256, 256, 0.00002, 0.00002, 633e-9);//出力画像

WaveField g_in_ma_2(1024, 512, 0.00001, 0.00002, 633e-9);//入力画像
WaveField g_rim_ma_2(1024, 512, 0.00001, 0.00002, 633e-9);//シフト角スペクトル伝搬(再構成画像)（1/2mask）
WaveField p_2(1024, 512, 0.00001, 0.00002, 633e-9);//mask
WaveField g_rim_ma_2_out(512, 256, 0.00001, 0.00002, 633e-9);//出力画像

WaveField g_in_ma_4(2048, 512, 0.000005, 0.00002, 633e-9);//入力画像
WaveField g_rim_ma_4(2048, 512, 0.000005, 0.00002, 633e-9);//シフト角スペクトル伝搬(再構成画像)（3/4mask）
WaveField p_4(2048, 512, 0.000005, 0.00002, 633e-9);//mask
WaveField g_rim_ma_4_out(1024, 256, 0.000005, 0.00002, 633e-9);//出力画像

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
WaveField propCalc(WaveField in, double x, double y, int z, WaveField mask = NULL);

WaveField sumIntensity(WaveField in1, WaveField in2);

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
	//tr1:nomask,tr2:1/2mask,tr4:3/4mask
	int tr1 = 1;
	int tr2 = 1;
	int tr4 = 1;

	//周波数選択
	int n = 4;

	int loop = 5;

	for (int z = 100; z <= 100; z = z + 5) {

		//読み込み画像名
		string tr1_n = "sine_exp_512_" + itos(n);
		string tr2_n = "sine_exp_512_2_" + itos(n);
		string tr4_n = "sine_exp_512_4_" + itos(n);
		string tr8_n = "centerB16384_2048";
		string tr16_n = "centerB8192";

		//-------------設定ここまで---------------------------

		string tr1_name = tr1_n + ".bmp";
		string tr2_name = tr2_n + ".bmp";
		string tr4_name = tr4_n + ".bmp";
		string tr8_name = tr8_n + ".bmp";
		string tr16_name = tr16_n + ".bmp";

		//-------------変数名定義-------------------------

		string sdelta_x;
		string sdelta_y;
		string x_or_y = "x";
		double delta_x, delta_y;

		//------------文字列への変換-----------------
		string sz;
		sz = itos(z);
		//-------------------------------------------

		//出力画像ファイル名
		string output_filename;

		//----------------------画像読み込み--------------------------------------
		if (tr1) {
			g_in_no.Clear();
			g_rim_no.Clear();
			g_in_no.LoadBmp(tr1_name.c_str(), AMPLITUDE);
		}
		//************************************************************************
		if (tr2) {
			g_in_ma_2.Clear();
			g_rim_ma_2.Clear();
			g_in_ma_2.LoadBmp(tr2_name.c_str(), AMPLITUDE);
		}
		//************************************************************************

		if (tr4) {
			g_in_ma_4.Clear();
			g_rim_ma_4.Clear();
			g_in_ma_4.LoadBmp(tr4_name.c_str(), AMPLITUDE);
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
					g_rim_no = propCalc(g_in_no, delta_x, 0, z);
					for (int i = 1; i < loop; i++) {
						g_rim_no = sumIntensity(g_rim_no, propCalc(g_in_no, delta_x, 0, z));
					}
					//再生画像のファイル名
					output_filename = "..\\result\\nomask\\" + tr1_n + "_z" + sz + "_x" + dtos(l) + "_" + itos(loop) + ".bmp";
				}
				else if (x_or_y == "y") {
					g_rim_no = propCalc(g_in_no, 0, delta_y, z);

					//再生画像のファイル名
					output_filename = "..\\result\\nomask\\" + tr1_n + "_z" + sz + "_y" + dtos(l) + "_" + itos(loop) + ".bmp";

				}

				//正規化
				double max = 0;
				g_rim_no.Normalize();

				//切り出し
				for (int i = 0; i < g_rim_no_out.GetNx(); i++) {
					for (int j = 0; j < g_rim_no_out.GetNy(); j++) {
						g_rim_no_out.SetAmplitude(i, j, g_rim_no.GetAmplitude(i + g_rim_no.GetNx()/4, j + g_rim_no.GetNy()/4));
					}
				}

				//再生画像保存
				g_rim_no_out.SaveAsBmp(output_filename.c_str(), AMPLITUDE);
			}
			//*****************************************************************************************************************************
			if (tr2) {
				p_2.Clear();
				p_2.LoadBmp("mask2.bmp", AMPLITUDE);
				if (x_or_y == "x") {
					g_rim_ma_2 = propCalc(g_in_ma_2, delta_x, 0, z, p_2);
					for (int i = 1; i < loop; i++) {
						g_rim_ma_2 = sumIntensity(g_rim_ma_2, propCalc(g_in_ma_2, delta_x, 0, z, p_2));
					}
					//再生像のファイル名の決定
					output_filename = "..\\result\\mask2\\" + tr2_n + "_z" + sz + "_x" + dtos(l) + "_" + itos(loop) + ".bmp";

				}
				else if (x_or_y == "y") {
					g_rim_ma_2 = propCalc(g_in_ma_2, 0, delta_y, z, p_2);

					//再生像のファイル名の決定
					output_filename = "..\\result\\mask2\\" + tr2_n + "_z" + sz + "_y" + dtos(l) + "_" + itos(loop) + ".bmp";

				}

				//正規化
				g_rim_ma_2.Normalize();

				//切り出し
				for (int i = 0; i < g_rim_ma_2_out.GetNx(); i++) {
					for (int j = 0; j < g_rim_ma_2_out.GetNy(); j++) {
						g_rim_ma_2_out.SetAmplitude(i, j, g_rim_ma_2.GetAmplitude(i + g_rim_ma_2.GetNx()/4, j + g_rim_ma_2.GetNy()/4));
					}
				}

				//再生画像保存
				g_rim_ma_2_out.SaveAsBmp(output_filename.c_str(), AMPLITUDE);
			}
			//*****************************************************************************************************************************
			if (tr4) {
				p_4.Clear();
				p_4.LoadBmp("mask4.bmp", AMPLITUDE);
				if (x_or_y == "x") {
					g_rim_ma_4 = propCalc(g_in_ma_4, delta_x, 0, z, p_4);
					for (int i = 1; i < loop; i++) {
						g_rim_ma_4 = sumIntensity(g_rim_ma_4, propCalc(g_in_ma_4, delta_x, 0, z, p_4));
					}
					//再生像のファイル名の決定
					output_filename = "..\\result\\mask4\\" + tr4_n + "_z" + sz + "_x" + dtos(l) + "_" + itos(loop) + ".bmp";
				}
				else if (x_or_y == "y") {
					g_rim_ma_4 = propCalc(g_in_ma_4, 0, delta_y, z, p_4);

					//再生像のファイル名の決定
					output_filename = "..\\result\\mask4\\" + tr4_n + "_z" + sz + "_y" + dtos(l) + "_" + itos(loop) + ".bmp";
				}

				//正規化
				g_rim_ma_4.Normalize();

				//切り出し
				for (int i = 0; i < g_rim_ma_4_out.GetNx(); i++) {
					for (int j = 0; j < g_rim_ma_4_out.GetNy(); j++) {
						g_rim_ma_4_out.SetAmplitude(i, j, g_rim_ma_4.GetAmplitude(i + g_rim_ma_4.GetNx()/4, j + g_rim_ma_4.GetNy()/4));
					}
				}

				//再生画像保存
				g_rim_ma_4_out.SaveAsBmp(output_filename.c_str(), AMPLITUDE);
			}
		}
	}
}



WaveField propCalc(WaveField in, double x, double y, int z, WaveField mask) {
	WaveField ho, rim;
	double bx, by, bz;
	bx = static_cast<double>(x) * 0.001;
	by = static_cast<double>(y) * 0.001;
	bz = static_cast<double>(z) * 0.001;
	ho.Clear();
	in.ModRandomPhase(1);
	in.Embed();
	ho.CopyParam(in);
	ho.SetNx(in.GetNx());
	ho.SetNy(in.GetNy());
	ho.Init();
	in.SetOrigin(Vector(0, 0, 0));
	ho.SetOrigin(Vector(bx, by, bz));
	ho.ShiftedAsmProp(in);
	ho.ConvToConjugate();
	for (int j = 0; j < ho.GetNy(); j++) {
		for (int i = 0; i < ho.GetNx(); i++) {
			ho.SetAmplitude(i, j, 1);
		}
	}

	//maskにより遮蔽
	if (mask.GetN() > 0) {
		mask.Embed();
		if (ho.GetNx() == mask.GetNx() && ho.GetNy() == mask.GetNy()) {
			for (int i = 0; i < ho.GetNx(); i++) {
				for (int j = 0; j < ho.GetNy(); j++) {
					ho.SetAmplitude(i, j, mask.GetAmplitude(i, j));
				}
			}
		}
		else {
			cout << "error:mask size don't match image" << "\n";
			exit(-1);
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
	rim.Extract();
	return rim;
}

WaveField sumIntensity(WaveField in1, WaveField in2) {
	WaveField out;
	out.Clear();
	out.CopyParam(in1);
	out.SetNx(in1.GetNx());
	out.SetNy(in1.GetNy());
	out.Init();
	for (int j = 0; j < in1.GetNx(); j++) {
		for (int k = 0; k < in1.GetNy(); k++) {
			out.SetAmplitude(j, k, in1.GetAmplitude(j, k) + in2.GetAmplitude(j, k));
		}
	}
	return out;
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
