/* ============================================================================
    MAZE RUNNER - OOP CONSOLE GAME
    Reg no. : [2024373_2024584_2024687]
    Description: Terminal-based game using OOP concepts 
============================================================================ */

#include "includes.h"

// Utility function to center-align text in the console
void centerText(const string& text, int width = 80) {
    int padding = (width - text.length()) / 2;
    if (padding > 0) 
        cout << string(padding, ' ');
    cout << text << endl;
}

// Forward declaration for use in Tile class methods
class Maze;


// Player class: Represents the player in the game
class Player {
private:
    int x, y;
    int score;
    int lives;

public:
    // Constructor: initializes player position, score, and lives
    Player(int startX, int startY) {
        x = startX;
        y = startY;
        score = 0;
        lives = 2;
    }

    // Getters
    int getX() const { 
        return x; 
    }
    int getY() const { 
        return y; 
    }
    int getScore() const { 
        return score; 
    }
    int getLives() const { 
        return lives; 
    }

    // Setters 
    void setPosition(int newX, int newY) {
        x = newX;
        y = newY;
    }

    void increaseScore() {
        score++;
    }

    void decreaseLife() {
        lives--;
    }

    void resetPosition() {
        x = 1;
        y = 1;
    }
};


// Abstract Base Class: Tile -  Represents a tile on the maze grid
class Tile {

public:
    // Returns the character symbol shown on the maze grid
    virtual char getSymbol() = 0;

    // Defines what happens when the player steps on the tile
    virtual void onPlayerStep(Player& player, Maze& maze) = 0;

    // Defines whether player can walk over the tile
    virtual bool isWalkable() { 
        return true; 
    }

    // Defines whether this tile is the finish point
    virtual bool isExit() { 
        return false; 
    }

    virtual ~Tile() {}
};

// Tile Types - Inherit from Tile

// Empty space tile - walkable
class EmptyTile : public Tile {

public:
    char getSymbol() override { 
        return ' '; 
    }

    void onPlayerStep(Player&, Maze&) override {}
};

//Wall tile (non-walkable)
class WallTile : public Tile {

public:
    char getSymbol() override { 
        return '#'; 
    }

    void onPlayerStep(Player&, Maze&) override {}

    bool isWalkable() override { 
        return false; 
    }
};

// Finish tile (ends the game when reached)
class ExitTile : public Tile {

public:
    char getSymbol() override { 
        return 'F'; 
    }

    void onPlayerStep(Player&, Maze&) override { }

    bool isExit() override { 
        return true; 
    }
};

//Treasure tile - boosts score
class TreasureTile : public Tile {

private:
    bool collected = false;

public:
    char getSymbol() override {
        if (collected)
            return ' ';
        else
            return '$';
    }

    void onPlayerStep(Player& player, Maze&) override {
        if (collected == false)
        {
            collected = true;

            system("cls");
            centerText("********************************\n");
            centerText("*   YOU FOUND A TREASURE! +1   *\n");
            centerText("********************************\n\n");
        
            player.increaseScore();
            Sleep(1200);
        }
    }
};

// Trap tile - decreases player's life
class TrapTile : public Tile {

public:
    char getSymbol() override { 
        return 'T'; 
    }

    void onPlayerStep(Player& player, Maze& maze) override;
};

// Enemy tile - sends player back to start
class EnemyTile : public Tile {

public:
    char getSymbol() override { 
        return 'E'; 
    }

    void onPlayerStep(Player& player, Maze& maze) override;
};

