#include "Application.h"
#include "GameText.h"
#include <fstream>
#include <iostream>


#define MAX_NAME_LENGTH 20

GameText *scoreText;
GameText *healthText;
GameText *timerText;
GameText *gameOverText;
GameText *menuText;
GameText *difficultyText;
GameText *FPSText;
GameText *leaderboardText;
GameText *namesText;
GameText *scoresText;
GameText *nameText;

TextList menuTexts;
TextList gameTexts;
TextList gameOverTexts;


Application *Application::app = NULL;

Application::Application              (void) :
  currentMode           (WindowMode::Game),
  lastMode              (currentMode),
  difficulty            (1),
  performances          (1),
  currentName           (""),
  window                (&GameWindow::window()),
  data                  (&GameData::data()),
  clock                 (&GameClock::clock()),
  asteroidsObjects      (&AsteroidsObjects::objects()),
  player                (&Player::access())
{
  //MENU
  menuText = new GameText(
    "MENU",
    150,
    Color(255, 255, 180)
  );
  difficultyText = new GameText(
    "Difficulty : ",
    50
  );
  leaderboardText = new GameText(
    "LEADERBOARD",
    50,
    Color(255, 180, 100)
  );
  namesText = new GameText(
    "Toto",
    35,
    Color(255, 220, 160)
  );
  scoresText = new GameText(
    "0",
    35,
    Color(255, 220, 160)
  );
  menuTexts.add(menuText);
  menuTexts.add(difficultyText);
  menuTexts.add(namesText);
  menuTexts.add(scoresText);
  
  //GAME
  scoreText = new GameText(
    "Score : 0",
    35,
    Color(255, 255, 200)
  );
  healthText = new GameText(
    "Health : 0",
    35,
    Color(255, 200, 255)
  );
  timerText = new GameText(
    "Time : 0",
    35,
    Color(200, 255, 255)
  );
  gameTexts.add(scoreText);
  gameTexts.add(healthText);
  gameTexts.add(timerText);

  //GAMEOVER
  gameOverText = new GameText(
    "GAME OVER",
    150,
    Color(255, 200, 200)
  );
  nameText = new GameText(
    "Name : Toto",
    50,
    Color::White
  );
  gameOverTexts.add(gameOverText);
  gameOverTexts.add(scoreText);
  gameOverTexts.add(timerText);
  gameOverTexts.add(nameText);

  //ALL
  FPSText = new GameText(
    "FPS : 60",
    35,
    Color(150, 150, 150)
  );

  //asynchronous storage
  getSavedData();
}

void Application::initialize          (void)
{
  //GAME
  menuText->center(0.5f, 0.33f);
  menuText->centerContent();
  difficultyText->alignTo(*menuText, 0.f, 100.f);
  leaderboardText->alignTo(*difficultyText, 0.f, 100.f);
  namesText->alignTo(*leaderboardText, -20.f, 40.f);
  scoresText->alignTo(*leaderboardText, 20.f, 40.f);

  // GAMEOVER
  gameOverText->centerContent();
  gameOverText->center(0.5f, 0.33f);

  //ALL
  FPSText->centerContent(1.f, 0.f);
  FPSText->center(1.f, 0.f);
  FPSText->move(20.f * Vector2f(-1.f, 1.f));
}

void Application::setMode             (const WindowMode &mode)
{
  lastMode    = currentMode;
  currentMode = mode;
}
 
Application &Application::application (void)
{
  if (app == NULL)
    app = new Application();
  return *app;
}

bool sortByVal(
  const pair<int, string> &a,
  const pair<int, string> &b
) 
{ 
  return (a.first > b.first); 
}

