"""Chapter 10: Files - text, JSON, pickle, CSV (lessons 10.1-10.10).

Includes the chapter's explicit exercise ("Pickling time!", lesson 10.6):
pickle a list of Vector objects to vectors.pkl and load it back.
All file I/O happens in a temporary directory so no artifacts are left behind.
"""

import csv
import json
import os
import pickle
import tempfile

tmpdir = tempfile.TemporaryDirectory()
path = lambda name: os.path.join(tmpdir.name, name)


# 10.1 Handling text files: with-block (context manager) closes the file
with open(path("hello.txt"), "w") as file:
    file.write("Hello, Imperial!\nSecond line.\n")

with open(path("hello.txt"), "r") as file:
    content = file.read()
assert content == "Hello, Imperial!\nSecond line.\n"

with open(path("hello.txt"), "r") as file:
    lines = [line.rstrip("\n") for line in file]
assert lines == ["Hello, Imperial!", "Second line."]


# 10.2-10.4 JSON: human-readable, language-independent serialisation
courses = {
    70053: {"lecturer": "Josiah Wang", "title": "Python Programming"},
    70051: {"lecturer": "Robert Craven", "title": "Symbolic AI"},
}

with open(path("courses.json"), "w") as file:
    json.dump(courses, file)

with open(path("courses.json"), "r") as file:
    loaded_courses = json.load(file)

# JSON object keys are always strings, so int keys come back as str
assert loaded_courses == {str(k): v for k, v in courses.items()}
assert loaded_courses["70053"]["title"] == "Python Programming"


# 10.5/10.6 pickle: binary serialisation of arbitrary Python objects.
# === Exercise (lesson 10.6): pickle `vector` to vectors.pkl and load it back ===
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __str__(self):
        return f"Vector ({self.x}, {self.y})"

    def __repr__(self):
        """This makes the unique string representation
        of the object instance look more readable
        """
        return str(self)


vector1 = Vector(2, 3)
vector2 = Vector(4, 3)
vector = [vector1, vector2]

# Save vector to disk. Note binary mode!
with open(path("vectors.pkl"), "wb") as file:
    pickle.dump(vector, file)

# Load pickled file that you saved earlier from disk
with open(path("vectors.pkl"), "rb") as file:
    pickled_vectors = pickle.load(file)

print(pickled_vectors)               # [Vector (2, 3), Vector (4, 3)]
assert pickled_vectors == vector     # round-trip preserves values
assert type(pickled_vectors) is list
assert isinstance(pickled_vectors[0], Vector)

# pickle preserves exact types, unlike JSON (int keys stay int)
with open(path("courses.pkl"), "wb") as file:
    pickle.dump(courses, file)
with open(path("courses.pkl"), "rb") as file:
    pickled_courses = pickle.load(file)
assert pickled_courses == courses
assert 70053 in pickled_courses      # int key survived


# 10.7-10.10 CSV files
rows = [
    ["code", "title"],
    ["60012", "Introduction to Machine Learning"],
    ["60006", "Computer Vision"],
]
with open(path("courses.csv"), "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerows(rows)

with open(path("courses.csv"), "r", newline="") as file:
    reader = csv.reader(file)
    assert list(reader) == rows

with open(path("courses.csv"), "r", newline="") as file:
    dict_reader = csv.DictReader(file)
    as_dicts = list(dict_reader)
assert as_dicts[0] == {"code": "60012", "title": "Introduction to Machine Learning"}
assert as_dicts[1]["title"] == "Computer Vision"

tmpdir.cleanup()
print("chapter10 OK")
