#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define QUIT 27

using namespace std;

class Snake
{
private:
	int* scores;

	string faultReason = "";

	enum Direction { STOP, LEFT, RIGHT, UP, DOWN };

	bool gameOver;

	string horBorderChar = "|";
	string verBorderChar = "-";
	const int width = 30;
	const int height = 15;

	int headX, headY, appleX, appleY, score;
	int tailX[100], tailY[100];
	int nTail;

	Direction dir;

	void Setup()
	{
		gameOver = false;
		dir = STOP;
		headX = width / 2;
		headY = height / 2;
		appleX = rand() % width;
		appleY = rand() % height;
		score = 0;
		nTail = 0;
	}

	void Draw()
	{
		system("cls");

		cout << "If you want to exit, just press [Esc] twice." << endl;
		cout << "\n\tScore: " << score << endl;
		cout << endl;
		for (int i = 0; i < width + 2; i++)
			cout << verBorderChar;
		cout << endl;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (j == 0)
					cout << horBorderChar;
				if (i == headY && j == headX)
				{
					switch (dir)
					{
					case STOP:
						cout << "@";
						break;
					case LEFT:
						cout << "<";
						break;
					case RIGHT:
						cout << ">";
						break;
					case UP:
						cout << "^";
						break;
					case DOWN:
						cout << "v";
						break;
					default:
						break;
					}

				}
				else if (i == appleY && j == appleX)
					cout << "O";
				else
				{
					bool print = false;
					for (int k = 0; k < nTail; k++)
					{
						if (tailX[k] == j && tailY[k] == i)
						{
							cout << "x";
							print = true;
						}
					}
					if (!print)
						cout << " ";
				}


				if (j == width - 1)
					cout << horBorderChar;
			}
			cout << endl;
		}

		for (int i = 0; i < width + 2; i++)
			cout << verBorderChar;
	}

	void Action()
	{
		Sleep(30);
		if (_kbhit())
		{
			_getch();
			switch (_getch())
			{
			case QUIT:
				gameOver = true;
				faultReason = "* Termination Reason: You Pressed [Esc] key twice! *";
				break;
			case KEY_LEFT:
				dir = LEFT;
				break;
			case KEY_RIGHT:
				dir = RIGHT;
				break;
			case KEY_UP:
				dir = UP;
				break;
			case KEY_DOWN:
				dir = DOWN;
				break;
			}
		}


		int prevX = tailX[0];
		int prevY = tailY[0];
		int prev2X, prev2Y;
		tailX[0] = headX;
		tailY[0] = headY;

		for (int i = 1; i < nTail; i++)
		{
			prev2X = tailX[i];
			prev2Y = tailY[i];
			tailX[i] = prevX;
			tailY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}

		switch (dir)
		{
		case LEFT:
			headX--;
			break;
		case RIGHT:
			headX++;
			break;
		case UP:
			headY--;
			break;
		case DOWN:
			headY++;
			break;
		default:
			break;
		}

		if (headX >= width || headX < 0 || headY >= height || headY < 0)
		{
			gameOver = true;
			faultReason = "* Termination Reason: You touched the wall! *";
		}

		for (int i = 0; i < nTail; i++)
			if (tailX[i] == headX && tailY[i] == headY)
			{
				gameOver = true;
				faultReason = "* Termination Reason: You ate yourself! *";
			}

		if (headX == appleX && headY == appleY)
		{
			score++;
			appleX = rand() % width;
			appleY = rand() % height;
			nTail++;
		}
	}

	void File()
	{
		string path = "results.txt";
		ifstream inFile(path);
		string line;
		int scoresCount = 0;
		int maxScore = INT_MIN;

		while (std::getline(inFile, line))
			scoresCount++;
		inFile.clear();
		inFile.seekg(0, ios::beg);

		scores = new int[scoresCount];

		int i = 0;
		while (std::getline(inFile, line))
		{
			scores[i] = stoi(line);
			if (scores[i] > maxScore)
				maxScore = scores[i];
			i++;
		}

		if (score > maxScore)
			maxScore = score;

		cout << endl << endl << faultReason << endl;
		cout << "\n\nYour Score: " << score << endl;
		cout << "Best Ever Score: " << maxScore << endl;
		cout << "* Your score will be added to score log. *" << endl;

		ofstream outFile(path, ios::out);
		for (int i = 0; i < scoresCount; i++)
			outFile << scores[i] << endl;
		outFile << score << endl;
		outFile.close();
	}

public:
	void Go()
	{
		Setup();
		while (!gameOver)
		{
			Draw();
			Action();
		}
		File();
	}

	~Snake()
	{
		delete scores;
	}
};
int main()
{
	Snake snake;
	snake.Go();

	return 0;
}