# class GameManager

Handles the gameplay by providing an interface to `main()`.

`main()` 에서 호출되어 게임을 실행하는 클래스.

## Description

This class handles the entire gameplay with all `...Manager` objects and handles the interaction between those objects. Instantiated and used by `main()`.

This class is expected to be used like the example below.

<details>
<summary><code>main.cpp</code></summary>

```cpp
#include ... // include required components

// global variables here

int main()
{
    GameManager game; // handles everything (user input, rendering, etc.) with initialization

    return 0;
}
```

</details>

## Member variables

### m_ObjectManager

```cpp
private ObjectManager m_ObjectManager;
```

The `ObjectManager` object to use. Will be referenced in `ObjectGenerator` and `SceneManager`.

### m_ObjectGenerator

```cpp
private ObjectGenerator m_ObjectGenerator;
```

The `ObjectGenerator` object to use.

### m_InputManager

```cpp
private InputManager m_InputManager;
```

The `InputManager` object to use.

### m_SceneManager

```cpp
private SceneManager m_SceneManager;
```

The `SceneManager` object to use.

### high_score

```cpp
private int high_score;
```

The highest score in the previous gameplay, from `highscore.dat`. Loaded from `LoadHighScore()`.

### is_paused

```cpp
private bool is_paused;
```

True if the user paused the game, false otherwise.

## Constructor

```cpp
public GameManager()
```

Instantiates the `GameManager` object. Initializes all member variables and shows the ***title scene***.

### Scene: _Title_

***Title scene*** contains the following components.

- Game title
- How to play (brief explanation of controls)
- High scores
- *Press spacebar to start surfing* text

Since this constructor shows the ***title screen***, the `main()` function only has to instantiate this class.

## Member functions

### Ready

```cpp
private void Ready()
```

Shows the ***ready scene***. Initializes `Player` object into default position. Called as a callback function from `InputManager` or on game over situation.

### Scene: _Ready_

***Ready scene*** contains the following components.

- Health bar
- High score
- Boost bar
- Player (at the center of the scene) 

### Start

```cpp
private void Start()
```

Starts playing the game on the ***gameplay scene***. Initialize the default starting `GameObject` objects into position. Called as a callback function from `InputManager`.

### Scene: _Gameplay_

***Gameplay scene*** contains the following components.

- Health bar
- High score
- Boost bar
- Player (at the center of the scene)
- `GameObject` (`Obstacle`s, `Item`s, `Drawback`s, `Surfer`s, `Kraken`)

### Render

```cpp
private void Render()
```

Renders a frame of gameplay.

#### Render mechanism

There is no _fixed_ time between frames. The value `deltaTime` depends on the processing speed of the computer, and the game re-calculates every entities' position values according to `deltaTime`. The following is an example using `deltaTime` logic for rendering.

<details>
<summary>Example C++ code</summary>

```cpp
/**
 A function called to render a frame.
 */
int currentTime, previousTime = -1; // used to calculate deltaTime
void render()
{
    int timeDelta; // time difference between frames in milliseconds
    currentTime = getTime(); // not a stl function, but there should be a function that can get precise current time in milliseconds

    if (previousTime == -1)
        timeDelta = 0;
    else
        timeDelta = currentTime - previousTime;

    // for example, moving the player from its position with its speed
    // the displacement of the player is calculated with computed timeDelta value
    // 0.002 value is just a constant, which can be used to change the speed of the whole game
    m_player.set_center(
        m_player.get_pos_x() + timeDelta * 0.002 * m_player.get_dir_x() * m_player.get_speed(),
        m_player.get_pos_y() + timeDelta * 0.002 * m_player.get_dir_y() * m_player.get_speed(),
        m_player.get_pos_z() + timeDelta * 0.002 * m_player.get_dir_z() * m_player.get_speed()
    );

    // do collision detection and displacement calculations here

    m_SceneManager.render(); // display the computed values onto console
    previousTime = currentTime;
}
```

</details>

### DistanceToScore

```cpp
private int DistanceToScore(float distance)
```

Returns the score from the given distance.

Since the distance is in `float` type, which is an abstract representation of the position of the player in game, and the score is in `int` type, which is denoted as _meters_ in the scoreboard, the abstract position should be converted into distance traveled with a reasonable conversion rate.

### LoadHighScore

```cpp
private void LoadHighScore()
```

Reads and loads high score data from `highscore.dat` file into `high_score`.

Returns `0` if the content of the file is not a positive integer or the file doesn't exist.

### File content structure

`highscore.dat` file consists of 1 positive integer value as text. Should be written with `cout` or `printf` and should be able to be read with `cin` or `scanf`.

<details>
<summary>Example <code>highscore.dat</code></summary>

```
420
```

</details>

### SaveHighScore

```cpp
private void SaveHighScore()
```

Saves and writes high score data from `high_score` into `highscore.dat` file.

Creates `highscore.dat` file if the file doesn't exist.