#pragma once

#include <SFML/Graphics.hpp>
#include <list>

using namespace sf;
using namespace std;

template<class T>
class GameList: public list<T *>
{
public:
  GameList (void){};
  virtual ~GameList(void){};

public:
  bool contains  (T *elt)
  {
    for (auto *otherElt : *this)
      if (otherElt == elt)
        return true;
  
    return false;
  }
  void add		   (T *elt)
  {
    if (elt == NULL || contains(elt))
		  return;
	  list<T *>::push_back(elt);
  }
};

