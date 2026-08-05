# Run 1 of Tetris terminal

A tetrimino
pieces used in Tetris.
they officially call it that because
from “tetra” meaning four and “mino” meaning block.

Yes, with one small clarification.

"Tetromino" is formed from:

tetra- (Greek téssares / tetra) = four
-omino, borrowed from domino

So it does not literally come from a word meaning "block." Instead, the name was modeled after domino.

The naming progression is:

Monomino = 1 square
Domino = 2 connected squares
Tromino = 3 connected squares
Tetromino = 4 connected squares
Pentomino = 5 connected squares
Hexomino = 6 connected squares

The suffix -omino became the standard mathematical naming convention for connected square shapes after "domino" was already an established word.

So the statement:

"from 'tetra' meaning four and 'mino' meaning block."

is not quite true. A more accurate version is:

"Tetromino" comes from the Greek prefix tetra- ('four') combined with the -omino suffix derived from domino, denoting a connected polyomino shape made of four squares.

knowing the data types:

- `char` represents an ordinary character.
- `string` stores a sequence of `char` values.
- `wchar_t` represents a wide character.

Not “one more space” in the usual sense.

A wide character is a character type that can represent larger Unicode code points, usually with a size of 2 or 4 bytes instead of 1 byte for a normal char. In practice:

char is for ordinary single-byte text
wchar_t is for wide text, often used for Unicode characters

So the difference is mostly about compatibility and character support, not about adding an extra blank space. A wide character can represent characters that a normal char cannot, such as accented letters, non-Latin scripts, or some special symbols.

In your example, the important part is:

normal string literal: "Hello"
wide string literal: L"Hello"
The L just tells C++ to make it a wide-character string.

- `wstring` stores a sequence of `wchar_t` values.
- The `L` prefix creates a wide character or wide string literal.

Matching types and literals:

```cpp
string normal = "Hello";
wstring wide = L"Hello";
```

The original Windows tutorial uses wide strings because it renders through a wide-character Windows console buffer. Tetris itself does not require wide strings. The macOS version could use regular strings for ASCII characters such as `.` and `X`.

## Building and running

In VS Code, use **Run C/C++ File** and select the `clang++` configuration.

From the terminal:

```bash
clang++ -std=c++17 -Wall -Wextra src/run1.cpp -o src/run1
./src/run1
```

Use `clang++`, not `clang`. The `clang++` driver automatically links the C++ standard library.

The program also initializes the user's terminal locale before using `wcout`. This lets wide text such as `L"naïve"` display correctly instead of showing a replacement character.

Current output:

```text
naïve
naïve
Tetris - Run 1
```

## Next step

Define the remaining six tetrominoes, keeping every shape exactly 16 characters long.