// Maze class holds a 2D grid of Tile pointers representing the maze layout : Handles display, movement, and game logic interactions
class Maze {

private:
    Tile* grid[6][30];

public:
    Maze() {
        
        string layout[6] = {

            "##############################",
            "#                  $       #F#",
            "#  ###   ##   ###     ###    #",
            "#   E    ##   T   ##    E    #",
            "#     ##   ###   ###   $     #",
            "##############################"
        };

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 30; j++) {

                char ch = layout[i][j];

                if (ch == '#') grid[i][j] = new WallTile();
                else if (ch == 'F') grid[i][j] = new ExitTile();
                else if (ch == '$') grid[i][j] = new TreasureTile();
                else if (ch == 'E') grid[i][j] = new EnemyTile();
                else if (ch == 'T') grid[i][j] = new TrapTile();

                else grid[i][j] = new EmptyTile();
            }
    }

    // Moves a given tile (Trap/Enemy) to a random empty position
    void moveTileToNewLocation(Tile* tile) {

        int oldX = -1, oldY = -1;
        int newX, newY;

        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 30; j++)

                if (grid[i][j] == tile) 
                {
                    oldX = i;
                    oldY = j;
                }

       
        do {
            newX = rand() % 6;
            newY = rand() % 30;

        } while (grid[newX][newY]->getSymbol() != ' ');

        delete grid[newX][newY];

        grid[newX][newY] = tile;
        grid[oldX][oldY] = new EmptyTile();
    }

    // Reads game_summary.txt and prints previous scores
    void display(int playerX, int playerY) {
        
        system("cls");
    
        centerText("=========== MAZE RUNNER ===========\n\n");
        centerText("=========== Symbols ===========\n");
        centerText("P = Player    # = Wall    F = Exit\n");
        centerText("$ = Treasure  T = Trap    E = Enemy\n");
        centerText("================================\n\n");
        centerText(">>> Tip: Grab treasures ($) to boost your final score!\n");
        centerText(">>> Beware of traps (T) and enemies (E)!\n\n");
        centerText(".................................\n");
    
        for (int i = 0; i < 6; i++) 
        {
            string row = "#";

            for (int j = 0; j < 30; j++) 
            {
                if (i == playerX && j == playerY)
                    row += "P";
                else
                    row += grid[i][j]->getSymbol();
            }

            row += "#";
            centerText(row);
        }
        centerText(".................................\n");
    }

    // Moves a Exit "F" to a new random empty location
    void moveExitToRandomLocation() {

        Tile* exitTile = nullptr;

        int oldX = -1, oldY = -1;
        int newX, newY;

        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 30; j++) 
            {
                if (grid[i][j]->isExit()) 
                {
                    exitTile = grid[i][j];
                    oldX = i;
                    oldY = j;
                    break;
                }
            }
            if (exitTile) break;
        }

        if (!exitTile) 
            return;
        
       
        do 
        {
            newX = rand() % 6;
            newY = rand() % 30;
        } while (grid[newX][newY]->getSymbol() != ' ');
    
        delete grid[newX][newY];

        grid[newX][newY] = exitTile;
        grid[oldX][oldY] = new EmptyTile();
    }
    
    //Checks if tile is a wall
    bool isWall(int x, int y) {
        return !grid[x][y]->isWalkable();
    }

    //Checks if tile is the exit
    bool isExit(int x, int y) {
        return grid[x][y]->isExit();
    }

    // Trigger the action of the tile when player steps on it
    void triggerTileEffect(int x, int y, Player& player) {
        grid[x][y]->onPlayerStep(player, *this);
    }

    // Destructor: Deallocates all tile memory
    ~Maze() 
    {
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 30; j++)
                delete grid[i][j];
    }
};

// This function is triggered when a player steps on a TrapTile.
void TrapTile::onPlayerStep(Player& player, Maze& maze) {
  
   player.decreaseLife();

    system("cls");

    centerText("********************************\n");
    centerText("*      TRAP! LIFE LOST (-1)    *\n");
    centerText("*     Lives Left: " + to_string(player.getLives()) + "             *");
    centerText("********************************\n\n");

    Sleep(1700);

    if (player.getLives() > 0) 
    {
        player.resetPosition();  
        maze.moveTileToNewLocation(this);
    }
}

// This function is called when player steps on an EnemyTile.
void EnemyTile::onPlayerStep(Player& player, Maze& maze) {

    system("cls");

    centerText("********************************\n");
    centerText("*        ENEMY ENCOUNTERED!    *\n");
    centerText("*     Sent Back to Start!      *\n");
    centerText("********************************\n\n");

    Sleep(1700);

    player.resetPosition();  
    maze.moveTileToNewLocation(this);
}


