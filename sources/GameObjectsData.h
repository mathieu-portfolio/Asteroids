#pragma once

#include "GameObjects.h"
#include <list>

/**
 * @class GameList
 * @brief A templated list class for managing game objects.
 * @tparam T Type of objects stored in the list.
 */
template<class T>
class GameList: public list<T *>
{
public:
  GameList(void) {}
  virtual ~GameList(void) {}

public:
  bool contains(T *elt)
  {
    for (auto *otherElt : *this)
      if (otherElt == elt)
        return true;
    return false;
  }

  void add(T *elt)
  {
    if (elt == NULL || contains(elt))
      return;
    list<T *>::push_back(elt);
  }
};

/**
 * @class ObjectList
 * @brief A templated list for managing objects.
 * @tparam T Type of objects stored in the list.
 */
template<class T>
class ObjectList: public GameList<T>
{
public:
  ObjectList(void) {}
  virtual ~ObjectList(void) {}

public:
  virtual void update(int performances)
  {
    for (auto elt = GameList<T>::begin(); elt != GameList<T>::end(); ) {
      auto nextElt = elt;
      nextElt++;
      auto *object = (RenderedObject *) *elt;
      object->update(performances);
      elt = nextElt;
    }
  }

  virtual void draw(int performances)
  {
    for (auto *elt : *this)
      ((RenderedObject *) elt)->draw(performances);
  }

  virtual void reset(void) {}
};
