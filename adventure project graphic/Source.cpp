#include "includes.h"
#include "Map.h"
#include "NPC.h"
#include "EventChecker.h"
#include "Player.h"

int renderPlay(sf::RenderWindow *window) {
	Map m("Map4", 32, 32);	//wczytujê mapê		"Map", 20, 20			"Map3", 32, 32
	Player p(m.getRespawn().x, m.getRespawn().y, "Player");	//tworzê nowego gracza na pozycji 30x30 z textur¹ ball.png
	int velocity = 5;	//prêdkoœæ o ile pikseli siê porusza gracz
	int DamageCoolDown = 0;	//jak dostaniesz damage to masz chwilkê przez któr¹ nic nie zada ci damage (taka nieœmiertelnoœæ)

	//font i zmienna odpowidzialna za tekst
	sf::Font font;	
	if (!font.loadFromFile("Data/aerial.ttf"))
		exit(-1);
	sf::Text hpText;	
	hpText.setFont(font);
	hpText.setString("HP: " + std::to_string(p.getHealt()));	
	hpText.setFillColor(sf::Color::Black); 
	hpText.setCharacterSize(12);	
	sf::Vector2f textPosition;	

	//ustawienia kamery
	sf::View view;	
	view.setCenter(p.getPosition()); 
	view.setSize(window->getSize().x, window->getSize().y);	
	view.zoom(0.5);

	//std::cout << "Entering main loop\n";
	while (window->isOpen()) {	//podstawowy loop
		sf::Event event;
		int move = 0;

		//sprawdŸ czy gracz jeszcze ¿yje
		if (!p.isAlive()) {	
			return 0;	
		}
		
		//sprawdŸ kolizje z ramp¹
		if (m.collision(p.getSprite(), 2)) 
			p.jump(40, true); 

		if (m.collision(p.getSprite(), 10)) {
			m.load("Map5", 32, 32, false);
			p.setPosition(m.getRespawn());
			view.setCenter(p.getPosition());
		}

		//damage detection
		if (DamageCoolDown <= 0) {
			if (m.collision(p.getSprite(), 3)) {
				p.setDamage(1);	
				hpText.setString("HP: " + std::to_string(p.getHealt()));
				DamageCoolDown = 60;
				p.setPosition(m.getRespawn());
				view.setCenter(p.getPosition());
			}
		}
		else
			DamageCoolDown--;

		//sterowanie postaci¹
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){	
			p.move(-velocity, 0, m); 
			move += -velocity;
			if (p.getMoveState() < 5 || p.getMoveState() > 8) {
				p.setDeley(0);
				p.setMoveState(5);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			p.move(velocity, 0, m);
			move += velocity;
			if (p.getMoveState() < 1 || p.getMoveState() > 4) {
				p.setDeley(0);
				p.setMoveState(1);
			}
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D) && p.getMoveState() != 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			p.setMoveState(0);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))	
			p.jump(20, false);	

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return 0;


		//eventy zwi¹zane z oknem
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Resized) {	
				view.setSize(event.size.width, event.size.height);	
				view.zoom(0.5);
				view.setCenter(p.getPosition());
			}

			if (event.type == sf::Event::Closed) {
				window->close();
				return -1;
			}
		}

		//aktualizacje zmiennych i rysowanie na ekranie po wykonaniu loopa
		p.gravity(m, velocity); 
		view.setCenter(p.updateCamera(window, p.getPosition(), &m, &view));
		//std::cout << view.getCenter().x << " " << view.getCenter().y << std::endl;
		//view.setCenter(p.getPosition()); 
		std::cout << p.getPosition().x << " " << p.getPosition().y << std::endl;
		hpText.setPosition(sf::Vector2f(view.getCenter().x - (window->getSize().x / 5 + 20), view.getCenter().y - (window->getSize().y / 5 + 20)));
		p.animationComponent();
		window->clear(sf::Color::Black);	
		window->setView(view);	
		m.Draw(window);
		p.Draw(window); 
		window->draw(hpText);
		window->display();	
	}
	return 0;
}

