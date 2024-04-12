# CPPND: Capstone Snake Game Example

This is the repo for Cherif Jazra's Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was originally provided by Udacity and inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

## CC Attribution-ShareAlike 4.0 International

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg


# New Features

This project adds the following features to the basic code provided by Udacity:

1. The ability to provide custom game configuration options through a json file, where frame speed, screen size, and grid size can be specified.
2. A new background Game logger capability which keeps tracks of all games events and periodically stores them in a log file.


## General Description and Code Layout

### Custom Game Configuration

To support Custom Game configuration through json files, the [RapidJson](https://rapidjson.org/) library is added to the project. RapidJson header files are included under `include/rapidjson` and a new `cmake` file is added for building an linking the project. See `cmake/rapidjson.cmake` for details.

The `main` function in `main.cpp` takes a new user input, the url to the json configuration file. The `GameConfig` object defined in `gameConfig.h` implements loading the configuration from a json file. Its constructor will take care of reading the file and store the game parameters in internal private variables such as `_kFramesPerSecond`. Public Getter functions are added for each parameter. The `RAII` pattern is used to manage the file pointer used to open the configuration file. See class `gameConfig.cpp#configFileFp`. If no file is provided by the user, or if the url provided is incorrect, the program will default to the constant game parameters defined in `GameConfig.h`. An example config file can be found in `src/config/example_config.json`

### The Game Logger

The second and main feature added to this program is a Game Logging capability. The logger is implemented using the [Singleton pattern](https://en.wikipedia.org/wiki/Singleton_pattern), in the `GameLogger` class in `gameLogger.h`.  This pattern is used to make it easier to invoke the logger from anywhere within the codebase, without having to include a reference to the logger in all classes that need to access it.

The logger must be invoked through the `GameLogger#getLogger()` static function, which returns a reference to the single static `GameLogger` variable. To insure that there are no other ways to instantiate this class, `copy` & `move` initializers and constructors are explicitely deleted. There is only one single **private** constructor, accessible internally to initialize the singleton's vairable.

The Game logger is intended to run as a separate thread and provides several public functions for submitting game event. Following are currently supported events:
- change of direction
- Eating event
- Death Event

All these events are stored inside a `GameLogger` vector of events and protected by a `mutex` for concurrent access.

The `GameLogger` object is initialized with parameters indicating the log file location and the frequency at which to update the file. These parameters are currently hardcoded to write to `./game_logger.txt` every 3 seconds (see `GameLogger::getLogger()`)

The thread started by Game::Run() will invoke the `GameLogger::main()` function which will run an infinite while loop, checking whether it is time to flush the `event` vector or not and sleeping for a constant time otherwise (currently 20ms). In addition, a reference to the `Game::running` flag is provided by the `Game` object when launching the thread, and indicates whether the game has ended and the program is shutting down. In addition, a `stopped` flag on `GameLogger` is also used as a separate mechanism to end the logger thread loop when the snake dies, but without ending the program.

The method `GameLogger::flush()` implements the mechanism for writing all stored events into the log file. The method always appends to the end of the file, never clearing it (using the open mode `std::ios_base::app`). In order to minimize the time the mutex locks on the `event` variable, the vector is first moved to a temporary location and the `events` variable cleared before the lock is released, after which the function proceeds to write events to the filesystem. During that time, new events can continue populating a the internal `events` vector. finally, the logger destructor will flush any remaining events to the file to ensure nothing is lost.

Future Improvement to the current implementation includes:
- Using a separate task to flush the content of the file, avoiding holding the thread for too long.
- Defining different classes for the `event` types, and using a `std::variant` to store them in the `event` structure. 
- Extend the Game so as to have a `replay` mode which using the Game logger file to replay the game as it was played. 


## Installing and Running 
 
### Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

### Steps

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it with default game configuration `./SnakeGame`.
5. Run with Custom Config: `./SnakeGame ../src/config/example_config.json`.

Output will show the game configuration used and log Game events, for example:

```
% ./SnakeGame ../src/config/example_config.json 
::GameConfig - Reading Config File: ../src/config/example_config.json
::GameConfig - Success reading config file!
== ::GameConfig == 
kFramesPerSecond: 80
kMsPerFrame: 12
kScreenWidth: 320
kScreenHeight: 320
kGridWidth: 24
kGridHeight: 24
== 
::GameLogger - Thread Started: 
::GameLogger - updateFrequencySec - 3
::GameLogger -logPath - game_logger.txt
::Logger - update Log - last update 3 sec ago
::GameLogger - Event List: 
--Fri Apr 12 12:07:06 2024	-- GO: Left --
--Fri Apr 12 12:07:07 2024	-- GO: Up --
--Fri Apr 12 12:07:07 2024	-- GO: Left --
::Logger - update Log - last update 3 sec ago
::GameLogger - Event List: 
--Fri Apr 12 12:07:08 2024	-- GO: Down --
--Fri Apr 12 12:07:08 2024	-- GO: Right --
--Fri Apr 12 12:07:09 2024	-- GO: Up --
--Fri Apr 12 12:07:09 2024	-- YUMMY!  score: 1 size: 1 speed: 0.120000 --
--Fri Apr 12 12:07:10 2024	-- GO: Left --
--Fri Apr 12 12:07:10 2024	-- YUMMY!  score: 2 size: 2 speed: 0.140000 --
--Fri Apr 12 12:07:10 2024	-- GO: Up --
::Logger - update Log - last update 3 sec ago
```

The game logs will be stored in the `./game_logger.txt` file. This file can be observed while playing using
`tail -f ./game_logger.txt`


# Submission Rubric


| Rubric  | Description | Project Meets ? |
| :-------- | :---------| :-------------: |
|||
| I. |  <u>**Compiling and Testing (All Rubric Points REQUIRED)**</u> | |
| 1. |  The submission must compile and run without errors on the Udacity project workspace. | Yes | 
|||
| II. | <u>**Loops, Functions, I/O - meet at least 2 criteria** </u>| |
| 1. |  The project demonstrates an understanding of C++ functions and control structures. | YES, see the various classes and structure in `.cpp|.h` files for `gameConfig` and `gameLogger` |
| 2. |  The project reads data from a file and process the data, or the program writes data to a file. | YES - see both reading json.config and logger#flush |
| 3. |  The project accepts user input and processes the input | YES see `main()` method |
| 4. |  The project uses data structures and immutable variables. | YES, see constant in logger/GameConfig | 
|||
| III.  | <u>**Object Oriented Programming - meet at least 3 criteria**</u>  | |
|||
| 1. |  One or more classes are added to the project with appropriate access specifiers for class members | YES see `gameLogger.h` |
| 2. |  Class constructors utilize member initialization lists | YES see `gameLogger.h` |
| 3. |  Classes abstract implementation details from their interfaces | yes, `gameLogger.h` implementation in `cpp` file |
| 4. |  Overloaded functions allow the same function to operate on different parameters | NOT RIGHT NOW | 
| 5. |  Classes follow an appropriate inheritance hierarchy with virtual and override functions | NOT RIGHT NOW | 
| 6. |  Templates generalize functions or classes in the project. | NOT RIGHT NOW | 
|||
| IV. |  <u>**Memory Management - meet at least 3 criteria**</u>  | |  
|||
| 1.  | The project makes use of references in function declarations | YES SEE `void main(bool &running);` and `static GameLogger& getLogger();` in  `gameLogger.h` | 
| 2. |  The project uses destructors appropriately. | YES see `~GameLogger()` |
| 3. |  The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. | YES see class `configFileFp` in `gameConfig.cpp` |
| 4. |  The project follows the Rule of 5 | yes, see `gameLogger.h` |
| 5. |  The project uses move semantics to move data instead of copying it, where possible |  YES, see example in `GameLogger::logDirectionChange()` |
| 6. |  The project uses smart pointers instead of raw pointers. | NOT RIGHT NOW |
|||
| V. |  <u>**Concurrency - meet at least 2 criteria**</u>  | | 
|||
| 1. |  The project uses multithreading. | YES, `GameLogger` is running in separate thread and uses mutexes |
| 2. |  A promise and future is used in the project. | NOT RIGHT NOW |
| 3. |  A mutex or lock is used in the project  | YES see `GameLogger` use of `_mtx` | 
| 4. |  A condition variable is used in the project. | NOT RIGHT NOW |