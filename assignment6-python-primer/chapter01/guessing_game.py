"""Chapter 1: Introduction - example Python program.

The guessing game from lesson 1.3, written as a script (no main function,
no semicolons, indentation instead of braces). The game logic is factored
into play() so it can be tested non-interactively.
"""


def play(secret_number, guesses, max_guesses=5):
    """Play the guessing game against an iterable of guesses.

    Returns "Correct" if any of the first max_guesses guesses matches
    secret_number, otherwise "Incorrect. Game over."
    """
    guesses = iter(guesses)
    num_of_guesses = 1
    user_guess = next(guesses)

    while user_guess != secret_number and num_of_guesses < max_guesses:
        print("Incorrect.")
        user_guess = next(guesses)
        num_of_guesses = num_of_guesses + 1

    if user_guess == secret_number:
        return "Correct"
    return "Incorrect. Game over."


def main():
    secret_number = 42

    num_of_guesses = 1

    # Read in user's guess as an integer
    user_guess = int(input("Please enter a number: "))

    while user_guess != secret_number and num_of_guesses < 5:
        print("Incorrect.")
        user_guess = int(input("Please enter a number: "))
        num_of_guesses = num_of_guesses + 1

    if user_guess == secret_number:
        print("Correct")
    else:
        print("Incorrect. Game over.")


if __name__ == "__main__":
    main()


# --- self-test ---
assert play(42, [42]) == "Correct"
assert play(42, [1, 2, 3, 42]) == "Correct"
assert play(42, [1, 2, 3, 4, 5]) == "Incorrect. Game over."
print("chapter01 OK")
