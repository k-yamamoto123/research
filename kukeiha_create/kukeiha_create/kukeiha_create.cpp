// kukeiha_create.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
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
WaveField g(256, 256, 0.00002, 0.00002, 633e-9);//入力画像

string itos(int n) {
	ostringstream ss;
	ss << n;
	return ss.str();
}

int main(){
	Start();
	g.Clear();
	//cross
	/*for (int i = 0; i < g.GetNx(); i++) {
		g.SetAmplitude(i, i, 1);
		g.SetAmplitude(g.GetNx() - i, i, 1);
	}
	string f = "C:\\Users\\cs13\\source\\repos\\kukeiha_create\\result\\mask_pos.bmp";
	g.SaveAsBmp(f.c_str(), AMPLITUDE);*/
	//sine_pattern
	/*for (double n = 4; n <= 128; n += 4) {
		for (int i = 0; i < g.GetNx()/2; i++) {
			val = (sin(i*PI / ((double)g.GetNx() / n / 4 ) - PI / 2) + 1) / 2;
			for (int j = 0; j < g.GetNy()/2; j++) {
				g.SetAmplitude(i+g.GetNx()/4, j+g.GetNy()/4, val);
			}

		}
		string f = "C:\\Users\\cs13\\source\\repos\\kukeiha_create\\result\\sine_exp_512_4_" + itos(n) + ".bmp";
		g.SaveAsBmp(f.c_str(), AMPLITUDE);
	}
	//rectangular_pattern
	/*for (double n = 4; n <= 128; n += 4) {
		for (int i = 128; i < g.GetNx()-128; i++) {
			if ((int)(i /(128 / n)+0.5) % 2 == 0) val = 0;
			if ((int)(i /(128 / n)+0.5) % 2 == 1) val = 1;
			for (int j = 128; j < g.GetNy()-128; j++) {
				g.SetAmplitude(i, j, val);
			}

		}
		string f = "C:\\Users\\cs13\\source\\repos\\kukeiha_create\\result\\kukei_exp_" + itos(n) + ".bmp";
		g.SaveAsBmp(f.c_str(), AMPLITUDE);
	}*/
	//ichimatsu
	int width = 32;//市松模様の大きさ
	double val = 0.5;
	for (int i = 0; i < g.GetNx(); i++) {
		for (int j = 0; j < g.GetNy(); j++) {
			if (((i / width) % 2 == 0) & ((j / width) % 2 == 0)) {
				g.SetAmplitude(i, j, val);
			}
			else if (((i / width) % 2 == 1) & ((j / width) % 2 == 1)) {
				g.SetAmplitude(i, j, val);
			}
		}
	}
	g.SaveAsBmp("C:\\Users\\cs13\\source\\repos\\picture\\\ichimatsu32.bmp", AMPLITUDE);
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
