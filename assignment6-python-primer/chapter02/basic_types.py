"""Chapter 2: Basic data types - numbers, booleans, strings.

Covers the "try these" REPL examples from lessons 2.1-2.5 as runnable
assertions.
"""

# 2.1/2.2 Numbers: int, float, complex
i = 2020
f = 3.2
c = 4 + 5j
assert isinstance(i, int)
assert isinstance(f, float)
assert isinstance(c, complex)

# Integer division vs float division (unlike C++, / is always float)
assert 7 / 2 == 3.5
assert 7 // 2 == 3
assert 7 % 2 == 1
assert 2 ** 10 == 1024

# 2.3 Booleans: bool is a subclass of int; True == 1, False == 0
assert isinstance(True, bool)
assert True + True == 2
assert True == 1 and False == 0

# Truthiness: empty containers and zero are falsy
assert bool(0) is False
assert bool("") is False
assert bool([]) is False
assert bool(5) is True
assert bool("COVID-19") is True

# 2.4 Strings: immutable, single or double quotes are equivalent
s = "Imperial"
assert s[0] == "I"
assert s[-1] == "l"
assert len(s) == 8
assert "per" in s

# Strings are immutable: item assignment raises TypeError
try:
    s[0] = "i"
    raise AssertionError("strings must be immutable")
except TypeError:
    pass

# 2.5 Multiline strings with triple quotes
poem = """Roses are red,
Violets are blue."""
assert poem.count("\n") == 1
assert poem.startswith("Roses")

print("chapter02 OK")
