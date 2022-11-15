//доделать: создать несолько карт, создать историю, контроль правильности карты
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <Windows.h>
#include <cstdio>
#include <iomanip>
#include <algorithm>
#include <random>
#include <vector>
#include <iterator>
#include <cctype>

using namespace std;

class thingPlace {
	private:
		int i;//координата х в массиве карты
		int j;//координата у в массиве карты
	public:
		thingPlace():i(0),j(0)
		{}
		int & get_i() {
			return i;
		}
		int & get_j() {
			return j;
		}
};

class thingScore {
	private:
		int light;  //степень освещения
		int fortune;//оличество жизней, удачи
		int jimmy;  //количество отмычек
		int money;  //сумма денег

		int priceL;
		int priceF;
		int priceJ;
	public:
		thingScore():light(1), fortune(0), jimmy(0), money(0), priceL(30), priceF(20), priceJ(10)
		{}
		int & get_light() {
			return light;
		}
		int & get_fortune() {
			return fortune;
		}
		int & get_jimmy() {
			return jimmy;
		}
		int & get_money() {
			return money;
		}
		int & get_pticeL() {
			return priceL;
		}
		int & get_priceF() {
			return priceF;
		}
		int & get_priceJ() {
			return priceJ;
		}
		bool lightBy() {
			if(money>=priceL){light++; money-=priceL; return true;}
			else return false;
		}
		bool fortuneBy() {
			if(money>=priceF){fortune++; money-=priceF; return true;}
			else return false;
		}
		bool jimmyBy() {
			if(money>=priceJ){jimmy++; money-=priceJ; return true;}
			else return false;
		}
		bool lightSell() {
			if(light>1){light--; money+=(priceL/2); return true;}
			else return false;
		}
		bool fortuneSell() {
			if(fortune>0){fortune--; money+=(priceF/2); return true;}
			else return false;
		}
		bool jimmySell() {
			if(jimmy>0){jimmy--; money+=(priceJ/2); return true; }
			else return false;
		}
};

//картинка при входе в игру
//void PictureWelcom();
//сама игра
int Game(vector<vector<char>> map, vector<vector<char>> mapS, thingScore & things, int level);
//вывод меню
char Menu();
//вывод сюжета, истории
void StartStory();
void StoryLevel0();
void StoryLevel1();
void StoryLevel2();
void StoryLevel3();
void StoryLevel4();
//вывод существующих уровней
char Levels();
//вывод подсказок
void Question();
//магазин
void Score(thingScore & things);
//картинка когда попал в пропасть
void PictureGulf();
//когда ищейка нашла его
void PictureArest();
//картинка когда дошел до человека
void PicturePerson();
//вывод конца игры
void WinStory();

