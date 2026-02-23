import random

# Directions: (dx, dy)
DIRS = [(0, 2), (2, 0), (0, -2), (-2, 0)]

def dfs_maze(N, M):
    # Initialize all walls
    maze = [[0 for _ in range(M)] for _ in range(N)]

    def is_valid(x, y):
        return 0 <= x < N and 0 <= y < M

    def dfs(x, y):
        maze[x][y] = 1   # mark as path
        dirs = DIRS[:]
        random.shuffle(dirs)

        for dx, dy in dirs:
            nx, ny = x + dx, y + dy
            if is_valid(nx, ny) and maze[nx][ny] == 0:
                # Carve the wall between cells
                maze[x + dx//2][y + dy//2] = 1
                dfs(nx, ny)

    # Start DFS at (1,1)
    dfs(1, 1)
    return maze


if __name__ == "__main__":
    N = 15
    M = 15
    maze = dfs_maze(N, M)
    for row in maze:
        print("".join(["  " if x else "██" for x in row]))