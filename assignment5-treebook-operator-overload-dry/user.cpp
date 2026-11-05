#include "user.h"

/**
 * Creates a new User with the given name and no friends.
 */
User::User(const std::string& name)
  : _name(name)
{
}

/**
 * Adds a friend to this User's list of friends.
 * @param name The name of the friend to add.
 */
void
User::add_friend(const std::string& name)
{
  _friends.add(name);
}

/**
 * Returns the name of this User.
 */
std::string
User::get_name() const
{
  return _name;
}

/**
 * Returns the number of friends this User has.
 */
size_t
User::size() const
{
  return _friends.size();
}

/**
 * Sets the friend at the given index to the given name.
 * @param index The index of the friend to set.
 * @param name The name to set the friend to.
 */
void
User::set_friend(size_t index, const std::string& name)
{
  _friends.set(index, name);
}

/**
 * Adds rhs to this user's friend list, and this user to rhs's list
 * (friending is symmetric). Returns a reference to this user.
 */
User&
User::operator+=(User& rhs)
{
  add_friend(rhs._name);
  rhs.add_friend(_name);
  return *this;
}

/**
 * Compares users alphabetically by name.
 */
bool
User::operator<(const User& other) const
{
  return _name < other._name;
}

/**
 * Prints a user in the format:
 *   User(name=Alice, friends=[Bob, Charlie])
 */
std::ostream&
operator<<(std::ostream& os, const User& user)
{
  return os << "User(name=" << user._name << ", friends=" << user._friends << ")";
}
