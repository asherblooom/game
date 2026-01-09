# A 2D Card Game Engine

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![OpenGL](https://img.shields.io/badge/OpenGL-%23FFFFFF.svg?style=for-the-badge&logo=opengl)

## Setup instructions

### Linux (with x11)

<details>
<summary>Prerequisites</summary>
  
- Ensure the glfw, freetype, openGL, openAL soft, ogg, vorbis, x11, xInput and xrandr development libraries are installed
  
  - _on ubuntu you can run_

    ```
    sudo apt install libgl-dev libx11-dev libxi-dev libxrandr-dev libglfw3-dev libfreetype6-dev libopenal-dev libvorbis-dev libogg-dev
    ```

  - _on fedora you can run_

    ```
    sudo dnf install libglvnd-devel mesa-libGL-devel libXi-devel libX11-devel libXrandr-devel glfw-devel freetype-devel openal-soft-devel libvorbis-devel libogg-devel
    ```

</details>

<details>
<summary>Building and running your game</summary>

- Clone the repository
- Run `make` in the root directory
- Run the resulting `game` file (located in the root directory)

</details>

### Windows

<details>
<summary>Prerequisites</summary>
  
- [Install GLFW](https://www.glfw.org/download.html)
- [Install freetype](https://freetype.org/download.html)
- Something else?

</details>

<details>
<summary>Building and running your game</summary>

- Clone the repository
- Something else?

</details>

## Demo Game instructions

Press:

- 'Left mouse button' while cursor is not over a card to create a new card at cursor position
- while cursor is over a card:
  - 'f' to flip it
  - 'm' to make it move to the top left of the screen
  - 'Right mouse button' to move and flip it at the same time
  - '+' to make it bigger
  - '-' to make it smaller
  - 'd' to delete it
- 'c' to clear all cards
- 'p' to pause or resume the countdown
- 's' to stop the countdown
- 'q' or 'esc' to exit the game

Click and drag cards to move them around

## Debugging

The [RenderDoc](https://renderdoc.org/) program can be _very_ helpful in debugging issues with shaders, opengl api calls, etc.

## Learning OpenGL and C++

### OpenGL

- [This website](https://www.learnopengl.com/) teaches OpenGl ver. 3 (but is still very good and widely recommended) \
_Note: the boilerplate code (and the accompanying libraries, i.e. GLAD and glfw) and the maths library (GLM) are
both taken from this website_

- [This book](https://linker2.worldcat.org/?jHome=https%3A%2F%2Fbris.idm.oclc.org%2Flogin%3Furl%3Dhttps%3A%2F%2Flearning.oreilly.com%2Flibrary%2Fview%2F~%2F9780133365023%2F%3Far%26orpq%26email%3D%5Eu&linktype=best&jHomeSig=18d1b86a967d5206742890e689759fabdaea0ebe6659ddee4ef995593108d486)
teaches up to date OpenGL (ver. 4.5) - it is somewhat detailed \
_Note: the link should take you to a sign-in page for UoB which will then direct
you to the **6th edition** e-book. You can access the **7th edition** (most recent edition) by scrolling down and selecting it_

- [This website](https://antongerdelan.net/opengl/index.html) is another good OpenGL 4 tutorial, which is simpler than the above
but covers some topics in a slightly different order

### C++

- The book **"A Tour of C++" by Bjarne Stroustrup** (2nd or 3rd edition are good)
is good as a quick reference guide to C++ compared
to the more indepth website linked below \
(You might be able to find a free pdf
online or access an ebook from the library website)

- [Here](https://www.learncpp.com/) is a good website to learn C++\
Just note it is _very_ detailed and sometimes in a strange order
