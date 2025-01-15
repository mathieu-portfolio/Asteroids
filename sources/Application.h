#pragma once

#include "Player.h"
#include "AsteroidsObjects.h"


enum class WindowMode { GameOver, Game, Menu, Options, Debug };

class Application
{
protected:
  WindowMode                  currentMode;
  WindowMode                  lastMode;
  Event                       event;
  int                         difficulty;
  int                         performances;
  string                      currentName;
  vector<pair<int, string>>   scores;

  GameWindow          *window;
  GameData            *data;
  GameClock           *clock;
  AsteroidsObjects    *asteroidsObjects;
  Player              *player;

public:
  static  Application *app;

  Application(void);

  static Application &application(void);

protected:
  void    initialize        (void);
  void    setMode           (const WindowMode &mode);
  void    changeScreen      (const WindowMode &mode = WindowMode::Menu);
  void    quit              (void);
  void    inputManager      (void);
  void    getSavedData      (void);
  void    saveData          (const pair<int, string> &data);
  void    update            (void);
  void    draw              (void);
  
public:
  int     compute       (void);
};
