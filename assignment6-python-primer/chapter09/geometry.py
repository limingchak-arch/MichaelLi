"""Chapter 9: Modules - a custom module (lesson 9.2).

A module is just a .py file; importing it runs its top-level code and
binds its names. __name__ is "__main__" only when run directly (9.3),
which is the Python analogue of a C++ main() guarded from library use.
"""

PI = 3.14159


def circle_area(radius):
    return PI * radius ** 2


def circle_circumference(radius):
    return 2 * PI * radius


if __name__ == "__main__":
    # Only runs when executed as a script, not when imported.
    print(f"area(1) = {circle_area(1)}")
