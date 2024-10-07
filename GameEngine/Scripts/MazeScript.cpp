#include "MazeScript.h"
#include <algorithm>

// Directions: Up, Down, Left, Right
const int DX[] = { -1, 1, 0, 0 };
const int DY[] = { 0, 0, -1, 1 };

void printMaze(const std::vector<std::vector<bool>>& maze) {
    int n = maze.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (maze[i][j]) {
                std::cout << "  ";  // Path is open (true)
            }
            else {
                std::cout << "##";  // Wall is present (false)
            }
        }
        std::cout << std::endl;
    }
}

bool isValid(int x, int y, int n, const std::vector<std::vector<bool>>& maze) {
    // Ensure the point is within bounds and is a wall (not yet visited)
    return x > 0 && y > 0 && x < n - 1 && y < n - 1 && !maze[x][y];
}

void generateMaze(int x, int y, std::vector<std::vector<bool>>& maze) {
    // Randomly shuffle directions
    int directions[] = { 0, 1, 2, 3 };

    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(std::begin(directions), std::end(directions), generator);

    for (int i = 0; i < 4; ++i) {
        int nx = x + DX[directions[i]] * 2;
        int ny = y + DY[directions[i]] * 2;

        if (isValid(nx, ny, maze.size(), maze)) {
            // Mark path between current and new cell
            maze[x + DX[directions[i]]][y + DY[directions[i]]] = true;
            maze[nx][ny] = true;

            // Recurse to the next cell
            generateMaze(nx, ny, maze);
        }
    }
}

std::vector<std::vector<bool>> createMaze(int n) {
    // Initialize a maze full of walls (false)
    std::vector<std::vector<bool>> maze(n, std::vector<bool>(n, false));

    // Seed the random generator
    std::srand(std::time(0));

    // Set the start point as true (open space)
    maze[1][1] = true;

    // Generate maze starting from (1,1)
    generateMaze(1, 1, maze);

    return maze;
}

void MazeScript::OnStart()
{
    int size = 20;
    maze = createMaze(size);
    printMaze(maze);
        
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (!maze[i][j])
            {
                int scale = 20;

                auto entity = registry->CreateEntity();
                registry->AddComponent<TransformComponent>(entity);
                registry->AddComponent<ShapeComponent>(entity);
                registry->AddComponent<MaterialComponent>(entity);
                registry->AddComponent<BoxColliderComponent>(entity);
                registry->AddComponent<RigidbodyStaticComponent>(entity);

                registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(scale * j, 0, scale * i);
                registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(scale / 2.0);
                registry->GetComponent<ShapeComponent>(entity).shape = ResourceManager::Instance()->GetGeometry("Cube");
                registry->GetComponent<MaterialComponent>(entity).diffuse = TextureManager::Instance()->LoadImageTexture("../Assets/Maze/Wall.jpg");
            }
        }
    }
}

void MazeScript::OnUpdate(float deltaTime)
{

}
