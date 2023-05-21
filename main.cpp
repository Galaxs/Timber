#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>


using namespace sf;

// METHODS
int GetRandomSpeed(int fromRange, int toRange)
{
    int rnd;
    srand((int)time(0) );
    rnd = (rand() % toRange) + fromRange;
    return rnd;
}

// void setOriginToMiddle(Drawable &obj)
// (
    
// )

const int NUM_BRANCHES = 6;
enum class side { LEFT, RIGHT, NONE};
side branchesPositions[NUM_BRANCHES];
void UpdateBranches(int seed);
int main()
{
    
    // Windows objs
    VideoMode vm(VideoMode().getDesktopMode().width, VideoMode().getDesktopMode().height);
    sf::RenderWindow window(vm, "SFML works!", Style::Fullscreen);

    // Variables GENERAL
    Vector2f defScale(0.73f, 0.70f); // Scale to all textures
    Vector2f treesScale(0.46f, 0.65);
    side playerSide = side::LEFT;
    const float AXE_POS_LEFT = 400;
    const float AXE_POS_RIGHT = 800;
    float logXSpeed = 700;
    float logYSpeed =  -1100;
    bool logActive = false;
    bool acceptInput = true;
    bool paused = true;
    //bool debug = true;
    String message[3]={ "Press Enter", "Game Over", "0" };
    int score = 0;
    float widthFraction = -0.15f;
    int otherFraction = 1;
    int beeMaxSpeed = 139.0f;
    int beeMinSpeed = 210.9;
    int beeSpeed = GetRandomSpeed(beeMaxSpeed, beeMinSpeed);
    int rndSpeeds[3];               // Clouds Speeds
    for (int i = 0; i<3; i++)
    {
        rndSpeeds[i] = GetRandomSpeed(9 * otherFraction , 10 * otherFraction);
        //widthFraction+=0.355f;
        otherFraction += 4;
    }

    // Clock
    Clock clock;

    // Fonts & Text
    Font font;
    font.loadFromFile("./fonts/WABECK.otf");
    // Text coordsText;
    // coordsText.setCharacterSize(20);
    // coordsText.setFillColor(Color::Green);
    // coordsText.setFont(font);
    Text text[2];  // "Press Enter", "Game Over", "0"
    for (int i = 0; i < 2; i++)
    {
        text[i].setString(message[i]);
        text[i].setFillColor(Color::White);
        text[i].setCharacterSize(60);
        text[i].setFont(font);
        FloatRect textRect;
        textRect = text[i].getLocalBounds();
        if(i == 1)
        {
            text[1].setPosition(30, 30);
            text[1].setString(message[2]);
            continue;
        }

        text[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

        text[i].setPosition(vm.width / 2, vm.height / 2) ;
    }
    

    Texture textureBackground, textureCloud, textureBee, textureTree, textureBranch, texturePlayer, textureGravestone, textureAxe, textureLog;
    texturePlayer.loadFromFile("./graphics/player.png");
    textureBackground.loadFromFile("./graphics/background.png");
    textureTree.loadFromFile("./graphics/tree.png");
    textureBee.loadFromFile("./graphics/bee.png");
    textureCloud.loadFromFile("./graphics/cloud.png");
    textureBranch.loadFromFile("./graphics/branch.png");
    textureGravestone.loadFromFile("./graphics/rip.png");
    textureAxe.loadFromFile("./graphics/axe.png");
    textureLog.loadFromFile("./graphics/log.png");


    // Sprites
    Sprite spriteBackground, spriteBee, spritePlayer, spriteGravestone, spriteAxe, spriteLog; //spriteTree; //spriteCloud1, spriteCloud2, spriteCloud3,
    Sprite spriteCloud[3]; // array of 3 sprites of clouds
    Sprite spriteTree[12];
    Sprite branches[6];
    
    // TIME BAR

    RectangleShape timeBar, timeBarOutline;
    float startWidth = 250;
    timeBar.setSize(Vector2f(startWidth, 30));
    timeBar.setOrigin(timeBar.getLocalBounds().width / 2, timeBar.getLocalBounds().height / 2);
    timeBar.setPosition(vm.width / 2, vm.height - 120);
    timeBar.setFillColor(Color::White);
        // OutLiner
    timeBarOutline.setSize(Vector2f(startWidth, 30));
    timeBarOutline.setOrigin(timeBar.getLocalBounds().width / 2, timeBar.getLocalBounds().height / 2);
    timeBarOutline.setPosition(vm.width / 2, vm.height - 120);
    timeBarOutline.setFillColor(Color::Transparent);
    timeBarOutline.setOutlineColor(Color::Black);
    timeBarOutline.setOutlineThickness(7);
    Time gameTimeTotal;
    float timeRemaining = 9.0f;
    float timeBarWidthPerSecond = startWidth / timeRemaining;

        // SETTING UP TEXTURES
    spriteBackground.setTexture(textureBackground);
    for (short i = 0; i < 3; i++)
    {
        spriteCloud[i].setTexture(textureCloud);
    }
                                            
    spriteBee.setTexture(textureBee);
                                            // Banch SetUP ==========
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setScale(defScale);
        branches[i].setPosition(2000, 2000);
        branches[i].setOrigin(branches[i].getLocalBounds().width / 2, branches[i].getLocalBounds().height / 2);
    }
        // Player, Gravestone and Axe also log
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setScale(defScale);
    spritePlayer.setOrigin(spritePlayer.getLocalBounds().width / 2, spritePlayer.getLocalBounds().height / 2);
    spritePlayer.setPosition(vm.width / 2 - spritePlayer.getLocalBounds().width - 50, vm. height - 200);
    spriteGravestone.setTexture(textureGravestone);
    spriteGravestone.setScale(defScale);
    spriteGravestone.setOrigin(spriteGravestone.getLocalBounds().width / 2, spriteGravestone.getLocalBounds().height / 2);
    spriteGravestone.setPosition(spritePlayer.getPosition().x , spritePlayer.getPosition().y);
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setScale(defScale);
    spriteAxe.setOrigin(spriteAxe.getLocalBounds().width / 2, spriteAxe.getLocalBounds().height / 2);
    spriteAxe.setPosition(spritePlayer.getPosition().x + 40, spritePlayer.getPosition().y);
    spriteLog.setTexture(textureLog);
    spriteLog.setScale(defScale);
    spriteLog.setOrigin(spriteLog.getLocalBounds().width / 2, spriteLog.getLocalBounds().height / 2);
    spriteLog.setPosition(vm.width / 2, spriteTree[0].getGlobalBounds().height - (spriteLog.getLocalBounds().height / 2));

        // Setting the scale
    spriteBackground.setScale(0.73f, 0.70f);

    int ytree  = 20;
    int xtree  = 30;
    for (short i = 0; i < 12; i++)
    {
        spriteTree[i].setTexture(textureTree);
        if (i == 0)
        {   // Main Sprite Tree
            spriteTree[0].setScale(defScale);
            spriteTree[0].setOrigin(spriteTree[0].getLocalBounds().width / 2, 0);
            spriteTree[0].setPosition(vm.width / 2, 0);
            continue;
        }
        if (i>6)
        {    
            spriteTree[i].setScale(Vector2f(treesScale.x * 0.6f, treesScale.y * 1.6f));
            spriteTree[i].setPosition(40 + ytree,  -380);
            ytree += 190;
            continue;
        }
        spriteTree[i].setScale(treesScale);
        spriteTree[i].setPosition(xtree, 0);
        xtree += 230;
    }
    
    spriteBee.setScale(defScale);
        
        // Setting the pos =============================================
    spriteBackground.setPosition(0, 0);
    spriteBee.setPosition(vm.width / GetRandomSpeed(-1.0f, 10.0f), 350);

    short cloudHeight = 0;
    for (short i = 0; i < 3; i++)
    {
        spriteCloud[i].setPosition(vm.width * widthFraction , cloudHeight);
        cloudHeight += 60;
        widthFraction += 0.4f;
    }
    
    spritePlayer.setScale(spritePlayer.getScale().x * -1, spritePlayer.getScale().y);
// <-- MAIN LOOP -->
    while (window.isOpen())
    {
        // Updatable Variables
        Time dt = clock.restart();
        clock.restart();

        // HANDLE PLAYER INPUT
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.KeyReleased && !paused)
            {
                // Listen for key press again
                acceptInput = true;
                // Hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
            if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();
        }
            // NEW GAME
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                timeRemaining = 9.0f;
                score = 0;
                paused = false;

                // CLEAR BRANCHES
                for (int i = 1; i < NUM_BRANCHES; i++)
                {
                    branchesPositions[i] = side::NONE;
                }
                // HIDE RIP
                spriteGravestone.setPosition(2000,2000);
                // Reset Player
                spritePlayer.setPosition(vm.width / 2 - spritePlayer.getLocalBounds().width - 50, vm. height - 200);
                playerSide = side::LEFT;
                acceptInput = true;
            }

            if (acceptInput)
            {
                if(Keyboard::isKeyPressed(Keyboard::Right))
                {
                    if (playerSide == side::LEFT)
                    {
                        spritePlayer.setScale(spritePlayer.getScale().x * -1, spritePlayer.getScale().y);
                    }
                    playerSide = side::RIGHT;
                    
                    score++;
                    timeRemaining += (2 / score) * .15;
                    spriteAxe.setPosition(AXE_POS_RIGHT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(vm.width / 2 + spritePlayer.getLocalBounds().width + 50, vm. height - 200);
                    UpdateBranches(score);
                    logXSpeed = -700;
                    logActive = true;
                    acceptInput = false;

                }
                if (Keyboard::isKeyPressed(Keyboard::Left))
                {
                    if (playerSide == side::RIGHT)
                    {    
                        spritePlayer.setScale(spritePlayer.getScale().x * -1, spritePlayer.getScale().y);
                    }
                    playerSide = side::LEFT;


                    score++;
                    timeRemaining += (2 / score) * .15;
                    spriteAxe.setPosition(AXE_POS_LEFT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(vm.width / 2 - spritePlayer.getLocalBounds().width - 50, vm. height - 200);
                    UpdateBranches(score);
                    logXSpeed = 700;
                    logActive = true;
                    acceptInput = false;
                }
                
            }




        //  ------------------ UPDATE ----------------------------------
            



            // TEST //
        // std::stringstream ss;
        // ss << "X: " << Mouse::getPosition().x << " Y: "  << Mouse::getPosition().y;
        // coordsText.setString(ss.str());
        // coordsText.setPosition(vm.width / 2 - 40, vm.height / 2 - 40);


        // UPDATE SCREEN
        if (!paused)
        {
            
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeRemaining * timeBarWidthPerSecond, timeBar.getSize().y));
            
            if (timeRemaining <= 0.0f)
            {
                paused = true;
                text[0].setString(message[1]);
            }
            std:: stringstream ss;
            ss << score;                        // <----
            text[1].setString(ss.str());

            // Branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 105.0f;
                if (branchesPositions[i] == side::LEFT)
                {
                    branches[i].setPosition((vm.width / 2) - (branches[i].getLocalBounds().width / 1.65), height);
                    branches[i].setRotation(180);
                }
                else if(branchesPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition((vm.width / 2) + (branches[i].getLocalBounds().width / 1.65), height);
                    branches[i].setRotation(0);
                }
                else
                {
                    branches[i].setPosition(2000, height);
                }
            }
            



            // Clouds
        for (short i = 0; i < 3; i++)
        {
            if (spriteCloud[i].getPosition().x > vm.width + 40)
            {
                spriteCloud[i].setPosition(-310, spriteCloud[i].getPosition().y);
                continue;
            }

            spriteCloud[i].setPosition(spriteCloud[i].getPosition().x + rndSpeeds[i] * dt.asSeconds(), spriteCloud[i].getPosition().y);
        }
            // Bee Update
       
            if (spriteBee.getPosition().x < -40)
            {
                spriteBee.setPosition(vm.width + 50, spriteBee.getPosition().y);
                beeSpeed = GetRandomSpeed(beeMinSpeed, beeMaxSpeed);
            }

            spriteBee.setPosition(spriteBee.getPosition().x - beeSpeed * dt.asSeconds(), spriteBee.getPosition().y);
            
        }
        

        //DRAW THE SCREEN
        window.clear(); // Clear last frame
        window.draw(spriteBackground); // Draw stuff to the screen'
        for (short i = 12; i > 0; i--)
        {
            window.draw(spriteTree[i]);
            if(i < 3) 
           {
            window.draw(spriteCloud[i]);
            window.draw(spriteTree[i]);
            }
        }

        window.draw(spriteTree[0]); // Main Tree

        window.draw(text[1]); // middle screem message
        window.draw(spriteBee);
        window.draw(spriteAxe);
        window.draw(spriteGravestone);
        window.draw(spriteLog);


        window.draw(spritePlayer);
        // BRACH
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }


        // TEXT UI
         // Press enter, game over and Score.
         
        window.draw(timeBar);
        window.draw(timeBarOutline);
        if (paused)                     // TODO ADD CONDITION FOR GAME OVER
        {
            window.draw(text[0]);
        }
        window.display(); // Display all drawed
    }

    return 0;
}

    void UpdateBranches(int seed)
    {
        for (int i = NUM_BRANCHES - 1 ; i > 0; i--)
        {
            branchesPositions[i] = branchesPositions[i - 1];
        }

        // Spawn a randowm branch at pos 0
        // Left, right or none
        // make an int var instead
        srand((int)time(0) + seed);
        
        int r = (rand() % 6);
        switch (r)
        {
            case 0: branchesPositions[0] = side::LEFT;
                    break;
            case 1: branchesPositions[0] = side::RIGHT;
                    break;
            default:
                    branchesPositions[0] = side::NONE;
                    break;

        }    
    }