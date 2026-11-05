"""Chapter 8: Object-oriented programming (lessons 8.1-8.11)."""

import math


# 8.1/8.2/8.3/8.4 Class, constructor (__init__), attributes, methods
class Vector:
    """2D vector, mirroring the course example."""

    # 8.11 Class attribute: shared by all instances
    dimension = 2

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def norm(self):
        return math.sqrt(self.x ** 2 + self.y ** 2)

    # 8.5 Magic/dunder methods make custom classes act like built-ins
    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar):
        return Vector(self.x * scalar, self.y * scalar)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __lt__(self, other):
        return self.norm() < other.norm()

    def __len__(self):
        return self.dimension

    def __str__(self):
        return f"Vector({self.x}, {self.y})"

    def __repr__(self):
        return str(self)

    # 8.11 Class and static methods
    @classmethod
    def zero(cls):
        return cls(0, 0)

    @staticmethod
    def dot(v1, v2):
        return v1.x * v2.x + v1.y * v2.y


v1 = Vector(1, 2)
v2 = Vector(5, 7)
v3 = v1 + v2                       # invokes v1.__add__(v2)
assert (v3.x, v3.y) == (6, 9)
assert str(v3) == "Vector(6, 9)"
assert v1 + v2 == Vector(6, 9)
assert v2 - v1 == Vector(4, 5)
assert v1 * 3 == Vector(3, 6)
assert Vector(3, 4).norm() == 5.0
assert Vector(1, 0) < Vector(0, 2)     # __lt__ via norm
assert len(v1) == 2                    # __len__
assert Vector.zero() == Vector(0, 0)   # classmethod
assert Vector.dot(v1, v2) == 1 * 5 + 2 * 7  # staticmethod
assert v1.dimension == 2


# 8.6 Inheritance
class Person:
    def __init__(self, name):
        self.name = name

    def introduce(self):
        return f"I am {self.name}"


class Student(Person):
    def __init__(self, name, degree):
        super().__init__(name)      # call base constructor
        self.degree = degree

    def introduce(self):            # override
        return f"{super().introduce()}, studying {self.degree}"


s = Student("Josiah", "MSc Computing")
assert isinstance(s, Person)
assert s.introduce() == "I am Josiah, studying MSc Computing"


# 8.7-8.10 Encapsulation the Pythonic way:
# no true private; single underscore = convention, double underscore = name mangling
class BankAccount:
    def __init__(self, balance):
        self._balance = balance          # "protected" by convention

    @property
    def balance(self):                   # getter via @property
        return self._balance

    @balance.setter
    def balance(self, amount):           # setter with validation
        if amount < 0:
            raise ValueError("balance cannot be negative")
        self._balance = amount


acc = BankAccount(100)
assert acc.balance == 100
acc.balance = 250
assert acc.balance == 250
try:
    acc.balance = -1
    raise AssertionError("negative balance must be rejected")
except ValueError:
    pass

print("chapter08 OK")
