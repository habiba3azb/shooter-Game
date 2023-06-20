#include<iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace sf;

int main() {
	//WINDOW
	RenderWindow window(VideoMode(800, 600), "Shooter Game");
	//	window.setFramerateLimit(60);

    Texture gameOver;
    if (!gameOver.loadFromFile("GameOver.png"))
        cout << "Error Loading Image" << endl;
    Sprite spGameOver(gameOver);

    spGameOver.scale(.18, .18);
    spGameOver.setPosition(100, 100);

	//Player Texture

//    Music m;
//    m.openFromFile("Res/audio_Game.ogg");
//    m.play();

	Texture playerTex;
	if (!playerTex.loadFromFile("Res/playerTex.png"));

	// Projectile Texture
	Texture projectileTex;
	if (!projectileTex.loadFromFile("Res/projectileTex.png"));

	// Enemy texture

	Texture enemyTex;
	if (!enemyTex.loadFromFile("Res/enemyTex.png"));
//	enemyTex.setSmooth(true);

	// loosing Screen texture;
	Image Screenshot;//Image to store the screenshot
	Texture losing;
	losing.create(window.getSize().x, window.getSize().y);

	//PLAYER
	RectangleShape player(Vector2f(80.f, 80.f));
	player.setOrigin(player.getSize() / 2.f);
	player.setPosition(window.getSize().x / 2.f, window.getSize().y - 50.f);
	player.setTexture(&playerTex);
	player.setFillColor(Color(0, 0, 255, 200));

	//PROJECTILE
	CircleShape projectile(10.f);
	projectile.setOrigin(projectile.getOrigin().x + 10.f, projectile.getOrigin().y + 10.f);
	projectile.setTexture(&projectileTex);
//	projectile.setFillColor(Color::Red);
	vector <CircleShape> projectiles;
	projectiles.push_back(CircleShape(projectile));

	//ENEMY
	RectangleShape enemy(Vector2f(60.f, 60.f));
	enemy.setOrigin(enemy.getSize() / 2.f);
	enemy.setTexture(&enemyTex);
	//enemy.setFillColor(Color::Blue);
	vector <RectangleShape> enemies;

	//LOOSING

	RectangleShape loosing(Vector2f(800, 600));
	loosing.setOrigin(loosing.getSize() / 2.f);
	loosing.setTexture(&losing);
	loosing.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

	//TEXT
	Text scoreOnScreen;
	scoreOnScreen.setPosition(window.getSize().x / 2.f - 120, 3.f);
	scoreOnScreen.setCharacterSize(30);
	scoreOnScreen.setFillColor(Color::Black);
	Font gotham;
	if (!gotham.loadFromFile("Res/Gotham-Bold.otf"));
	scoreOnScreen.setFont(gotham);

	Vector2f mousePos;
	Vector2f enemyCenter;
	Vector2f aimDir;
	Vector2f aimNormal;
	Vector2f velocity;

	long double deltatime = 0; //for finding duration of clock
	double shot = 0;		//time counter for shooting
	double rain = 0;		//time counter for raining
	double chaser = 0;
	double shotspeed = 0.09;		//time limit for shooting
	double rainSpeed = 0.15;		//time limit for raining
	double chaserSpeed = 1;		//time limit for chaser ghosts
	double speed = 0;
	int score = 0;
	srand(time(NULL));

	bool lose = false;
	bool pause = false;
	sf::Clock clock;

	while (window.isOpen()) {


		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::LostFocus)
				pause = true;
			if (event.type == Event::GainedFocus)
				pause = false;
		}

		deltatime = clock.restart().asSeconds();

		if (score <= 50) speed = 8 * deltatime * 56.2;		//speed of falling ghosts


		// If Loosing Screen or pause
		if (lose || pause) {
			if (Keyboard::isKeyPressed(sf::Keyboard::R)) {	//Reset the game

				shotspeed = 0.09;	//time limit for shooting
				rainSpeed = 0.15;	//time limit for raining
				chaserSpeed = 1;		//time limit for chaser ghosts
				speed = 6 * deltatime * 45.2;		//speed of falling ghosts
				shot = 0; rain = 0; lose = false;
				score = 0;
				enemies.clear(); projectiles.clear();
			}

			// Clear the window
			window.clear(sf::Color::White);
			// Print message
			string theMessage;
			theMessage += "    SCORE: " + to_string(score);
            if (lose)
                theMessage += "\n     PRESS R\nTO GO AGAIN! ";
            else
                theMessage += "  \nPAUSED";
			scoreOnScreen.setString(theMessage);

			//Opacity decrease
			loosing.setFillColor(sf::Color(255, 255, 255, 40));
			//draw all
			window.draw(loosing);
			window.draw(scoreOnScreen);
			window.display();
			// continue the loop, do not go in game;
			continue;
		}

		//======== INCREMENTING LIMITS ======================================================================

		if (shot < shotspeed) shot += deltatime;
		if (rain < rainSpeed) rain += deltatime;
		if (chaser < chaserSpeed) chaser += deltatime;

		//===================================================================================================


		//=====================================================================================================

		//UPDATE



		//UPDATE PLAYER

		mousePos = player.getPosition();
		player.setPosition(sf::Mouse::getPosition(window).x, window.getSize().y - 50.f);

		// don't allow the rocket to go out of the screen (Right Side)
		if (player.getPosition().x > window.getSize().x - 40) {
			player.setPosition(window.getSize().x - 40, player.getPosition().y);
		}

		// don't allow the rocket to go out of the screen (Left Side)
		if (sf::Mouse::getPosition(window).x < 40) {
			player.setPosition(40, player.getPosition().y);
		}

		//================================================================================

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shot > shotspeed) {
			projectile.setPosition(player.getPosition());
			projectiles.push_back(sf::CircleShape(projectile));
			shot = 0;
		}


		// UPDATE ENEMIES

		// UPDATE PROJECTILES

		if (rain > rainSpeed) {

			if (chaser > chaserSpeed) {
				enemy.setFillColor(sf::Color(255, 0, 0, 128)); // color of the chaser
				chaser = 0;
				enemy.setPosition(rand() % ((window.getSize().x)), 0);
			}

			else {
				enemy.setFillColor(sf::Color(255, 255, 255, 255));  // color of normal enemies
				enemy.setPosition(rand() % ((window.getSize().x - 60)) + 30, 100); // to make the positionX [30, 540]
			}

			enemies.push_back(enemy);
			rain = 0;
		}





        for (int i = 0; i < enemies.size(); i++) {
			enemyCenter = enemies[i].getPosition();
			aimDir = mousePos - enemyCenter;
			aimNormal = aimDir / sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
			//cout << i << endl;
			velocity.x = 0;

			if (enemies[i].getFillColor() == sf::Color(255, 0, 0, 128)) { // if this enemy is a chaser
				velocity.x = aimNormal.x * float(speed);
				velocity.y = speed + 20 * deltatime * 56.2;
            }

			enemies[i].move(velocity.x, speed);

			if (enemies[i].getPosition().y >= player.getPosition().y - 10.f) {
				enemies.erase(enemies.begin() + i);
				score++;
			}
		}


		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i].move(0, -8.f * deltatime * 56.2);
			if (projectiles[i].getPosition().y < 40)
				projectiles.erase(projectiles.begin() + i);
		}

		// COLLISION:

		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
				lose = true;
				break;
			}
		}

		for (int i = 0; i < enemies.size() && lose == false; i++) {
			for (int j = 0; j < projectiles.size(); j++) {
				if (enemies[i].getGlobalBounds().intersects(projectiles[j].getGlobalBounds())) {
					score++;
					enemies.erase(enemies.begin() + i);
					projectiles.erase(projectiles.begin() + j);
					break;
				}
			}
		}

		if (!lose) {
			window.clear(sf::Color::White);

			string theScoreMessage;
			theScoreMessage += "    SCORE: " + to_string(score);
			scoreOnScreen.setString(theScoreMessage);
			window.draw(scoreOnScreen);


			// Enemies draw:
			for (auto& x : enemies) {
				window.draw(x);
			}

			// Projectile draw
			for (auto& x : projectiles) {
				window.draw(x);
			}

			// Player draw
			window.draw(player);


			losing.update(window);
			// DISPLAY
			if (lose)
                window.draw(spGameOver);
			window.display();
		}
	}
}
