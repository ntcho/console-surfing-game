# class SceneManager

Manages instantiated `ObjectManager` and `InputManager` to render the game scene.

`ObjectManager`와 `InputManager`를 사용해서 게임을 화면에 표시하는 클래스.

## Description

This class displays all `GameObject` objects in `ObjectManager` onto the console window. Refreshes the entire console window every frame.

**See also:**

[ObjectManager](ObjectManager.md)

## Member variables

### size_x

```cpp
private int size_x;
```

Width of the console window. Equal to the number of characters that can be displayed in one line on the current console window.

### size_y

```cpp
private int size_y;
```

Height of the console window. Equal to the number of lines that can be displayed in the current console window.

#### Displaying on console

Since 1 character on _(most)_ console font has double the height compared to width (e.g., font size of 14 point will be 14 points tall, 7 points wide), this game considers **2 characters as one square pixel** (not exact pixel as we see on the screen, but as a render unit).

### center_x

```cpp
private float center_x;
```

The x-axis position of the camera.

### center_y

```cpp
private float center_y;
```

The y-axis position of the camera.

#### Rendering mechanism

`Render()` renders all objects within calculated boundary of current `SceneManager` state, using `center_x`, `center_y`, `size_x`, `size_y`.

<details>
<summary>Render example diagram</summary>
</br>

![Diagram](diagram/SceneManager_render_diagram.jpg)

Click [here](https://app.diagrams.net/#Hbgb10%2Fconsole-surfing-game%2Fmaster%2Fdocs%2Fdiagram%2FSceneManager_render_diagram.drawio) to edit this diagram on _draw.io_.

</details>

## Constructor

### SceneManager

```cpp
public SceneManager()
```

Instantiates the `SceneManager` object with default width and height.

Default values for height is `45`, width is `160`.

### SceneManager(int size_x, int size_y)

```cpp
public SceneManager(int size_x, int size_y)
```

Instantiates the `SceneManager` object with given width and height.

## Member functions

### Render

```cpp
public void Render(ObjectManager& manager)
```

Renders all object from given `manager` onto console screen.

**See also:**

[SceneManager: Rendering mechanism](#Rendering-mechanism)