int main()
{
////////////////////как-то делаем максимальный размр окна////////////////////////
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD maxWindow = GetLargestConsoleWindowSize(out_handle); // размер самого большого возможного консольного окна
	SMALL_RECT srctWindow = { 0, 0, maxWindow.X - 1, maxWindow.Y - 1 };
	SMALL_RECT minWindow = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(out_handle, true, &minWindow);
	SetConsoleScreenBufferSize(out_handle, maxWindow);
	SetConsoleWindowInfo(out_handle, true, &srctWindow);

///////////////////американская раскладка, т.к. в русской некоторые символ не поддерживаются//////////////////////
	SetConsoleCP(437);
	SetConsoleOutputCP(437);
//	cout<<"\n\n\n\t\tWelcom to the Labirinth!"<<endl;
//	Sleep(2000);

	const int LEVELS = 5;
	int level(0);

	//наша графика, основные символы, которые и составляют всю графику нашей карты
	const char h = (char) 38;//герой
	const char d = (char) 35;//дверь, решетка
	const char w = (char)219;//стена
	const char m = (char) 36;//деньги
	const char k = (char)235;//ключ, отмычка
	const char p = (char)225;//люди, человек
	const char o = (char) 32;//пространство, пробел
	const char s = (char)176;//камень
	const char g = (char)233;//пропасть, дыра
	const char t = (char) 64;//телепорт
	const char q = (char) 63;//вопрос
	const char b = (char)157;//ищейка
	const char f = (char)234;//удача, жизни

	
	//здесь хранится весь переводимый шмот, что нам может понадобиться в игре и в магазине
	thingScore things;

	//////////////основная и секретная карты нулевого(первого) уровня/////////////////
	char mapG0[12][18] = {
		{ w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w },
		{ w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w },
		{ w,w,o,o,o,o,o,o,w,o,o,o,o,o,o,o,w,w },
		{ w,w,k,o,o,w,o,w,w,o,w,w,w,w,w,o,w,w },
		{ w,w,w,w,w,w,o,w,o,o,w,o,o,o,s,o,w,w },
		{ w,w,h,o,o,o,o,d,o,w,w,o,o,o,s,o,w,w },
		{ w,w,w,w,w,w,o,w,w,w,k,o,o,o,o,o,w,w },
		{ w,w,w,w,w,w,o,w,o,o,o,o,o,o,o,o,w,w },
		{ w,w,t,w,w,w,o,w,o,o,o,o,o,w,o,o,w,w },
		{ w,w,o,o,o,o,o,w,d,o,o,o,o,o,o,o,w,w },
		{ w,w,w,w,w,w,w,w,p,w,w,w,w,w,w,w,w,w },
		{ w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w }};
	char mapS0[11][19] = {
		{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
		{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
		{w,w,o,o,o,w,m,o,w,o,o,o,w,o,o,o,o,w,w},
		{w,w,o,o,o,w,g,o,o,o,w,o,w,m,o,w,o,w,w},
		{w,w,w,w,o,w,w,w,w,w,w,o,w,w,w,w,o,w,w},
		{w,w,t,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w},
		{w,w,w,w,o,w,w,w,w,w,w,o,w,w,w,w,o,w,w},
		{w,w,o,o,o,w,o,o,o,o,w,o,w,o,o,w,o,w,w},
		{w,w,o,o,o,w,k,o,w,o,o,o,w,o,o,o,o,w,w},
		{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
		{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w}};
	char mapG1[21][26] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,o,o,o,w,m,w,m,w,g,w,g,w,m,g,g,o,g,s,g,s,o,w,w},
	{w,w,w,o,w,o,w,g,s,g,s,g,s,o,w,m,w,s,w,s,w,s,w,g,w,w},
	{w,w,w,d,w,o,w,o,o,o,o,o,o,s,w,g,g,o,o,o,o,g,s,o,w,w},
	{w,w,w,m,w,o,w,o,o,o,o,o,o,o,w,g,o,o,w,w,w,w,w,o,w,w},
	{w,w,w,w,w,o,w,o,o,o,o,o,o,o,w,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,o,w,w,w,w,w,o,w,w,w,o,w,w,w,o,o,o,o,o,w,w},
	{w,w,h,o,o,o,o,o,w,w,w,o,w,w,o,o,o,o,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,o,w,o,o,o,o,o,o,w,k,o,w,o,o,o,o,o,o,o,w,w},
	{w,w,w,t,w,o,w,w,w,w,w,w,o,w,w,w,w,o,w,w,w,w,w,o,w,w},
	{w,w,o,o,w,o,s,o,g,o,o,w,o,o,o,o,o,o,w,o,o,m,w,o,w,w},
	{w,w,o,w,w,o,o,s,o,s,g,w,o,w,w,w,o,w,w,g,o,g,w,o,w,w},
	{w,w,g,o,w,w,w,w,w,w,o,w,o,o,o,w,o,o,o,g,w,s,w,w,w,w},
	{w,w,o,s,g,g,s,o,o,w,o,w,w,w,o,w,o,w,o,s,s,m,g,w,w,w},
	{w,w,g,o,g,w,w,s,o,w,o,w,m,w,o,w,o,w,o,o,w,o,w,w,w,w},
	{w,w,o,s,o,w,o,o,o,w,o,o,o,w,o,w,o,w,w,w,w,w,w,p,w,w},
	{w,w,o,o,o,o,o,o,o,w,w,w,w,w,o,w,o,o,o,s,m,o,o,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,w,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w}, };
	char mapS1[8][13] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,o,s,m,g,k,s,m,g,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,o,w,w},
	{w,w,o,o,o,o,t,o,o,o,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w}, };
	char mapG2[18][21] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,g,o,w,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,p,w,o,w,o,w,w,w,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,g,w,o,w,o,g,f,g,o,o,o,o,o,g,t,w,o,w,w},
	{w,w,g,w,o,w,o,g,f,g,g,s,o,o,o,w,w,w,o,w,w},
	{w,w,h,o,o,w,o,g,g,g,o,o,o,o,o,w,o,o,o,w,w},
	{w,w,o,w,o,g,o,o,o,o,o,o,o,o,o,w,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,w,g,g,w,o,w,o,w,o,w,w},
	{w,w,o,w,g,o,o,o,g,f,w,f,f,w,o,o,o,w,o,w,w},
	{w,w,o,w,o,o,o,o,g,f,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,o,w,o,o,o,o,g,f,w,o,o,o,o,g,f,w,o,w,w},
	{w,w,o,w,o,w,w,w,w,w,w,o,g,g,g,o,f,w,o,w,w},
	{w,w,o,w,o,o,o,o,o,o,w,o,o,o,g,g,f,w,o,w,w},
	{w,w,o,w,w,w,w,w,o,o,w,w,w,o,w,w,w,w,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w} };
	char mapS2[10][14] = {
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,o,o,o,o,o,o,g,o,o,o,w,w},
	{w,w,o,g,g,o,g,o,o,o,g,o,w,w},
	{w,w,o,g,o,o,g,g,g,o,g,o,w,w},
	{w,w,o,g,o,f,g,t,g,f,g,o,w,w},
	{w,w,o,g,g,g,g,o,g,g,g,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w} };
	char mapG3[26][31] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w,w,w,w,w,w,w,w,w,w,o,o,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,w,w,w,w,o,w,o,o,o,o,o,o,o,o,o,o,o,w,o,w,w},
	{w,w,o,w,o,o,o,o,o,o,o,o,o,w,o,w,o,w,w,w,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,o,w,o,w,w,w,w,w,w,w,o,w,o,w,o,w,o,o,o,o,o,o,o,o,o,w,o,w,w},
	{w,w,o,w,o,w,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w,w,w,w,w,w,o,w,o,w,w},
	{w,w,o,w,o,w,o,w,w,w,w,w,w,w,w,w,w,w,w,w,o,g,o,g,o,o,o,w,o,w,w},
	{w,w,o,w,o,w,o,w,h,k,k,k,d,o,d,o,d,o,o,o,o,o,o,o,o,o,o,w,o,w,w},
	{w,w,o,w,o,w,o,w,w,w,w,w,w,w,w,w,w,w,s,o,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,o,w,o,w,o,o,o,o,o,o,o,o,o,o,o,w,o,o,w,g,o,o,o,w,o,o,o,w,w},
	{w,w,o,w,o,w,w,w,w,w,w,w,w,w,w,w,o,w,w,w,w,o,o,w,o,w,o,w,w,w,w},
	{w,w,o,w,o,o,o,o,o,o,o,w,o,o,o,w,o,o,o,o,w,o,o,w,o,w,o,o,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,o,w,o,w,o,o,o,w,w,o,w,w,o,w,o,w,o,w,o,w,w},
	{w,w,o,o,o,o,w,w,w,w,o,o,o,w,w,w,w,o,w,o,o,w,o,w,o,o,o,w,o,w,w},
	{w,w,o,w,o,w,w,w,w,w,w,w,w,w,o,g,w,o,w,o,w,w,o,w,w,w,w,w,o,w,w},
	{w,w,o,w,o,w,t,w,w,o,o,o,w,o,o,w,w,o,w,o,o,o,o,w,o,o,o,w,o,w,w},
	{w,w,o,w,o,o,o,o,o,o,w,o,o,o,o,w,p,o,w,w,w,w,w,o,o,o,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,w,w,w,w,o,w,w,o,o,o,o,o,w,o,o,s,o,w,o,w,w},
	{w,w,o,o,o,o,o,o,w,o,o,o,o,o,o,o,o,w,w,w,w,o,w,o,o,o,o,w,o,w,w},
	{w,w,w,w,w,w,w,o,w,o,w,w,w,w,w,w,w,w,o,o,w,o,w,o,w,w,w,w,o,w,w},
	{w,w,o,o,o,o,o,o,w,o,o,w,o,o,o,o,o,w,o,o,w,o,w,o,o,o,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,o,w,w,o,w,w,w,w,w,o,w,o,w,g,o,w,o,w,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,o,o,o,o,w,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w} };
	char mapS3[11][19] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,f,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,w,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,t,w,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,m,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w} };
	char mapG4[22][27] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,h,w,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,g,o,o,o,w,w},
	{w,w,o,w,o,w,w,w,w,w,w,w,w,w,w,o,w,w,w,o,o,o,o,g,o,w,w},
	{w,w,o,o,o,w,f,g,g,w,w,w,w,w,w,o,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,o,w,w,w,w,o,o,o,o,o,o,o,o,m,w,o,o,o,o,o,o,w,o,w,w},
	{w,w,o,w,o,o,o,s,o,w,w,w,w,w,w,w,w,o,w,w,w,w,o,w,o,w,w},
	{w,w,o,w,o,w,o,o,o,w,o,o,o,o,o,o,w,o,o,o,o,w,o,w,o,w,w},
	{w,w,o,w,o,w,w,w,w,w,o,w,w,w,w,o,w,w,w,w,o,w,o,w,o,w,w},
	{w,w,o,w,o,o,o,o,o,w,o,o,w,o,o,o,w,g,m,w,o,w,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,o,w,w,o,w,g,s,o,o,s,o,w,o,w,o,w,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,w,o,w,m,w,o,o,o,w,w,o,w,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,w,o,w,o,w,w,w,w,w,w,w,w,o,w,o,w,o,w,w},
	{w,w,o,w,g,o,o,o,w,m,o,o,w,o,o,o,o,o,o,o,o,w,o,w,o,w,w},
	{w,w,o,w,o,o,s,o,w,w,w,w,w,o,w,w,w,w,w,w,w,w,o,w,o,w,w},
	{w,w,o,w,o,o,g,o,o,m,w,t,o,o,o,o,o,o,w,o,o,o,o,w,o,w,w},
	{w,w,o,w,w,w,w,w,w,o,w,w,w,w,w,w,w,o,w,o,w,w,w,w,o,w,w},
	{w,w,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w,w,w,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,o,w,w},
	{w,w,p,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w} };
	char mapS4[11][14] = { 
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,o,o,o,o,w,o,o,g,o,o,w,w},
	{w,w,o,o,w,o,w,o,o,g,g,o,w,w},
	{w,w,w,w,w,o,w,o,o,g,g,f,w,w},
	{w,w,t,o,o,o,o,o,s,g,g,f,w,w},
	{w,w,w,w,w,o,w,o,o,g,g,o,w,w},
	{w,w,o,o,w,o,w,o,o,g,g,o,w,w},
	{w,w,o,o,o,o,w,o,o,g,o,o,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w},
	{w,w,w,w,w,w,w,w,w,w,w,w,w,w} };
	//основная и секретная карты, которым мы присвоим основную и секретную карты соответствующего уровня, а потом передадим в функцию игры
	vector<vector<char>> mapG;
	vector<vector<char>> mapS;
	//вектор для помощи создания этих карт
	vector<char> mapString;

	//переменная выбора пункта меню
	char menuChoise(' ');
	while(menuChoise!='0'){
		system("color 0c");
		menuChoise = Menu();
		system("cls");
		cout << sizeof(bool) << endl;
		switch (menuChoise) {
			case '1':
				level=0;
				StartStory();
				break;
			case '2':
				switch (Levels()) {
					case '1':
						level=0;
						break;
					case '2':
						level=1;
						break;
					case '3':
						level=2;
						break;
					case '4':
						level=3;
						break;
					case '5':
						level=4;
						break;
					default:
						level=LEVELS;
						break;
				}
				break;
			case '3':
				system("cls");
				Question();
				system("pause");
				system("cls");
				level=LEVELS;
				break;
			case '0':
			level=LEVELS;
				break;
			default:
				cout<<"You are has been pressed wrong button"<<endl;
				level=LEVELS;
				break;
		}
		while(level < LEVELS){
			//////////////создание карты уровня////////////////
			switch(level){
				//////////////создание карты нулевого(первого) уровня////////////////
				case 0:
					for(int i(0);i<mapG.size();i++)
						mapG[i].clear();
					mapG.clear();
					mapString.reserve(sizeof(mapG0[0]));
					mapG.reserve(sizeof(mapG0));
					for (int i(0);i<sizeof(mapG0) / sizeof(mapG0[0]);i++) {
						for (int j(0);j<sizeof(mapG0[0]);j++) {
							mapString.push_back(mapG0[i][j]);
						}
						mapG.push_back(mapString);
						mapString.clear();
					}
					for (int i(0);i<mapS.size();i++)
						mapS[i].clear();
					mapS.clear();

					mapString.reserve(sizeof(mapS0[0]));
					mapS.reserve(sizeof(mapS0));
					for (int i(0);i<sizeof(mapS0) / sizeof(mapS0[0]);i++) {
						for (int j(0);j<sizeof(mapS0[i]);j++) {
							mapString.push_back(mapS0[i][j]);
						}
						mapS.push_back(mapString);
						mapString.clear();
					}
					StoryLevel0();
					break;
				//////////////создание карты первого(второго) уровня////////////////
				case 1:
					for (int i(0);i<mapG.size();i++)
						mapG[i].clear();
					mapG.clear();
					mapString.reserve(sizeof(mapG1[0]));
					mapG.reserve(sizeof(mapG1));
					for (int i(0);i<sizeof(mapG1) / sizeof(mapG1[0]);i++) {
						for (int j(0);j<sizeof(mapG1[0]);j++) {
							mapString.push_back(mapG1[i][j]);
						}
						mapG.push_back(mapString);
						mapString.clear();
					}
					for (int i(0);i<mapS.size();i++)
						mapS[i].clear();
					mapS.clear();

					mapString.reserve(sizeof(mapS1[0]));
					mapS.reserve(sizeof(mapS1));
					for (int i(0);i<sizeof(mapS1) / sizeof(mapS1[0]);i++) {
						for (int j(0);j<sizeof(mapS1[i]);j++) {
							mapString.push_back(mapS1[i][j]);
						}
						mapS.push_back(mapString);
						mapString.clear();
					}
					void StoryLevel1();
					break;
				//////////////создание карты второго(третьего) уровня////////////////
				case 2:
					for (int i(0);i<mapG.size();i++)
						mapG[i].clear();
					mapG.clear();
					mapString.reserve(sizeof(mapG2[0]));
					mapG.reserve(sizeof(mapG2));
					for (int i(0);i<sizeof(mapG2) / sizeof(mapG2[0]);i++) {
						for (int j(0);j<sizeof(mapG2[0]);j++) {
							mapString.push_back(mapG2[i][j]);
						}
						mapG.push_back(mapString);
						mapString.clear();
					}
					for (int i(0);i<mapS.size();i++)
						mapS[i].clear();
					mapS.clear();

					mapString.reserve(sizeof(mapS2[0]));
					mapS.reserve(sizeof(mapS2));
					for (int i(0);i<sizeof(mapS2) / sizeof(mapS2[0]);i++) {
						for (int j(0);j<sizeof(mapS2[i]);j++) {
							mapString.push_back(mapS2[i][j]);
						}
						mapS.push_back(mapString);
						mapString.clear();
					}
					void StoryLevel2();
					break;
				//////////////создание карты третьего(четвертого) уровня////////////////
				case 3:
					for (int i(0);i<mapG.size();i++)
						mapG[i].clear();
					mapG.clear();
					mapString.reserve(sizeof(mapG3[0]));
					mapG.reserve(sizeof(mapG3));
					for (int i(0);i<sizeof(mapG3) / sizeof(mapG3[0]);i++) {
						for (int j(0);j<sizeof(mapG3[0]);j++) {
							mapString.push_back(mapG3[i][j]);
						}
						mapG.push_back(mapString);
						mapString.clear();
					}
					for (int i(0);i<mapS.size();i++)
						mapS[i].clear();
					mapS.clear();

					mapString.reserve(sizeof(mapS3[0]));
					mapS.reserve(sizeof(mapS3));
					for (int i(0);i<sizeof(mapS3) / sizeof(mapS3[0]);i++) {
						for (int j(0);j<sizeof(mapS3[i]);j++) {
							mapString.push_back(mapS3[i][j]);
						}
						mapS.push_back(mapString);
						mapString.clear();
					}
					void StoryLevel3();
					break;
				//////////////создание карты четвертого(пятого) уровня////////////////
				case 4:
					for (int i(0);i<mapG.size();i++)
						mapG[i].clear();
					mapG.clear();
					mapString.reserve(sizeof(mapG4[0]));
					mapG.reserve(sizeof(mapG4));
					for (int i(0);i<sizeof(mapG4) / sizeof(mapG4[0]);i++) {
						for (int j(0);j<sizeof(mapG4[0]);j++) {
							mapString.push_back(mapG4[i][j]);
						}
						mapG.push_back(mapString);
						mapString.clear();
					}
					for (int i(0);i<mapS.size();i++)
						mapS[i].clear();
					mapS.clear();

					mapString.reserve(sizeof(mapS4[0]));
					mapS.reserve(sizeof(mapS4));
					for (int i(0);i<sizeof(mapS4) / sizeof(mapS4[0]);i++) {
						for (int j(0);j<sizeof(mapS4[i]);j++) {
							mapString.push_back(mapS4[i][j]);
						}
						mapS.push_back(mapString);
						mapString.clear();
					}
					void StoryLevel4();
					break;
				default:
					cerr<<"Error, unknoun level!"<<endl;
					break;
			}
			/////////////вход в игру/////////////////
			if (!Game(mapG, mapS, things, level)&& level != LEVELS) {
				level++;	
				if (level == LEVELS) {
					WinStory();
					cout << "You are WIN!!!!!!!!" << endl;
					Sleep(1000);
				}
				else Score(things);
			}
			else level = LEVELS;
		}
	}

	cout << "See you next time!" << endl;
	system("pause");
	return 0;
}
/*
void PictureWelcom() {
	cout << "    ______   _____   __ __   ___     _____   ________       ___             " << endl;
	cout << "   /_____/| /___ /| / // /| /_ /|   /___ /| /______ /|     /_ /|                   " << endl;
	cout << "  |  _  | ||  _ | || |/ | ||  | |_ | __ | ||__   __|/     |  | |_               " << endl;
	cout << "  | | | | ||  __|/ |    | ||__|/_/||  __|/    | | |       |__|/_/|            " << endl;
	cout << "  | | | | || | |   |    | ||  _ | || |/_ /|   | | |       |  _ | |           " << endl;
	cout << "  |_|/|_| /|_|/    |_/|_|/ |____|/ |____|/    |_|/        |____|/                        " << endl;
	cout << "                                              " << endl;
	cout << "                                              " << endl;
	cout << "                                              " << endl;
	cout << "                                              " << endl;
	cout << "                                              " << endl;
	cout << "                                              " << endl;
	cout << "                                              " << endl;
}
*/

