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

int main()
{
  cout << "Welcome To The CyberHack Demo. \nAre you ready to play our little game? >;)" << endl;
  string input;
  cout << "Yes Or No: ";
  cin >> input;
  cout << endl;
  if(input != "yes" && input != "Yes" && input != "YES" && input != "Y" && input != "y")
  {
      cout << "Ar3 Y0U $URE! :(" << endl;
  }
  bool playinggame = true;
  GameClass rungame;
  srand (time(NULL));
  //while(playinggame == true)
  //{
    rungame.readminigameonecodes("minigameonequestionsanswers.txt");
    rungame.readminigametwocodes("minigametwoquestionsanswers.txt");
    rungame.displaystartmenu();
    rungame.startmenu();
    rungame.writesavefile();
    rungame.displayHighScores();
    /*
    cout << "Would you like to play again" << endl;
    cin >> input;
    if(input != "yes" && input != "Yes" && input != "YES" && input != "Y" && input != "y")
    {
        playinggame = false;
    }
    */
  //}
  return 0;
};
