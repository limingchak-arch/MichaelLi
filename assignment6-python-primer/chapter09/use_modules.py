"""Chapter 9: Modules - using standard-library and custom modules."""

# 9.1 Standard library modules: import then use via the dot operator
import math
assert math.sqrt(9) == 3.0
assert math.log10(100) == 2.0
assert abs(math.cos(30 * math.pi / 180) - math.sqrt(3) / 2) < 1e-9

# Renaming on import
import math as m
assert m.sqrt(16) == 4.0

# Importing specific names into the current namespace
from math import sqrt, pi
assert sqrt(25) == 5.0
assert pi > 3.14

# 9.2 Custom modules: import our own geometry.py sitting in this folder
import os
import sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import geometry
assert geometry.PI == 3.14159
assert geometry.circle_area(1) == 3.14159
assert geometry.circle_circumference(1) == 2 * 3.14159

from geometry import circle_area
assert circle_area(2) == 3.14159 * 4

# 9.3 __name__ is the module name when imported, "__main__" when run directly
assert geometry.__name__ == "geometry"
assert __name__ == "__main__"

print("chapter09 OK")