int Game(vector<vector<char>> mapG, vector<vector<char>> mapS, thingScore & things, int level) {
	system("cls");
	if(!(level%2)) system("color 03");
	else system("color 0e");
	const char h = (char) 38;//герой
	const char d = (char) 35;//дверь, решетка
	const char w = (char)219;//стена
	const char m = (char) 36;//деньги
	const char k = (char)235;//ключ, отмычка
	const char p = (char)225;//люди, человек
	const char o = (char) 32;//пространство, пробел
	const char s = (char)176;//камень
	const char g = (char)233;//пропасть, дыра
	const char t = (char) 64;//телепорт
	const char q = (char) 63;//вопрос, помошник
	const char b = (char)157;//ищейка
	const char f = (char)234;//удача, жизни

	bool beagleWillCome(false);//условие, когда герой попал в яму, а ищейка его продолжает искать, герой не может двигаться
	bool beagleY_or_N(false);
	//////////////условие, при котором за нами будет гнаться ищейка////////////////
	if (level >= 3)
		beagleY_or_N=true;

	thingPlace bPlace;         //местонахождение ищейки
	thingPlace bPlaceStart;    //место появления ищейки
	vector<thingPlace> bMove;  //движение ищейки
	int  bMoveNow(0);          //номер ячейки на нужное движение ищейки
	int  bRadius(1);           //радиус на который ищейка успевает достать героя
	char bPlaceDown;           //на чем стоит собака
	int  bStartGo(5);		   //спустя сколько ходов ищейка начнет искать
	int  bStopGo(0);		   //переменная для задержки собаки, если у перса была фартуна, а собака его достала
	int  bStopClose(2);    //чтобы слезть с двери, камня, дыры нужен лишний ход
	const int BSTOPGO(2);      //сколько собака будет стоять, если у перса была фартуна, а собака его достала
	thingPlace hPlace;      //местонахождение героя
	thingPlace hPlaceStart; //начальная позиция
	thingPlace hMove;       //движение героя
	char hPlaceDown(o);     //на чем стоит герой
	char move(' ');           //выбор движения
	char choise;         //перейти ли на следующий уровень?
	int lightDay(3);     //сила освещения днем

	bool mapG_or_S(true);//для переключения между основной картой и секретной

	vector<vector<char>> map;//карта, на которой мы есть
	map.reserve(mapG.size()*mapG[0].size());
	vector<char> mapString;//для создания карт
	mapString.reserve(mapG.size());
	for(int i(0);i<mapG.size();i++){
		for(int j(0);j<mapG[i].size();j++)
			mapString.push_back(mapG[i][j]);
		map.push_back(mapString);
		mapString.clear();
	}

	///////////////нахождение начальной точки создания героя///////////////
	for (int i(0);i < map.size();i++){
		for (int j(0);j < map[i].size();j++)
			if (map[i][j] == h) { hPlace.get_i() = i; hPlace.get_j() = j; hPlaceStart.get_i()=i; hPlaceStart.get_j()=j; break; }
		if(hPlaceStart.get_i()!=0&& hPlaceStart.get_j() != 0) break;
	}
	if(beagleY_or_N){
		bPlaceStart.get_i() = hPlaceStart.get_i();
		bPlaceStart.get_j() = hPlaceStart.get_j();
	}
	cout << "For movement you can use next buttons: W - up, A - left, S - down, D - right, 0 - exit" << endl;
	cout << "First level" << endl;
	do {	
		//////////////в зависимости от выранного движения присваиваются спец значения/////////////////
		switch (move)
		{
			case 'w':
				hMove.get_i()=-1;
				break;
			case 'a':
				hMove.get_j()=-1;
				break;
			case 's':
				hMove.get_i()=+1;
				break;
			case 'd':
				hMove.get_j()=+1;
				break;
			case 'z':
				Sleep(500);
				cout<<"Beagle are going!"<<endl;
				break;
			case '0':
				return 1;
				break;
			default:
				cout << "You press wrong button!" << endl;
				break;
		}
		system("cls");

		/////////////в зависимости от того куда идет наш персонаж делаем соответствующие манипуляции над информацией///////////////
		switch (map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()])
		{
			/////////////наступает на стену//////////////
			case w:
				cout << "It's wall!" << endl;
				break;
			/////////////наступает на пол/////////////
			case o:
				cout<<"Move"<<endl;
				map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
				hPlaceDown = map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()];
				map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
				hPlace.get_i() += hMove.get_i();
				hPlace.get_j() += hMove.get_j();
				//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
				if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				break;
			//////////////наспупает на дверь///////////////
			case d:
				if (things.get_jimmy() <= 0) cout << "You need a jimmy!" << endl;
				else if (things.get_jimmy() > 0) {
					cout << "Open the door" << endl;
					things.get_jimmy()--;
					map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
					hPlaceDown = map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()];
					map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
					hPlace.get_i() += hMove.get_i();
					hPlace.get_j() += hMove.get_j();
					//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
					if(beagleY_or_N && mapG_or_S){ bMove.push_back(hMove); }
				}
				else cerr << "Error with jimmyAmount!" << endl;
				break;
			/////////////////наступает на отмычку(ключ)///////////////////
			case k:
				cout << "Move" << endl;
				things.get_jimmy()++;
				map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
				hPlaceDown = o;
				map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
				hPlace.get_i() += hMove.get_i();
				hPlace.get_j() += hMove.get_j();
				//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
				if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				break;
			/////////////наступает на деньги///////////////
			case m:
				cout << "Money" << endl;
				things.get_money()+=10;
				map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
				hPlaceDown = o;
				map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
				hPlace.get_i() += hMove.get_i();
				hPlace.get_j() += hMove.get_j();
				//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
				if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				break;
			//////////////наступает на камень////////////////
			case s:
				switch (map[hPlace.get_i() + 2 * hMove.get_i()][hPlace.get_j() + 2 * hMove.get_j()])
				{
					//камень катится в яму
					case g:
						cout << "You destroed the stone" << endl;
						map[hPlace.get_i() + 2 * hMove.get_i()][hPlace.get_j() + 2 * hMove.get_j()] = o;
						map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
						hPlaceDown = o;
						map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
						hPlace.get_i() += hMove.get_i();
						hPlace.get_j() += hMove.get_j();
						//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
						if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
						break;
					//камень катится на пол, деньги, отмычку, помошник
					case o:if(map[hPlace.get_i() + 2 * hMove.get_i()][hPlace.get_j() + 2 * hMove.get_j()]==o)cout << "Move the stone" << endl;
					case m:if(map[hPlace.get_i() + 2 * hMove.get_i()][hPlace.get_j() + 2 * hMove.get_j()]==m)cout<<"Money has been destroed"<<endl;
					case k:if(map[hPlace.get_i() + 2 * hMove.get_i()][hPlace.get_j() + 2 * hMove.get_j()]==k)cout<<"Jimmy(key) has been destroed"<<endl;
					case q:if(map[hPlace.get_i() + 2 * hMove.get_i()][hPlace.get_j() + 2 * hMove.get_j()]==q)cout<<"Helper has been destroed"<<endl;
						map[hPlace.get_i() + 2*hMove.get_i()][hPlace.get_j() + 2*hMove.get_j()] = s;
						map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
						hPlaceDown = o;
						map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
						hPlace.get_i() += hMove.get_i();
						hPlace.get_j() += hMove.get_j();
						//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
						if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
						break;
					//камень катится на камень, стену, телепорт, человека, дверь
					case s:
					case w:
					case t:
					case p:
					case d:
					case b:
						cout << "You can't move this stone in this route!" << endl;
						break;
					default:
						cerr << "Error, unknoun element!" << endl;
						break;
				}
				break;
			//////////////наступает на пропасть////////////////
			case g:
				if (things.get_fortune() > 0) {
					cout<<"You are falled to the gulf, but out from if so fast, grasping the bough. Bough has been wrested!("<<f<<"-1)"<<endl;
					things.get_fortune()--;
					map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
					hPlaceDown = g;
					map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
					hPlace.get_i() += hMove.get_i();
					hPlace.get_j() += hMove.get_j();
					//удачно походил и ты на главной карте - по этому пути пойдет собака, если она должна ходить на этой карте
					if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				}
				else if(beagleY_or_N && mapG_or_S){
					beagleWillCome=true;
					map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
					hPlaceDown=g;
					hPlace.get_i()+=hMove.get_i();
					hPlace.get_j()+=hMove.get_j();
				}
				else{
					system("cls");
					cout << "You are falled to the gulf, you are continue to play from the start!" << endl;
					PictureGulf();
					system("pause");
					system("cls");
					cout<<"falled to the gulf"<<endl;
					map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
					hPlaceDown = map[hPlaceStart.get_i()][hPlaceStart.get_j()];
					map[hPlaceStart.get_i()][hPlaceStart.get_j()] = h;
					hPlace.get_i() = hPlaceStart.get_i();
					hPlace.get_j() = hPlaceStart.get_j();
				}
				break;
			//////////////наступает на подсказки, помощник//////////////
			case q:
				system("cls");
				Question();
				system("pause");
				system("cls");
				cout<<"helper"<<endl;
				map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
				hPlaceDown = q;
				map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
				hPlace.get_i() += hMove.get_i();
				hPlace.get_j() += hMove.get_j();
				//удачно походил и ты на главной карте- по этому пути пойдет собака, если она должна ходить на этой карте
				if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				break;
			case f:
				cout << "Fortune" << endl;
				things.get_fortune()++;
				map[hPlace.get_i()][hPlace.get_j()] = hPlaceDown;
				hPlaceDown = o;
				map[hPlace.get_i() + hMove.get_i()][hPlace.get_j() + hMove.get_j()] = h;
				hPlace.get_i() += hMove.get_i();
				hPlace.get_j() += hMove.get_j();
				//удачно походил и ты на основной карте- по этому пути пойдет собака, если она должна ходить на этой карте
				if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				break;
			//////////////наступает на телепорт//////////////
			case t:
				cout<<"Teleport"<<endl;
				map[hPlace.get_i()][hPlace.get_j()]=hPlaceDown;
				//из основной в секретную
				if (mapG_or_S) {
					for(int i(0);i<mapG.size();i++)
						for(int j(0);j<mapG[i].size();j++)
							mapG[i][j]=map[i][j];
					for(int i(0);i<map.size();i++)
						map[i].clear();
					map.clear();
					map.reserve(mapS.size()*mapS[0].size());
					mapString.reserve(mapS[0].size());
					for (int i(0);i<mapS.size();i++) {
						for (int j(0);j<mapS[i].size();j++)
							mapString.push_back(mapS[i][j]);
						map.push_back(mapString);
						mapString.clear();
					}
				//удачно походил на телепорт и ты на основной карте- по этому пути пойдет собака, если она должна ходить на этой карте
				if (beagleY_or_N && mapG_or_S) { bMove.push_back(hMove); }
				mapG_or_S=false;
				}
				//из секретной карты в основную
				else {
					for (int i(0);i<mapS.size();i++)
						for (int j(0);j<mapS[i].size();j++)
							mapS[i][j] = map[i][j];
					for (int i(0);i<map.size();i++)
						map[i].clear();
					map.clear();
					map.reserve(mapG.size()*mapG[0].size());
					mapString.reserve(mapG[0].size());
					for (int i(0);i<mapG.size();i++) {
						for (int j(0);j<mapG[i].size();j++)
							mapString.push_back(mapG[i][j]);
						map.push_back(mapString);
						mapString.clear();
					}
				mapG_or_S=true;
				}
				//создание персонажа на новой карте
				for(int i(0);i<map.size();i++){
					for (int j(0);j < map[i].size();j++)
						if(map[i][j]==t){ 
							hPlace.get_i()=i;
							hPlace.get_j()=j;
							hPlaceStart.get_i()=i;
							hPlaceStart.get_j()=j;
							map[i][j]=h;
							hPlaceDown = t;
							break;
						}
					if(hPlaceDown == t)break;
				}
				break;
			//////////////наступает на человека, конечный пункт///////////////
			case p:
				system("cls");
				PicturePerson();
				cout<<"Your answer: ";
				cin >> choise;
				system("cls");
				if(choise == 'y') return 0;
				else cout<<"Nothing"<<endl;
				break;
			default:
				cerr << "Error, unknown element in game!" << endl;
				break;
		}
		hMove.get_i()=0;
		hMove.get_j()=0;
		//////////если ищейке пора бежать - она бежит, учитывая что ее ждет впереди///////////
		if (bMove.size() > bStartGo && mapG_or_S && bStopGo == 0 && bStopClose != 2) {
			switch (map[bPlace.get_i() + bMove[bMoveNow].get_i()][bPlace.get_j() + bMove[bMoveNow].get_j()]) {
			case k:
			case m:
			case f:
			case t:
			case q:
			case o:
				if ((bPlaceDown == g || bPlaceDown == d || bPlaceDown == s) && bStopClose==0) {
					bStopClose = 2;
				}
				else {
				bStopClose = 0;
				map[bPlace.get_i()][bPlace.get_j()] = bPlaceDown;
				bPlace.get_i() += bMove[bMoveNow].get_i();
				bPlace.get_j() += bMove[bMoveNow].get_j();
				bPlaceDown = map[bPlace.get_i()][bPlace.get_j()];
				map[bPlace.get_i()][bPlace.get_j()] = b;
				++bMoveNow;
				}
				break;
			case g:
			case d:
			case s:
				map[bPlace.get_i()][bPlace.get_j()] = bPlaceDown;
				bPlace.get_i() += bMove[bMoveNow].get_i();
				bPlace.get_j() += bMove[bMoveNow].get_j();
				bPlaceDown = map[bPlace.get_i()][bPlace.get_j()];
				map[bPlace.get_i()][bPlace.get_j()] = b;
				++bMoveNow;
				break;
			case w:
			case p:
			default:
				cerr << "Error, dog can't go here!" << endl;
				break;
			}

		}
		else if(bMove.size() > bStartGo && mapG_or_S && bStopGo>0) bStopGo--;
		else if(bMove.size() > bStartGo && mapG_or_S && bStopGo<0) bStopGo++;
		if(bStopClose>0)bStopClose--;
		cout<<f<<" - "<<things.get_fortune()<<'\t'<<m<<" - "<<things.get_money()<<'\t'<<k<<" - "<<things.get_jimmy()<<'\t'<<"Power of lighting - "<<things.get_light()<<endl;
		
		///////////если настало время выпускать ищейку(до того как покажут карту, но после того как она смодет сделать первые движения)//////////////
		if (beagleY_or_N && bMove.size() == bStartGo) {
			bPlaceDown = map[bPlaceStart.get_i()][bPlaceStart.get_j()];
			map[bPlaceStart.get_i()][bPlaceStart.get_j()] = b;
			bPlace.get_i() = bPlaceStart.get_i();
			bPlace.get_j() = bPlaceStart.get_j();
			bMoveNow = 0;
		}
		
		///////////////////вывод карты в ночное время суток//////////////////
		if (!(level % 2)) {
			for (int i(0);i < map.size();++i) {
				for (int j(0);j < map[i].size();++j) {
					if ((i <= hPlace.get_i() + things.get_light()) || i <= bPlace.get_i() && 0 != bPlace.get_i() && mapG_or_S) {
						if (i >= hPlace.get_i() - things.get_light() && i <= hPlace.get_i() + things.get_light() && j <= hPlace.get_j() + things.get_light() + 1 && j >= hPlace.get_j() - things.get_light() - 1)cout << map[i][j];
						else if (i == bPlace.get_i() && j == bPlace.get_j() && bPlace.get_i() != 0 && beagleY_or_N && mapG_or_S) cout << map[i][j];
						else cout << o;
					}
				}
				if ((i <= hPlace.get_i() + things.get_light()) || i <= bPlace.get_i() && 0 != bPlace.get_i()) cout << endl;
			}
		}
		//////////////вывод карты в дневное время суток/////////////////////
		else {
			for (int i(0);i < map.size();++i) {
				for (int j(0);j < map[i].size();++j) {
					if (i <= hPlace.get_i() + lightDay || i <= bPlace.get_i() && 0 != bPlace.get_i()) {
						if (i >= hPlace.get_i() - lightDay && i <= hPlace.get_i() + lightDay && j <= hPlace.get_j() + 1 + lightDay && j >= hPlace.get_j() - 1 - lightDay)cout << map[i][j];
						else if (map[i][j] == b) cout << map[i][j];
						else cout << o;
					}
				}
				if (i <= hPlace.get_i() + lightDay || i <= bPlace.get_i() && 0 != bPlace.get_i()) cout << endl;
			}
		}
		/////////////проверка на наличие рядом героя///////////////
		for (int i(0);i < map.size();++i) {
			for (int j(0);j < map[i].size();++j) {
				//если собака
				if (map[i][j] == b) {
					//если в радиусе прыжка собаки от персонажа собака
					if (i <= hPlace.get_i() + bRadius && j == hPlace.get_j() && i >= hPlace.get_i() - bRadius && j == hPlace.get_j() || i == hPlace.get_i() && j >= hPlace.get_j() - bRadius && i == hPlace.get_i() && j <= hPlace.get_j() + bRadius) {
						//если это приследующая собака и у нас еще есть фартуна, то остановить ее на пару ходов и отнять одну фартуну
						if (i == bPlace.get_i() && j == bPlace.get_j() && things.get_fortune()>0) {
							bStopGo = BSTOPGO;
							things.get_fortune()--;
						}
						//если это не наша собака, то просто отнять фортуну
						else if (things.get_fortune() > 0) {
							things.get_fortune()--;
						}
						//если больше нет фартуны - конец игры
						else {
							Sleep(1000);
							PictureArest();
							return 1;
						}
					}
				}
			}
		}

		if (!beagleWillCome) {
			///////////////ввод движения////////////////
			cout << "Move: ";
			cin >> move;
			cin.clear();
		}
		else move = 'z';
	} while (move != '0');
	return 1;
}

