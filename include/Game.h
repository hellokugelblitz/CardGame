enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game
{
    public:
        // game state stuff
        GameState State;
        bool Keys[1024];
        unsigned int Width, Height;

        // constructor
        Game(unsigned int width, unsigned int height);
        ~Game(); // destructor

        // init game state (load shaders, textures, levels)
        void Init();

        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};