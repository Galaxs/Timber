// #include <iostream>
// #include <SFML/System.hpp>
// #include <SFML/Window.hpp>
// #include <SFML/Graphics.hpp>


// // make code easier to type using namespace 
// using namespace sf;
// using namespace std;

// int main() {

//      /* code */
//     // create a video mode object
//     VideoMode vm (1366, 768);

//     // Create and open a windows for the game
//     RenderWindow window(vm, "Timber!", Style::Default);
//     window.setFramerateLimit(50);

//     // create a texture to hold a graphic on the GPU
//     Texture textureBackground;

//     // Load a graphic into the texture
//     textureBackground.loadFromFile("graphics/background.png");
    
//     // Create a Sprite
//     Sprite spriteBackground;
//     spriteBackground.setScale(0.8f, 0.65f);

//     // Attach the texture to the sprite
//     spriteBackground.setTexture(textureBackground);
//     // Set the sprite to cover the screen
//     spriteBackground.setPosition(0, 0);
    
//     // Make a tree sprite
//     Texture textureTree;
//     textureTree.loadFromFile("graphics/tree.png");
//     Sprite spriteTree;
//     spriteTree.setTexture(textureTree);
//     spriteTree.setPosition(window.getSize().x / 2 - 30, 0);    
//     spriteTree.setScale(0.6f, 0.6f);
    
//     // Prepare the bee
//     Texture textureBee;
//     textureBee.loadFromFile("graphics/bee.png");
//     Sprite spriteBee;
//     spriteBee.setTexture(textureBee);
//     spriteBee.setPosition(0, 500 );

//     // Is the bee currently moving?
//     bool beeActive = false;

//     // How fast can the bee fly
//     float beeSpeed = 0.0f;

//     // make 3 cloud sprites from 1 texture
//     Texture textureCloud;
//     textureCloud.loadFromFile("graphics/cloud.png");

//     // 3 New sprites with the same texture
//     Sprite spriteCloud1, spriteCloud2, spriteCloud3;
//     spriteCloud1.setTexture(textureCloud);
//     spriteCloud2.setTexture(textureCloud);
//     spriteCloud3.setTexture(textureCloud);

//     // Position the clouds on the left of the screen
//     spriteCloud1.setPosition(0, 0);
//     spriteCloud1.setScale(0.6f, 0.6f);
//     spriteCloud2.setPosition(0, 200);
//     spriteCloud2.setScale(0.6f, 0.6f);
//     spriteCloud3.setPosition(0, 400);
//     spriteCloud3.setScale(0.6f, 0.6f);

//     // Are the clouds currently on screen?
//     bool cloud1Active = false, cloud2Active = false, cloud3Active = false;

//     // How fast is each cloud?
//     float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;
//     // at different hights


//     // Variable to control the time itself
//     Clock clock;

//     // int playerScore = 0;
//     // char playerInitial[3] = {'A', 'S', 'S'};
//     // bool isAlive = true;    

//     while (window.isOpen())
//     {
   
//         /* ***************************************
//             Handle the players input
//         ****************************************
//         */
//         if(Keyboard::isKeyPressed(Keyboard::Escape))
//         {
//             window.close();
//         }
//         /*
//         ****************************************
//             Update the scene
//         ****************************************
//         */
//         // Measure time
//         Time dt = clock.restart();
//         /*
//         ****************************************
//             Draw the scene
//         ****************************************
//         */
//             // Clear everything from the last frame
//         window.clear(Color::Black);

//          // Draw our game scene here
//         window.draw(spriteBackground);

//         // Draw the clouds
//         window.draw(spriteCloud1);
//         window.draw(spriteCloud2);
//         window.draw(spriteCloud3);

//         // Draw the tree
//         window.draw(spriteTree);

//         // Draw the insect
//         window.draw(spriteBee);


//             // Show everything we just drew
//         window.display();
        
//     }
//     // cout << "Hello World" << endl;
 
//  return 0;   
// }