char Menu() {
	system("cls");
	char choise;
	cout<<endl<<endl<<endl<<endl<<endl;
	cout<<"\t\t\t\t\t\t\tMenu:"<<endl;
	cout<<"\t\t\t\t\t\t1.Start new game"<<endl;
	cout<<"\t\t\t\t\t\t2.Start game from the level"<<endl;
	cout<<"\t\t\t\t\t\t3.Read the rules"<<endl;
	cout<<"\t\t\t\t\t\t0.Exit"<<endl;
	cin>>choise;
	return choise;
}

void StartStory() {

}


void StoryLevel0() {

}

void StoryLevel1() {

}

void StoryLevel2() {

}

void StoryLevel3() {

}

void StoryLevel4() {

}

char Levels() {
	char choise;
	cout << endl << endl << endl << endl << endl;
	cout<<"\t\t\t\t\t\t\tLEVELS:"<<endl;
	cout<<"\t\t\t\t\t\t1.First level"<<endl;
	cout<<"\t\t\t\t\t\t2.Second level"<<endl;
	cout<<"\t\t\t\t\t\t3.Third level"<<endl;
	cout<<"\t\t\t\t\t\t4.Forth level"<<endl;
	cout<<"\t\t\t\t\t\t5.Fifth level"<<endl;
	cin>>choise;
	return choise;
}