// Reads and displays previous game summaries from file
void displayScores() {

    system("cls");

    centerText("========== PREVIOUS GAME SCORES ==========\n\n");

    ifstream file("game_summary.txt");
    if (!file.is_open()) 
    {
        centerText("No scores recorded yet.\n");
        return;
    }

    string line;
    while (getline(file, line)) 
    {
        cout << line << endl;
    }
    file.close();

    centerText("===========================================\n");
}

// Controls the main game loop:
    // - Player movement
    // - Trap/enemy/treasure logic
    // - Exit detection
    // - Score calculation
    // - Game summary saving

void playGame() {

    string playerName;

    centerText ("Enter your name to begin: ");
    getline(cin, playerName);

    Maze maze;
    Player player(1, 1);
    int moves = 0;
    time_t startTime = time(0);

    centerText ("Controls: W = Up, A = Left, S = Down, D = Right, Q = Quit\n");

    char move;
    bool gameRunning = true;

    while (gameRunning) 
    {
        maze.display(player.getX(), player.getY());

        int elapsed = time(0) - startTime;

        centerText("Moves: " + to_string(moves) +
                " | Score: " + to_string(player.getScore()) +
                " | Lives: " + to_string(player.getLives()) +
                " | Time: " + to_string(elapsed) + "s");


        centerText("Enter move (W = Up, A = Left, S = Down, D = Right, Q = Quit): ");
        move = getch(); 
        move = toupper(move);

        int newX =player.getX(), newY = player.getY();

        if (move == 'W') 
            newX--;
        else if (move == 'S') 
            newX++;
        else if (move == 'A') 
            newY--;
        else if (move == 'D') 
            newY++;
        else if (move == 'Q') 
        {
            centerText("You quit the game.\n");
            break;
        } 
        else 
        {
            centerText("Invalid input.\n");
            Sleep(1000);
            continue;
        }

        moves++;

        static bool exitMoved = false;

        if (moves > 0 && moves % 28 == 0) 
        {
            maze.moveExitToRandomLocation();

            system("cls");

            centerText("========================================");
            centerText("         UH OH! THE EXIT MOVED!        ");
            centerText("========================================");
            centerText("|        You thought you were close?   |");
            centerText("|                                      |");
            centerText("|         Find that 'F' again!         |");
            centerText("========================================\n");

            Sleep(3500);
            
        }
        
        if (maze.isWall(newX, newY)) 
        {
            centerText ( "\nYou bumped into a wall!\n");
            Sleep(1500);
        } 
        else 
        {
            player.setPosition(newX, newY);
            maze.triggerTileEffect(player.getX(), player.getY(), player);

            if (player.getLives() <= 0) 
            {
                int duration = time(0) - startTime;
                int rankScore = (player.getScore() * 1000) - (moves);

                if (rankScore < 0)
                    rankScore = 0;
                
                system("cls");

                centerText("========================================");
                centerText("              GAME OVER              ");
                centerText("========================================\n");
                centerText("+-------------------------------+");
                centerText(" | Total Moves     : " + to_string(moves) + "            |");
                centerText("  | Time Taken       : " + to_string(duration) + "   s       |");
                centerText("| Treasures Found : " + to_string(player.getScore()) + "             |");
                centerText("| Final Game Score: " + to_string(rankScore) + "             |");
                centerText("| Status          :Defeated      |");
                centerText("+-------------------------------+\n");
                centerText(">>> Don't give up! Try again and conquer the maze! \n");
            
                Sleep(5000); 

                ofstream file("game_summary.txt", ios::app);
                file << "Player: " << playerName
                     << " | Treasure Score: " << player.getScore()
                     << " | Moves: " << moves
                     << " | Time: " << duration << "s"
                     << " | Game Score: " << rankScore
                     << " | Status: Defeated\n";
                file.close(); 
                break;
            }  
        }              

        if (maze.isExit(player.getX(), player.getY()))  
        {
            int duration = time(0) - startTime;
            int rankScore = (player.getScore() * 1000) - (moves);

            if (rankScore < 0) 
                rankScore = 0;

            maze.display(player.getX(), player.getY()); 

            centerText("========================================");
            centerText("         YOU ESCAPED THE MAZE!          ");
            centerText("========================================\n");
            centerText("+-------------------------------+");
            centerText("| Total Moves   : " + to_string(moves) + "            |");
            centerText("| Time Taken       : " + to_string(duration) + " s       |");
            centerText("| Treasures Found: " + to_string(player.getScore()) + "            |");
            centerText("| Remaining Lives: " + to_string(player.getLives()) + "            |");
            centerText("| Final Game Score: " + to_string(rankScore) + "           |");
            centerText("| Status         : VICTORY      |");
            centerText("+-------------------------------+\n");
            centerText(">>> Well done, runner! \n");
            
            ofstream file("game_summary.txt", ios::app);
            file << "Player: " << playerName
                 << " | Treasure Score: " << player.getScore()
                 << " | Moves: " << moves
                 << " | Time: " << duration << "s"
                 << " | Game Score: " << rankScore
                 << " | Status: Victory\n";
            file.close();
            break;
        }
    }
}


