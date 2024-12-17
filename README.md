## Checkpoints

### Checkpoint 4
- **Contributor**: TimW
- **Status**: On Track
- **Details**:
  - Base implementation is "complete" but requires extensive black-box testing.
  - Black-box testing to be done extensively.
  - Discussed good design practices and considerations for video documentation.
  - Enhancements are in progress, mostly completed but require testing.

---

## README

### Group Info
- **Group Name**: budbud_77-on-ig
- **Group Number**: 104
- **Cohort**: 2
### Contributors
- **Armaan Singh Gill** - [GitHub: gill-armaan](https://github.com/gill-armaan) (s4051327)
- **Rahul Garg** - [GitHub: rgarg3865](https://github.com/rgarg3865) (s4008144)
- **Ishpreet Singh Arora** - [GitHub: eshpreet-of-rmit](https://github.com/eshpreet-of-rmit) (4093910)

---

### Project Files

| **Files**             | **Contributions**                 | **Contents**                                 |
|-----------------------|-----------------------------------|----------------------------------------------|
| `Agent.cpp/h`         | Armaan                            | Wall follower & BFS (enhancement 2)          |
| `Coord2D.h`           | Ishpreet                          | 2D coordinate object for Maze generation     |
| `Maze.cpp/h`          | All                               | Class to manage the maze as an object        |
| `MazeGenerator.cpp/h` | Ishpreet                          | Class to generate a terraformed maze         |
| `mazeRunner.cpp`      | All                               | Main class file                              |
| `MazeValidation.cpp/h`| Rahul                             | Maze validation class (enhancement 3)        |
| `Menu.cpp/h`          | Rahul (leading), Armaan, Ishpreet | User-facing and interface class              |
| `NoTerraform.cpp/h`   | Ishpreet                          | Non-terraforming maze (enhancement 1)        |
| `README.md`           | All                               | Project README                               |
| `Queue.h`             | Armaan                            | Queue implementation for BFS                 |
| `StackMan.h`          | Ishpreet                          | Stack implementation for Maze generation     |
| `Terraform.cpp/h`     | Armaan (leading), Rahul                     | Class to terraform the maze                  |
| `Tests`               | All                               | Folder for all test cases for the project    |

---

### Notes
- All enhancements can be accessed through the menu.
- `-fastmode` flag: Can be appended to the run command. This reduces the 50ms delay for normal mode testing.
- `-testmode` flag: Can be appended to the run command to enable test mode.

---

### Design Choices

- **No Terraform**: Wall height is set to three units above the highest walkable point, creating a flat top to prevent peeking over the maze.

- **Read Maze from Terminal**:
  - The maze is checked line-by-line for any issues.
  - If there are issues with maze exits, the user must re-enter the entire maze.

- **BFS (Enhancement 2)**: 
  - In a terraformed maze, the program shows a visualization of the BFS algorithm.
  - The Shortest Path can be found in a non terraformed maze.
 
- **Right Hand Follow Algorithm**:
  - The agent can traverse a Non Terraformed maze.

- **-fastmode flag**:
  - Mazes in this mode are built almost instantly so that the user doesn't have to wait if he doesn't want to.
Video:https://rmiteduau-my.sharepoint.com/:f:/g/personal/s4008144_student_rmit_edu_au/EiVDyNKGonRNpZw5n2bwVTAB-GhlFnVIWtFRwX-S43hJlg?e=TjoDxI