void Question() {
	const char h = (char) 38;//герой
	const char d = (char) 35;//дверь, решетка
	const char w = (char)219;//стена
	const char m = (char) 36;//деньги
	const char k = (char)235;//ключ, отмычка
	const char p = (char)225;//люди, человек
	const char o = (char) 32;//пространство, пробел
	const char s = (char)176;//камень
	const char g = (char)233;//пропасть, дыра
	const char t = (char) 64;//телепорт
	const char q = (char) 63;//вопрос
	const char b = (char)157;//ищейка
	const char f = (char)234;//удача, жизни
	cout << endl << endl << endl;
	cout<<"\tRules:"<<endl;
	cout<<"\t  Movements:"<<endl;
	cout<<"\t    W - go up, A - go left, S - go down, D - go right, 0 - exit."<<endl;
	cout<<"\t  Obgects:"<<endl;
	cout<<"\t    "<< h << " - it's you, you can move(W,A,S,D);" <<endl;
	cout<<"\t    "<< w << " - wall, you and stones can't move throught this;" <<endl;
	cout<<"\t    "<< d << " - door, it's opens throught jimmy(key). But after you go throught door, it's will close;" << endl;
	cout<<"\t    "<< m << " - money, with help this you can by something in score;" <<endl;
	cout<<"\t    "<< k << " - jimmy(key), it's help you open the doors;" <<endl;
	cout<<"\t    "<< s << " - stone. you can move this. Attention: stone destroy the next objects: "<<m<<", "<<k<<", " << g << ", " << q << ", but can't destroy the next objects:" << d << ", " << w << ", " << t << ", " << p << ", " << s <<';'<< endl;
	cout<<"\t    "<< g << " - gulf(hole). If you fall to the gulf, you continue play from the start;"<<endl;
	cout<<"\t    "<< t << " - teleport. teleport you to the sicret map and back;"<<endl;
	cout<<"\t    "<< q << " - helper;"<<endl;
	cout<<"\t    "<< f << " - fortune, amount of your lives;"<<endl;
	cout<<"\t    "<< b << " - beagle. After few steps it's began search you. Run from it!"<<endl;
	cout<<"\t    "<< p << " - person that help you go to the next level;" << endl;
	cout<<"\t  Mission: find a person to go out from this location."<<endl;
}

