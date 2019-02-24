#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <sstream>
#include "HackCUGameClass.h"
using namespace std;

/////////////     HELPER FUNCTIONS     /////////////
bool GameClass::helper_function_stoicheck(string input)
{
    //check for empty string
    if (input == ""){
        cout << "Input is empty. Please enter something else" << endl;
        return false;
    }
    //traverse the string
    for(int i = 0; i < input.length(); i++)
    {
        if (input[i] != '0' && input[i] != '1' && input[i] != '2' && input[i] != '3' && input[i] != '4' && input[i] != '5' && input[i] != '6' && input[i] != '7' && input[i] != '8' && input[i] != '9')
        {
            //Does it contain non integer 0-9?
            cout << "Invalid input. Please stop trying to break the game!"  << endl;
            return false;
        }
    }
    return true;
};
/////////////     CONSTRUCTORS     /////////////
GameClass::GameClass(void)
{
    head = nullptr;
    //top = nullptr;
    //    Default Stats   //
    PlayerStats[0] = "";       //username
    PlayerStats[1] = "0";      //pts (as a string)
    PlayerStats[2] = "";       //gamemode
    PlayerStats[3] = "0";      //current round they are on
    PlayerStats[4] = "";       //Questions Asked Already For MinigameOne
    PlayerStats[5] = "";       //Questions Asked Already For MinigameTwo
    for(int i = 0; i < 10; i++)
    {
      MinigameOneQuestions[i] = "";
      MiniGameOneQuestionsAsked[i] = "";
      MiniGameTwoQuestionsAsked[i] = "";
    }
};
/////////////     PLAYER STATS     /////////////
void GameClass::setUpdatedPlayerStats(int index, string newValue)
{
  if(index == 1 || index == 3)
  {
    int prevsValue = stoi(PlayerStats[index]);
    prevsValue = prevsValue + stoi(newValue);
    string transfer = to_string(prevsValue);
    PlayerStats[index] = transfer;
  }
  else
  {
    PlayerStats[index] = newValue;
  }
};
string GameClass::getPlayerStats(int index)
{
  return PlayerStats[index];
};
/////////////     FOR SAVED GAMES     /////////////
void GameClass::readsavefile(string readfilename)
{
  //  What to read  //
  //  - Username [YES]
  //  - Current number of pts earned [YES]
  //  - Gamemode (AKA Difficultity) [YES]
  //  - Current Round they are in [YES]
  //  - Questions that are already asked
  ifstream thefile;
  thefile.open(readfilename);
  if(thefile.is_open())
  {
    string whole_line;
    getline(thefile,whole_line);
    setUpdatedPlayerStats(0, whole_line);
    getline(thefile,whole_line);
    setUpdatedPlayerStats(1, whole_line);
    getline(thefile,whole_line);
    setUpdatedPlayerStats(2, whole_line);
    getline(thefile,whole_line);
    setUpdatedPlayerStats(3, whole_line);
    getline(thefile,whole_line);
    setUpdatedPlayerStats(4, whole_line);
    getline(thefile,whole_line);
    setUpdatedPlayerStats(5, whole_line);
    thefile.close();
    char character;
    string select = "";
    string display_answers[10];
    int counter = 0;
    string mgone_previous_answers = getPlayerStats(4);
    int length = mgone_previous_answers.length();
    for(int i = 0; i <= length; i++)
    {
      if((mgone_previous_answers[i] != '\n') && (mgone_previous_answers[i] != '|'))
      {
        character = mgone_previous_answers[i];
        select = select + character;
        display_answers[counter] = select;
      }
      else
      {
        counter++;
        select = "";
      }
    }
    for(int i = 0; i < 10; i++)
    {
        select = display_answers[i];
        display_answers[i] = "";
        setHasMGOQuestionBeenAsked(select);
    }
    counter = 0;
    select = "";
    string mgtwo_previous_answers = getPlayerStats(5);
    length = mgtwo_previous_answers.length();
    for(int i = 0; i <= length; i++)
    {
      if((mgtwo_previous_answers[i] != '\n') && (mgtwo_previous_answers[i] != '|'))
      {
        character = mgtwo_previous_answers[i];
        select = select + character;
        display_answers[counter] = select;
      }
      else
      {
        counter++;
        select = "";
      }
    }
    for(int i = 0; i < 10; i++)
    {
        select = display_answers[i];
        display_answers[i] = "";
        setHasMGTWOQuestionBeenAsked(select);
    }
    //displayAnsweredQuestionsLoaded();
    cout << "Your Progress Has Been Uploaded" << endl;
  }
  else
  {
    cout << "Invalid or Corrupt Savefile. Progress Has Not Been Uploaded" << endl;
  }
};
bool GameClass::writesavefile()
{
  //  What to save  //
  //  - Username [YES]
  //  - Current number of pts earned [YES]
  //  - Gamemode (AKA Round #) [YES]
  //  - Current Round they are in [YES]
  //  - Questions that are already asked
  ofstream save;
  save.open("savedgame.txt");
  save << getPlayerStats(0) << endl;
  save << getPlayerStats(1) << endl;
  save << getPlayerStats(2) << endl;
  save << getPlayerStats(3) << endl;
  string MiniGameOne = "";
  string MiniGameTwo = "";
  for(int i = 0; i < 10; i++)
  {
    if(MiniGameOneQuestionsAsked[i] != "")
    {
      if(i == 0 || i == 10)
      {
        MiniGameOne = MiniGameOne + MiniGameOneQuestionsAsked[i];
      }
      else
      {
        MiniGameOne = MiniGameOne + "|" + MiniGameOneQuestionsAsked[i];
      }
    }
    if(MiniGameTwoQuestionsAsked[i] != "")
    {
      if(i == 0 || i == 10)
      {
        MiniGameTwo = MiniGameTwo + MiniGameTwoQuestionsAsked[i];
      }
      else
      {
        MiniGameTwo = MiniGameTwo + "|" + MiniGameTwoQuestionsAsked[i];
      }
    }
  }
  //cout << MiniGameOne << endl;
  //cout << MiniGameTwo << endl;
  save << MiniGameOne << endl;
  save << MiniGameTwo << endl;
  cout << "Your Progress Has Been Saved" << endl;
  return true;
};
/////////////     RUNNING THE MINIGAMES     /////////////
/////////////           MINIGAME 1          /////////////
void GameClass::readminigameonecodes(string readfilename)
{
  ifstream thefile;
  thefile.open(readfilename);
  if(thefile.is_open())
  {
    int i = 0;
    string whole_line;
    while(getline(thefile,whole_line))
    {
        MinigameOneQuestions[i] = whole_line;
        //cout << MinigameOneQuestions[i] << endl;
        i++;
    }
  }
};
bool GameClass::isMGOCorrectAnswer(string answer, string code)
{
  if(answer == code)
  {
    return true;
  }
  return false;
};
string GameClass::GetMGOQuestion()
{
  int randomindex = rand() % 9 + 1;
  string code = MinigameOneQuestions[randomindex];
  return code;
};
bool GameClass::getHasMGOQuestionBeenAsked(string code)
{
  for(int i = 0; i < 20; i++)
  {
    if(MiniGameOneQuestionsAsked[i] == code)
    {
        return true;
    }
  }
  return false;
};
void GameClass::setHasMGOQuestionBeenAsked(string input)
{
  bool insert = false;
  int i = 0;
  while(insert == false)
  {
    if(MiniGameOneQuestionsAsked[i] == "")
    {
        MiniGameOneQuestionsAsked[i] = input;
        insert = true;
    }
    else if(MiniGameOneQuestionsAsked[i] == input && MiniGameOneQuestionsAsked[i] != "")
    {
      MiniGameOneQuestionsAsked[i] = input;
      insert = true;
    }
    i++;
  }
};
void GameClass::displayAnsweredQuestionsLoaded()
{
  for(int i = 0; i < 10; i++)
  {
    cout << "Question " << (i+1) << "of minigame one: " << MiniGameOneQuestionsAsked[i] << endl;
    cout << "Question " << (i+1) << "of minigame two: " << MiniGameTwoQuestionsAsked[i] << endl;
  }
};
/////////////           MINIGAME 2          /////////////
void GameClass::readminigametwocodes(string readfilename)
{
  ifstream thefile;
  thefile.open(readfilename);
  string whole_line;
  string question,answers,correct;
  if(thefile.is_open())
  {
      int i = 0;
      while(i < 10)
      {
        MinigameTwoQuestions[i].question_number = i;
        getline(thefile,question,':');
        MinigameTwoQuestions[i].question = question;
        //cout << MinigameTwoQuestions[i].question << endl;
        getline(thefile,answers,':');
        MinigameTwoQuestions[i].answers = answers;
        //cout << MinigameTwoQuestions[i].answers << endl;
        getline(thefile,correct);
        MinigameTwoQuestions[i].correct = correct;
        //cout << MinigameTwoQuestions[i].correct << endl;
        i++;
      }
    }
  thefile.close();
};
bool GameClass::isMGTWOCorrectAnswer(string answer, string correct)
{
  if(answer == correct)
  {
    return true;
  }
  return false;
};
int GameClass::GetMGTWOQuestionIndex()
{
  int randomindex = rand() % 9 + 1;
  return randomindex;
};
bool GameClass::getHasMGTWOQuestionBeenAsked(string input)
{
  for(int i = 0; i < 10; i++)
  {
    if(MiniGameTwoQuestionsAsked[i] == input)
    {
        return true;
    }
  }
  return false;
};
void GameClass::setHasMGTWOQuestionBeenAsked(string input)
{
  bool insert = false;
  int i = 0;
  while(insert == false)
  {
    if(MiniGameTwoQuestionsAsked[i] == "")
    {
      MiniGameTwoQuestionsAsked[i] = input;
      insert = true;
    }
    else if(MiniGameTwoQuestionsAsked[i] == input && MiniGameTwoQuestionsAsked[i] != "")
    {
      MiniGameTwoQuestionsAsked[i] = input;
      insert = true;
    }
    i++;
  }
};
/////////////           DISPLAYS          /////////////
void GameClass::displaystartmenu()
{
  cout << "=======================" << endl;
  cout << "1. Start A New Game" << endl;
  cout << "2. Continue From An Old Game" << endl;
  cout << "3. Read The Rules" << endl;
  cout << "4. Quit" << endl;
  cout << "=======================" << endl;
};
void GameClass::displaygamemodemenu()
{
  cout << "=======================" << endl;
  cout << "1. Easy (3 Rounds)" << endl;
  cout << "2. Medium (5 Rounds)" << endl;
  cout << "3. Hard (10 Rounds)" << endl;
  cout << "=======================" << endl;
};
void GameClass::displayHighScores()
{
  string highscores_name = "Computer";
  int topscore = 1000;
  if(topscore < stoi(getPlayerStats(1)))
  {
    cout << endl << "Congrats. You have proven yourself quite the Cybersecurity Expert." << endl;
    cout << "As An Award, You can play the final round to keep your high score." << endl;
    cout << "You must answer all the problems perfectly" << endl;
    int subtract = stoi(getPlayerStats(1)) * -1;
    string transfer = to_string(subtract);
    setUpdatedPlayerStats(1, transfer);
    for(int i = 0; i < 10; i++)
    {
      MiniGameOneQuestionsAsked[i] = "";
      MiniGameTwoQuestionsAsked[i] = "";
    }
    for(int i = 0; i < 10; i++)
    {
      cout << endl << getPlayerStats(0) << "'s Current Score: " << getPlayerStats(1) << endl;
      cout << "Round " << (i+1) << ":" << endl;
      cout << "Minigame Generated" << endl;
      minigameselect();
    }
    //setUpdatedPlayerStats(1, "2100");
    if(stoi(getPlayerStats(1)) >= 2000)
    {
      cout << "Congrats. You have beated a computer at cyber sercuity" << endl;
      cout << "You are the new record holder" << endl;
      cout << "Good-bye HackCU" << endl;
      cout << "=======================" << endl;
      cout << "Rank: 1st"<< "   Score: " << stoi(getPlayerStats(1)) << "   Username: " << getPlayerStats(0) << endl;
      cout << "Rank: 2nd"<< "   Score: " << topscore << "   Username: " << highscores_name << " :(" << endl;
      cout << "=======================" << endl;
    }
  }
  else
  {
    cout << "Oh Well. There's Always Next Time" << endl;
    cout << "=======================" << endl;
    cout << "Rank: 1st"<< "   Score: " << topscore << "   Username: " << highscores_name << " :)" << endl;
    cout << "Rank: 2nd"<< "   Score: " << stoi(getPlayerStats(1)) << "   Username: " << getPlayerStats(0) << endl;
    cout << "=======================" << endl;
  };
  writesavefile();
};
/////////////     RUNNING THE GAME     /////////////
void GameClass::minigameselect()
{
  int MinigameSelect = rand() % 2 + 1;  //generates number between 1 and 2
  if(MinigameSelect == 1)
  {
    cout << "Programmer" << endl;
    cout << "Time to test your programming skills" << endl;
    cout << "Type in the code displayed on screen correctly to receive points" << endl;
    bool find_question = false;
    string question;
    while(find_question == false)
    {
      question = GetMGOQuestion();
      //cout << question << endl;
      bool asked_before = getHasMGOQuestionBeenAsked(question);
      if(asked_before == false)
      {
        find_question = true;
      }
    }
    cout << question << endl << "Your Answer: ";
    string answer;
    cin.ignore();
    getline(cin,answer);
    bool correct = isMGOCorrectAnswer(answer, question);
    if(correct == true)
    {
      setUpdatedPlayerStats(1, "100");
    }
    else
    {
      cout << "I'm sorry. Perphas you had a typo cause this answer was not correct." << endl;
      cout << "Be wary of typos in the computer science field." << endl;
    }
    setHasMGOQuestionBeenAsked(question);
  }
  if(MinigameSelect == 2)
  {
    //Concepts Quiz (A,B,C,or,D)
    cout << "Pop Quiz" << endl;
    cout << "It's Multiple Choice. Enter The Correct Answer (A, B, C, or D)" << endl;
    int index;
    string question;
    bool find_question = false;
    while(find_question == false)
    {
      index = GetMGTWOQuestionIndex();
      question = MinigameTwoQuestions[index].question;
      bool asked_before = getHasMGTWOQuestionBeenAsked(question);
      if(asked_before == false)
      {
        find_question = true;
      }
    }
    //cout << MinigameTwoQuestions[index].question_number << endl;
    cout << MinigameTwoQuestions[index].question << ":" << endl;
    //cout << MinigameTwoQuestions[index].answers << endl;
    string split_answers = MinigameTwoQuestions[index].answers;
    int length = split_answers.length();
    char character;
    string select = "";
    string display_answers[4];
    int counter = 0;
    for(int i = 0; i <= length; i++)
    {
      if((split_answers[i] != '\n') && (split_answers[i] != ','))
      {
        character = split_answers[i];
        select = select + character;
        display_answers[counter] = select;
      }
      else
      {
        select = "";
        counter++;
      }
    }
    for(int i = 0; i < 4; i++)
    {
      if(i == 0)
      {
        cout << "Option A: ";
      }
      if(i == 1)
      {
        cout << "Option B: ";
      }
      if(i == 2)
      {
        cout << "Option C: ";
      }
      if(i == 3)
      {
        cout << "Option D: ";
      }
      cout << display_answers[i] << endl;
    }
    string answer;
    cin >> answer;
    //cout << MinigameTwoQuestions[index].correct << endl;
    string correct = MinigameTwoQuestions[index].correct;
    bool right = isMGTWOCorrectAnswer(answer, correct);
    if(right == true)
    {
      setUpdatedPlayerStats(1, "100");
    }
    setHasMGTWOQuestionBeenAsked(question);
  }
};
void GameClass::GameLoop(bool new_or_saved)
{
  string choice;
  int rounds = 0;
  int current_round = 0;
  int integer_check = false;
  if(new_or_saved == true)  //means new game
  {
    cout << "What gamemode would you like to play?" << endl;
    while(integer_check == false)
    {
        cin >> choice;
        integer_check = helper_function_stoicheck(choice);
        if((stoi(choice) >= 1) && (stoi(choice) <= 3))
        {
            integer_check = true;
        }
        else
        {
            integer_check = false;
        }
    }
    if(stoi(choice) == 1)
    {
      rounds = 3;
      setUpdatedPlayerStats(2, "Easy");
    }
    else if(stoi(choice) == 2)
    {
      rounds = 5;
      setUpdatedPlayerStats(2, "Medium");
    }
    else if(stoi(choice) == 3)
    {
      rounds = 10;
      setUpdatedPlayerStats(2, "Hard");
    }
  }
  else
  {
    current_round = stoi(getPlayerStats(3));
    string saved_gamemode = getPlayerStats(2);
    if(saved_gamemode == "Easy")
    {
      rounds = 3;
    }
    else if(saved_gamemode == "Medium")
    {
      rounds = 5;
    }
    else if(saved_gamemode == "Hard")
    {
      rounds = 10;
    }
  }
  for(int i = current_round; i < rounds; i++)
  {
    cout << endl << getPlayerStats(0) << "'s Current Score: " << getPlayerStats(1) << endl;
    cout << "Round " << (i+1) << ":" << endl;
    cout << "Minigame Generated" << endl;
    minigameselect();
    cout << "Would you like to save your progress?" << endl;
    cin >> choice;
    if(choice == "yes")
    {
      writesavefile();
    }
  }
};
void GameClass::startmenu()
{
  //switch case for selecting options
  string choice;
  bool runninggame = true;
  cout << endl;
  while (choice != "4" && runninggame == true){
    cout << "What option would you like to select? ";
    int integer_check = false;
    while (integer_check == false)
    {
        cin >> choice;
        integer_check = helper_function_stoicheck(choice);
        if (integer_check == true)
        {
            integer_check =  true;
        }
    }
      switch (stoi(choice)) {
          case 1:            //rungame
            {
              string name;
              cout << "Enter your username" << endl;
              cin.ignore();
              cin >> name;
              setUpdatedPlayerStats(0, name);
              displaygamemodemenu();
              GameLoop(true);
              runninggame = false;
            }
            break;
          case 2:            //run old save file
            {
              readsavefile("loadsavegametest.txt");
              //displayAnsweredQuestionsLoaded();
              GameLoop(false);
              runninggame = false;
            }
            break;
          case 3:
            {
              cout << "- You can either choice to start a new game or continue from an old game using an old saved file" << endl;
              cout << "- If you choose to start a new game, you can select the amount of rounds you want to play for." << endl;
              cout << "- You received points for correct answers." << endl;
              cout << "- High scores are saved and/or updated for the returning users" << endl;
              cout << "- If you are able to replace the top high score of the computer, you will move forward to final round." << endl;
              cout << "- The ultimate goal is to beat the computer. AnD W3 W0n'T L3T ThAt haPpeN >:)" << endl;
              cout << "- There are multiple minigames that will be randomly generated throughout the game." << endl;
            }
            break;
          case 4:
            cout << "Makes me sad that you don't want to play. Until next time :)" << endl;
            break;
          default:
            cout << "invalid input" << endl << endl;
                            }
        }
};
