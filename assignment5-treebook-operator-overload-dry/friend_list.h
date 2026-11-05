/*
 * Assignment 4: TreeBook
 * FriendList — owns and manages a user's list of friend names.
 * Adapted by Tinkercademy from Stanford CS106L Assignment 5.
 */

#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

/**
 * FriendList stores the names of a user's friends.
 *
 * Rule of Zero: the only member is a std::vector<std::string>, which
 * already manages its own memory, so we declare no destructor, copy
 * constructor, or copy/move assignment operators at all — the
 * compiler-synthesised versions are correct.
 */
class FriendList
{
public:
  void add(const std::string& name);
  void set(size_t index, const std::string& name);
  size_t size() const;

  friend std::ostream& operator<<(std::ostream& os, const FriendList& list);

private:
  std::vector<std::string> _friends;
};
