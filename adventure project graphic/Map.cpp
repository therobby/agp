#include "Map.h"

Map::Map(std::string _MapName, int _texturesHeight, int _texturesWidth) {
	load(_MapName, _texturesHeight, _texturesWidth, true);
}

Map::~Map() {
	delete[] M;
	delete[] map;
	delete[] Collide;
	delete[] textures;
}



void Map::load(std::string _MapName, int _texturesHeight, int _texturesWidth, bool firstLoad) {
	x = y = 0;
	MapName = _MapName;
	texturesHeight = _texturesHeight;
	texturesWidth = _texturesWidth;
	if (!firstLoad) {
		delete[] M;
		delete[] map;
		delete[] Collide;
		delete[] textures;
	}

	std::ifstream file;
	file.open("Data/" + MapName + ".txt");

	if (file.is_open()) {	//jak plik mapy zosta³ uruchomiony
		std::string texturename;
		getXY(x, y, &file);	//pobierz wymiary

		M = new int[x*y];	//ustaw tablice w zale¿noœci od wymiarów
		map = new sf::Sprite[x*y];

		for (int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
				file >> M[j + (y * i)];	//wczytaj info na temat rozmieszczenia textur na mapie

		std::ifstream textfile;
		textfile.open("Data/" + MapName + "_Textures.txt");

		if (textfile.is_open()) {
			getY(texturessize, &textfile);
			textures = new sf::Texture[texturessize];

			for (int i = 0; i < texturessize; i++) {
				std::getline(textfile, texturename);
				if (!textures[i].loadFromFile("Textures/" + texturename))	//wczytaj textury
					exit(-1);
				textures[i].setSmooth(true);
			}
		}
		else {
			std::cout << "Can't load " << "Data/textures.txt\n";
			exit(2);
		}

		for (int j = 0; j < y; j++)
			for (int i = 0; i < x; i++) {
				if (M[i + (x * j)] >= texturessize) {
					std::cout << "Error: map file is asking for nonexisting texture!\n";
					exit(-1);
				}
				map[i + (x * j)].setTexture(textures[M[i + (x * j)]]);	//przypisz textury
				map[i + (x * j)].setPosition(sf::Vector2f(texturesHeight * i, texturesWidth * j));	//i ustaw sprity
			}
	}
	else {
		std::cout << "Can't load " << "Data/" + MapName + ".txt\n";
		exit(2);
	}
	file.close();

	file.open("Data/" + MapName + "_Collide.txt");
	if (file.is_open()) {
		getX(CollideSize, &file);

		Collide = new int[CollideSize];	//ustaw tablice w zale¿noœci od wymiarów

		for (int i = 0; i < CollideSize; i++)
			file >> Collide[i];	//wczytaj info na temat koliduj¹cych bloków
	}
	else {
		std::cout << "Can't load " << "Data/" + MapName + "_Collide.txt\n";
		exit(2);
	}
	file.close();
}

void Map::Draw(sf::RenderWindow *window) {
	for (int i = 0; i < x*y; i++)
		window->draw(map[i]);
}

bool Map::collision(sf::Sprite npc) {
	for (int i = 0; i < x*y; i++)
		for(int j = 0; j < CollideSize; j++)
			if (map[i].getTexture() == &textures[Collide[j]]) {	//sprawdŸ czy npc kooliduje z tym spritem
				sf::FloatRect colide = map[i].getGlobalBounds();	//pobierz wymiary sprite na mapie
				sf::FloatRect n = npc.getGlobalBounds();	//pobierz wymiary npc
				if (colide.intersects(n))	//sprawdŸ czy npc zawiera siê w sprite
					return true;	//jak tak to zwróæ ¿e jest kolizja
			}
	return false;
}

bool Map::collision(sf::Sprite npc, int object) {
	for (int i = 0; i < x*y; i++)
		if (map[i].getTexture() == &textures[object]) {	//to samo co wy¿ej tylko szukasz danego elementu, a nie wszystkich z którymi mo¿esz koolidowaæ
			sf::FloatRect colide = map[i].getGlobalBounds();
			sf::FloatRect n = npc.getGlobalBounds();
			if (colide.intersects(n))
				return true;
		}
	return false;
}

void Map::getXY(int &x, int &y, std::ifstream *file) {
	std::string s;	//zmienna pomocnicza
	char k; //zmienna pomocnicza
	do
	{
		*file >> k;	//zlicz iloœæ elementów w pliku
		x++;
	} while (!file->eof());
	file->clear();	//cofnij wskaŸnik do pocz¹tku pliku
	file->seekg(0);
	do
	{
		std::getline(*file, s);	//zlicz iloœæ linijek w pliku
		y++;
	} while (!file->eof());

	file->clear();	//cofnij wskaŸnik do pocz¹tku pliku
	file->seekg(0);
	x--;	//udejmij znak (chyba) '\0'
	x /= y;	//podziel x przez y by uzyskaæ ilosæ elementów w jednej linijce
}

void Map::getX(int &x, std::ifstream *file) {
	char k; //zmienna pomocnicza
	do
	{
		*file >> k;	//zlicz iloœæ elementów w pliku
		x++;
	} while (!file->eof());

	file->clear();	//cofnij wskaŸnik do pocz¹tku pliku
	file->seekg(0);
	x--;	//udejmij znak (chyba) '\0'
}

void Map::getY(int &y, std::ifstream *file) {
	std::string s;	//zmienna pomocnicza
	do
	{
		std::getline(*file, s);	//zlicz iloœæ linijek w pliku
		y++;
	} while (!file->eof());
	file->clear();	//cofnij wskaŸnik do pocz¹tku pliku
	file->seekg(0);
}

void Map::ChangeSprite(int x, int y, int texturenumber) {
	map[x * y].setTexture(textures[texturenumber]);	//zmieñ sprite na taki z danym numerem
}

std::string Map::getMapName() {
	return MapName;
}

sf::Vector2f Map::getRespawn() {
	sf::Vector2f resp;

	std::ifstream file;
	file.open("Data/" + MapName + "_Respawn.txt");
	if (file.is_open()) {
		file >> resp.x;
		file >> resp.y;
	}
	else {
		std::cout << "Can't load " << "Data/" + MapName + "_Respawn.txt\n";
		exit(-1);
	}
	return resp;
}

sf::Vector2f Map::getMapLastPosition() {
	sf::Vector2f pos(x*texturesHeight, y*texturesWidth);
	return pos;
}