void Score(thingScore & things) {
	system("cls");

	const char m = (char)36;//деньги
	const char k = (char)235;//ключ, отмычка
	const char f = (char)234;//удача, жизни

	char choise('0');

	cout << "\t\t\t\t\t\t\tScore:" << endl;
	cout << "\t\t\t\t\tHello! I'm glad to see you, my favort costomer!" << endl;
	do{
		cout << "\t\t\t\t\tIn the score you can by things or sell those things in low price(1/2)!" << endl;
		cout << "\t\t\t\t\tNow you have:" << f << " - " << things.get_fortune() << ",   " << m << " - " << things.get_money() << ",   " << k << " - " << things.get_jimmy() << ",   " << "Power of lighting - " << things.get_light() << endl;
		cout << "\t\t\t\t\t\tMake you coise:" << endl;
		cout << "\t\t\t\t\t  1.By 1 jimmy(key)       - " << things.get_priceJ() << m << endl;
		cout << "\t\t\t\t\t  2.By 1 fortune(live)    - " << things.get_priceF() << m << endl;
		cout << "\t\t\t\t\t  3.Enhance lighting on 1 - " << things.get_pticeL() << m << endl;
		cout << "\t\t\t\t\t  4.Get " << things.get_priceJ() / 2 << m << "\t- 1 jimmy" << endl;
		cout << "\t\t\t\t\t  5.Get " << things.get_priceF() / 2 << m << "\t- 1 fortune" << endl;
		cout << "\t\t\t\t\t  6.Get " << things.get_pticeL() / 2 << m << "\t- 1 power of lighting" << endl;
		cout << "\t\t\t\t\t  0.Exit." << endl;
		cin >> choise;
		system("cls");
		switch (choise) {
			case '1':
				things.jimmyBy() ? cout << "Good!" << endl : cout << "You need more money to by jimmy!" << endl;
				break;
			case '2':
				things.fortuneBy() ? cout<<"Good!"<<endl : cout<<"You need more money to by fortune!"<<endl;
				break;
			case '3':
				things.lightBy() ? cout << "Good!" << endl : cout << "You need more money to enhance lighting!" << endl;
				break;
			case '4':
				things.jimmySell() ? cout << "Good!" << endl : cout << "You haven't jimmies to get money!" << endl;
				break;
			case '5':
				things.fortuneSell() ? cout << "Good!" << endl : cout << "You haven't fortunes to get money!" << endl;
				break;
			case '6':
				things.lightSell() ? cout << "Good!" << endl : cout << "You can't make power of lighting weaker to get money!" << endl;
				break;
			case '0':
				cout<<"Come again!"<<endl;
				Sleep(1000);
				break;
			default:
				cout<<"You are has been pressed wrong button!"<<endl;
				break;
		}
		if(choise!='0')cout << "\t\t\t\t\tScore:" << endl;
	}while(choise!='0');
	system("cls");
}