void Application::changeScreen        (const WindowMode &mode)
{
  auto windowSize = (Vector2f) window->getSize();

  if (mode == WindowMode::Menu)
  {
    // configure UI
    Color difficultyColors[] {
      Color(255, 255, 180),
      Color(255, 180, 100),
      Color(255, 100, 100)
    };
    difficultyText->setFillColor(difficultyColors[difficulty - 1]);
    difficultyText->setString("Difficulty : " + to_string(difficulty));
    
    string scoresString = "";
    string namesString  = "";
    sort(scores.begin(), scores.end(), sortByVal);
    auto maxIdx = min((int) scores.size(), 10);
    for (int i = 0; i < maxIdx; i++) {
      auto name     = scores[i].second;
      auto score    = to_string(scores[i].first);
      namesString  += name  + "\n";
      scoresString += score + "\n";
    }
    namesText->setString (namesString);
    scoresText->setString(scoresString);
    difficultyText->centerContent();
    auto namesWidth           = namesText->getLocalBounds().width;
    auto scoresWidth          = scoresText->getLocalBounds().width;
    auto namesTop             = namesText->getPosition().y;
    auto offset               = 0.5f * (namesWidth - scoresWidth);
    namesText->centerContent(1.f, 0.f);
    namesText->setPosition     (Vector2f(0.5f * windowSize.x + offset - 20.f, namesTop));
    scoresText->setPosition    (Vector2f(0.5f * windowSize.x + offset + 20.f, namesTop));
  }
  else if (mode == WindowMode::Game)
  {
    // set seed
    time_t now = time(0);
    struct tm newtime;
    #ifdef _WIN32
      localtime_s(&newtime, &now);
    #endif
    #ifdef __unix__
      localtime_r(&now, &newtime);
    #endif
    srand(newtime.tm_min * 60 + newtime.tm_sec);

    // reset entities
    data->reset();
    data->setDifficulty(difficulty);
    clock->reset();
    AsteroidsObjects::objects().reset();
    player->reset();

    // configure UI
    scoreText->setCharacterSize(25);
    healthText->setCharacterSize(25);
    timerText->setCharacterSize(25);
    scoreText->center(0.f, 0.f, 20.f, 20.f);
    scoreText->centerContent(0.f, 0.f);
    healthText->alignTo(*scoreText, 0.f, 10.f);
    timerText->alignTo(*healthText, 0.f, 10.f);

    currentName = "";
  }
  else if (mode == WindowMode::GameOver) 
  {
    // configure UI
    scoreText->setCharacterSize(50);
    scoreText->setFillColor(Color(255, 255, 200));
    
    timerText->setCharacterSize(50);
    timerText->setFillColor(Color(200, 255, 255));

    scoreText->alignTo(*gameOverText, 0.f, 100.f);
    timerText->alignTo(*scoreText, 0.f, 40.f);
    if (timerText->positionRelativeToWindow().y > 0.5f)
      nameText->alignTo(*timerText, 0.f, 50.f);
    else {
      nameText->center(0.5f, 0.66f);
      nameText->centerContent();
    }
  }
  
  currentMode = mode;
  lastMode    = currentMode;
}

void Application::quit                (void)
{
  if (currentMode == WindowMode::Game)
    saveData(pair<int, string>(data->getScore(), ""));
  window->close();
}

void Application::inputManager        (void)
{
  while (window->pollEvent(event))
  {
    if(event.type == Event::Closed)
     quit();
          
    if(event.type == Event::KeyPressed) {
      if(event.key.code ==Keyboard::Escape) {
        quit();
      }

      if(event.key.code ==Keyboard::A) {
        if (currentMode == WindowMode::Debug)
          AsteroidsObjects::objects().getAsteroids().spawn(performances);
      }

      if(event.key.code ==Keyboard::Space) {
        if (currentMode == WindowMode::Game)
          player->spaceBar();
      }

      if(event.key.code ==Keyboard::Up) {
        if (currentMode == WindowMode::Menu) {
          difficulty = min(difficulty + 1, 3);
          changeScreen(WindowMode::Menu);
        }
      }

      if(event.key.code ==Keyboard::Down) {
        if (currentMode == WindowMode::Menu) {
          difficulty = max(1, difficulty - 1);
          changeScreen(WindowMode::Menu);
        }
      }

      if(event.key.code == Keyboard::Backspace) {
        if(currentMode == WindowMode::GameOver) {
          auto separation = (size_t) (currentName.length() - 1);
          currentName = currentName.substr (0, separation);
        }
      }

      if(event.key.code == Keyboard::Enter) {
        if(currentMode == WindowMode::GameOver) {
          auto newPair = pair<int, string>(data->getScore(), currentName);
          scores.push_back(newPair);
          saveData(newPair);
          changeScreen();
        }
      }
    }

    if(event.type == Event::TextEntered) {
      auto unicode = event.text.unicode;
      if(unicode < 128) {
        if(currentMode == WindowMode::GameOver) {
          unsigned int avoidValues[] = {
            0x0000003A,           // colon
            0x00000008,           // backspace
            0x0000000D            // return
          };
          auto enterText = currentName.length() < MAX_NAME_LENGTH;
          for (const auto &value : avoidValues)
            if (unicode == value) {
              enterText = false;
              break;
            }
          if (enterText) {
            currentName += (char) unicode;
          }
        }
      }
    }

    if(event.type == Event::MouseButtonPressed) {
      if (event.mouseButton.button == Mouse::Right) {
        player->rightClick();
      }

      if (event.mouseButton.button == Mouse::Left) {
        if (currentMode == WindowMode::Menu)
          changeScreen(WindowMode::Game);

        if (currentMode == WindowMode::Game)
          player->leftClick();
      }
    }

    if(event.type == Event::MouseButtonReleased) {
      if (event.mouseButton.button == Mouse::Left) {
        if (currentMode == WindowMode::Game)
          player->leftClick(false);
      }
    }
  }
}