// Shows main menu and handles user choices to play/view scores/quit
int main() 
{
    srand(time(0));
    int choice;

    while (true) {
        system("cls");
    
    centerText("========================================================");
    centerText("    __  __      /\\     ______   ______          ");
    centerText("    |  \\/  |   /  \\   |___  /  |  ____|         ");
    centerText("     | |\\/| |  / /\\ \\     / /   | |___            ");
    centerText("     | |  | | / ____ \\   / /__  |  ___|           ");
    centerText("     |_|  |_/ _/    \\_\\ /____|  |_____|           ");
    centerText("========================================================");
    centerText("Loading Maze Runner...99%\n");

    centerText("+------------------------+");
    centerText("|       MAIN MENU        |");
    centerText("+------------------------+");
    centerText("| 1. Play Game           |");
    centerText("| 2. View Scores         |");
    centerText("| 3. Quit                |");
    centerText("+------------------------+\n");
    centerText("Enter your choice: ");
    
        cin >> choice;

        if (cin.fail()) 
        {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            centerText("Invalid choice. Please enter 1, 2, or 3.\n");
            Sleep(1500);
            continue;
        }

        if (choice == 1) 
        {
            cin.ignore(); 
            playGame();

            int afterGameChoice;
            while (true) {
                centerText("========================================\n");
                centerText("         WHAT WOULD YOU LIKE TO DO NEXT?\n");
                centerText("========================================\n\n");
                centerText("+------------------------+\n");
                centerText("| 1. Return to Main Menu |\n");
                centerText("| 2. View Your Scores    |\n");
                centerText("| 3. Exit Game           |\n");
                centerText("+------------------------+\n\n");
                centerText("Enter your choice: ");
                
                cin >> afterGameChoice;

                if (cin.fail()) 
                {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    centerText ("\nInvalid choice. Try again.\n");
                    Sleep(1500);
                    continue;
                }

                if (afterGameChoice == 1) {
                    break; 
                } 
                 if (afterGameChoice == 2) {
                    displayScores();
                    centerText ("\nPress Enter to return to main menu...");
                    cin.ignore();
                    cin.get();
                } 
                else if (afterGameChoice == 3) {
                    centerText ("\nThanks for playing! Goodbye!\n");
                    return 0;
                } 
                else {
                    centerText ("\nInvalid choice. Try again.\n");
                    Sleep(1500);
                }
            }
        }
        else if (choice == 2) {
            displayScores();
            centerText ("\nPress Enter to return to main menu...");
            cin.ignore();
            cin.get();
        }
        else if (choice == 3) {
            centerText ("\nThanks for playing! Goodbye!\n");
            break;
        }
        else {
            centerText ("\nInvalid choice. Please enter 1, 2, or 3.\n");
            Sleep(1500);
        }
    }

    return 0;
}
