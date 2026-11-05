"""Chapter 3: Variables and operators - everything is an object.

The hands-on theme of the lecture: in Python, all values are objects
(lessons 3.1-3.7 as runnable assertions).
"""

# 3.1 Everything is an object
assert isinstance(2020, object)
assert isinstance(4.33, object)
assert isinstance(5j, object)
assert isinstance(True, object)
assert isinstance("COVID-19", object)

# int(), float(), ... are actually constructors of built-in types
assert int() == 0
assert float() == 0.0
assert complex() == 0j
assert bool() is False
assert str() == ""

assert int(-9.789) == -9          # truncation towards zero, like C++ cast
assert float(9) == 9.0
import math
assert math.isnan(float("nan"))
assert float("inf") == math.inf
assert float("-inf") == -math.inf
assert complex(1, 2) == 1 + 2j
assert bool(1) is True
assert bool("") is False
assert bool(5) is True
assert str("Hello") == "Hello"
assert str(10) == "10"

# 3.3/3.4 Variables are names bound to objects, not typed boxes.
# A name can be rebound to an object of a different type (dynamic typing).
x = 42
assert isinstance(x, int)
x = "forty-two"
assert isinstance(x, str)

# 3.5 Every value is an object instance with methods
assert (10).bit_length() == 4
assert "hello".upper() == "HELLO"

# 3.6 Operators: / is true division, // is floor division,
# and there is no ++/-- in Python
a, b = 7, 2
assert a / b == 3.5
assert a // b == 3
assert a % b == 1
assert a ** b == 49

# 3.7 Assignment binds a name to an object; aliasing shares the object
list_a = [1, 2, 3]
list_b = list_a          # both names refer to the SAME object
list_b.append(4)
assert list_a == [1, 2, 3, 4]
assert list_a is list_b

# Augmented assignment
n = 10
n += 5
assert n == 15
n *= 2
assert n == 30

print("chapter03 OK")