bool is_number(const string &s) 
{
  return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

void Application::getSavedData  (void)
{
  scores.clear();

  string line;
  ifstream myfile ("./data/Scores.txt");
  if (myfile.is_open())
  {
    while (getline (myfile, line))
    {
      auto separation = line.find(":");
      if ((int) separation == -1)
        continue;
      auto name     = line.substr (0, separation);
      auto scoreStr = line.substr (separation + 1, line.length());
      if (!is_number(scoreStr))
        continue;
      auto score    = stoi (scoreStr);
      scores.push_back(pair<int, string>(score, name));
    }
    myfile.close();
  }
  else
    cout << "Unable to open file" << endl;
}

void Application::saveData      (const pair<int, string> &data)
{
  ofstream myfile;
  myfile.open("./data/Scores.txt", fstream::app);
  if (myfile.is_open())
  {
    auto name  = data.second;
    auto score = data.first;
    if (name == "")
      name = "???";
    myfile << name << ":" << score << endl;
  }
  else
    cout << "Unable to open file" << endl;
}

void Application::update              (void)
{
  if (currentMode == WindowMode::Menu ) {
  }

  else if (currentMode == WindowMode::Game ) {
    // update objects
    if ( player->getHealth() == 0 )
      setMode( WindowMode::GameOver );
    player->moveMouse();
    asteroidsObjects->update(performances);

    // update UI
    auto score  = data->getScore();
    auto health = player->getHealth();
    auto time   = to_string((clock->getCurrentUpdate() - clock->getStartingTime()).asSeconds());
    scoreText->setString ( "Score  : " + to_string (score) );
    healthText->setString( "Health : " + to_string (health) );
    timerText->setString ( "Time   : " + time.substr ( 0, time.find(".") + 3 ) );
  }

  else if (currentMode == WindowMode::GameOver ) {
    if ( lastMode != WindowMode::GameOver ) {
      changeScreen ( WindowMode::GameOver );
      lastMode = WindowMode::GameOver;
    }
    nameText->setString( "Name : " + currentName );
    nameText->centerContent();
  }
}

void Application::draw                (void)
{
  window->clear();

  switch (currentMode)
  {
    case (WindowMode::Menu):
      menuTexts.draw();
      if (scores.size() > 0)
        leaderboardText->draw();
      break;

    case (WindowMode::Game):
      asteroidsObjects->draw(performances);
      gameTexts.draw();
      break;

    case (WindowMode::GameOver):
      gameOverTexts.draw();
      break;

    default:
      break;
  }

  FPSText->draw();

  window->display();
}

int Application::compute              (void)
{
  ContextSettings settings;
  settings.antialiasingLevel = 8;

  initialize();
  changeScreen();

  const unsigned int FPS         = 60;
  const unsigned int RPS         = 60;
  const unsigned int maxFPS      = 120;
  const int frameDelay  = 1000 / FPS;
  const int renderDelay = 1000 / RPS;

  Time  frameStart;

  while (window->isOpen())
  {
    frameStart = clock->getElapsedTime();
    unsigned int updateElapsedTime = (frameStart - clock->getCurrentUpdate()).asMilliseconds();
    unsigned int renderElapsedTime = (frameStart - clock->getCurrentRender()).asMilliseconds();

    // update
    if (updateElapsedTime > frameDelay) {
      update();
      inputManager();
      clock->setCurrentUpdate();

      if(1000 / updateElapsedTime > maxFPS)
        performances = 0;
      else
        performances = 1;

      auto elapsedFrames = 1000 / updateElapsedTime;
      FPSText->setString("FPS : " + to_string(elapsedFrames));
    }

    // render
    if (renderElapsedTime > renderDelay) {
      draw();
      clock->setCurrentRender();
    }
  }

  return 0;
}



int main (void)
{
  auto &application = Application::application();

  sf::Font arial;
  if (!arial.loadFromFile("./fonts/arial.ttf"))
    perror("font error");
  else {
    scoreText->setFont         (arial);
    healthText->setFont        (arial);
    timerText->setFont         (arial);
    gameOverText->setFont      (arial);
    menuText->setFont          (arial);
    difficultyText->setFont    (arial);
    FPSText->setFont           (arial);
    leaderboardText->setFont   (arial);
    namesText->setFont         (arial);
    scoresText->setFont        (arial);
    nameText->setFont          (arial);
  }

  application.compute();
}