int renderMenu(sf::RenderWindow *window) {
	sf::View view;

	sf::Font font;	//wczytaj u¿ywany przez nas font
	if (!font.loadFromFile("Data/aerial.ttf"))
		exit(-1);
	sf::Text menu, play, quit;
	menu.setFont(font);
	play.setFont(font);
	quit.setFont(font);
	menu.setCharacterSize(24);
	play.setCharacterSize(18);
	quit.setCharacterSize(18);
	menu.setString("Menu:");
	play.setString("Play");
	quit.setString("Quit");
	menu.setFillColor(sf::Color::Red);
	play.setFillColor(sf::Color::Green);
	quit.setFillColor(sf::Color::Green);
	menu.setPosition(sf::Vector2f(135, 40));
	play.setPosition(sf::Vector2f(150, 80));
	quit.setPosition(sf::Vector2f(150, 115));

	view.setCenter(quit.getPosition());
	view.setSize(window->getSize().x, window->getSize().y);	//ustawiam rozdzielczoœæ kamery
	view.zoom(0.5);

	while (window->isOpen()) {	//podstawowy loop
		sf::Event event;	

		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			if (event.type == sf::Event::Resized) {	//jak zmieniasz rozmiar okienka
				view.setCenter(quit.getPosition());
				view.setSize(event.size.width, event.size.height);	//to wszystko skaluje siê tak by pozosta³y proporcje
				view.zoom(0.5);
			}
		}
		
		sf::FloatRect ob = play.getGlobalBounds();
		if (ob.contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			return 1;

		ob = quit.getGlobalBounds();
		if (ob.contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			return -1;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			return -1;

		window->setView(view);
		window->clear(sf::Color::Black);	//wyczyœæ okno i ustaw jego t³o na czarny
		window->draw(menu);
		window->draw(play);
		window->draw(quit);
		window->display();	//wyœwietl
	}
	return 0;
}

int main() {
	bool Menus[2] = { false };	//zmienna do kontroli menu; jak bêdzie wiêcej menu ni¿ tylko to g³ówne to bêdzie to przydatne
	int MenuState = 0;	//ró¿ne menu mog¹ zwracaæ ró¿ne wartoœci, wiêc jak¹œ zmienn¹ musimy przetrzymywaæ te wartoœci
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game");	//tworzymy nasze g³ówne okno z gr¹...
	window.setFramerateLimit(60);	//... i ustawiamy limit fps na 60 ;p

	Menus[0] = true;	//aby uruchomi³ menu g³ówne

	while (window.isOpen()) {	//g³ówna pêtla startowa
		sf::Event event;	//tworzê obiekt typu event do kontroli... eventów?
		window.setView(window.getDefaultView());

		while (window.pollEvent(event))	//sprawdzanie eventów
			if (event.type == sf::Event::Closed)	//jak klikn¹³eœ tego X na okienku
				window.close();						//to zamknij okno

		if (Menus[0] == true) {		//jak tak to uruchom menu
			Menus[0] = false;
			MenuState = renderMenu(&window);	//przekazujemy adres window by u¿ywaæ go jako wskaŸnik
		}
		else if (Menus[1] == true) {	//jak tak to uruchom grê
			Menus[1] = false;
			MenuState = renderPlay(&window);
		}

		if (MenuState == -1)	// jak funkcja zwróci³a -1 to znaczy ¿e ktoœ klikn¹³ w menu quit (wy³¹cz grê)
			return 0;
		else if (MenuState == 0)	// ktoœ wróci³ do menu
			Menus[0] = true;
		else if (MenuState == 1)	//ktoœ klikn¹³ play w menu (uruchom grê)
			Menus[1] = true;

	}
}

/*
	NOTATKI:

	
	



*/