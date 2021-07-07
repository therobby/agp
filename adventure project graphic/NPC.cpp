#include "NPC.h"

NPC::NPC(float x, float y, std::string _texture) {
	canJump = true;
	Position.x = x;
	Position.y = y;
	lastPosition = Position;
	Jump = height = moveState = airState = deley = 0;
	for (int i = 0; i < 15; i++)
	{
		std::string tname = "Textures/" + _texture + "/" + _texture + std::to_string(i) +".png";
		if (!texture[i].loadFromFile(tname))	//jak nie za�auje si� tutaj textura
			exit(-1);	//to wypierdol program z kodem -1
		texture[i].setSmooth(true);	//wyg�ad� textur�
	}
	
	sprite.setTexture(texture[0]);	//przypisz textur� spritowi
	sprite.setPosition(Position);	//ustaw sprite na pozycji
}

void NPC::Draw(sf::RenderWindow *window) {
	window->draw(sprite);
}

sf::Vector2f NPC::getPosition() {
	return Position;
}

bool NPC::move(float x, float y, Map &m) {
	sf::Vector2f fPosition = Position;	//pomocniczy vector (future vector)
	fPosition.x += x;	//utaw mu przysz�e koordynaty
	fPosition.y += y;
	sf::Sprite future;	//ustaw tymczasowy sprite
	future.setTextureRect(sprite.getTextureRect());	//ustaw mu wymiary npc
	future.setPosition(fPosition);	//ustaw mu przysz�� pozycj�...
	if (!m.collision(future)) {	//... i sprawd� czy ma z czym� kolizj� 
		Position = fPosition;	//jak nie to go tam przesu�
		sprite.setPosition(Position);
	}
	else {	//jak nie
		fPosition = Position;	//to sprawd� czy kolidowa� by z punkcie o jeden piksel dalszym
		if (x < 0)
			fPosition.x--;
		else if (x > 0)
			fPosition.x++;
		if (y < 0)
			fPosition.y--;
		else if (y > 0)
			fPosition.y++;

		future.setPosition(fPosition);
		if (!m.collision(future)) {	//jak nie
			bool b = false;
			do {	//to szukaj ostatniego piksela kt�ry by nie kolidowa�
				fPosition = Position;
				if (y == 0 && x > 0)
					x--;
				else if (y == 0 && x < 0)
					x++;
				else if (x == 0 && y > 0)
					y--;
				else
					y++;

				fPosition.x += x;
				fPosition.y += y;
				future.setPosition(fPosition);
				b = !m.collision(future);
				//std::cout << (x == 0 && y == 0) << "\t" << b << std::endl;
			} while (!((x == 0 && y == 0) || b));

			//i jak znajdziesz to ustaw odpowiednio npc
			Position = fPosition;
			sprite.setPosition(Position);
			
			return true;	//zwr�� true bo mo�esz i si� ruszy�e�
		}
		else {
			return false;
		}	//jak tak to zwr�� false jako �e nie mo�esz si� ruszy�
	}
	return true;
}

void NPC::gravity(Map &m, float velocity) {
	if (height < Jump) {	//jak wysoko�� wzgl�dem skoku jest mniejsza (tj jeszce nie dolecia�e� tak wysoko)
		if (move(0, -velocity, m)){	//je�li mo�esz polecie� jeszce wy�ej
			height++;	//to pole� i zwi�ksz swoj� wysok��
		}
		else { //jak nie to zablokiuj skok
			height = Jump;
			airState = 2;
		}
	}
	else if (height < Jump + 4 && Jump != 0 && canJump == false) {	//	by� wisia� w powietrzu przez 4 (czego�)
		height++;
		if(airState != 2)
			airState = 2;
	}
	else {		// jak sko�czy�e� kod to ustaw pola na 0 i pozw�l na skakanie
		bool b = move(0, velocity, m);
		if (!b && Jump != 0) {
			Jump = height = 0;
			canJump = true;
		}
		else if (b && Jump == 0) {
			canJump = false;
			if (airState != 2)
				airState = 2;
		}
		else if(!b && Jump == 0){
			if (airState != 0)
				airState = 0;
			canJump = true;
		}
		
		//std::cout << b << " " << canJump << " " << Jump << " " << height << std::endl;	//debug
	}
	if(canJump && airState != 0)
		airState = 0;
}

void NPC::jump(float HowHigh, bool force) {
	if (Jump == 0 && height == 0 && canJump) {	//sprawd� czy mo�esz skaka�
		Jump = HowHigh;	//jak mo�esz to ustaw odpowiedno zmienne
		canJump = false;
		//std::cout << "NO FORCE" << std::endl;
		airState = 1;
	}
	else if (force) {	// jak nie mo�esz, ale masz forsa
		Jump = HowHigh;	// to ustaw odpowiedno zmienne
		height = 0;
		canJump = false;
		//std::cout << "FORCE" << std::endl;
		airState = 1;
	}
}

sf::Sprite NPC::getSprite() {
	return sprite;
}

void NPC::setPosition(sf::Vector2f position) {
	Position = position;
}

void NPC::animationComponent() {
	int deleyTime = 3;
	if (airState == 1 && moveState == 0) {
		sprite.setTexture(texture[9]);	//leci w g�r� w miejscu
	}
	else if (airState == 2 && moveState == 0) {
		sprite.setTexture(texture[10]);	//leci w d� w miejscu
	}
	else if (airState == 1 && moveState >= 1 && moveState <= 4) {
		sprite.setTexture(texture[11]);	//leci w prawo w g�r�
	}
	else if (airState == 1 && moveState >= 5 && moveState <= 8) {
		sprite.setTexture(texture[12]);	//leci w lewo w g�r�
	}
	else if (airState == 2 && moveState >= 1 && moveState <= 4) {
		sprite.setTexture(texture[13]);	//spada w prawo
	}
	else if (airState == 2 && moveState >= 5 && moveState <= 8) {
		sprite.setTexture(texture[14]);	//spada w lewo
	}
	else if (moveState == 0)
		sprite.setTexture(texture[0]);	//stoi w miejscu
	else if (moveState >= 1 && moveState <= 4) {	//idzie w prawo
		if (moveState == 1) {
			if (deley == 0) {
				sprite.setTexture(texture[1]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
		else if (moveState == 2) {
			if (deley == 0) {
				sprite.setTexture(texture[2]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
		else if (moveState == 3) {
			if (deley == 0) {
				sprite.setTexture(texture[3]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
		else if (moveState == 4) {
			if (deley == 0) {
				sprite.setTexture(texture[4]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
	}
	else if (moveState >= 5 && moveState <= 8) {	//idzie w lewo
		if (moveState == 5) {
			if (deley == 0) {
				sprite.setTexture(texture[5]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
		else if (moveState == 6) {
			if (deley == 0) {
				sprite.setTexture(texture[6]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
		else if (moveState == 7) {
			if (deley == 0) {
				sprite.setTexture(texture[7]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
		else if (moveState == 8) {
			if (deley == 0) {
				sprite.setTexture(texture[8]);
				deley++;
			}
			else if (deley != deleyTime)
				deley++;
			if (deley == deleyTime) {
				moveState++;
				deley = 0;
			}
		}
	}
}

int NPC::getMoveState() {
	return moveState;
}

void NPC::setMoveState(int state) {
	moveState = state;
}

void NPC::setDeley(int _deley) {
	deley = _deley;
}