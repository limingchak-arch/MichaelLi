"""Chapter 6: Control flow (lessons 6.1-6.7)."""

# 6.1 if/elif/else - indentation, not braces, defines blocks
def grade(mark):
    if mark >= 70:
        return "first"
    elif mark >= 60:
        return "2:1"
    elif mark >= 50:
        return "2:2"
    else:
        return "fail"

assert grade(75) == "first"
assert grade(65) == "2:1"
assert grade(55) == "2:2"
assert grade(10) == "fail"

# 6.2 Loops: for iterates over sequences directly (no index bookkeeping)
total = 0
for x in [1, 2, 3, 4]:
    total += x
assert total == 10

# while + break/continue
count = 0
while True:
    count += 1
    if count == 3:
        continue
    if count >= 5:
        break
assert count == 5

# 6.3 range: lazy arithmetic sequence
assert list(range(5)) == [0, 1, 2, 3, 4]
assert list(range(1, 6)) == [1, 2, 3, 4, 5]
assert list(range(0, 10, 3)) == [0, 3, 6, 9]
assert sum(range(101)) == 5050

# 6.4 enumerate: index + element pairs
letters = ["a", "b", "c"]
assert list(enumerate(letters)) == [(0, "a"), (1, "b"), (2, "c")]
assert list(enumerate(letters, start=1)) == [(1, "a"), (2, "b"), (3, "c")]

# 6.5 zip: iterate sequences in parallel
codes = [60012, 60006]
titles = ["ML", "CV"]
assert list(zip(codes, titles)) == [(60012, "ML"), (60006, "CV")]
assert dict(zip(codes, titles)) == {60012: "ML", 60006: "CV"}

# 6.6 List comprehension: map/filter in one expression
squares = [x * x for x in range(10)]
assert squares == [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]
evens = [x for x in range(20) if x % 2 == 0]
assert evens == list(range(0, 20, 2))

# 6.7 Set and dict comprehensions
assert {x % 3 for x in range(10)} == {0, 1, 2}
square_map = {x: x * x for x in range(5)}
assert square_map[4] == 16 and len(square_map) == 5

print("chapter06 OK")
