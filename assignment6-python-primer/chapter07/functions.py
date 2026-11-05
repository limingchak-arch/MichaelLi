"""Chapter 7: Functions (lessons 7.1-7.7)."""

# 7.1 Defining functions: no return-type/parameter-type declarations
def add(a, b):
    return a + b

assert add(1, 2) == 3
assert add("foo", "bar") == "foobar"     # duck typing: works for any addable

# A function with no return statement returns None
def nothing():
    pass

assert nothing() is None

# 7.2 Positional and default arguments
def greet(name, greeting="Hello"):
    return f"{greeting}, {name}!"

assert greet("Josiah") == "Hello, Josiah!"
assert greet("Josiah", "Hi") == "Hi, Josiah!"

# 7.3 Keyword arguments: any order, self-documenting call sites
def describe(name, code, degree):
    return f"{name} ({code}) - {degree}"

assert describe(code="60012", degree="MSc", name="Josiah") == "Josiah (60012) - MSc"

# 7.4 Built-in functions
assert abs(-3) == 3
assert max(1, 5, 2) == 5
assert min([4, 2, 8]) == 2
assert sorted([3, 1, 2]) == [1, 2, 3]
assert sum([1, 2, 3]) == 6
assert round(3.14159, 2) == 3.14

# 7.5 Functions are objects too: pass them around, store them
def shout(s):
    return s.upper() + "!"

def whisper(s):
    return s.lower() + "..."

def apply(func, value):
    return func(value)

assert apply(shout, "hello") == "HELLO!"
assert apply(whisper, "HELLO") == "hello..."

funcs = [shout, whisper, len]
assert funcs[0]("hi") == "HI!"
assert funcs[2]("hi") == 2

# 7.6 Arbitrary positional arguments: *args collects into a tuple
def total(*args):
    return sum(args)

assert total() == 0
assert total(1, 2, 3, 4) == 10

# 7.7 Arbitrary keyword arguments: **kwargs collects into a dict
def show(**kwargs):
    return kwargs

assert show(a=1, b=2) == {"a": 1, "b": 2}

# Combined: positional, *args, keyword-only, **kwargs
def combined(first, *rest, scale=1, **extra):
    return (first + sum(rest)) * scale, extra

assert combined(1, 2, 3) == (6, {})
assert combined(1, 2, 3, scale=2, tag="x") == (12, {"tag": "x"})

print("chapter07 OK")
