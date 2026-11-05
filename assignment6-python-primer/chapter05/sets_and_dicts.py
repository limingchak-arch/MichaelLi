"""Chapter 5: Sets and dictionaries (lessons 5.1-5.8)."""

# 5.1/5.2 Sets: unordered, unique elements, ~std::unordered_set
seen = {1, 2, 2, 3, 3, 3}
assert seen == {1, 2, 3}
assert len({1, 1, 1}) == 1

# 5.3 Set operations
a = {1, 2, 3, 4}
b = {3, 4, 5, 6}
assert a | b == {1, 2, 3, 4, 5, 6}      # union
assert a & b == {3, 4}                  # intersection
assert a - b == {1, 2}                  # difference
assert a ^ b == {1, 2, 5, 6}            # symmetric difference
assert {3, 4} < a                       # proper subset

# 5.4 Set methods
s = {1, 2}
s.add(3)
s.discard(99)       # discard does not raise, remove would
assert s == {1, 2, 3}

# 5.5/5.6 Dictionaries: key -> value, ~std::unordered_map
student = {
    "name": "Josiah",
    "id": "00-02-11",
    "degree": "MSc Computing",
}
assert student["name"] == "Josiah"
assert student.get("year") is None           # no KeyError with .get
assert student.get("year", 1) == 1           # default value
assert "degree" in student

# 5.7 Dictionary methods
assert sorted(student.keys()) == ["degree", "id", "name"]
assert ("name", "Josiah") in student.items()
student["year"] = 1
assert student.pop("year") == 1 and "year" not in student

# 5.8 Grouping data with dict (dict as a flexible C++ struct) + nesting
student = {
    "name": "Josiah",
    "id": "00-02-11",
    "degree": "MSc Computing",
    "courses": [
        {"title": "Introduction to Machine Learning", "code": "60012"},
        {"title": "Computer Vision", "code": "60006"},
    ],
}
assert len(student["courses"]) == 2
assert student["courses"][0]["title"] == "Introduction to Machine Learning"
assert student["courses"][0]["code"] == "60012"
assert student["courses"][1]["code"] == "60006"

# Same record grouped with tuples
course1 = ("60012", "Introduction to Machine Learning")
course2 = ("60006", "Computer Vision")
student_record = ("Josiah", "00-02-11", "MSc Computing", [course1, course2])
assert student_record[1] == "00-02-11"
assert student_record[3][0] == course1
assert student_record[3][1][1] == "Computer Vision"

print("chapter05 OK")
