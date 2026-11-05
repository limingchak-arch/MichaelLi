#include "friend_list.h"

/**
 * Appends a friend name to the list.
 */
void
FriendList::add(const std::string& name)
{
  _friends.push_back(name);
}

/**
 * Sets the friend at the given index to the given name.
 */
void
FriendList::set(size_t index, const std::string& name)
{
  _friends.at(index) = name;
}

/**
 * Returns the number of friends in the list.
 */
size_t
FriendList::size() const
{
  return _friends.size();
}

/**
 * Prints the friend list in the format [Alice, Bob, Charlie].
 */
std::ostream&
operator<<(std::ostream& os, const FriendList& list)
{
  os << "[";
  for (size_t i = 0; i < list._friends.size(); ++i) {
    os << list._friends[i];
    if (i + 1 != list._friends.size()) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}
