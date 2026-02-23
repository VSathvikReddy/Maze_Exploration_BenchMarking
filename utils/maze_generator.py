import random
import sys

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
    # We now expect at least 2 arguments (mode and algorithm)
    if len(sys.argv) < 3:
        print("Usage: python script.py [print|gen] [dfs|clean] [Height] [Width]")
        sys.exit(1)

    mode = sys.argv[1].lower()
    algo = sys.argv[2].lower() # New argument for the algorithm
    
    # Grab dimensions from command line if provided, otherwise default to 30x30
    # Notice the indices shifted to 3 and 4 because of the new 'algo' argument
    N = int(sys.argv[3]) if len(sys.argv) > 3 else 30 # Height
    M = int(sys.argv[4]) if len(sys.argv) > 4 else 30 # Width
    
    # Decide which function to call based on the 'algo' argument
    if algo == "clean":
        maze = clean_maze(N, M)
    elif algo == "dfs":
        maze = dfs_maze(N, M)
    else:
        print(f"Unknown algorithm '{algo}'. Defaulting to 'dfs'.")
        maze = dfs_maze(N, M)

    # --- INJECT THE GOAL TILE ---
    goal_placed = False
    for i in range(N - 1, -1, -1):
        for j in range(M - 1, -1, -1):
            if maze[i][j] == 1:
                maze[i][j] = 3 # 3 represents the goal in our python logic
                goal_placed = True
                break
        if goal_placed:
            break

    # --- PROCESS COMMAND ---
    if mode == "print":
        for row in maze:
            line = ""
            for x in row:
                if x == 2:
                    line += "██" # Wall
                elif x == 1:
                    line += "  " # Floor
                elif x == 3:
                    line += "GG" # Goal
            print(line)
            
    elif mode == "gen":
        filename = "maze.txt"
        with open(filename, "w") as f:
            f.write(f"{M} {N}\n")
            
            for row in maze:
                mapped_row = []
                for x in row:
                    if x == 2:
                        mapped_row.append("0") # TileType::WALL
                    elif x == 1:
                        mapped_row.append("1") # TileType::FLOOR
                    elif x == 3:
                        mapped_row.append("2") # TileType::GOAL
                
                f.write(" ".join(mapped_row) + "\n")
                
        print(f"Success! Saved {M}x{N} '{algo}' maze to {filename}.")
        
    else:
        print(f"Unknown argument: '{mode}'. Please use 'print' or 'gen'.")