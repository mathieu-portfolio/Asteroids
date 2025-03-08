#pragma once

#include <SFML/Graphics.hpp>
#include <list>

using namespace sf;
using namespace std;

/**
 * @class GameList
 * @brief A templated list class for managing game objects.
 * @tparam T Type of objects stored in the list.
 */
template<class T>
class GameList: public list<T *>
{
public:
  /**
   * @brief Default constructor.
   */
  GameList(void) {}

  /**
   * @brief Destructor.
   */
  virtual ~GameList(void) {}

  /**
   * @brief Checks if the list contains a specific object.
   * @param elt Pointer to the object.
   * @return True if the object exists in the list, false otherwise.
   */
  bool contains(T *elt)
  {
    for (auto *otherElt : *this)
      if (otherElt == elt)
        return true;
  
    return false;
  }

  /**
   * @brief Adds an object to the list if it's not already present.
   * @param elt Pointer to the object.
   */
  void add(T *elt)
  {
    if (elt == NULL || contains(elt))
      return;
    list<T *>::push_back(elt);
  }
};
