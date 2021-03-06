// Resize.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
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

WaveField in( 128, 128, 0.00002, 0.00002, 633e-9);

string itos(int n) {
	ostringstream s;
	s << n;
	return s.str();
}

int main()
{
	Start();
	in.Clear();
	//画像ファイル名
	string name1 = "sine_exp_";
	string name2 = "_z";
	int z = 100;
	string theta = "_theta5";
	string bmp = ".bmp";
	//出力画像ファイル名
	string outName = "_r";
	
	for (int n = 8; n <= 8; n = n + 4) {
		//in.LoadBmp((name1 + itos(n) + name2 + itos(z) + theta + bmp).c_str(), AMPLITUDE);
		in.LoadBmp("mandoriru.bmp",AMPLITUDE);
		//画像切り抜き
		//画像サイズ設定
		int width = 512;
		int height = 512;
		//元画像の左下を(0,0)として切り抜きの座標移動の設定(CCDからの画像は192*16黒埋めされている)
		int dx = 384;
		int dy = 450;

		WaveField out(width, height, 0.00002, 0.00002, 633e-9);
		out.Clear();
		for (int i = 0; i < width/2; i++) {
			for (int j = 0; j < height/2; j++) {
				out.SetAmplitude(i + 128, j+ 128 , in.GetAmplitude(i + dx, j + dy));
			}
		}
		string curr = "C:\\Users\\cs13\\source\\repos\\Resize\\result\\";
		//out.SaveAsBmp((curr + name1 + itos(n) + name2 + itos(z) + theta + outName + bmp).c_str(), AMPLITUDE);
		out.SaveAsBmp((curr + "mandoriru.bmp").c_str(), AMPLITUDE);
	}
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
