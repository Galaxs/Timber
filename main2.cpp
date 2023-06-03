// include important libraries here
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// make code easier to type using namespace 
using namespace sf;
// using namespace std;

// Function declaration
void updateBranches(int seed);
void updateScore();


// Draw some text
int score = 0;
Text scoreText;

const int NUM_BRANCHES = 6;
Sprite branches [NUM_BRANCHES];

// Where is the player and branch?
// Left or Right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

// This is where our game start from
int main()
{
    // create a video mode object
    VideoMode vm(1366, 768);
        
    // Create and open a windows for the game
    RenderWindow window(vm, "Timber", Style::Fullscreen);
    Vector2f scaleV2f(0.71, 0.65);


    // @ BACKGROUND
    // Create a texture to hold a graphic on the GPU
    Texture textureBackground;

    // Load a graphic into the texture
    textureBackground.loadFromFile("./graphics/background.png");
    
    // Create a sprite
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);

    // Set the spriteBackground to cover the screen
    spriteBackground.setScale(scaleV2f);
    spriteBackground.setPosition(0, 0);


    ///@ Make a tree sprite
    Texture treeTexture;
    treeTexture.loadFromFile("./graphics/tree.png");
    Sprite treeSprite;
    treeSprite.setTexture(treeTexture);
    treeSprite.setScale(scaleV2f);
    treeSprite.setPosition(570, 0);

    ///@ Bee
    Texture textureBee;
    textureBee.loadFromFile("./graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setScale(scaleV2f);
    spriteBee.setPosition(0, 670);
    // Is the bee currently moving
    bool beeActive = false;

    // How fast can the bee fly
    float beeSpeed = 0.0f;

    ///# Clouds
    Texture textureCloud;
    textureCloud.loadFromFile("./graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;

    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    // Clouds's scales
    spriteCloud1.setScale(scaleV2f);
    spriteCloud2.setScale(scaleV2f);
    spriteCloud3.setScale(scaleV2f);

    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 120);
    spriteCloud3.setPosition(0, 250);

    // Are the clouds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    // How fast is each cloud?
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables to control the time itself
    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarStartHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarStartHeight));
    timeBar.setFillColor(Color::Red);
    //timeBar.setOrigin()
    timeBar.setPosition((window.getSize().x / 2) - timeBarStartWidth / 2, 600); // 980

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
    
    // Track wether the game is running
    bool paused = true;

    Text messageText;
    // We need to choose a font
    Font font;
    font.loadFromFile("./fonts/KOMIKAP_.ttf");
    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);
    // Assign the actual message
    messageText.setString("Press Enter to Start!");
    scoreText.setString("Score = 0");

    // Make it really big
    messageText.setCharacterSize(60);
    scoreText.setCharacterSize(75);

    // Choose a color
    messageText.setColor(Color::White);
    scoreText.setColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
    messageText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    scoreText.setPosition(20, 20);

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("./graphics/branch.png");

    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);

        // Set the prite's origin to dead centre
        // We can then spin it around without changing its position
        branches[i].setOrigin(220, 20);
        branches[i].setScale(scaleV2f);
    }
    
    // Prepare the player
    Vector2f playerLeftPosition(450, 525);
    Vector2f playerRightPosition(900, 525);

    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(playerLeftPosition);
    spritePlayer.setScale(scaleV2f);
    FloatRect rectPlayer = spritePlayer.getLocalBounds();
    spritePlayer.setOrigin(rectPlayer.left + rectPlayer.width / 2, rectPlayer.top + rectPlayer.height / 2);
    
    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the player gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(430, 510);
    spriteRIP.setScale(scaleV2f);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(535, 545);
    spriteAxe.setScale(scaleV2f);
    FloatRect rectAxe = spriteAxe.getLocalBounds();
    spriteAxe.setOrigin(rectAxe.left + rectAxe.width /2, rectAxe.top + rectAxe.height / 2);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 535;
    const float AXE_POSITION_RIGHT = 825; 

    // Preparing the flying log
    Vector2f logPosition(571, 512);
    Texture textureLog;
    textureLog.loadFromFile("./graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(logPosition);
    spriteLog.setScale(scaleV2f);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 5000;
    float logSpeedY = -5000;

    // Control the player input
    bool acceptInput = false;

    // Prepare the sounds
    // The player chopping sound
    sf::SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("./sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    // The player has met his end under a branch
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("./sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    // Out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("./sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    { 
        /* ***************************************
            Handle the players input
        ****************************************
        */
       Event event;     
       while (window.pollEvent(event))
       {
           if (event.type == Event::KeyPressed && !paused)
           {
               // Listen for key presses again
               acceptInput = true;

               // hide the axe
               spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
           }
       }
       

        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6;

            // Make all the branches disappear
            // starting in the second position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(430, 2000);

            // Move the player into position
            spritePlayer.setPosition(playerLeftPosition);

            acceptInput = true;
        }

        // Wrap the player controls to
        // Make sure we are accepting input
        if (acceptInput)
        {
            // More code here next...
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // Make sure the player is on the right
                playerSide = side::RIGHT;
                score ++;
                updateScore();

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

                if (playerSide == side::RIGHT)
                {spritePlayer.setScale(scaleV2f);}
                spritePlayer.setPosition(playerRightPosition);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the left
                spriteLog.setPosition(logPosition);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                // Play a chop sound
                chop.play();
            }

            // Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                // Make sure that the player is on the left
                playerSide = side::LEFT;
                
                score ++;
                updateScore();

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(playerLeftPosition);

                if (playerSide == side::LEFT)
                {spritePlayer.setScale(scaleV2f.x * -1, scaleV2f.y);}
                // Update the branches
                updateBranches(score);

                // set the log flying
                spriteLog.setPosition(logPosition);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                // Play a chop sound
                chop.play();
            }
        }

        /*
        ****************************************
        Update the scene
        **************************************** 
        */
       
    if (!paused)
    {
        // Measure time
        Time dt = clock.restart();

        // Subtract from the amount of time remaining
        timeRemaining -= dt.asSeconds();
        // Size up the time bar
        timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarStartHeight));

        if (timeRemaining <= 0.0f)
        {
            // Pause the game
            paused = true;
            messageText.setString("Out of time!");

            // Reposition thetext based on its new size
            FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height /2.0f);

            messageText.setPosition(window.getSize().x /2.0f, window.getSize().y / 2.0f);

            // Play theout of time sound
            outOfTime.play();
        }

        // Setup the bee
        if (!beeActive)
        {
            // How fast is the bee
            srand((int)time(0));
            beeSpeed = (rand() % 100) + 100;

            // How high is the bee
            srand((int)time(0) * 10);
            float height = (rand() % 250) + 250;
            spriteBee.setPosition(2000, height);

            beeActive = true;
        }
        else
        {
            spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

            // Has the bee reached the edge of the screen?
            if (spriteBee.getPosition().x < -100)
            {
                // Set itup ready to be a whole new bee next frame
                beeActive = false;
            }
        }

        // Manage the clouds
        // Cloud 1
        if (!cloud1Active)
        {
            // How fast is the cloud
            srand((int)time(0) * 10);
            cloud1Speed = (rand() % 80) + 10;

            // How high is the cloud
            srand((int)time(0) * 10);
            float height = (rand() % 100);
            spriteCloud1.setPosition(-200, height);
            cloud1Active = true;
        }
        else
        {
            spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
            
            // Has the cloud reached the right hand edge of the screen?
            if (spriteCloud1.getPosition().x > 1400)
            {
                // Set it up ready to be a whole new cloud next frame
                cloud1Active = false;
            }
        }
        
        // Cloud 2
        if (!cloud2Active)
        {
            // How fast is the cloud
            srand((int)time(0) * 20);
            cloud2Speed = (rand() % 80) + 10;

            // How high is the cloud
            srand((int)time(0) * 20);
            float height = (rand() % 200) - 110;
            spriteCloud2.setPosition(-200, height);
            cloud2Active = true;
        }
        else
        {
            spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

            // Has the cloud reached the right hand edge of the screen?
            if (spriteCloud2.getPosition().x > 1400)
            {
                // Set it up ready to be a whole new cloud next frame
                cloud2Active = false;
            }
        }

        // Cloud 3
        if (!cloud3Active)
        {
            // How fast is the cloud
            srand((int)time(0) * 30);
            cloud3Speed = (rand() % 80) + 10;

            // How high is the cloud
            srand((int)time(0) * 30);
            float height = (rand() % 300) - 110;
            spriteCloud3.setPosition(-200, height);
            cloud3Active = true;
        }
        else
        {
            spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

            // Has the cloud reached the right hand edge of the screen?
            if (spriteCloud3.getPosition().x > 1400)
            {
                // Set up ready to be a whole new cloud next frame
                cloud3Active = false;
            }
        }   
                    
 
    // Update the branch sprites
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        float height = i * 95;

        if (branchPositions[i] == side::LEFT)
        {
            // Move the sprite to the left side
            branches[i].setPosition(429, height);

            // Flip the sprite round the other way
            branches[i].setRotation(180);
        }
        else if (branchPositions[i] == side::RIGHT)
        {
            // Move the sprite to the right side
            branches[i].setPosition(931, height);

            // Set the sprite rotation to normal
            //if (branches[i].getRotation() != 0)
            branches[i].setRotation(0);
        }
        else
        {
            // Hide the branch
            branches[i].setPosition(3000, height);
        }
    }

    // Handle a flying log
    if (logActive)
    {
        spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                                spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
        
        // Has the log reached the right hand edge?
        if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
        {
            // Set it up ready to be a whole new log next frame
            logActive = false;
            spriteLog.setPosition(logPosition);
        }
    }

    // has the player been squished by a branch?
    if (branchPositions[5] == playerSide)
    {
        // death
        paused = true;
        acceptInput = false;

        // Draw the gravestone
        spriteRIP.setPosition(playerLeftPosition);

        // Hide the player
        spritePlayer.setPosition(2000, spritePlayer.getPosition().y);

        // Change the text of the message
        messageText.setString("SQUISHED!");

        // Center it on the screen
        FloatRect textRect = messageText.getLocalBounds();

        messageText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);

        messageText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        // Play the death sound
        death.play();
    }
    } // End if(!paused)
            
            /*
            ****************************************
            Draw the scene
            ****************************************
            */

        // Clear everything from the last frame
            window.clear();

        // Draw our game scene here

            // Draw the clouds
            window.draw(spriteBackground);
            window.draw(spriteCloud1);
            window.draw(spriteCloud2);
            window.draw(spriteCloud3);

            // Draw the branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                window.draw(branches[i]);
            }
            
            // Draw the tree
            window.draw(treeSprite);

            // Draw the player
            window.draw(spritePlayer);

            // Draw the axe
            window.draw(spriteAxe);

            // Draw the flying log
            window.draw(spriteLog);

            // Draw the gravestone
            window.draw(spriteRIP);

            window.draw(spriteBee);
            window.draw(scoreText);
            if (paused)
            {window.draw(messageText);}

            window.draw(timeBar);


            // Show everything we just drew
            window.display();
        }
    return 0;

}

// Function definition
void updateBranches(int seed)
{
    // Move all the branches down one place
    for (int j = NUM_BRANCHES -1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j -1];
    }
    
    // Spawn a new branch at position 0
    // Left, Right or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }

}

void updateScore()
{
       // Update the score text

    std::stringstream ss;
    ss << "Score : " << score << " Mi Reina "; // score;
    scoreText.setString(ss.str());
    

}
