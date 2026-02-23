import random

def clean_maze(N, M):
    # Initialize all walls
    maze = [[2 for _ in range(M)] for _ in range(N)]

    def is_valid(x, y):
        return 0 <= x < N and 0 <= y < M

    def dfs(x, y):
        maze[x][y] = 1   # mark as path
        dirs = [(0, 2), (2, 0), (0, -2), (-2, 0)]
        random.shuffle(dirs)

        for dx, dy in dirs:
            nx, ny = x + dx, y + dy
            if is_valid(nx, ny) and maze[nx][ny] == 2:
                # Carve the wall between cells
                maze[x + dx//2][y + dy//2] = 1
                dfs(nx, ny)

    # Start DFS at (1,1)
    dfs(1, 1)
    return maze

def dfs_maze(N, M):
    # Initialize all walls
    maze = [[2 for _ in range(M)] for _ in range(N)]

    def is_valid(x, y):
        return 0 <= x < N and 0 <= y < M

    def dfs(x, y):
        maze[x][y] = 1   # mark as path
        dirs = [(0, 1), (1, 0), (0, -1), (-1, 0)]
        random.shuffle(dirs)

        for dx, dy in dirs:
            nx, ny = x + dx, y + dy
            if not is_valid(nx, ny):
                continue
            if  maze[nx][ny] == 1:
                continue
            work = True
            new_dirs = dirs[:]
            random.shuffle(new_dirs)
            for ddx, ddy in new_dirs:
                if (dx + ddx, dy + ddy) == (0, 0):
                    continue
                mx, my = nx + ddx, ny + ddy
                if is_valid(mx, my) and maze[mx][my] == 1:
                    work = False
                    break
            if not work:
                continue
            dfs(nx, ny)

    maze[0][0] = 1  # Start point
    dfs(0, 0)
    return maze


if __name__ == "__main__":
    N = 30
    M = 30
    maze = dfs_maze(N, M)
    for row in maze:
        # print(row)
        print("".join(["  " if x == 2 else "██" for x in row]))