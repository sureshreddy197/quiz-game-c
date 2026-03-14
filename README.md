# quiz-game-c

A console-based quiz game in C with multiple-choice questions, score tracking, categories, lifelines, and leaderboard support.

## Features

- Clean terminal UI with adaptive formatting for different terminal widths
- Three categories: **C Programming**, **General Knowledge**, and **Science**
- Randomized question order for every round
- Two lifelines per game:
  - **50-50** removes two wrong options
  - **Skip** lets you skip one question
- Score system with streak bonus points
- Persistent leaderboard stored in `data/leaderboard.txt`
- Modular codebase with separate files for UI, questions, storage, and game flow
- Cross-platform friendly build using a simple `Makefile`

## Demo Flow

1. Launch the game
2. Pick a category
3. Enter your name
4. Answer five random questions
5. Use lifelines strategically
6. Save your score to the leaderboard

## Project Structure

```text
quiz-game-c/
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md
│   │   └── feature_request.md
│   ├── workflows/
│   │   └── ci.yml
│   └── PULL_REQUEST_TEMPLATE/
│       └── pull_request_template.md
├── data/
│   └── .gitkeep
├── include/
│   ├── question.h
│   ├── quiz.h
│   ├── storage.h
│   └── ui.h
├── src/
│   ├── main.c
│   ├── questions.c
│   ├── quiz.c
│   ├── storage.c
│   └── ui.c
├── .clang-format
├── .editorconfig
├── .gitignore
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── LICENSE
├── Makefile
├── README.md
└── SECURITY.md
```

## Build and Run

### Linux / macOS

```bash
make
./quiz-game
```

### Windows (MinGW or MSYS2)

```bash
make
./quiz-game.exe
```

### Clean build artifacts

```bash
make clean
```

## Requirements

- A C compiler with C11 support
  - `gcc`
  - `clang`
  - `mingw-w64`
- Terminal/console that supports standard input and output

## Sample Gameplay

```text
============================================================
                        QUIZ GAME IN C
                          Play Quiz
============================================================
Player: Suresh
Score: 20 | Correct: 2 | Streak: 2

Question 3 of 5 | Category: C Programming
------------------------------------------------------------
Which loop is guaranteed to execute its body at least once?

  1) for
  2) while
  3) do-while
  4) foreach
```

## Leaderboard Storage

Scores are saved to:

```text
data/leaderboard.txt
```

You can override the path with an environment variable:

```bash
export QUIZ_LEADERBOARD_PATH="/custom/path/leaderboard.txt"
```

On Windows PowerShell:

```powershell
$env:QUIZ_LEADERBOARD_PATH = "C:\temp\leaderboard.txt"
```

## Future Improvements

- Timer-based questions
- Difficulty levels
- Admin mode for adding custom questions from a file
- Multiplayer mode
- Unit tests for storage and question parsing

## Contributing

Pull requests are welcome. Read [CONTRIBUTING.md](CONTRIBUTING.md) before opening one.

## Security

To report a vulnerability, follow the process in [SECURITY.md](SECURITY.md).

## License

This project is licensed under the [MIT License](LICENSE).
