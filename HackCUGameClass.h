#ifndef HACKCUGAMECLASS_H
#define HACKCUGAMECLASS_H
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <thread>
using namespace std;
/*
struct highscores
{
  string username;
  int score;
  highscores *next;
};

struct encrypt
{

};
*/
struct MinigameTwoQuestion
{
    int question_number;
    string question;
    string answers;
    string correct;
    MinigameTwoQuestion *next;
};
class GameClass
{
  private:
    MinigameTwoQuestion* head;
    MinigameTwoQuestion MinigameTwoQuestions[10];
    //highscores *top;
    string PlayerStats[6];
    string MinigameOneQuestions[10];
    string MiniGameOneQuestionsAsked[10];
    string MiniGameTwoQuestionsAsked[10];
  public:
    bool helper_function_stoicheck(string input); //D
    GameClass(void);
    void setUpdatedPlayerStats(int index, string newValue);//D
    string getPlayerStats(int index);
    void readsavefile(string readfilename);
    bool writesavefile();
    void displaystartmenu();//D
    void displaygamemodemenu();//D
    void minigameselect();
    void GameLoop(bool new_or_saved);
    void startmenu();
    void readminigameonecodes(string readfilename);
    bool isMGOCorrectAnswer(string answer,string code);
    string GetMGOQuestion();
    bool getHasMGOQuestionBeenAsked(string code);
    void setHasMGOQuestionBeenAsked(string input);
    void readminigametwocodes(string readfilename);
    bool isMGTWOCorrectAnswer(string answer,string correct);
    int GetMGTWOQuestionIndex();
    bool getHasMGTWOQuestionBeenAsked(string input);
    void setHasMGTWOQuestionBeenAsked(string input);
    void displayHighScores();
    void displayAnsweredQuestionsLoaded();
};
#endif