void PictureGulf() {
	cout << endl << endl << endl;
	cout << "\t\t *              *       *        *        " << endl
		 << "\t\t       *            *                *    " << endl
		 << "\t\t             *            *               " << endl
		 << "\t\t   *             *             !    *     " << endl
		 << "\t\t                              (..)__/     " << endl
		 << "\t\t                               /|         " << endl
		 << "\t\t                              / |         " << endl
		 << "\t\t                               / |        " << endl
		 << "\t\t________                    __/__|________" << endl
		 << "\t\t=======|                   /==============" << endl
		 << "\t\t=======|                  /===============" << endl
		 << "\t\t=======|                 /==========X=====" << endl
		 << "\t\t===X===|                 |================" << endl
		 << "\t\t=======|                 |================" << endl
		 << "\t\t=======|       ?         |==X=============" << endl
		 << "\t\t=======|      ('')       |===============X" << endl
		 << "\t\t=======|      /||_       |================" << endl
		 << "\t\t=======|     / |_       /=================" << endl
		 << "\t\t=======|      /  |     /===========X=X====" << endl
		 << "\t\t======X|_____/___=____/===================" << endl
		 << "\t\t==========================================" << endl
		 << "\t\t===============================X==========" << endl;
}

void PictureArest() {

}

void PicturePerson() {
	cout << endl << endl << endl;
	cout << "\t\t   ____*________________*___________  *      " << endl
		 << "\t\t  /             *                   |        " << endl
		 << "\t\t | *   Are you ready go       *     |        " << endl
		 << "\t\t |            to the next level?    |   *    " << endl
		 << "\t\t |     *   (y - yes, n - no)        |        " << endl
		 << "\t\t |___________________*_____________/         " << endl
		 << "\t\t  *        /       __________     *          " << endl
		 << "\t\t        * /       /    *     |               " << endl
		 << "\t\t         /      *|    ...    |        *      " << endl
		 << "\t\t   *    /        |_________*/                " << endl
		 << "\t\t       /                   /                 " << endl
		 << "\t\t     _( ')                (' )               " << endl
		 << "\t\t    / /|__                 /|                " << endl
		 << "\t\t    |_|___                / |                " << endl
		 << "\t\t     | | /                 / |               " << endl
		 << "\t\t_____|_|/_________________/__|_______________" << endl
		 << "\t\t=============================================" << endl
		 << "\t\t=============================================" << endl;
}

void WinStory() {

}