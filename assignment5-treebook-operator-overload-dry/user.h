/*
 * Assignment 4: TreeBook
 * Adapted by Tinkercademy from Stanford CS106L Assignment 5
 * (originally by Fabio Ibanez, with modifications by Jacob Roberts-Baca).
 */

#pragma once

#include <iostream>
#include <string>

#include "friend_list.h"

class User
{
public:
  User(const std::string& name);
  void add_friend(const std::string& name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string& name);

  /* Special member functions: User no longer manages any raw resources
   * (all memory lives inside FriendList's std::vector), so the destructor,
   * copy constructor and copy assignment are defaulted. Moves are still
   * deleted: a User represents a person's identity on the network, so we
   * deliberately forbid moving them (copies are allowed). */
  ~User() = default;
  User(const User& user) = default;
  User& operator=(const User& user) = default;
  User(User&& user) = delete;
  User& operator=(User&& user) = delete;

  // Non member function using the friend keyword
  friend std::ostream& operator<<(std::ostream& os, const User& user);
  bool operator<(const User& other) const;
  User& operator+=(User& rhs);

private:
  std::string _name;
  FriendList _friends;
};
