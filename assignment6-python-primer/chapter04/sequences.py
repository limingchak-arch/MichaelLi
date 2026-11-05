"""Chapter 4: Sequence types - lists, tuples, strings (lessons 4.1-4.9)."""

# 4.1/4.2 Lists: heterogeneous, mutable, ~std::vector
modules = [60012, 60006, 70053, "Python Programming"]
assert modules[0] == 60012
assert modules[-1] == "Python Programming"   # negative indexing
assert len(modules) == 4

# 4.3 Slicing: list[start:stop:step]
nums = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
assert nums[2:5] == [2, 3, 4]
assert nums[:3] == [0, 1, 2]
assert nums[7:] == [7, 8, 9]
assert nums[::2] == [0, 2, 4, 6, 8]
assert nums[::-1] == [9, 8, 7, 6, 5, 4, 3, 2, 1, 0]  # reverse
assert nums[:] is not nums and nums[:] == nums        # slice copies a list

# 4.4 Modifying lists
courses = ["ML", "CV"]
courses.append("NLP")
assert courses == ["ML", "CV", "NLP"]
courses.insert(0, "Python")
assert courses[0] == "Python"
courses.remove("CV")
assert courses == ["Python", "ML", "NLP"]
popped = courses.pop()
assert popped == "NLP" and courses == ["Python", "ML"]

# 4.5 List operators
assert [1, 2] + [3] == [1, 2, 3]
assert [0] * 3 == [0, 0, 0]
assert 2 in [1, 2, 3]

# 4.6 Tuples: immutable sequences, ~std::tuple
course = (60012, "Introduction to Machine Learning")
assert course[0] == 60012
assert course[1].startswith("Introduction")
code, title = course        # tuple unpacking
assert code == 60012 and title.endswith("Learning")
try:
    course[0] = 60000
    raise AssertionError("tuples must be immutable")
except TypeError:
    pass

# 4.7 Strings are sequences too
s = "Imperial"
assert s[1:4] == "mpe"
assert s[::-1] == "lairepmI"

# 4.8 Formatting strings: f-strings
name, year = "Josiah", 2020
assert f"{name} teaches Python in {year}" == "Josiah teaches Python in 2020"
assert f"{1/3:.2f}" == "0.33"
assert "{}-{}".format(60012, "ML") == "60012-ML"

# 4.9 Escaping strings
assert "it\'s" == "it's"
assert "a\nb".count("\n") == 1
assert "C:\\path" == "C:" + chr(92) + "path"
assert r"C:\path" == "C:" + chr(92) + "path"   # raw string

print("chapter04 OK")
